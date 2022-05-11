#include "Player.h"

SDL_mutex* VideoPlay::qlock = SDL_CreateMutex();
AVPacket* VideoPlay::pkt = (AVPacket*)av_malloc(sizeof(AVPacket));
AVFrame* VideoPlay::pFrame = av_frame_alloc();
double VideoPlay::videoClock = 0;

double VideoPlay::synchronize_video(double timebase, AVFrame* pFrame) {
    double pts = pFrame->pts * timebase;           //timebase单位为ms
    if(pts > 0) {
        VideoPlay::videoClock = pts;
    }else {
        pts = VideoPlay::videoClock;
    }
    double frame_delay = timebase + pFrame->repeat_pict * timebase * 0.5;    //timebase为一帧显示完所用时间，pFrame->repeat_pict * timebase * 0.5为渲染重复帧所需时间
    VideoPlay::videoClock += frame_delay;      //该帧（包括重复帧）播放完的相对时间
    return pts;   //该帧开始播放的相对时间
}

int VideoPlay::videoDecode(void* arg) {
    Player* player = (Player*)arg;
    SDL_Event event;

    while(1) {
        if(player->quit) {
            break;
        }
        AVFrame *pFrameYUV = NULL;
        for(int i = 10; i > 0; i--) {
            pFrameYUV = av_frame_alloc();                           //保存输出24-bit RGB的PPM文件数据
            if (pFrameYUV == NULL) {              //检查初始化操作是否成功
                SDL_Delay(5);
                continue;
            }
            break;
        }
        if (pFrameYUV == NULL) {
            event.type = ALLOC_FAILED_EVENT;
            SDL_PushEvent(&event);
            break;
        }

        // 解码数据输出缓存指针
        // 根据像素格式及图像尺寸计算内存大小
        uint8_t *out_buffer = NULL;
        for(int i = 10; i > 0; i--) {
            out_buffer = (uint8_t *)av_malloc(
                        av_image_get_buffer_size(AV_PIX_FMT_YUV420P, player->vpCodecCtx->width, player->vpCodecCtx->height, 1));
            if (out_buffer == NULL) {              //检查初始化操作是否成功
                SDL_Delay(5);
                continue;
            }
            break;
        }
        if (out_buffer == NULL) {
            av_frame_free(&pFrameYUV);
            event.type = ALLOC_FAILED_EVENT;
            SDL_PushEvent(&event);
            break;
        }

        // 为AVFrame对象安装图像缓存，将out_buffer缓存挂到pFrameYUV->data指针结构上
        av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P,
                             player->vpCodecCtx->width, player->vpCodecCtx->height, 1);

        SDL_LockMutex(VideoPlay::qlock);
        if(player->vpQueue.p_dequeue(&player->quit, VideoPlay::pkt, false)==QUIT) {
            av_free(out_buffer);
            av_frame_free(&pFrameYUV);
            SDL_UnlockMutex(VideoPlay::qlock);
            break;
        }

        int ret = avcodec_send_packet(player->vpCodecCtx, VideoPlay::pkt);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            av_free(out_buffer);
            av_frame_free(&pFrameYUV);
            event.type = VIDEO_FAILED_EVENT;
            SDL_PushEvent(&event);
            break;
        }

        ret = avcodec_receive_frame(player->vpCodecCtx, VideoPlay::pFrame);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            av_free(out_buffer);
            av_frame_free(&pFrameYUV);
            event.type = VIDEO_FAILED_EVENT;
            SDL_PushEvent(&event);
            break;
        }

        double pts = VideoPlay::synchronize_video(av_q2d(player->vpCodecCtx->pkt_timebase), VideoPlay::pFrame);
        sws_scale(player->sws_ctx, (const uint8_t* const*)VideoPlay::pFrame->data, VideoPlay::pFrame->linesize, 0, player->vpCodecCtx->height,
                                  pFrameYUV->data, pFrameYUV->linesize);

        if(player->vfQueue.f_enqueue(&player->quit, pFrameYUV, out_buffer, pts)==QUIT) {
            av_free(out_buffer);
            av_frame_free(&pFrameYUV);
            SDL_UnlockMutex(VideoPlay::qlock);
            break;
        }

        av_packet_unref(VideoPlay::pkt);
        SDL_UnlockMutex(VideoPlay::qlock);
    }
    return QUIT;
}

int VideoPlay::videoDisplay(Player* player,YUVFrame* yuvFrame) {
    SDL_UpdateTexture(player->sdlTexture, NULL, yuvFrame->pFrameYUV->data[0], player->vpCodecCtx->width);
    if(player->aspect_ratio<=1) {
        int width = player->screen_h * player->aspect_ratio;
        player->sdlRect.w = width < player->screen_w ? width : player->screen_w;
        player->sdlRect.h = player->screen_h;
    }else {
        int height = player->screen_w / player->aspect_ratio;
        player->sdlRect.w = player->screen_w;
        player->sdlRect.h = height < player->screen_h ? height : player->screen_h;
    }

    player->sdlRect.x = (player->screen_w - player->sdlRect.w) / 2;
    player->sdlRect.y = (player->screen_h - player->sdlRect.h) / 2;

    SDL_RenderClear(player->sdlRenderer);
    SDL_RenderCopy(player->sdlRenderer, player->sdlTexture, NULL, &player->sdlRect);
    SDL_RenderPresent(player->sdlRenderer);
    av_free(yuvFrame->out_buffer);
    av_frame_free(&yuvFrame->pFrameYUV);
    delete yuvFrame;
    return SUCCESS;
}

Uint32 VideoPlay::videoFreshCallback(Uint32 interval, void *opaque) {
    SDL_Event event;//SDL事件对象
    event.type = REFRESH_EVENT;//视频显示刷新事件
    event.user.data1 = opaque;//传递用户数据
    SDL_PushEvent(&event);//发送事件
    return 0; // 0 means stop timer
}

void VideoPlay::schedule_refresh(Player* player, int delay) {
    SDL_AddTimer(delay, VideoPlay::videoFreshCallback, player);      //在指定的时间(ms)后回调用户指定的函数
}

void VideoPlay::videoFresh(void* arg) {
    Player* player = (Player*)arg;
    if(player->vpCodecCtx) {         //判断文件是否已加载，避免想要触发刷新图像事件时由于文件还未加载而出错
        double delay;          //当前帧与上一帧的延时
        double diff;            //当前帧与当前播放的音频间的时间差
        YUVFrame* yuvFrame;
        int ret = player->vfQueue.f_dequeue(&player->quit, &yuvFrame, false);
        if(ret==QUIT) {
            return;
        }
        if(ret == FAILED) {
            schedule_refresh(player, 1);
            return;
        }
//        delay = yuvFrame->pts - player->last_frame_pts;
//        if(delay<=0 || delay>= 1000) {
//            delay = player->last_frame_delay;
//        }
//        player->last_frame_pts = yuvFrame->pts;
//        player->last_frame_delay = delay;

//        diff = yuvFrame->pts - AudioPlay::get_audioClock(player);
//        if(fabs(diff) <= AV_NOSYNC_THRESHOLD) {
//            if(diff < 0) {
//                delay -= diff;
//            }
//            if(diff > 0) {
//                delay += diff;
//            }
//        }
//        if(delay < AV_SYNC_THRESHOLD) {
//            delay = AV_SYNC_THRESHOLD;
//        }

        //SDL_AddTimer(delay, VideoPlay::videoFreshCallback, NULL);
        VideoPlay::schedule_refresh(player,40);
        //videoDecode(player);
        videoDisplay(player, yuvFrame);       //刷新当前图像，Show the picture
    }else {
        VideoPlay::schedule_refresh(player,100);
    }
}

void VideoPlay::Clear() {
    SDL_LockMutex(VideoPlay::qlock);
    av_packet_unref(VideoPlay::pkt);
    av_frame_free(&VideoPlay::pFrame);
    SDL_UnlockMutex(VideoPlay::qlock);
}
