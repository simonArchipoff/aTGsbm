#include "ringbuffer.h"

RingBuffer::RingBuffer(){
    reset(0);
}

RingBuffer::RingBuffer(int size){
    reset(size);
}

void RingBuffer::reset(int size){
    buff.resize(size,0);
    begin=0;
    full=false;
}

void RingBuffer::push(const QVector<float> &v){
    for(auto const i : v){
        push(i);
    }
}

void RingBuffer::push(float v){
    buff[begin] = v;
    full = full | (++begin == buff.size());
}

QVector<float> RingBuffer::read(){
    if(full){
        QVector<float> res(buff.size());
        for(int i = 0; i < res.size(); i++){
            res[i] = buff[(begin + 1 + i) % res.size()];
        }
        return res;
    }else{
        QVector<float> res(begin);
        for(int i = 0; i < res.size(); i++){
            res[i] = buff[i];
        }
        return res;
    }
}

float RingBuffer::rms(){
    double f = 0.0;
    int end = full ? buff.size(): begin;
    for(int i = 0; i < end; i++){
        f += buff[i]*buff[i];
    }
    f /= buff.size();
    return sqrt(f);
}
