#ifndef SIGNALPROCESS_H
#define SIGNALPROCESS_H

#include <QObject>

#include <ChebyshevI.h>
#include <ringbuffer.h>

class SignalProcess : public QObject{
    Q_OBJECT
public:
    SignalProcess();

    void input(QVector<float>);

    void setSampleRate(int sr);

signals:
    void decimatedEnvelop(QVector<float>);
    void decimatedSampleRate(uint sr);
    void sampleRate(uint sr);

protected:
    Dsp::SimpleFilter <Dsp::ChebyshevI::HighPass<4>, 1> highpass_input;
    float highpass_frequency = 4000;


    Dsp::SimpleFilter<Dsp::ChebyshevI::LowPass<4>,1> lowpass_envelop;
    float lowpass_envelop_freqency = 200;
    float peak_holder = 0.0;
    int nextIndex = 0;

    int decimation = 16;

    //RingBuffer analyse_buffer;
};

#endif // SIGNALPROCESS_H
