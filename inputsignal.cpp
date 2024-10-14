#include "inputsignal.h"

//InputSignal::InputSignal() {}

InputSignal::InputSignal():QAudioDecoder(){
    setSource(QUrl::fromLocalFile("/home/simon/seiko.oga"));
    qDebug() << this->source();

    QAudioFormat format;
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Float);
    setAudioFormat(format);
    connect(this,&QAudioDecoder::bufferReady, this, &InputSignal::read);
    qDebug() << this->error();


}

void InputSignal::read(){
    QAudioBuffer r = QAudioDecoder::read();
    auto rawData = r.constData<float>();
    int frameCount = r.frameCount();  // Nombre de trames dans le tampon
    int sampleCount = r.sampleCount();
    int nb_chan = sampleCount / frameCount;
    QVector<float> b(frameCount);
    for(int i = 0; i < frameCount ; i++){
        b[i]=rawData[nb_chan*i+1];
    }

    //float *pb = b.data();
    //bandpass.process(frameCount,&pb);
    emit buffer(b);
    //qDebug() << frameCount << sampleCount << "\n";
}
