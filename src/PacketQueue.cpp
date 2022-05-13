#include "PacketQueue.h"

//PacketList::PacketList(AVPacket* pkt) {
//    av_packet_ref(packet, pkt);
//    next = NULL;
//}

PacketQueue::PacketQueue() {
    head = tail = NULL;
    size = 0;
    p_qlock = SDL_CreateMutex();     //创建互斥量对象
    p_qready = SDL_CreateCond();     //创建条件变量对象
}

int PacketQueue::getSize() {
    return size;
}

int PacketQueue::p_enqueue(AVPacket* pkt) {
    PacketList* pkl = (PacketList*)av_malloc(sizeof(PacketList));
    pkl->packet = *pkt;             //将输入数据包赋值给新建链表节点对象中的数据包对象
    pkl->next = NULL;                   //链表后继指针为空
    SDL_LockMutex(p_qlock);
    if(!size) {
        head = tail = pkl;
    } else {
        tail->next = pkl;
        tail = tail->next;
    }
    size++;
    SDL_CondSignal(p_qready);
    SDL_UnlockMutex(p_qlock);
    return SUCCESS;
}

//注意使用条件，因为pkt是指针，但在此函数内其指向的值要被修改，所以要事先为pkt分配内存空间，即调用该函数前pkt = (AVPacket*)av_malloc(sizeof(AVPacket));
int PacketQueue::p_dequeue(bool* quit,AVPacket* pkt,bool block) {
    int ret;              //返回状态码
    PacketList* pkl;
    SDL_LockMutex(p_qlock);
    while(1) {
        if(*quit) {
            ret = QUIT;
            break;
        }
        if(size) {
            pkl = head;
            *pkt = head->packet;
            if(size==1) {
                head = tail = NULL;
            }else {
                head = head->next;
            }            
            size--;
            av_free(pkl);
            ret = SUCCESS;
            break;
        }else if(block) {
            ret = BLOCK;
            break;
        }else {
            SDL_CondWait(p_qready, p_qlock);        //队列中没有Packet，发出信号等待新Packet到来
        }
    }
    SDL_UnlockMutex(p_qlock);
    return ret;
}

void PacketQueue::Clear() {
    SDL_CondBroadcast(p_qready);
    SDL_LockMutex(p_qlock);
    PacketList* pkl = head;
    while(size--) {
        head = head->next;
        av_free(pkl);
        pkl = head;
    }
    head = tail = pkl = NULL;
    delete head;
    delete tail;
    delete pkl;
    SDL_UnlockMutex(p_qlock);
}
