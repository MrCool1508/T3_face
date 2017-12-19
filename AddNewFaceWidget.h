#ifndef ADDNEWFACE_H
#define ADDNEWFACE_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QSqlDatabase>
#include <QDateTime>
#include "FaceEngine.h"

namespace Ui {
class AddNewFace;
}

class AddNewFace : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewFace(QWidget *parent = 0);
    ~AddNewFace();




private slots:
    void on__pictureBtn__clicked();

    void on__undoBtn__clicked();

    void on__saveBtn__clicked();

private:
    Ui::AddNewFace *ui;
    QImage _faceimage;
    FaceEngine *_faceEngine;
    QByteArray _feature;

signals:
    void updataFaceInfo();

};

#endif // ADDNEWFACE_H
