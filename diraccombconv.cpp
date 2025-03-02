#include "diraccombconv.h"
#include <kiss_fft.h>
#include <algorithm>
#include <stdexcept>

using namespace std;


template <typename Container>
size_t findMaxIndex(const Container& container) {
    if (std::begin(container) == std::end(container)) {
        throw std::invalid_argument("Container is empty");
    }

    size_t maxIndex = 0;
    size_t currentIndex = 0;
    auto maxIt = std::begin(container);

    for (auto it = std::begin(container); it != std::end(container); ++it) {
        if (*it > *maxIt) {
            maxIt = it;
            maxIndex = currentIndex;
        }
        ++currentIndex;
    }
    return maxIndex;
}



static std::pair<std::vector<float>,std::vector<int>> computeEnergyAroundFrequency(const std::vector<kiss_fft_cpx>& signalFFT, int period, int numPeriods) {
    std::vector<float> energies(numPeriods,0);
    std::vector<int> periods(numPeriods);

    for(int i = 0; i < numPeriods; i++)
    {
        periods[i] = period + i - numPeriods/2;
    }

    for (size_t i = 0; i < signalFFT.size(); ++i) {
        for (int j = 0; j < numPeriods; ++j) { // TODO optimize that: larges chucks of the input can be skipped, we just want the ones mod something close do period
                                               // one index should iterate over i*period, and the other one should look around that.
                                               // and now that I figured out the exact algorithm I wonder why I dont optimize it now
            if (static_cast<int>(i) % periods[j] == 0) {
                energies[j] += signalFFT[i].r * signalFFT[i].r + signalFFT[i].i * signalFFT[i].i;
            }
        }
    }
    return pair{energies,periods};
}


ConvResult computeConvWithDiracComb(const vector<float> &signal, int sampleRate, float frequency)
{
    std::vector<kiss_fft_cpx> output(signal.size() * 2);
    std::vector<kiss_fft_cpx> input(signal.size() * 2);
    kiss_fft_cfg cfgdirect  = kiss_fft_alloc(input.size(),false /*inverse fft*/,nullptr, nullptr),
                 cfginverse = kiss_fft_alloc(input.size(),true                 ,nullptr, nullptr);

    for(int i = 0; i < input.size(); i++)
    {
        if(signal.size() > i)
        {
            input[i].i = 0;
            input[i].r = signal[i];
        }
        else
        {
            input[i].i = input[i].r = 0;
        }
    }

    kiss_fft(cfgdirect, input.data(), output.data());


    const int period = input.size() *  frequency / sampleRate;
    auto c = computeEnergyAroundFrequency(output,period,8);

    auto imax = findMaxIndex(c.first);
    const auto periodMax = c.second[imax];
    double interpolPeriodMax = c.second[imax] + (c.first[imax - 1] - c.first[imax + 1])
                                     / (2 * (c.first[imax - 1] - 2 * c.first[imax] + c.first[imax + 1]));
    for(int i = 0; i < output.size() ; i++)
    {
        if(i % periodMax != 0){
            output[i].i = output[i].r = 0;
        }
    }

    kiss_fft(cfginverse,output.data(), input.data());

    ConvResult res;
    res.res.resize(signal.size());
    res.sampleRate = sampleRate;

    res.frequency = interpolPeriodMax * sampleRate / input.size();

    std::transform(input.data(), input.data()+signal.size()
                   , res.res.begin()
                   , [](const kiss_fft_cpx c) { return  std::sqrt(c.i * c.i + c.r * c.r);});
    free(cfgdirect);
    free(cfginverse);
    return res;
}
