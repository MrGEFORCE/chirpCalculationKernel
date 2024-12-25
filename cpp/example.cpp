#include "chirpParameters.h"

int main() {
    chirpParameters::ChirpParameterHandler instance;
    instance.languageCfgFileFolder = "../locales";
    instance.setLanguage(langZhCNIdx);  // must set language first before using
    // instance.set_default();
    // instance.save_cfg("../test.cfg");
    instance.loadCfg("../test.cfg");

    return 0;
}
