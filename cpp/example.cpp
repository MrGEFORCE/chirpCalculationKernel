#include "chirpParameters.h"

int main() {
    chirpParameters::ChirpParameterHandler instance;
    instance.languageCfgFileFolder = "../locales";
    instance.set_language(langZhCNIdx);  // must set language first before using
    // instance.set_default();
    // instance.save_cfg("../test.cfg");
    instance.load_cfg("D:/workSpace/coding/mixed/chirpCalculationKernel/MATLAB/test.cfg");

    return 0;
}
