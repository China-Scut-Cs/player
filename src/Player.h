#ifndef PLAYER_H
#define PLAYER_H

#include "Parameter.h"
#include "PacketQueue.h"
#include "FrameQueue.h"

struct MediaInfo{
    QString filename;   //文件名

    int aBitRate;    //码率    b/s
    const char* aFormat;     //编码格式  avc
    int chanelSize; //声道数

    int vBitRate;   //码率   b/s
    const char* vFormat;    //编码格式
    double vFrameRate;      //帧率    FPS
    int width;      //宽度
    int height;     //高度
    QString dpi;        //分辨率


    QString albumInfo; //专辑信息
    QString artistInfo; //演唱者信息

};

class Player {
public:
    char* filename;                      //文件路径
    AVFormatContext *pFormatCtx;        //保存文件容器封装信息及码流参数的结构体
    int videoIndex, audioIndex;         //视频和音频的位置索引
    AVIOContext *io_context;
    bool quit;                           //退出状态码

    //有关音频的成员变量
    struct SwrContext* swr_ctx;          //音频重采样结构体
    AVCodecContext* apCodecCtx;          //音频编码器上下文对象,用于PPM文件输出
    PacketQueue apQueue;                //音频包队列
    int sampleRate;                      //将采样率转化为每秒多少字节
    uint8_t* audioBuffer;               //缓存解码后得到的帧
    int audioBufferSize;                    //audioBuffer被填充的长度
    int audioBufferIndex;                  //audioBuffer中的数据累计被送入声卡的长度
    int volume;                         //音量

    //有关视频的成员变量
    struct SwsContext *sws_ctx;         //描述转换器参数的结构体
    AVCodecContext* vpCodecCtx = NULL;       //视频编码器上下文对象,用于PPM文件输出
    PacketQueue vpQueue;                //视频包队列
    FrameQueue vfQueue;                 //视频帧队列
    double aspect_ratio;                //像素宽高比
    int preview_time;                    //预览时间
    int now_time;                        //现在的时间
    int jump_time;                       //跳转的时间


    //同步用变量
    double frame_timer;             //视频播放到当前帧时的累计已播放时间
    double last_frame_pts;          //上一帧开始播放的相对时间
    double last_frame_delay;            //上两帧开始播放时间的时间差，若上一帧和当前要播放的帧的开始播放时间差异常时，将沿用上两帧开始播放时间差



    //播放窗口成员变量
    SDL_Window *screen;                 //窗口
    SDL_Renderer* sdlRenderer;         //渲染器
    SDL_Texture* sdlTexture;            //纹理
    SDL_Rect sdlRect;                   //绘制矩形框
    int screen_w, screen_h;             //播放窗口的宽高



    //有关线程的成员变量
    SDL_Thread* StartPlay;                 //sartPlay的线程，负责初始化各种变量，以及对文件进行解封装，将packet放到相应的队列中
    SDL_Thread* VideoDecode;               //videoDecode的线程，负责解码视频
    SDL_Thread* Preview;


    //有关跳转的成员变量
    int seq_req;
    int seq_flag;
    std::int64_t seq_pos;
    AVPacket *flush_pkt;

    //倍速有关的变量，前端传来
    int speed;                          //倍速
    //jump_time                         //跳转时间
    //preview_time                      //预览时间
    bool stopPlay;                     //是否停止

    Player();
    ~Player();
    int playerInit(char* filepath, int sw, int sh);
    static int stream_component_open(Player* player, int streamIndex);
    static int startPlay(void *arg);
    int Play();

    void receive_jump_time(int time);
    void receive_preview_time(int time);
    void receive_speed(int spd);
    static MediaInfo* getMediaInfo(char* fileName);
};

class VideoPlay {
public:
    static SDL_mutex* qlock;
    static AVPacket* pkt;
    static AVFrame *pFrame;
    static double videoClock;                   //记录播放完队列尾的Frame的时间（相对时间），当下一个入队列的Frame的pts异常时（即不知道何时播放该Frame），可以用此变量补正
    static double synchronize_video(double timebase, AVFrame* pFrame);
    static int videoDecode(void* arg);        //可启用多个线程进行视频解码
    static int videoDisplay(Player* player, YUVFrame* yuvFrame);
    static Uint32 videoFreshCallback(Uint32 interval, void *opaque);              //回调函数，当videoFresh设定的计时器触发时，会调用该函数，向主函数发出刷新信号
    static void schedule_refresh(Player* player, int delay);
    static void videoFresh(void* arg);             //刷新图像并计算下一帧图像的显示时间，设定计时器
    static void Clear();
    static void vedioPreview(void* arg);    //预览线程函数
};

class AudioPlay {
public:
    static SDL_mutex* qlock;
    static AVPacket* pkt;               //存储正在解码的packet
    static AVFrame *pFrame;             //存储解码出来的帧
    static bool end;                    //标记正在解码的packet是否已经全部解码完毕
    static double audioClock;                 //记录播放完audioBuffer里的帧（audioBuffer里只有一帧）的时间（相对时间）
    static int frameNum;
    static int audioDecode(Player* player);        //由音频回调函数调用
    static void audio_callback(void* arg, Uint8 *stream, int len);                //音频回调函数
    static double get_audioClock(Player* player);
    static void Clear();
};

class Jump{
public:
    static void doSeek(double increase,Player* player);
};

#endif // PLAYER_H
