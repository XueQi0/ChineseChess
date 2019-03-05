#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);

    int mSelectId;
    int mNextId;
    int msCol;
    int msRow;
    int mnCol;
    int mnRow;

signals:

public slots:
};

#endif // STEP_H
