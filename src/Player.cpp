#include "Player.h"


Player::Player() : vfQueue(VIDEO_FRAME_QUEUE_SIZE) {
    filename = new char[1024];
    pFormatCtx = NULL;
    videoIndex = audioIndex = -1;
    io_context = NULL;
    quit = false;

    swr_ctx = NULL;
    apCodecCtx = NULL;
    sampleRate = 0;
    audioBuffer = (uint8_t*)av_malloc((MAX_AUDIO_FRAME_SIZE*3)/2);
    audioBufferSize = 0;
    audioBufferIndex = 0;


    sws_ctx = NULL;
    vpCodecCtx = NULL;
    aspect_ratio = 0;


    frame_timer = 0;
    last_frame_pts = 0;
    last_frame_delay = 0;


    screen = NULL;
    sdlRenderer = NULL;
    sdlTexture = NULL;
    screen_w = screen_h = 0;


    StartPlay = NULL;
    VideoDecode = NULL;
}

Player::~Player() {
    av_free(audioBuffer);
    sws_freeContext(sws_ctx);
    swr_free(&swr_ctx);
    avcodec_close(apCodecCtx);
    avcodec_close(vpCodecCtx);

    avformat_close_input(&pFormatCtx);
}

int Player::playerInit(char* filepath, int sw, int sh) {
    strncpy(filename, filepath, 1024);                  //复制视频文件路径名
    screen_w = sw;
    screen_h = sh;
    return SUCCESS;
}

int Player::stream_component_open(Player* player, int streamIndex) {
    AVDictionary *optionsDict = NULL;
    const AVCodec *pCodec = NULL;
    AVCodecContext *pCodecCtx = NULL;           //编码器上下文对象,用于PPM文件输出
    AVCodecParameters *pCodecParam = player->pFormatCtx->streams[streamIndex]->codecpar;    //取得解码器上下文

    // 复制编解码器上下文对象，用于保存从视频流中抽取的帧
    pCodecCtx = avcodec_alloc_context3(NULL);   //创建AVCodecContext结构体对象pCodecCtx
    if (avcodec_parameters_to_context(pCodecCtx, pCodecParam) != 0) {   //复制编解码器上下文对象
        qDebug()<<"无法复制编码器上下文";
        return FAILED;
    }

    /*-----------------------
     * 根据视频流对应的解码器上下文查找对应的解码器，返回对应的解码器(信息结构体)
     -----------------------*/
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec || (avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0)) {
        qDebug()<<"不支持的解码器";
        return FAILED;
    }

    //检查解码器类型
    switch (pCodecCtx->codec_type) {
        case AVMEDIA_TYPE_AUDIO://音频解码器
            player->apCodecCtx = pCodecCtx;
            player->apCodecCtx->pkt_timebase = player->pFormatCtx->streams[streamIndex]->time_base;
            player->swr_ctx = swr_alloc();
            if (!player->swr_ctx) {
                qDebug() << "无法为swr_ctx分配内存空间";
                return FAILED;
            }
            player->swr_ctx = swr_alloc_set_opts(NULL,
                            av_get_default_channel_layout(pCodecCtx->channels),   //输出
                            AV_SAMPLE_FMT_S16,
                            pCodecCtx->sample_rate,                     //输出
                            pCodecCtx->channel_layout,     //输入
                            pCodecCtx->sample_fmt,      //PCM源文件的采样格式
                            pCodecCtx->sample_rate,         //输入
                            0, NULL);
            swr_init(player->swr_ctx);
            SDL_AudioSpec wanted_spec;
            wanted_spec.freq = pCodecCtx->sample_rate;
            wanted_spec.format = AUDIO_S16SYS;
            wanted_spec.channels = pCodecCtx->channels;
            wanted_spec.silence = 0;
            wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
            wanted_spec.callback = AudioPlay::audio_callback;
            wanted_spec.userdata = player;
            if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
                qDebug()<<"打开音频设备失败";
                return FAILED;
            }
            SDL_PauseAudio(0);              // 开启音频设备，如果这时候没有获得数据那么它就静音
            player->sampleRate = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16) * player->apCodecCtx->channels * player->apCodecCtx->sample_rate;
            if(player->sampleRate<=0) {
                return FAILED;
            }

            break;
        case AVMEDIA_TYPE_VIDEO://视频解码器
            player->vpCodecCtx = pCodecCtx;
            player->vpCodecCtx->pkt_timebase = player->pFormatCtx->streams[streamIndex]->time_base;
            //以系统时间为基准，初始化播放到当前帧的已播放时间值，该值为真实时间值、动态时间值、绝对时间值
            player->frame_timer=(double)av_gettime()/1000000.0;
            player->last_frame_delay = 40e-3;//初始化上一帧图像的动态刷新延迟时间
            // 设置图像转换像素格式为AV_PIX_FMT_YUV420P
            player->sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                             pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
            player->aspect_ratio = double(pCodecCtx->width) / pCodecCtx->height;
            player->sdlTexture = SDL_CreateTexture(player->sdlRenderer,SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);
            for(int i = 10; i > 0; i--) {
                player->VideoDecode = SDL_CreateThread(VideoPlay::videoDecode,NULL,player);        //创建解码线程
                if (player->VideoDecode) {      //检查线程是否创建成功
                    break;
                }
                SDL_Delay(10);                  //创建失败则等待10s后重新创建，若重复10次都失败，则退出
            }
            if(!player->VideoDecode) {
                return FAILED;
            }
            break;
        default:
            break;
    }
    return SUCCESS;
}


int Player::startPlay(void *arg) {
    Player* player = (Player*)arg;
    AVDictionary *io_dict = NULL;
    SDL_Event event;

    player->pFormatCtx = avformat_alloc_context();          //分配空间
    if (avformat_open_input(&player->pFormatCtx, player->filename, NULL, NULL) != 0) {
        qDebug() << "无法打开文件";
        event.type = PREPARE_FAILED_EVENT;
        SDL_PushEvent(&event);
        return FAILED;
    }
    if (avformat_find_stream_info(player->pFormatCtx, NULL) < 0) {
        qDebug() << "找不到流信息";
        event.type = PREPARE_FAILED_EVENT;
        SDL_PushEvent(&event);
        return FAILED;
    }

    // 打印pFormatCtx中的码流信息
    av_dump_format(player->pFormatCtx, 0, player->filename, 0);

    for (int i = 0; i < player->pFormatCtx->nb_streams; i++) {          //遍历文件中包含的所有流媒体类型(视频流、音频流、字幕流等)
        if (player->pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO) {             //若文件中包含有视频流
            player->videoIndex=i;               //用视频流类型的标号修改标识，使之不为-1
        }
        if (player->pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO) {             //若文件中包含有音频流
            player->audioIndex=i;               //用音频流类型的标号修改标识，使之不为-1
        }
    }
    if (player->audioIndex >= 0) {              //检查文件中是否存在音频流
        if(stream_component_open(player, player->audioIndex)==FAILED){         //根据指定类型打开音频流
            event.type = PREPARE_FAILED_EVENT;
            SDL_PushEvent(&event);
            return FAILED;
        }
    }
    if (player->videoIndex >= 0) {//检查文件中是否存在视频流
        if(stream_component_open(player, player->videoIndex)==FAILED) {     //根据指定类型打开视频流
            event.type = PREPARE_FAILED_EVENT;
            SDL_PushEvent(&event);
            return FAILED;
        }
    }

    if (player->videoIndex < 0 || player->audioIndex < 0) {     //检查文件中是否存在音视频流
        qDebug() << "缺少音频或视频流";
        event.type = PREPARE_FAILED_EVENT;
        SDL_PushEvent(&event);
        return FAILED;
    }

    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    while(1) {
        if(player->quit) {
            break;
        }
        if (player->apQueue.getSize() > MAX_AUDIOQ_SIZE || player->vpQueue.getSize() > MAX_VIDEOQ_SIZE) {
            SDL_Delay(10);
            continue;
        }
        //从文件中依次读取每个图像编码数据包，并存储在AVPacket数据结构中
        if (av_read_frame(player->pFormatCtx, packet) >= 0) {
            // Is this a packet from the video stream?
            if (packet->stream_index == player->videoIndex) {      //检查数据包是否为视频类型
                player->vpQueue.p_enqueue(packet);      //向队列中插入数据包
            } else if (packet->stream_index ==  player->audioIndex) {       //检查数据包是否为音频类型
                 player->apQueue.p_enqueue(packet);     //向队列中插入数据包
            }
        }else {
            av_packet_unref(packet);        //释放packet中保存的编码数据
            break;
        }
    }
    av_free(packet);
    return QUIT;
}

int Player::Play() {

    // SDL初始化
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        qDebug() << "无法初始化SDL";
        return -1;
    }

    screen = SDL_CreateWindow("视频播放器", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            screen_w, screen_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if(!screen) {
        qDebug() << "SDL: 无法创建窗口";
        return FAILED;
    }
    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

    StartPlay = SDL_CreateThread(Player::startPlay,NULL,this);   //创建编码数据包解析线程
    if (!StartPlay) {//检查线程是否创建成功
        return -1;
    }

    VideoPlay::schedule_refresh(this, 40);   //在指定的时间(40ms)后回调用户指定的函数，进行图像帧的显示更新

    SDL_Event event;
    while(1){
        SDL_WaitEvent(&event);
        if(event.type == AUDIO_FAILED_EVENT) {
            qDebug() <<"QUIT";
            quit = true;
            // 把锁打开
            SDL_UnlockMutex(AudioPlay::qlock);
        }
        if(event.type == VIDEO_FAILED_EVENT) {
            qDebug() <<"QUIT";
            quit = true;
            // 把锁打开
            SDL_UnlockMutex(VideoPlay::qlock);
        }
        switch(event.type) {
            case AUDIO_FAILED_EVENT:
            case VIDEO_FAILED_EVENT:
            case PREPARE_FAILED_EVENT:
            case ALLOC_FAILED_EVENT:
            case SDL_QUIT:
                this->quit = true;
                // 清理资源
                this->apQueue.Clear();
                this->vpQueue.Clear();
                this->vfQueue.Clear();
                AudioPlay::Clear();
                VideoPlay::Clear();
                SDL_CloseAudio();
                SDL_Quit();
                exit(0);
                break;
            case REFRESH_EVENT:
                VideoPlay::videoFresh(event.user.data1);//视频显示刷新事件响应函数
                break;
            case SDL_WINDOWEVENT:
                SDL_GetWindowSize(screen,&screen_w,&screen_h);
                break;
            default:
                break;
        }
    }

    return 0;
}
