#include <QMessageBox>
#include <QSqlError>

#include "FaceinfoDatabaseWidget.h"
#include "ui_FaceinfoDatabaseWidget.h"
#include "t3_log.h"

FaceInfoDatabaseWidget::FaceInfoDatabaseWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FaceInfoDatabaseWidget)
{
    QSqlDatabase database_ = QSqlDatabase::database("QSQLITE");
    database_.setDatabaseName("face.db");
    ui->setupUi(this);
    _model = new QSqlTableModel(this);
    _model->setTable("faceInfo");
    _model->select();
    _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->_faceInfoTableView_->setModel(_model);

}

FaceInfoDatabaseWidget::~FaceInfoDatabaseWidget()
{
    delete ui;
}

void FaceInfoDatabaseWidget::on__submitChangeBtn__clicked()
{
    _model->database().transaction();
    if(_model->submitAll())
    {
        if(_model->database().commit())
        {
            QMessageBox::information(this,tr("tableModel"),tr("数据修改成功！"));
        }
    }else
    {
        _model->database().rollback();
        QMessageBox::information(this,tr("tableModel"),
                                 tr("数据库错误：%1").arg(_model->lastError().text()),
                                 QMessageBox::Ok);
    }
}

void FaceInfoDatabaseWidget::on__undoChangeBtn__clicked()
{
    _model->revertAll();
}

void FaceInfoDatabaseWidget::on__queryBtn__clicked()
{
    QString name_ = ui->_nameEditLine_->text();
    _model->setFilter(QString("name = '%1'").arg(name_));
    _model->select();
}

void FaceInfoDatabaseWidget::on__showAllTable__clicked()
{
    _model->setTable("faceInfo");
    _model->select();
}

void FaceInfoDatabaseWidget::on__ascondingSortById__clicked()
{
    _model->setSort(0,Qt::AscendingOrder);
    _model->select();
}

void FaceInfoDatabaseWidget::on__descendingSortById__clicked()
{
    _model->setSort(0,Qt::DescendingOrder);
    _model->select();
}

void FaceInfoDatabaseWidget::on__deleteRecordBtn__clicked()
{
    int curRow_ = ui->_faceInfoTableView_->currentIndex().row();

    _model->removeRow(curRow_);

    int ok = QMessageBox::warning(this,tr("删除当前行！"),
                                  tr("你确定删除当前行吗？"),
                                  QMessageBox::Yes|
                                  QMessageBox::No);

    if(ok == QMessageBox::No)
    {
        _model->revertAll();

    }else
    {
        _model->submitAll();

    }
    _model->database().commit();
}

void FaceInfoDatabaseWidget::on_addNewRecordBtn__clicked()
{
    int rowNum = _model->rowCount();
    int id = rowNum+1;
    _model->insertRow(rowNum);
    _model->setData(_model->index(rowNum,0),id);
    _model->submitAll();

}
