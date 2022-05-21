#include"Player.h"

void Jump::doSeek(double increase,Player* player){
    double pos= AudioPlay::get_audioClock(player);//单位s
    pos+=increase;
    //qDebug()<<"pos="<<pos;
    pos=static_cast<int64_t>(pos*AV_TIME_BASE);   //s->us
    if(!player->seq_req){
        player->seq_flag = increase < 0 ? AVSEEK_FLAG_BACKWARD : 0;//确定[快进]还是[后退]操作
        player->seq_pos=pos;
        //qDebug()<<"increase="<<increase<<"  pos="<<pos;
        player->seq_req = 1;//开启[快进]/[后退]标志位
        //qDebug()<<"已开启标志位\n";
    }
}
