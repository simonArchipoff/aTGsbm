#pragma once

#include <QAudioInput>
#include <QObject>
#include <QAudioDecoder>
#include <QAudioBuffer>

#include <QUrl>
#include <QAudioFormat>
#include <QAudioBuffer>





class InputSignal : public QAudioDecoder
{
    Q_OBJECT
public:
    InputSignal();
    void read();
    QVector<float> filter(QVector<float> f);

signals:
    void buffer(QVector<float> f);
private:

};
