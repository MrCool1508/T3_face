#include <QMessageBox>
#include <QSqlError>

#include "RecordDatabaseWidget.h"
#include "ui_RecordDatabaseWidget.h"
#include "t3_log.h"

RecordDatabaseWidget::RecordDatabaseWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDatabaseWidget)
{
    QSqlDatabase database_ = QSqlDatabase::database("QSQLITE");
    database_.setDatabaseName("face.db");
    ui->setupUi(this);
    _model = new QSqlTableModel(this);
    _model->setTable("detail");
    _model->select();
    _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->_faceInfoTableView_->setModel(_model);

}

RecordDatabaseWidget::~RecordDatabaseWidget()
{
    delete ui;
}

void RecordDatabaseWidget::on__submitChangeBtn__clicked()
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

void RecordDatabaseWidget::on__undoChangeBtn__clicked()
{
    _model->revertAll();
}

void RecordDatabaseWidget::on__queryBtn__clicked()
{
    QString name_ = ui->_nameEditLine_->text();
    _model->setFilter(QString("name = '%1'").arg(name_));
    _model->select();
}

void RecordDatabaseWidget::on__showAllTable__clicked()
{
    _model->setTable("faceInfo");
    _model->select();
}

void RecordDatabaseWidget::on__ascondingSortById__clicked()
{
    _model->setSort(0,Qt::AscendingOrder);
    _model->select();
}

void RecordDatabaseWidget::on__descendingSortById__clicked()
{
    _model->setSort(0,Qt::DescendingOrder);
    _model->select();
}

void RecordDatabaseWidget::on__deleteRecordBtn__clicked()
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

void RecordDatabaseWidget::on_addNewRecordBtn__clicked()
{
    int rowNum = _model->rowCount();
    int id = rowNum+1;
    _model->insertRow(rowNum);
    _model->setData(_model->index(rowNum,0),id);
    _model->submitAll();

}
