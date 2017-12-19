#ifndef T3_LOG_H
#define T3_LOG_H

#include <QDebug>
#include <string.h>
#include <stdio.h>


#define T3LOG qDebug() << "["  << __FILE__ << ":" << __LINE__ << ":"<< __TIME__ "]"

#endif // T3_LOG_H


