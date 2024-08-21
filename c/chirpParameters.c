#include "chirpParameters.h"

static bool is2power(int x) {
    return (x > 0) && ((x & (x - 1)) == 0);
}

void set_default(ChirpParameterHandler_t* h) {
    h->data.intData.v[minADCPointsIdx] = 0;
    h->data.intData.v[minChirpLoopsIdx] = 0;
    h->data.intData.v[minRangeFFTSizeIdx] = 0;
    h->data.intData.v[minDopplerFFTSizeIdx] = 0;
    h->data.intData.v[maxADCPointsIdx] = 0;  // unlimited
    h->data.intData.v[maxChirpLoopsIdx] = 0;
    h->data.intData.v[maxRangeFFTSizeIdx] = 0;
    h->data.intData.v[maxDopplerFFTSizeIdx] = 0;
    h->data.boolData.v[staticClutterRemovalIdx] = true;
    h->data.floatData.v[startFrequency_MHzIdx] = 77e3f;
    h->data.floatData.v[bandWidth_MHzIdx] = 3e3f;
    h->data.floatData.v[idleTime_usIdx] = 20.f;
    h->data.floatData.v[rampTime_usIdx] = 100.f;
    h->data.floatData.v[periodicity_msIdx] = 10.f;
    h->data.intData.v[ADCPointsIdx] = 256;
    h->data.floatData.v[sampleRate_kspsIdx] = 3000.f;
    h->data.floatData.v[ADCDelay_usIdx] = 10.f;
    h->data.intData.v[chirpLoopsIdx] = 64;
    h->data.boolData.v[iqSampleIdx] = true;
    h->data.intData.v[antTDMIdx] = 1;
    h->data.intData.v[rxIdx] = 1;
    h->data.intData.v[rangeFFTSizeIdx] = h->data.intData.v[ADCPointsIdx];
    h->data.intData.v[dopplerFFTSizeIdx] = h->data.intData.v[chirpLoopsIdx];
    h->data.cfar.boolData.v[enabledIdx] = false;
    compute_and_validate(h);
}

void compute_and_validate(ChirpParameterHandler_t* h) {
    if (h->slopePriority == spSlopeFirst) {
        h->data.floatData.t.bandWidth_MHz = h->data.floatData.t.rampTime_us * h->data.floatData.t.slope_MHzus;
    } else if (h->slopePriority == spBandwidthFirst) {
        h->data.floatData.t.slope_MHzus = h->data.floatData.t.bandWidth_MHz / h->data.floatData.t.rampTime_us;
    }
    h->data.floatData.t.lambdaStart_mm = 3e5f / h->data.floatData.t.startFrequency_MHz;
    h->data.floatData.t.lambdaCenter_mm = 3e5f / (h->data.floatData.t.startFrequency_MHz + h->data.floatData.t.bandWidth_MHz / 2.f);
    h->data.floatData.t.Tc_us = h->data.floatData.t.idleTime_us + h->data.floatData.t.rampTime_us;
    h->data.floatData.t.TcTDM_us = h->data.floatData.t.Tc_us * (float)h->data.intData.t.antTDM;
    h->data.floatData.t.Tf_us = h->data.floatData.t.TcTDM_us * (float)h->data.intData.t.chirpLoops;
    h->data.floatData.t.dutyCycle_percent = h->data.floatData.t.Tf_us / h->data.floatData.t.periodicity_ms * 1e-1f;
    h->data.floatData.t.maxADCTime_us = h->data.floatData.t.rampTime_us - h->data.floatData.t.ADCDelay_us;
    h->data.floatData.t.ADCTime_us = (float)h->data.intData.t.ADCPoints / h->data.floatData.t.sampleRate_ksps * 1e3f;
    h->data.floatData.t.ADCBandWidth_MHz = h->data.floatData.t.slope_MHzus * h->data.floatData.t.ADCTime_us;
    h->data.floatData.t.dMax_m = h->data.floatData.t.sampleRate_ksps * 15e-2f / h->data.floatData.t.slope_MHzus;
    if (!h->data.boolData.t.iqSample) {
        h->data.floatData.t.dMax_m /= 2.f;
    }
    h->data.floatData.t.dRes_m = 1.5e2f / h->data.floatData.t.ADCBandWidth_MHz;
    h->data.floatData.t.dResCompute_m = h->data.floatData.t.dMax_m / (float)h->data.intData.t.rangeFFTSize;
    if (!h->data.boolData.t.iqSample) {
        h->data.floatData.t.dResCompute_m *= 2.f;  // lost half of the FFT valid length
    }
    h->data.floatData.t.vMax_m_s = h->data.floatData.t.lambdaCenter_mm / 4.f / h->data.floatData.t.TcTDM_us * 1e3f;
    h->data.floatData.t.vRes_m_s = h->data.floatData.t.lambdaCenter_mm / 2.f / h->data.floatData.t.Tf_us * 1e3f;
    h->data.floatData.t.vResCompute_m_s = h->data.floatData.t.vMax_m_s / (float)h->data.intData.t.dopplerFFTSize * 2.f;
    h->data.floatData.t.dopplerSampleRate_sps = 1.f / h->data.floatData.t.TcTDM_us * 1e6f;
    h->data.floatData.t.oneMeterIF_kHz = 20.f * h->data.floatData.t.slope_MHzus / 3.f;
    h->data.floatData.t.oneResBinIF_kHz = h->data.floatData.t.oneMeterIF_kHz * h->data.floatData.t.dRes_m;
    h->data.floatData.t.oneResComputeBinIF_kHz = h->data.floatData.t.oneMeterIF_kHz * h->data.floatData.t.dResCompute_m;
    h->data.floatData.t.maxIF_kHz = h->data.floatData.t.oneMeterIF_kHz * h->data.floatData.t.dMax_m;
    h->data.floatData.t.f32radarCube_kB = (float)h->data.intData.t.antTDM * (float)h->data.intData.t.rx * (float)h->data.intData.t.rangeFFTSize * (float)h->data.intData.t.dopplerFFTSize / 128.f;
    h->computeErrorFlag = CP_ERR;
    if (h->data.floatData.t.dutyCycle_percent > 100) {
        h->errType = errDutyCycleGT100;
        return;
    }

    if (h->data.intData.t.maxADCPoints != 0) {
        if (h->data.intData.t.ADCPoints > h->data.intData.t.maxADCPoints) {
            h->errType = errADCPointsGTLimit;
            return;
        }
    }

    if (h->data.intData.t.maxChirpLoops != 0) {
        if (h->data.intData.t.chirpLoops > h->data.intData.t.maxChirpLoops) {
            h->errType = errChirpLoopsGTLimit;
            return;
        }
    }

    if (h->data.intData.t.maxRangeFFTSize != 0) {
        if (h->data.intData.t.rangeFFTSize > h->data.intData.t.maxRangeFFTSize) {
            h->errType = errRangeFFTSizeGTLimit;
            return;
        }
    }

    if (h->data.intData.t.maxDopplerFFTSize != 0) {
        if (h->data.intData.t.dopplerFFTSize > h->data.intData.t.maxDopplerFFTSize) {
            h->errType = errDopplerFFTSizeGTLimit;
            return;
        }
    }

    if (h->data.intData.t.ADCPoints < h->data.intData.t.minADCPoints) {
        h->errType = errADCPointsLTLimit;
        return;
    }

    if (h->data.intData.t.chirpLoops < h->data.intData.t.minChirpLoops) {
        h->errType = errChirpLoopsLTLimit;
        return;
    }

    if (h->data.intData.t.rangeFFTSize < h->data.intData.t.minRangeFFTSize) {
        h->errType = errRangeFFTSizeLTLimit;
        return;
    }

    if (h->data.intData.t.dopplerFFTSize < h->data.intData.t.minDopplerFFTSize) {
        h->errType = errDopplerFFTSizeLTLimit;
        return;
    }

    if ((h->data.floatData.t.maxADCTime_us < 0) || (h->data.floatData.t.ADCDelay_us + h->data.floatData.t.ADCTime_us > h->data.floatData.t.rampTime_us)) {
        h->errType = errADCDelayTooLong;
        return;
    }

    if (h->data.floatData.t.ADCTime_us > h->data.floatData.t.maxADCTime_us) {
        h->errType = errADCSampleTimeTooLong;
        return;
    }

    if (!is2power(h->data.intData.t.rangeFFTSize)) {
        h->errType = errRangeFFTSIzeNotPower2;
        return;
    }

    if (h->data.intData.t.rangeFFTSize < h->data.intData.t.ADCPoints) {
        h->errType = errRangeFFTSizeLTADCPoints;
        return;
    }

    if (!is2power(h->data.intData.t.dopplerFFTSize)) {
        h->errType = errDopplerFFTSIzeNotPower2;
        return;
    }

    if (h->data.intData.t.dopplerFFTSize < h->data.intData.t.chirpLoops) {
        h->errType = errDopplerFFTSizeLTChirpLoops;
        return;
    }

    h->computeErrorFlag = CP_OK;
    h->errType = errNoErr;
}