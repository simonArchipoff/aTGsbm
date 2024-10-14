#pragma once

#include <QObject>

class RingBuffer : public QObject
{
    Q_OBJECT

public:
    RingBuffer();
    RingBuffer(int size);
    void reset(int size);
    void push(const QVector<float> &v);
    void push(float v);
    QVector<float> read();
    float rms();
private:
    QVector<float> buff;
    int begin = 0;
    bool full = false;
};
