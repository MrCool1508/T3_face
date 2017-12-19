#ifndef RECORDDATABASEWIDGET_H
#define RECORDDATABASEWIDGET_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlDatabase>

namespace Ui {
class RecordDatabaseWidget;
}
class QSqlTableModel;

class RecordDatabaseWidget : public QDialog
{
    Q_OBJECT

public:
    explicit RecordDatabaseWidget(QWidget *parent = 0);
    ~RecordDatabaseWidget();

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
    Ui::RecordDatabaseWidget *ui;
    QSqlTableModel *_model;
};

#endif // RECORDDATABASEWIDGET_H
