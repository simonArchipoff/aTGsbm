#pragma once
#include <vector>
#include <kiss_fft.h>

using namespace std;
struct ConvResult{
    int sampleRate;
    vector<float> res;
    double frequency;
};



struct ConvResult computeConvWithDiracComb(const vector<float> & signal, int sampleRate, float frequency);
