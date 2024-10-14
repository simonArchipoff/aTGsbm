#include "signalprocess.h"

SignalProcess::SignalProcess(){
    //rms_buffer.reset(4096);
    //analyse_buffer.reset(1024*64*2);
}

void SignalProcess::setSampleRate(int sr){
    highpass_input.setup(2, sr, highpass_frequency,1);

    lowpass_envelop.setup(1,sr,lowpass_envelop_freqency,1);

    emit(sampleRate(sr));
    assert(sr% decimation == 0);
    emit(decimatedSampleRate(sr/decimation));
}



void SignalProcess::input(QVector<float> v){
    auto p = v.data();
    highpass_input.process(v.size(),&p);
    //analyse_buffer.push(v);

    for(auto & i : v){
        i = abs(i);
        if(i > peak_holder){
            peak_holder = i;
        } else {
            i = peak_holder = i + 0.5 * peak_holder;
        }
    }
    lowpass_envelop.process(v.size(),&p);
    if(nextIndex >= v.size()){
        nextIndex -= v.size();
        return;
    }
    QVector<float> decimated((v.size() - nextIndex)/decimation);

    for(int i = 0; i < decimated.size(); i++){
        decimated[i] = v[nextIndex + i*decimation] * 5;
    }
    nextIndex = (nextIndex + v.size()) % decimation;

    emit decimatedEnvelop(decimated);
}






/*
bandpass_input.setup(4,    // order
               48000,// sample rate
               10000, // center frequency
               5000);
*/
