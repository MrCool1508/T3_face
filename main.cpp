#include "FaceWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FaceWidget w;
    w.show();

    return a.exec();
}
