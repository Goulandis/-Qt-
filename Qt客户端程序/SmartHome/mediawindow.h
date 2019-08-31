#ifndef MEDIAWINDOW_H
#define MEDIAWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QJsonObject>

namespace Ui {
class mediaWindow;
}

class mediaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mediaWindow(QWidget *parent = 0);
    ~mediaWindow();
    void changeVolume(int values);//改变音量
    void updatePosition(qint64 position);//更新播放位置
    void addItem(QString playname);
    void createKeyValue(const QString playname,const QString playurl,const QString lyric);
    void playSong(QListWidgetItem *item);
    void allLoopPlay(int position);
    void signalLoopPlay(int position);
    void randomPlay(int position);
    void signalPlay(int position);

private slots:
    void on_searchPushButton_clicked();
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply *reply);

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_pausePushButton_clicked(bool checked);

    void on_slider_sliderMoved(int position);

    void on_lastPushButton_clicked();

    void on_nextPushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);
    void setFocusOn();

    void on_playProcessSlider_sliderMoved(int position);

    void getDuration(qint64 duration);

    void getPosition(qint64 position);

private:
    Ui::mediaWindow *ui;
    QNetworkAccessManager *manager1;
    QNetworkAccessManager *manager2;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QJsonObject key_value;
};

#endif // MEDIAWINDOW_H
