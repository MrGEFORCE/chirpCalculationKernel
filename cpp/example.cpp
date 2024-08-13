#include "chirpParameters.h"

int main() {
    chirpParameters::ChirpParameterHandler instance;
    instance.set_default();
    instance.save_cfg("D:/workSpace/coding/mixed/chirpCalculationKernel/cpp/test.cfg");
    instance.load_cfg("D:/workSpace/coding/mixed/chirpCalculationKernel/cpp/test.cfg");

    return 0;
}
