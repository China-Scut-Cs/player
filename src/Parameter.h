#ifndef PARAMETER_H
#define PARAMETER_H

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include <libavutil/time.h>
    #include <libavutil/imgutils.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_thread.h>
    #include <stdio.h>
}
#include <QDebug>

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)     //音频最大paket数
//#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)
#define MAX_VIDEOQ_SIZE (80)                //视频最大paket数

#define AV_SYNC_THRESHOLD 0.01//前后两帧间的显示时间间隔的最小值0.01s
#define AV_NOSYNC_THRESHOLD 10.0//最小刷新间隔时间10s

#define REFRESH_EVENT (SDL_USEREVENT)
#define PREPARE_FAILED_EVENT (SDL_USEREVENT + 1)
#define ALLOC_FAILED_EVENT (SDL_USEREVENT + 2)
#define VIDEO_FAILED_EVENT (SDL_USEREVENT + 3)
#define AUDIO_FAILED_EVENT (SDL_USEREVENT + 4)
#define PREVIEW_EVENT (SDL_USEREVENT + 5)    // 预览

#define VIDEO_FRAME_QUEUE_SIZE 10

//状态码
#define FAILED 0
#define SUCCESS 1
#define QUIT 2
#define BLOCK 3


#endif // PARAMETER_H
