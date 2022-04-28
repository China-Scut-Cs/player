#include "widget.h"
#include "ui_widget.h"
#include <QStandardPaths>
#include <QInputDialog>
#include <QTime>
#include <QPalette>
#include <QSettings>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化ui
    ui->btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));//播放
    ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));//音量喇叭
    ui->btn_pre->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));//上一首
    ui->btn_next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));//下一首
    ui->btn_stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));//停止
    ui->btn_playmodel->setIcon(QIcon("image/loop2.png"));//顺序播放
    ui->volume_slider->setRange(0,100);
    ui->volume_slider->setValue(ui->volume_slider->maximum());
    //将控件设为无焦点模式，以便响应键盘事件
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->time_label->setFocusPolicy(Qt::NoFocus);
    ui->positionslider->setFocusPolicy(Qt::NoFocus);
    ui->volume_slider->setFocusPolicy(Qt::NoFocus);
    ui->btn_fullscreen->setFocusPolicy(Qt::NoFocus);
    ui->btn_mute->setFocusPolicy(Qt::NoFocus);
    ui->btn_next->setFocusPolicy(Qt::NoFocus);
    ui->btn_open->setFocusPolicy(Qt::NoFocus);
    ui->btn_play->setFocusPolicy(Qt::NoFocus);
    ui->btn_playmodel->setFocusPolicy(Qt::NoFocus);
    ui->btn_pre->setFocusPolicy(Qt::NoFocus);
    ui->btn_stop->setFocusPolicy(Qt::NoFocus);
    init_play();
    //获取键盘事件
    this->grabKeyboard();
    init_database();
    read();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::init_play()
{
    //初始化播放器、播放列表以及播放窗口
    m_player=new QMediaPlayer(this);
    m_playlist=new QMediaPlaylist;
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_player->setPlaylist(m_playlist);
    m_videowidget=new QVideoWidget(ui->label);
    m_videowidget->resize(471,291);
    m_videowidget->setVisible(true);
    m_player->setVideoOutput(m_videowidget);
    m_videowidget->setFocusPolicy(Qt::NoFocus);
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0));
    m_videowidget->setAutoFillBackground(true);
    m_videowidget->setPalette(palette);
    //记录播放窗口的位置
    m_location = m_videowidget->geometry();


    //连接槽和信号
    connect(m_player, &QMediaPlayer::positionChanged, this, &Widget::updatePosition);//获取当前时长
    connect(m_player, &QMediaPlayer::durationChanged, this, &Widget::updateDuration);//获取总时长
    connect(m_player, &QMediaPlayer::stateChanged, this, &Widget::updatePlayBtn);//播放按钮的格式
    connect(m_player,&QMediaPlayer::currentMediaChanged,this,&Widget::updateState);//获取当前播放的文件
    connect(ui->positionslider,&QAbstractSlider::valueChanged,this,&Widget::seek);//获取滑块的位置

}

void Widget::init_database()
{
    //创建一个数据库
    QSqlDatabase db_music;
    db_music=QSqlDatabase::addDatabase("QSQLITE");
    db_music.setDatabaseName("music.db");
    if(!db_music.open())
    {
        QMessageBox::critical(this,"无法打开数据库文件：Music.db",db_music.lastError().databaseText());
        exit(-1);
    }
    //如果数据库中不存在music表，则创建一个
    QSqlQuery query;
    query=QSqlQuery(db_music);
    bool set=query.exec("select * from sys.tables where name='music';");
    if(!set)
    {
        query.exec("create table music(address varchar(100));");
    }

}



void Widget::insertsql(QString arg)
{
    //向数据库中添加信息
    QSqlQuery query;
    QString insert_sql="insert into music values(?);";
    query.prepare(insert_sql);
    query.addBindValue(arg);
    query.exec();
}

void Widget::read()
{
    //从数据库中读取信息到列表中
    QSqlQuery query;
    QString select_sql="select * from music;";
    query.prepare(select_sql);
    if(query.exec())
    {
        while(query.next())
        {
            //从列表尾部添加信息
            ui->listWidget->insertItems(ui->listWidget->count(),query.value(0).toStringList());
            //对列表中的每个文件进行url转化，并加入播放列表中
            foreach(QString const& arg,query.value(0).toStringList())
            {
                QUrl url(arg);
                m_playlist->addMedia(url);
            }
            QStringList address=query.value(0).toStringList();
            qDebug()<<address;
        }

    }


}

void Widget::isRepeat(QString arg)
{
    QSqlQuery query;
    QString insertarg="'"+arg+"'";
    //查询符合条件的数据的条数
    QString select_sql="select count(*) from music where address="+insertarg+";";
    query.prepare(select_sql);
    query.exec();
    if(query.next())
    {
        if(query.value(0).toInt()>0)
        {
            flag=true;
        }
        else
        {
            flag=false;
        }
    }

}

void Widget::on_btn_open_clicked()
{
    m_strlist.clear();
    //获取用户选中的文件，并存入m_strlist
    m_strlist=QFileDialog::getOpenFileNames(this,"打开",".","allfiles(*.*);;");
    //遍历每一个文件，如果没添加过，就加入数据库和播放列表中，否则跳出警告信息，提示用户文件已存在
    foreach(QString const& arg,m_strlist)
    {
        isRepeat(arg);
        if(!flag)
        {
            ui->listWidget->insertItem(ui->listWidget->count(),arg);
            insertsql(arg);
            QUrl url(arg);
            m_playlist->addMedia(url);
            //qDebug()<<0;
        }
        else
        {
            QMessageBox::warning(this,"Warning",arg+"already exists in the list",QMessageBox::Ok);
            //qDebug()<<1;
        }
    }
}
//上一首
void Widget::on_btn_pre_clicked()
{
    m_player->stop();
    int count=m_playlist->mediaCount();
    if(count==0)
    {
        return;
    }
    m_playlist->previous();
    m_player->play();
}

//播放控制
void Widget::on_btn_play_clicked()
{
    if(m_player->state()==QMediaPlayer::PlayingState)
    {
        m_player->pause();
    }
    else if(m_player->state()==QMediaPlayer::PausedState)
    {
        m_player->play();
    }
    //当列表不为空时，点击播放按钮自动播放第一条
    else if(!m_playlist->isEmpty()&&(m_player->state()==QMediaPlayer::StoppedState))
    {
        m_playlist->setCurrentIndex(0);
        m_player->play();
        ui->btn_stop->setEnabled(true);
    }

}
//更新播放按钮样式
void Widget::updatePlayBtn()
{
    if(m_player->state()==QMediaPlayer::PlayingState)
    {
        ui->btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else if(m_player->state()==QMediaPlayer::PausedState)
    {
        ui->btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
//下一首
void Widget::on_btn_next_clicked()
{
    m_player->stop();
    int count=m_playlist->mediaCount();
    if(count==0)
    {
        return;
    }
    m_playlist->next();
    m_player->play();
}
//设置音量
void Widget::on_volume_slider_valueChanged(int value)
{
    m_player->setVolume(value);
}
//双击列表中的文件进行播放
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int i=index.row();
    m_playlist->setCurrentIndex(i);
    m_player->play();
}
//设置静音
void Widget::on_btn_mute_clicked()
{
    if(m_player->isMuted())
    {
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        m_player->setMuted(false);
    }
    else
    {
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        m_player->setMuted(true);
    }
}
//设置播放模式（顺序播放，循环播放，随机播放）
void Widget::on_btn_playmodel_clicked()
{
    if(m_playlist->playbackMode()==QMediaPlaylist::Loop)
    {
        ui->btn_playmodel->setIcon(QIcon("image/random2.png"));
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
    else if(m_playlist->playbackMode()==QMediaPlaylist::Random)
    {
        ui->btn_playmodel->setIcon(QIcon("image/loop-one2.png"));
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else if(m_playlist->playbackMode()==QMediaPlaylist::CurrentItemInLoop)
    {
        ui->btn_playmodel->setIcon(QIcon("image/loop2.png"));
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}
//选中当前正在播放的文件
void Widget::updateState()
{
    int i=m_playlist->currentIndex();
    QListWidgetItem *item=ui->listWidget->item(i);
    item->setSelected(true);
}
//设置时间的格式
QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    qint64 minutes = seconds / 60;
    const qint64 hours=minutes/60;
    seconds -= minutes * 60;
    minutes-=hours*60;
    return QStringLiteral("%1:%2:%3")
        .arg(hours,2,10,QLatin1Char('0'))
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}
//获取当前时长并设置时间的label
void Widget::updatePosition(qint64 position)
{
    ui->positionslider->setValue(position);
    ui->time_label->setText(formatTime(position)+"/"+formatTime(m_player->duration()));
}
//获取总时长并设置滑块的步长
void Widget::updateDuration(qint64 duration)
{
    ui->positionslider->setRange(0, duration);
    ui->positionslider->setPageStep(duration / 10);
}
//停止
void Widget::on_btn_stop_clicked()
{
    if(m_player->state()==QMediaPlayer::PlayingState||m_player->state()==QMediaPlayer::PausedState)
    {
        m_player->stop();
        ui->btn_stop->setEnabled(false);
        ui->btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
//键盘事件
void Widget::keyPressEvent(QKeyEvent *event)
{
    //空格控制播放暂停
    if(event->key()==Qt::Key_Space)
    {
        if(m_player->state()==QMediaPlayer::PlayingState)
        {
            //qDebug()<<"2";
            m_player->pause();
            event->accept();
        }
        else if(m_player->state()==QMediaPlayer::PausedState)
        {
            //qDebug()<<"3";
            m_player->play();
            event->accept();
        }
    }
    else if(event->modifiers()==Qt::ControlModifier)
    {
        //control+I打开资源导入窗口
        if(event->key()==Qt::Key_I)
        {
            on_btn_open_clicked();
            event->accept();
        }
        //control+⬅上一首
        else if(event->key()==Qt::Key_Left)
        {
            on_btn_pre_clicked();
            event->accept();
        }
        //control+➡下一首
        else if(event->key()==Qt::Key_Right)
        {
            on_btn_next_clicked();
            event->accept();
        }
        //control+⬆调高音量
        else if(event->key()==Qt::Key_Up)
        {
            m_player->setVolume(m_player->volume()+5);
            ui->volume_slider->setValue(m_player->volume()+5);
            event->accept();
        }
        //control+⬇降低音量
        else if(event->key()==Qt::Key_Down)
        {
            m_player->setVolume(m_player->volume()-5);
            ui->volume_slider->setValue(m_player->volume()-5);
            event->accept();
        }
        //control+F全屏或者小屏
        else if(event->key()==Qt::Key_F)
        {
            on_btn_fullscreen_clicked();
            event->accept();
        }
    }
    //ESC键退出全屏
    else if(event->key()==Qt::Key_Escape)
    {
        if(m_videowidget->isFullScreen())
        {
            qDebug()<<"1";
            m_videowidget->setWindowFlags(Qt::SubWindow);
            m_videowidget->showNormal();
            m_videowidget->setGeometry(m_location);
            event->accept();
        }
    }

}
//滑块当前位置
void Widget::seek(int position)
{
    m_player->setPosition(position);
}
//全屏
void Widget::on_btn_fullscreen_clicked()
{
    if(m_videowidget->isFullScreen())
    {
        m_videowidget->setWindowFlags(Qt::SubWindow);
        m_videowidget->showNormal();
        m_videowidget->setGeometry(m_location);
    }
    else
    {
        m_videowidget->setWindowFlags(Qt::Window);
        m_videowidget->showFullScreen();

    }

}
//删除列表中选中的文件（从数据库和列表中同时删除）
void Widget::on_btn_delete_clicked()
{
    QString a=ui->listWidget->currentItem()->text();
    QSqlQuery query;
    QString delete_sql="delete from music where address='"+a+"'";
    //qDebug()<<delete_sql;
    query.prepare(delete_sql);
    query.exec();
    QListWidgetItem *item=ui->listWidget->currentItem();
    ui->listWidget->removeItemWidget(item);
    delete item;
}
