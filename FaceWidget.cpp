#include "FaceWidget.h"
#include "ui_FaceWidget.h"
#include "t3_log.h"

FaceWidget::FaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceWidget)
{
    this->showFullScreen();
    int height = QApplication::desktop()->height();
    int width = QApplication::desktop()->width();
    T3LOG << height;
    T3LOG << width;
    QImage bgImage;
    bgImage.load("bg.jpg");
    this->setAutoFillBackground(true);
    QPalette pal(palette());
    pal.setBrush(QPalette::Window,QBrush(bgImage.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    setPalette(pal);
    ui->setupUi(this);
    //ui->_video_->setGeometry(0,0,640,480);
    ui->_getVideo_->setDisabled(true);
    ui->_newFaceBtn_->setDisabled(true);
    ui->_video_->show();
    ui->_checkRecord_->show();
    ui->_editFaceInfo_->show();
    ui->_newFaceBtn_->show();
    ui->_getVideo_->show();
    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName("face.db");
    if(!_database.open())
    {
        T3LOG << "database open faile";
    }
    _aNewFace = new AddNewFace(this);
    _decoder.initDecoder();
    _server = new QTcpServer(this);
    _server->listen(QHostAddress::Any,6666);
    connect(_server,&QTcpServer::newConnection,this,&FaceWidget::getSocket);
    connect(&_decoder,&Decoder::newFrame,this,&FaceWidget::printFrame);
    connect(_aNewFace,&AddNewFace::updataFaceInfo,this,&FaceWidget::updateClientDataBase);
    T3LOG << QApplication::desktop()->width();
    T3LOG << QApplication::desktop()->height();

}

FaceWidget::~FaceWidget()
{
    delete ui;
}

void FaceWidget::on__newFaceBtn__clicked()
{

    _aNewFace->show();

}

void FaceWidget::on__editFaceInfo__clicked()
{
    _FIDWidget = new FaceInfoDatabaseWidget(this);
    _FIDWidget->show();
}
void FaceWidget::printFrame(QImage image)
{
    image = image.mirrored(true,false);
    QPainter paint(&image);
    QPen pen(Qt::yellow,1);
    paint.setPen(pen);
   for(int i = 0; i<_personNum; i++)
   {

       paint.drawLine(QPoint(_widgetWidth-_rightList[i],_topList[i]),QPoint(_widgetWidth-_leftList[i],_topList[i]));
       paint.drawLine(QPoint(_widgetWidth-_rightList[i],_bottomList[i]),QPoint(_widgetWidth-_leftList[i],_bottomList[i]));
       paint.drawLine(QPoint(_widgetWidth-_rightList[i],_topList[i]),QPoint(_widgetWidth-_rightList[i],_bottomList[i]));
       paint.drawLine(QPoint(_widgetWidth-_leftList[i],_topList[i]),QPoint(_widgetWidth-_leftList[i],_bottomList[i]));
       _database = QSqlDatabase::database();
       _database.setDatabaseName("face.db");
       QSqlQuery query_(_database);
       query_.prepare("select * from faceInfo where id = ?");
       query_.bindValue(0,_idList[i]);
       T3LOG << _idList[i];
       query_.exec();
       query_.next();
       QString name_ = query_.value(1).toString();
        //QString name_ = "";
       paint.drawText(QPointF(_widgetWidth-1-_dot2List[i],_dot1List[i]),name_);
   }

    paint.end();
    ui->_video_->setPixmap(QPixmap::fromImage(image));
}

void FaceWidget::getSocket()
{
    _socket = _server->nextPendingConnection();
    connect(_socket,&QTcpSocket::readyRead,this,&FaceWidget::savebuffer);
    connect(this,&FaceWidget::bufferReady,this,&FaceWidget::readMessage);
    ui->_getVideo_->setDisabled(false);
    ui->_newFaceBtn_->setDisabled(false);
    emit socketConnection();
}

void FaceWidget::savebuffer()
{
    _buffer.append(_socket->readAll());
    emit bufferReady();
}

void FaceWidget::readMessage()
{
    QDataStream _stream(&_buffer,QIODevice::ReadOnly) ;
    _stream.setVersion(QDataStream::Qt_5_7);
    if(0 == _blockSize)
    {
        _stream >> _blockSize;
//        T3LOG << _blockSize;
        _stream >> _readSign;

        if(1 == _readSign)
        {
            readFrameData();

        }
        if(2 == _readSign)
        {
            readNewFaceInfo();

        }
        if(3 == _readSign)
        {
            readRecord();

        }

    }else
    {
        if(1 == _readSign)
        {
            readFrameData();

        }
        if(2 == _readSign)
        {
            readNewFaceInfo();

        }
        if(3 == _readSign)
        {
            readRecord();

        }
    }
}

void FaceWidget::readFrameData()
{



    QByteArray buf;
    quint32 lenght = _buffer.size();
    if(_buffer.size() <_blockSize)
    {
        return ;
    }
    if(_buffer.size() >= _blockSize)
    {
        QDataStream stream_(&_buffer,QIODevice::ReadOnly);
        //stream_.device()->seek(sizeof(quint32)*2);

        quint32 a;
        stream_  >> a;
        stream_  >> a;


        stream_ >> _personNum;
        T3LOG << _personNum;
        for(int i =0 ; i<_personNum; i++)
        {

            stream_ >> _id;
            stream_ >> _left;
            stream_ >> _right;
            stream_ >> _top;
            stream_ >> _bottom;
            stream_ >> _dot1;
            stream_ >> _dot2;
            T3LOG << _id;
            //T3LOG << _dot2;
            if(_idList.size() <= _personNum)
            {
                _idList << _id;
                _leftList << _left;
                _rightList << _right;
                _topList << _top;
                _bottomList << _bottom;
                _dot1List << _dot1;
                _dot2List << _dot2;
            }

            _idList.replace(i,_id);
            _leftList.replace(i,_left);
            _rightList.replace(i,_right);
            _bottomList.replace(i,_bottom);
            _topList.replace(i,_top);
            _dot1List.replace(i,_dot1);
            _dot2List.replace(i,_dot2);





        }
        stream_ >> _frameData;

        _decoder.decoderFrame(_frameData.data(),_frameData.size());
        buf = _buffer.right(lenght - _blockSize);
        _buffer = buf;
        _blockSize = 0;
        _readSign = 0;

    }

}

void FaceWidget::readNewFaceInfo()
{

    QByteArray buf;
    quint32 lenght = _buffer.size();
    if(_buffer.size() <_blockSize)
    {
        return ;
    }
    if(_buffer.size() >= _blockSize)
    {
        QDataStream stream_(&_buffer,QIODevice::ReadOnly);
        stream_.setVersion(QDataStream::Qt_5_7);
        quint32 a;
        stream_  >> a;
        stream_  >> a;
        stream_ >> _id;
        stream_ >> _name;
        stream_ >> _feature;
        //stream_ >> _thumbnail;
        //stream_ >> _state;
        T3LOG << _name;
        _database = QSqlDatabase::database();
        _database.setDatabaseName("face.db");
        QSqlQuery query_(_database);

        query_.prepare("insert into faceInfo values(NULL,?,?,?,?,?,?)");
        query_.bindValue(0,_name);
        query_.bindValue(1,"");
        query_.bindValue(2,"");
        query_.bindValue(3,_feature,QSql::Binary);
        QString dateTime_ = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        query_.bindValue(4,0);
        query_.bindValue(5,dateTime_);
        query_.exec();

        T3LOG << "getNewFace";
        T3LOG << _blockSize;
        buf = _buffer.right(lenght - _blockSize);
        _buffer = buf;
        _blockSize = 0;
        _readSign = 0;
    }

}

void FaceWidget::readRecord()
{
    QByteArray buf;
    quint32 lenght = _buffer.size();
    if(_buffer.size() <_blockSize)
    {
        return ;
    }
    if(_buffer.size() >= _blockSize)
    {
        QDataStream stream_(&_buffer,QIODevice::ReadOnly);
        stream_.setVersion(QDataStream::Qt_5_7);
        quint32 a;
        stream_  >> a;
        stream_  >> a;
        stream_ >> _faceID;
        stream_ >> _dateTime;
        stream_ >> _faceData;
        //stream_ >> _thumbnail;
        //stream_ >> _state;
        T3LOG << _faceID;
        _database = QSqlDatabase::database();
        _database.setDatabaseName("face.db");
        QSqlQuery query_(_database);
        query_.exec("create table detail(id INTEGER PRIMARY KEY AUTOINCREMENT ,faceID INTEGER , updateTime TEXT ,Image BLOB)");


        query_.prepare("insert into detail values(NULL,?,?,?)");
        query_.bindValue(0,_faceID);
        query_.bindValue(1,_dateTime);
        query_.bindValue(2,_faceData,QSql::Binary);
        query_.exec();
        T3LOG << _blockSize;
        buf = _buffer.right(lenght - _blockSize);
        _buffer = buf;
        _blockSize = 0;
        _readSign = 0;
    }
}

void FaceWidget::getVideo()
{

    if(!_startVideo)
    {
        _startVideo = true;
        _sign = 2;
    }else
    {
        _startVideo = false;
        _sign = 3;
    }
    T3LOG << "getVideo";

    QByteArray block_;
    QDataStream stream_(&block_,QIODevice::WriteOnly);
    stream_.setVersion(QDataStream::Qt_5_7);

    stream_ << (quint32)0;
    stream_ << (quint32)_sign;

    _socket->write(block_);


}

void FaceWidget::on__getVideo__clicked()
{
    getVideo();
}

void FaceWidget::updateClientDataBase()
{
    _sign = 1;
    quint32 databaseCount_ = 0;
    QByteArray block_ ;
    QDataStream stream_(&block_,QIODevice::WriteOnly);
    stream_.setVersion(QDataStream::Qt_5_7);

    _database = QSqlDatabase::database();
    _database.setDatabaseName("face.db");
    QSqlQuery query_(_database);
    query_.exec("select * from faceInfo");
    while (query_.next())
    {
        databaseCount_ = query_.value(0).toInt();
    }

    T3LOG << databaseCount_;


    stream_ << (quint32) 0;
    stream_ << (quint32)_sign ;
    stream_ << (quint32) 1;
    //stream_ << databaseCount_;
    //query_.exec("select * from faceInfo ");
    query_.prepare("select * from faceinfo where id = ?");
    query_.bindValue(0,databaseCount_);
    query_.exec();
    while(query_.next())
    {
        _id = query_.value(0).toInt();
        _name = query_.value(1).toString();
        _role = query_.value(2).toString();
        _age = query_.value(3).toString();
        _feature = query_.value(4).toByteArray();

        _state = query_.value(5).toInt();
        _updateTime = query_.value(6).toString();

        T3LOG << _id;
        T3LOG << _name;
        stream_ << _id;
        stream_ << _name;
        stream_ << _role;
        stream_ << _age;
        stream_ << _feature;
        stream_ << _state;
        stream_ << _updateTime;
        T3LOG << _updateTime;
        T3LOG << _name;

    }
    stream_.device()->seek(0);
    stream_ << (quint32) (block_.size()-sizeof(quint32)*2);
    T3LOG << block_.size();
    _socket->write(block_);

}

void FaceWidget::on__checkRecord__clicked()
{
    _RDWidget = new RecordDatabaseWidget();
    _RDWidget->show();
}
