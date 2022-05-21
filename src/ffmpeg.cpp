#include "Player.h"

int main() {
    Player *player = new Player();
    //player->playerInit("C:/Users/19243/Desktop/Player/resources/music/太妍 - Weekend (周末).flac", 900, 600);
    player->playerInit((char *)"C:/Users/19243/Desktop/player1/resources/video/Stray Kids - 말할 수 없는 비밀 (Secret Secret).mkv", 900, 600);
    player->Play();
    qDebug()<<"SUCCESS";

}

//int main() {
//    MediaInfo* temp = Player::getMediaInfo((char *)"C:/Users/19243/Desktop/player1/resources/video/Stray Kids - 말할 수 없는 비밀 (Secret Secret).mkv");
//    qDebug() << temp->aBitRate;
//    qDebug() << temp->aFormat;
//    qDebug() << temp->chanelSize;
//    qDebug() << temp->vBitRate;
//    qDebug() << temp->vFormat;
//    qDebug() << temp->vFrameRate;
//    qDebug() << temp->width;
//    qDebug() << temp->height;
//    qDebug() << temp->dpi;
//    qDebug() << temp->filename;
//}



