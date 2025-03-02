#include "diraccombconv.h"
#include <vector>
using namespace std;
int main(){
    int sr = 100;
    double f = 10.4;

    vector<float> signal(sr*2);

    for(int i = 0; i < signal.size(); i++){
        double e = sin(i*2*M_PI*f/sr);
        if(e > 0.9)
        {
            signal[i]=e;
        }
        else
        {
            signal[i] = 0;
        }
    }

    struct ConvResult r = computeConvWithDiracComb(signal, sr, 10);


    return 0;
}
