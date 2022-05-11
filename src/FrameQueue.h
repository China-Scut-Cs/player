#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H

#include "Parameter.h"

class YUVFrame {
public:
    AVFrame *pFrameYUV;
    uint8_t *out_buffer;
    double pts;             //该帧开始播放的相对时间
    YUVFrame(AVFrame* yuv, uint8_t *buffer, double time);
};

class FrameQueue {
    YUVFrame **frameYUV;      //采用环形队列
    int front;
    int rear;
    int max_size;                 //队列大小
    int length;                 //队列的长度
    SDL_mutex *f_qlock;
    SDL_cond *f_qready;         //Frame队列就绪条件变量(即Frame队列可被填充数据时会唤醒此信号量)
public:
    FrameQueue(int max);
    int f_enqueue(bool* quit, AVFrame* yuv, uint8_t *buffer, double time);            //进队列
    int f_dequeue(bool* quit, YUVFrame** yuv, bool block);            //出队列
    void Clear();
};

#endif // FRAMEQUEUE_H
