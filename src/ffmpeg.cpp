#include "Player.h"

int main() {
    Player *player = new Player();
    player->playerInit("D:/Qt/Project/VideoPlayer/Player/resources/video/Stray Kids - 말할 수 없는 비밀 (Secret Secret).mkv", 900, 600);
    player->Play();
    qDebug()<<"SUCCESS";
}
