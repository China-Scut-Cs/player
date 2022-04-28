#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QString>
#include <QFileDialog>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    //初始化播放器和播放列表以及播放窗口
    void init_play();
    //初始化数据库
    void init_database();
    //向数据库里添加i信息
    void insertsql(QString arg);
    //从数据库中读取信息到listwidget中
    void read();
    //获取当前时长
    void updatePosition(qint64 m_duration);
    //获取总时长
    void updateDuration(qint64 m_position);
    //更新播放按钮的风格（播放与暂停）
    void updatePlayBtn();
    //获取滑块当前位置
    void seek(int position);
    //选中列表中正在播放的音频
    void updateState();
    //void updateTime(qint64 position);
    //向数据库添加数据时，判断数据是否重复添加
    void isRepeat(QString arg);

private slots:
    void on_btn_open_clicked();

    void on_btn_pre_clicked();

    void on_btn_play_clicked();

    void on_btn_next_clicked();

    void on_volume_slider_valueChanged(int value);

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_btn_mute_clicked();

    void on_btn_playmodel_clicked();

    void on_btn_stop_clicked();


    void on_btn_fullscreen_clicked();



    void on_btn_delete_clicked();

protected:
    //键盘事件
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Widget *ui;
    QStringList m_strlist;
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
    QVideoWidget *m_videowidget;
    QString address;
    bool flag=true;
    QSqlDatabase db_music;
    QRect m_location;
};
#endif // WIDGET_H
