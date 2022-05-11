#include "FrameQueue.h"

/*
 * 使用方法：
 *                 int r1 = vfQueue.f_enqueue(&thread_exit,pFrameYUV);
                AVFrame* yuv;
                int r2 = vfQueue.f_dequeue(&thread_exit,&yuv,false);
*/

YUVFrame::YUVFrame(AVFrame* yuv, uint8_t *buffer, double time) {
    pFrameYUV = yuv;
    out_buffer = buffer;
    pts = time;
}

FrameQueue::FrameQueue(int max) {
    max_size = max;
    frameYUV = new YUVFrame*[max_size];
    front = length = 0;
    rear = max_size - 1;
    f_qlock = SDL_CreateMutex();
    f_qready = SDL_CreateCond();
}

int FrameQueue::f_enqueue(bool* quit, AVFrame* yuv, uint8_t *buffer, double time) {
    SDL_LockMutex(f_qlock);
    while(length == max_size&&!(*quit)) {
        SDL_CondWait(f_qready, f_qlock);
    }
    if(*quit) {
        SDL_UnlockMutex(f_qlock);
        return QUIT;
    }
    rear = (rear + 1) % max_size;
    frameYUV[rear] = new YUVFrame(yuv, buffer, time);
    length++;
    SDL_UnlockMutex(f_qlock);
    return SUCCESS;
}

int FrameQueue::f_dequeue(bool* quit, YUVFrame** yuvFrame, bool block) {
    int ret;              //返回状态码
    SDL_LockMutex(f_qlock);
    while(1) {
        if(*quit) {
            ret = QUIT;
            break;
        }
        if(length) {
            *yuvFrame = frameYUV[front];
            frameYUV[front] = NULL;
            front = (front + 1) % max_size;
            length--;
            ret = SUCCESS;
            SDL_CondSignal(f_qready);
            break;
        }else if(block) {
            ret = BLOCK;
            break;
        }else {
            ret = FAILED;
            break;
        }
    }    
    SDL_UnlockMutex(f_qlock);
    return ret;
}

void FrameQueue::Clear() {
    SDL_CondBroadcast(f_qready);
    SDL_LockMutex(f_qlock);
    for(int i = front; length > 0; i = (i + 1) % max_size, length--) {
        av_frame_free(&frameYUV[i]->pFrameYUV);
        av_free(frameYUV[i]->out_buffer);
    }
    for(int i = 0; i < max_size; i++) {
        delete []frameYUV[i];
    }
    delete []frameYUV;
    SDL_UnlockMutex(f_qlock);
}
