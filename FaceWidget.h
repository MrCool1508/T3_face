#ifndef FACEWIDGET_H
#define FACEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPainter>
#include <QtWidgets>

#include "AddNewFaceWidget.h"
#include "FaceinfoDatabaseWidget.h"
#include "Decoder.h"
#include "RecordDatabaseWidget.h"

namespace Ui {
class FaceWidget;
}

class FaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FaceWidget(QWidget *parent = 0);
    ~FaceWidget();

private slots:
    void on__newFaceBtn__clicked();

    void on__editFaceInfo__clicked();

private:
    Ui::FaceWidget *ui;
    AddNewFace *_aNewFace;
    FaceInfoDatabaseWidget *_FIDWidget;
    RecordDatabaseWidget *_RDWidget;
    QSqlDatabase _database;
    Decoder _decoder;


    int thread_exit=0;
    int thread_pause=0;
    FILE *out ;
    QImage image;
    QList <QByteArray>_list;
    quint8 _listIndex = 0;
    quint8 _listIndexOfGl = 0;
    QTcpServer *_server;
    QTcpSocket *_socket;
    quint32 _blockSize = 0;
    quint8 _sign = 0;//写数据标识位,用来发送命令,1表示更新数据库,2表示请求视频数据,3用来停止视频传输.
    int _id = 0;
    QList<int> _idList;
    QString _name ;
    QList<QString> _nameList;
    QByteArray _feature;
    QList<QByteArray> _featureList;
    quint32 _featureSize = 0;
    QByteArray _thumbnail;
    QList<QByteArray> _thumbnailList;
    quint32 _thumbnailSize = 0;
    QString _age;
    QString _role;
    QString _updateTime;
    quint8 _state = 0;//写数据状态位 表示出现次数,当次数为4的时候不再增加,表示已注册.
    quint32 _readSign = 0;//读数据标识符 ,1表示传过来的数据为帧数据,2表示传过来的数据为新注册的用户数据.
    quint32 _dot1 = 0;
    QList<quint32> _dot1List;
    quint32 _dot2 = 0;
    QList<quint32> _dot2List;
    QByteArray _frameData;
    QByteArray _frameAndFaceData;
    quint32 _personNum = 0;
    quint32 _frameSize = 0;
    bool _readStart = true;
    QByteArray _newFaceInfoData;
    quint32 _right = 0;
    QList<quint32> _rightList;
    quint32 _left = 0;
    QList<quint32> _leftList;
    quint32 _top = 0;
    QList<quint32>_topList;
    quint32 _bottom = 0;
    QList<quint32> _bottomList;
    int _widgetWidth = 640;
    int _widgetHeight = 480;
    bool _startVideo = false;
    QByteArray _buffer;
    QTimer *timer[];
    quint32 _faceID;
    QByteArray _faceData;
    QString _dateTime;




    void initDataBase();
    void readFrameData();
    void readNewFaceInfo();
    void readRecord();

private slots:
    void getSocket();
    void printFrame(QImage image);
    void savebuffer();
    void readMessage();
    void getVideo();
    void updateClientDataBase();
    void on__getVideo__clicked();

    void on__checkRecord__clicked();

signals:
    void updataVideo();
    void bufferReady();
    void socketConnection();
};

#endif // FACEWIDGET_H
