#include "Player.h"

SDL_mutex* AudioPlay::qlock = SDL_CreateMutex();
AVPacket* AudioPlay::pkt = (AVPacket*)av_malloc(sizeof(AVPacket));
AVFrame* AudioPlay::pFrame = av_frame_alloc();
bool AudioPlay::end = true;
double AudioPlay::audioClock = 0;

int AudioPlay::audioDecode(Player* player) {
    int dataSize = 0;
    int ret;
    while(1) {        
        if(player->quit) {
            return QUIT;
        }
        SDL_LockMutex(AudioPlay::qlock);
        while(!end) {
            ret = avcodec_receive_frame(player->apCodecCtx, AudioPlay::pFrame);
            if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                end = true;
                av_packet_unref(AudioPlay::pkt);
                break;
            }
            if (ret < 0) {
                return FAILED;
            }

            int out_samples = swr_convert(player->swr_ctx, &player->audioBuffer, MAX_AUDIO_FRAME_SIZE,
                                          (const uint8_t **)pFrame->data,pFrame->nb_samples);
            if(out_samples > 0){
                 dataSize = av_samples_get_buffer_size(NULL,player->apCodecCtx->channels ,out_samples, AV_SAMPLE_FMT_S16, 1);
            }

            SDL_UnlockMutex(AudioPlay::qlock);
            if(dataSize<=0) {
                continue;
            }
            player->audioBufferSize = dataSize;
            AudioPlay::audioClock += (double)dataSize / (double)player->sampleRate;
            return SUCCESS;
        }

        if(player->apQueue.p_dequeue(&player->quit,AudioPlay::pkt, false)==QUIT) {
            SDL_UnlockMutex(AudioPlay::qlock);
            return QUIT;
        }
        ret = avcodec_send_packet(player->apCodecCtx, AudioPlay::pkt);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            return FAILED;
        }
        end = false;
        if(AudioPlay::pkt->pts != AV_NOPTS_VALUE) {
            AudioPlay::audioClock = AudioPlay::pkt->pts * av_q2d(player->apCodecCtx->pkt_timebase);
        }
    }
}

void AudioPlay::audio_callback(void* arg, Uint8 *stream, int len) {
    Player* player = (Player*)arg;
    int length = 0;       //送入声卡的缓存的长度
    SDL_memset(stream, 0, len);
    SDL_Event event;
    while (len > 0) {       //检查音频缓存的剩余长度
        if (player->audioBufferIndex >= player->audioBufferSize) {//检查是否需要执行解码操作
            int ret = AudioPlay::audioDecode(player);
            // 从缓存队列中提取数据包、解码，并返回解码后的数据长度，audioBuffer缓存中可能包含多帧解码后的音频数据
            if (ret != SUCCESS) {             //检查解码操作是否成功
                // 出错的话，全零重置缓冲区，将0送入声卡（即沉默）
                player->audioBufferSize = 1024;
                memset(player->audioBuffer, 0, player->audioBufferSize);//全零重置缓冲区
                if(ret == FAILED) {
                    event.type = AUDIO_FAILED_EVENT;
                    SDL_PushEvent(&event);
                }
            }
            player->audioBufferIndex = 0;           //初始化累计写入缓存长度
        }

        length = player->audioBufferSize - player->audioBufferIndex;
        if (length > len) {             //检查每次写入缓存的数据长度是否超过指定长度(1024)
            length = len;               //指定长度从解码的缓存中取数据
        }
        //每次从解码的缓存数据中以指定长度抽取数据并送入声卡传递给声卡
        SDL_MixAudio(stream, (uint8_t *)player->audioBuffer + player->audioBufferIndex, length, SDL_MIX_MAXVOLUME);
        len -= length;              //更新解码音频缓存的剩余长度
        stream += length;               //更新缓存写入位置
        player->audioBufferIndex += length;         //更新累计写入缓存数据长度        
    }
}

double AudioPlay::get_audioClock(Player* player) {
    if(player->vpCodecCtx) {
        return AudioPlay::audioClock - (player->audioBufferSize - player->audioBufferIndex) / player->sampleRate;
    }
    return AudioPlay::audioClock;
}

void AudioPlay::Clear() {
    av_frame_free(&AudioPlay::pFrame);
    av_packet_unref(AudioPlay::pkt);
}
