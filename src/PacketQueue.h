#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include "Parameter.h"

class PacketList {
public:
    AVPacket packet;
    PacketList* next;
    PacketList(AVPacket* pkt);
};

class PacketQueue {
private:
    PacketList* head;      //指向Packet队列头的指针
    PacketList* tail;      //指向Packet队列尾的指针
    int size;              //队列的长度
    SDL_mutex *p_qlock;       //Packet队列互斥量，保护队列数据
    SDL_cond *p_qready;       //Packet队列就绪条件变量(即Packet队列被填充数据时会唤醒此信号量)
public:
    PacketQueue();
    int getSize();
    int p_enqueue(AVPacket* pkt);            //进队列
    int p_dequeue(bool* quit, AVPacket* pkt, bool block);            //出队列
    void Clear();
};

#endif // PACKETQUEUE_H
