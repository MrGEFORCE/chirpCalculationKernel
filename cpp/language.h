#ifndef CHIRPPARAMETERS_LANGUAGE_H
#define CHIRPPARAMETERS_LANGUAGE_H

enum strErr_e {
    errDutyCycleGT100 = 0,
    errADCPointsGTLimit,
    errChirpLoopsGTLimit,
    errRangeFFTSizeGTLimit,
    errDopplerFFTSizeGTLimit,
    errADCPointsLTLimit,
    errChirpLoopsLTLimit,
    errRangeFFTSizeLTLimit,
    errDopplerFFTSizeLTLimit,
    errADCDelayTooLong,
    errADCSampleTimeTooLong,
    errRangeFFTSIzeNotPower2,
    errRangeFFTSizeLTADCPoints,
    errDopplerFFTSIzeNotPower2,
    errDopplerFFTSizeLTChirpLoops,
    errNoErr,
    errFileReadErr,
    errStrNums
};

enum languageType_e {
    langZhCNIdx = 0,
    langEnUSIdx
};

#define langZhCNFileName "zh_CN.cfg"
#define langEnUSFileName "en_US.cfg"

#endif
