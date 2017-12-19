#include <QSqlQuery>

#include "AddNewFaceWidget.h"
#include "ui_AddNewFaceWidget.h"
#include "t3_log.h"



AddNewFace::AddNewFace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewFace)
{
    ui->setupUi(this);
    _faceEngine = new FaceEngine();
    ui->_roleBox_->addItem("测试人员");
    ui->_roleBox_->addItem("工作人员");
    ui->_roleBox_->addItem("普通市民");

}

AddNewFace::~AddNewFace()
{
    delete ui;
    if(_faceEngine)
    {
        delete _faceEngine;
        _faceEngine = nullptr;
    }
}

void AddNewFace::on__pictureBtn__clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open file"),"/home",tr("Images (*.jpg)"));
    _faceimage.load(filePath);
    QImage _faceimage1;
    _faceimage1 = _faceimage.scaled(640,480,Qt::KeepAspectRatio);
    QImage showImage_ = _faceimage.scaled(140,60,Qt::KeepAspectRatio);
    ui->_pictureShowLabel_->setPixmap(QPixmap::fromImage(showImage_));
    AFR_FSDK_FACEMODEL faceModel_;
    _faceEngine->getFaceFeatureFromImage(_faceimage,&faceModel_);
    _feature.resize(faceModel_.lFeatureSize);
    memcpy(_feature.data(),faceModel_.pbFeature,faceModel_.lFeatureSize);

}

void AddNewFace::on__undoBtn__clicked()
{
    this->close();
}

void AddNewFace::on__saveBtn__clicked()
{

    QString name_ = ui->_nameLineEdit_->displayText();
    QString age_ = ui->_ageLineEdit_->displayText();
    QString role_ = ui->_roleBox_->currentText();
    T3LOG << role_;

    QSqlDatabase database_ = QSqlDatabase::database("QSQLITE");
    QSqlQuery query_(database_);

    query_.exec("create table faceInfo(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT,role TEXT,age TEXT, feature BLOB,state INTEGER, updateTime TEXT)");


    T3LOG << "faceInfo table create success";
    query_.prepare("insert into faceInfo values(NULL,?,?,?,?,?,?)");
    query_.bindValue(0,name_);
    query_.bindValue(1,role_);
    query_.bindValue(2,age_);
    query_.bindValue(3,_feature,QSql::Binary);
    QString dateTime_ = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    query_.bindValue(4,10);
    query_.bindValue(5,dateTime_);
    query_.exec();

    emit updataFaceInfo();

    this->close();

}
