#include "chirpParameters.h"

int main() {
    ChirpParameterHandler_t instance;
    instance.slopePriority = spBandwidthFirst;
    set_default(&instance);
    return 0;
}
