#ifndef FACEINFODATABASEWIDGET_H
#define FACEINFODATABASEWIDGET_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlDatabase>

namespace Ui {
class FaceInfoDatabaseWidget;
}
class QSqlTableModel;

class FaceInfoDatabaseWidget : public QDialog
{
    Q_OBJECT

public:
    explicit FaceInfoDatabaseWidget(QWidget *parent = 0);
    ~FaceInfoDatabaseWidget();

private slots:
    void on__submitChangeBtn__clicked();

    void on__undoChangeBtn__clicked();

    void on__queryBtn__clicked();

    void on__showAllTable__clicked();

    void on__ascondingSortById__clicked();

    void on__descendingSortById__clicked();

    void on__deleteRecordBtn__clicked();

    void on_addNewRecordBtn__clicked();

private:
    Ui::FaceInfoDatabaseWidget *ui;
    QSqlTableModel *_model;
};

#endif // FACEINFODATABASEWIDGET_H
