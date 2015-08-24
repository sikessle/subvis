#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <QObject>

class IOController : public QObject
{
    Q_OBJECT
public:
    explicit IOController(QObject *parent = 0);

signals:

public slots:
};

#endif // IOCONTROLLER_H
