#include "chirpParameters.h"

int main() {
    chirpParameters::ChirpParameterHandler instance;
    instance.setLanguageFileDir("../locales");
    instance.changeLanguage(langZhCNIdx);  // must set language first before using
    instance.set_default();
    instance.save_cfg("../test.cfg");
    instance.load_cfg("../test.cfg");

    return 0;
}
