#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "config.h"
#include "chirpParameters.h"

namespace chirpParameters {
    static bool is2power(int x) {
        return (x > 0) && ((x & (x - 1)) == 0);
    }

    static std::string precision_control(float x, int precision) {
        std::stringstream buffer;
        buffer << std::fixed << std::setprecision(precision) << x;
        return buffer.str();
    }

    ChirpParameterHandler::ChirpParameterHandler() {
        // init unordered map
        this->strRefTypeMap[STR_KEY_startFrequency_MHz] = typeFloat;
        this->strRefTypeMap[STR_KEY_bandWidth_MHz] = typeFloat;
        this->strRefTypeMap[STR_KEY_idleTime_us] = typeFloat;
        this->strRefTypeMap[STR_KEY_rampTime_us] = typeFloat;
        this->strRefTypeMap[STR_KEY_periodicity_ms] = typeFloat;
        this->strRefTypeMap[STR_KEY_ADCPoints] = typeInt;
        this->strRefTypeMap[STR_KEY_sampleRate_ksps] = typeFloat;
        this->strRefTypeMap[STR_KEY_ADCDelay_us] = typeFloat;
        this->strRefTypeMap[STR_KEY_chirpLoops] = typeInt;
        this->strRefTypeMap[STR_KEY_iqSample] = typeBool;
        this->strRefTypeMap[STR_KEY_antTDM] = typeInt;
        this->strRefTypeMap[STR_KEY_rx] = typeInt;
        this->strRefTypeMap[STR_KEY_rangeFFTSize] = typeInt;
        this->strRefTypeMap[STR_KEY_dopplerFFTSize] = typeInt;
        this->strRefTypeMap[STR_KEY_minADCPoints] = typeInt;
        this->strRefTypeMap[STR_KEY_minChirpLoops] = typeInt;
        this->strRefTypeMap[STR_KEY_minRangeFFTSize] = typeInt;
        this->strRefTypeMap[STR_KEY_minDopplerFFTSize] = typeInt;
        this->strRefTypeMap[STR_KEY_maxADCPoints] = typeInt;
        this->strRefTypeMap[STR_KEY_maxChirpLoops] = typeInt;
        this->strRefTypeMap[STR_KEY_maxRangeFFTSize] = typeInt;
        this->strRefTypeMap[STR_KEY_maxDopplerFFTSize] = typeInt;
        this->strRefTypeMap[STR_KEY_staticClutterRemoval] = typeBool;
        this->strRefTypeMap[STR_KEY_rangeGuard] = typeInt;
        this->strRefTypeMap[STR_KEY_rangeReference] = typeInt;
        this->strRefTypeMap[STR_KEY_rangeAlpha] = typeFloat;
        this->strRefTypeMap[STR_KEY_rangeBias_dB] = typeFloat;
        this->strRefTypeMap[STR_KEY_dopplerGuard] = typeInt;
        this->strRefTypeMap[STR_KEY_dopplerReference] = typeInt;
        this->strRefTypeMap[STR_KEY_dopplerAlpha] = typeFloat;
        this->strRefTypeMap[STR_KEY_dopplerBias_dB] = typeFloat;

        this->strRefIdxMap[STR_KEY_startFrequency_MHz] = startFrequency_MHzIdx;
        this->strRefIdxMap[STR_KEY_bandWidth_MHz] = bandWidth_MHzIdx;
        this->strRefIdxMap[STR_KEY_idleTime_us] = idleTime_usIdx;
        this->strRefIdxMap[STR_KEY_rampTime_us] = rampTime_usIdx;
        this->strRefIdxMap[STR_KEY_periodicity_ms] = periodicity_msIdx;
        this->strRefIdxMap[STR_KEY_ADCPoints] = ADCPointsIdx;
        this->strRefIdxMap[STR_KEY_sampleRate_ksps] = sampleRate_kspsIdx;
        this->strRefIdxMap[STR_KEY_ADCDelay_us] = ADCDelay_usIdx;
        this->strRefIdxMap[STR_KEY_chirpLoops] = chirpLoopsIdx;
        this->strRefIdxMap[STR_KEY_iqSample] = iqSampleIdx;
        this->strRefIdxMap[STR_KEY_antTDM] = antTDMIdx;
        this->strRefIdxMap[STR_KEY_rx] = rxIdx;
        this->strRefIdxMap[STR_KEY_rangeFFTSize] = rangeFFTSizeIdx;
        this->strRefIdxMap[STR_KEY_dopplerFFTSize] = dopplerFFTSizeIdx;
        this->strRefIdxMap[STR_KEY_minADCPoints] = minADCPointsIdx;
        this->strRefIdxMap[STR_KEY_minChirpLoops] = minChirpLoopsIdx;
        this->strRefIdxMap[STR_KEY_minRangeFFTSize] = minRangeFFTSizeIdx;
        this->strRefIdxMap[STR_KEY_minDopplerFFTSize] = minDopplerFFTSizeIdx;
        this->strRefIdxMap[STR_KEY_maxADCPoints] = maxADCPointsIdx;
        this->strRefIdxMap[STR_KEY_maxChirpLoops] = maxChirpLoopsIdx;
        this->strRefIdxMap[STR_KEY_maxRangeFFTSize] = maxRangeFFTSizeIdx;
        this->strRefIdxMap[STR_KEY_maxDopplerFFTSize] = maxDopplerFFTSizeIdx;
        this->strRefIdxMap[STR_KEY_staticClutterRemoval] = staticClutterRemovalIdx;
        this->strRefIdxMap[STR_KEY_rangeGuard] = rangeGuardIdx;
        this->strRefIdxMap[STR_KEY_rangeReference] = rangeReferenceIdx;
        this->strRefIdxMap[STR_KEY_rangeAlpha] = rangeAlphaIdx;
        this->strRefIdxMap[STR_KEY_rangeBias_dB] = rangeBias_dBIdx;
        this->strRefIdxMap[STR_KEY_dopplerGuard] = dopplerGuardIdx;
        this->strRefIdxMap[STR_KEY_dopplerReference] = dopplerReferenceIdx;
        this->strRefIdxMap[STR_KEY_dopplerAlpha] = dopplerAlphaIdx;
        this->strRefIdxMap[STR_KEY_dopplerBias_dB] = dopplerBias_dBIdx;

        this->keyStringsBasic = std::vector<std::string>(CFG_BASIC_LINES);
        this->keyStringsBasic[0] = STR_KEY_startFrequency_MHz;
        this->keyStringsBasic[1] = STR_KEY_bandWidth_MHz;
        this->keyStringsBasic[2] = STR_KEY_idleTime_us;
        this->keyStringsBasic[3] = STR_KEY_rampTime_us;
        this->keyStringsBasic[4] = STR_KEY_periodicity_ms;
        this->keyStringsBasic[5] = STR_KEY_ADCPoints;
        this->keyStringsBasic[6] = STR_KEY_sampleRate_ksps;
        this->keyStringsBasic[7] = STR_KEY_ADCDelay_us;
        this->keyStringsBasic[8] = STR_KEY_chirpLoops;
        this->keyStringsBasic[9] = STR_KEY_iqSample;
        this->keyStringsBasic[10] = STR_KEY_antTDM;
        this->keyStringsBasic[11] = STR_KEY_rx;
        this->keyStringsBasic[12] = STR_KEY_rangeFFTSize;
        this->keyStringsBasic[13] = STR_KEY_dopplerFFTSize;
        this->keyStringsBasic[14] = STR_KEY_minADCPoints;
        this->keyStringsBasic[15] = STR_KEY_minChirpLoops;
        this->keyStringsBasic[16] = STR_KEY_minRangeFFTSize;
        this->keyStringsBasic[17] = STR_KEY_minDopplerFFTSize;
        this->keyStringsBasic[18] = STR_KEY_maxADCPoints;
        this->keyStringsBasic[19] = STR_KEY_maxChirpLoops;
        this->keyStringsBasic[20] = STR_KEY_maxRangeFFTSize;
        this->keyStringsBasic[21] = STR_KEY_maxDopplerFFTSize;
        this->keyStringsBasic[22] = STR_KEY_staticClutterRemoval;

        this->keyStringsCfar = std::vector<std::string>(CFG_CFAR_LINES);
        this->keyStringsCfar[0] = STR_KEY_rangeGuard;
        this->keyStringsCfar[1] = STR_KEY_rangeReference;
        this->keyStringsCfar[2] = STR_KEY_rangeAlpha;
        this->keyStringsCfar[3] = STR_KEY_rangeBias_dB;
        this->keyStringsCfar[4] = STR_KEY_dopplerGuard;
        this->keyStringsCfar[5] = STR_KEY_dopplerReference;
        this->keyStringsCfar[6] = STR_KEY_dopplerAlpha;
        this->keyStringsCfar[7] = STR_KEY_dopplerBias_dB;

        this->strErr = std::vector<std::string>(errStrNums);
        this->languageCfgFileFolder = "";

        this->computeErrorFlag = CP_OK;
        this->slopePriority = spBandwidthFirst; // default
    }

    void ChirpParameterHandler::set_language(languageType_e flag) {
        std::ifstream langFile;
        switch (flag) {
            case langZhCNIdx:
                langFile.open(this->languageCfgFileFolder + "/" + langZhCNFileName);
                break;
            case langEnUSIdx:
                langFile.open(this->languageCfgFileFolder + "/" + langEnUSFileName);
                break;
            default:  // en_US
                langFile.open(this->languageCfgFileFolder + "/" + langEnUSFileName);
        }

        for (int i=0;i<errStrNums;i++) {
            std::getline(langFile, this->strErr[i]);
        }
    }

    void ChirpParameterHandler::set_default() {
        this->data.intData.v[minADCPointsIdx] = 0;
        this->data.intData.v[minChirpLoopsIdx] = 0;
        this->data.intData.v[minRangeFFTSizeIdx] = 0;
        this->data.intData.v[minDopplerFFTSizeIdx] = 0;
        this->data.intData.v[maxADCPointsIdx] = 0;  // unlimited
        this->data.intData.v[maxChirpLoopsIdx] = 0;
        this->data.intData.v[maxRangeFFTSizeIdx] = 0;
        this->data.intData.v[maxDopplerFFTSizeIdx] = 0;
        this->data.boolData.v[staticClutterRemovalIdx] = true;
        this->data.floatData.v[startFrequency_MHzIdx] = 77e3f;
        this->data.floatData.v[bandWidth_MHzIdx] = 3e3f;
        this->data.floatData.v[idleTime_usIdx] = 20.f;
        this->data.floatData.v[rampTime_usIdx] = 100.f;
        this->data.floatData.v[periodicity_msIdx] = 10.f;
        this->data.intData.v[ADCPointsIdx] = 256;
        this->data.floatData.v[sampleRate_kspsIdx] = 3000.f;
        this->data.floatData.v[ADCDelay_usIdx] = 10.f;
        this->data.intData.v[chirpLoopsIdx] = 64;
        this->data.boolData.v[iqSampleIdx] = true;
        this->data.intData.v[antTDMIdx] = 1;
        this->data.intData.v[rxIdx] = 1;
        this->data.intData.v[rangeFFTSizeIdx] = this->data.intData.v[ADCPointsIdx];
        this->data.intData.v[dopplerFFTSizeIdx] = this->data.intData.v[chirpLoopsIdx];
        this->data.cfar.boolData.v[enabledIdx] = false;
        this->compute_and_validate();
    }

    void ChirpParameterHandler::compute_and_validate() {
        if (this->slopePriority == spSlopeFirst) {
            this->data.floatData.t.bandWidth_MHz = this->data.floatData.t.rampTime_us * this->data.floatData.t.slope_MHzus;
        } else if (this->slopePriority == spBandwidthFirst) {
            this->data.floatData.t.slope_MHzus = this->data.floatData.t.bandWidth_MHz / this->data.floatData.t.rampTime_us;
        }
        this->data.floatData.t.lambdaStart_mm = 3e5f / this->data.floatData.t.startFrequency_MHz;
        this->data.floatData.t.lambdaCenter_mm = 3e5f / (this->data.floatData.t.startFrequency_MHz + this->data.floatData.t.bandWidth_MHz / 2.f);
        this->data.floatData.t.Tc_us = this->data.floatData.t.idleTime_us + this->data.floatData.t.rampTime_us;
        this->data.floatData.t.TcTDM_us = this->data.floatData.t.Tc_us * (float)this->data.intData.t.antTDM;
        this->data.floatData.t.Tf_us = this->data.floatData.t.TcTDM_us * (float)this->data.intData.t.chirpLoops;
        this->data.floatData.t.dutyCycle_percent = this->data.floatData.t.Tf_us / this->data.floatData.t.periodicity_ms * 1e-1f;
        this->data.floatData.t.maxADCTime_us = this->data.floatData.t.rampTime_us - this->data.floatData.t.ADCDelay_us;
        this->data.floatData.t.ADCTime_us = (float)this->data.intData.t.ADCPoints / this->data.floatData.t.sampleRate_ksps * 1e3f;
        this->data.floatData.t.ADCBandWidth_MHz = this->data.floatData.t.slope_MHzus * this->data.floatData.t.ADCTime_us;
        this->data.floatData.t.dMax_m = this->data.floatData.t.sampleRate_ksps * 15e-2f / this->data.floatData.t.slope_MHzus;
        if (!this->data.boolData.t.iqSample) {
            this->data.floatData.t.dMax_m /= 2.f;
        }
        this->data.floatData.t.dRes_m = 1.5e2f / this->data.floatData.t.ADCBandWidth_MHz;
        this->data.floatData.t.dResCompute_m = this->data.floatData.t.dMax_m / (float)this->data.intData.t.rangeFFTSize;
        if (!this->data.boolData.t.iqSample) {
            this->data.floatData.t.dResCompute_m *= 2.f;  // lost half of the FFT valid length
        }
        this->data.floatData.t.vMax_m_s = this->data.floatData.t.lambdaCenter_mm / 4.f / this->data.floatData.t.TcTDM_us * 1e3f;
        this->data.floatData.t.vRes_m_s = this->data.floatData.t.lambdaCenter_mm / 2.f / this->data.floatData.t.Tf_us * 1e3f;
        this->data.floatData.t.vResCompute_m_s = this->data.floatData.t.vMax_m_s / (float)this->data.intData.t.dopplerFFTSize * 2.f;
        this->data.floatData.t.dopplerSampleRate_sps = 1.f / this->data.floatData.t.TcTDM_us * 1e6f;
        this->data.floatData.t.oneMeterIF_kHz = 20.f * this->data.floatData.t.slope_MHzus / 3.f;
        this->data.floatData.t.oneResBinIF_kHz = this->data.floatData.t.oneMeterIF_kHz * this->data.floatData.t.dRes_m;
        this->data.floatData.t.oneResComputeBinIF_kHz = this->data.floatData.t.oneMeterIF_kHz * this->data.floatData.t.dResCompute_m;
        this->data.floatData.t.maxIF_kHz = this->data.floatData.t.oneMeterIF_kHz * this->data.floatData.t.dMax_m;
        this->data.floatData.t.f32radarCube_kB = (float)this->data.intData.t.antTDM * (float)this->data.intData.t.rx * (float)this->data.intData.t.rangeFFTSize * (float)this->data.intData.t.dopplerFFTSize / 128.f;
        this->computeErrorFlag = CP_ERR;
        if (this->data.floatData.t.dutyCycle_percent > 100) {
            this->errMsg = this->strErr[errDutyCycleGT100];
            return;
        }

        if (this->data.intData.t.maxADCPoints != 0) {
            if (this->data.intData.t.ADCPoints > this->data.intData.t.maxADCPoints) {
                this->errMsg = this->strErr[errADCPointsGTLimit];
                return;
            }
        }

        if (this->data.intData.t.maxChirpLoops != 0) {
            if (this->data.intData.t.chirpLoops > this->data.intData.t.maxChirpLoops) {
                this->errMsg = this->strErr[errChirpLoopsGTLimit];
                return;
            }
        }

        if (this->data.intData.t.maxRangeFFTSize != 0) {
            if (this->data.intData.t.rangeFFTSize > this->data.intData.t.maxRangeFFTSize) {
                this->errMsg = this->strErr[errRangeFFTSizeGTLimit];
                return;
            }
        }

        if (this->data.intData.t.maxDopplerFFTSize != 0) {
            if (this->data.intData.t.dopplerFFTSize > this->data.intData.t.maxDopplerFFTSize) {
                this->errMsg = this->strErr[errDopplerFFTSizeGTLimit];
                return;
            }
        }

        if (this->data.intData.t.ADCPoints < this->data.intData.t.minADCPoints) {
            this->errMsg = this->strErr[errADCPointsLTLimit];
            return;
        }

        if (this->data.intData.t.chirpLoops < this->data.intData.t.minChirpLoops) {
            this->errMsg = this->strErr[errChirpLoopsLTLimit];
            return;
        }

        if (this->data.intData.t.rangeFFTSize < this->data.intData.t.minRangeFFTSize) {
            this->errMsg = this->strErr[errRangeFFTSizeLTLimit];
            return;
        }

        if (this->data.intData.t.dopplerFFTSize < this->data.intData.t.minDopplerFFTSize) {
            this->errMsg = this->strErr[errDopplerFFTSizeLTLimit];
            return;
        }

        if ((this->data.floatData.t.maxADCTime_us < 0) || (this->data.floatData.t.ADCDelay_us + this->data.floatData.t.ADCTime_us > this->data.floatData.t.rampTime_us)) {
            this->errMsg = this->strErr[errADCDelayTooLong];
            return;
        }

        if (this->data.floatData.t.ADCTime_us > this->data.floatData.t.maxADCTime_us) {
            this->errMsg = this->strErr[errADCSampleTimeTooLong];
            return;
        }

        if (!is2power(this->data.intData.t.rangeFFTSize)) {
            this->errMsg = this->strErr[errRangeFFTSIzeNotPower2];
            return;
        }

        if (this->data.intData.t.rangeFFTSize < this->data.intData.t.ADCPoints) {
            this->errMsg = this->strErr[errRangeFFTSizeLTADCPoints];
            return;
        }

        if (!is2power(this->data.intData.t.dopplerFFTSize)) {
            this->errMsg = this->strErr[errDopplerFFTSIzeNotPower2];
            return;
        }

        if (this->data.intData.t.dopplerFFTSize < this->data.intData.t.chirpLoops) {
            this->errMsg = this->strErr[errDopplerFFTSizeLTChirpLoops];
            return;
        }

        this->computeErrorFlag = CP_OK;
        this->errMsg = this->strErr[errNoErr];
    }

    void ChirpParameterHandler::save_cfg(const std::string& saveFileName) {
        if (saveFileName.empty()) {
            return;
        }
        this->compute_and_validate();
        if (this->computeErrorFlag == CP_ERR) {
            return;
        }

        std::ofstream saveFile;
        saveFile.open(saveFileName);
        saveFile << "[" << STR_SEC_CHIRP << "]\n";
        saveFile << STR_KEY_startFrequency_MHz << " = " << precision_control(this->data.floatData.t.startFrequency_MHz, 2) << "\n";
        saveFile << STR_KEY_bandWidth_MHz << " = " << precision_control(this->data.floatData.t.bandWidth_MHz, 2) << "\n";
        saveFile << STR_KEY_idleTime_us << " = " << precision_control(this->data.floatData.t.idleTime_us, 2) << "\n";
        saveFile << STR_KEY_rampTime_us << " = " << precision_control(this->data.floatData.t.rampTime_us, 2) << "\n";
        saveFile << STR_KEY_periodicity_ms << " = " << precision_control(this->data.floatData.t.periodicity_ms, 2) << "\n";
        saveFile << STR_KEY_ADCPoints << " = " << this->data.intData.t.ADCPoints << "\n";
        saveFile << STR_KEY_sampleRate_ksps << " = " << precision_control(this->data.floatData.t.sampleRate_ksps, 2) << "\n";
        saveFile << STR_KEY_ADCDelay_us << " = " << precision_control(this->data.floatData.t.ADCDelay_us, 2) << "\n";
        saveFile << STR_KEY_chirpLoops << " = " << this->data.intData.t.chirpLoops << "\n";
        saveFile << STR_KEY_iqSample << " = " << (this->data.boolData.t.iqSample ? "yes" : "no") << "\n";
        saveFile << STR_KEY_antTDM << " = " << this->data.intData.t.antTDM << "\n";
        saveFile << STR_KEY_rx << " = " << this->data.intData.t.rx << "\n";
        saveFile << STR_KEY_rangeFFTSize << " = " << this->data.intData.t.rangeFFTSize << "\n";
        saveFile << STR_KEY_dopplerFFTSize << " = " << this->data.intData.t.dopplerFFTSize << "\n";
        saveFile << STR_KEY_minADCPoints << " = " << this->data.intData.t.minADCPoints << "\n";
        saveFile << STR_KEY_minChirpLoops << " = " << this->data.intData.t.minChirpLoops << "\n";
        saveFile << STR_KEY_minRangeFFTSize << " = " << this->data.intData.t.minRangeFFTSize << "\n";
        saveFile << STR_KEY_minDopplerFFTSize << " = " << this->data.intData.t.minDopplerFFTSize << "\n";
        saveFile << STR_KEY_maxADCPoints << " = " << this->data.intData.t.maxADCPoints << "\n";
        saveFile << STR_KEY_maxChirpLoops << " = " << this->data.intData.t.maxChirpLoops << "\n";
        saveFile << STR_KEY_maxRangeFFTSize << " = " << this->data.intData.t.maxRangeFFTSize << "\n";
        saveFile << STR_KEY_maxDopplerFFTSize << " = " << this->data.intData.t.maxDopplerFFTSize << "\n";
        saveFile << STR_KEY_staticClutterRemoval << " = " << (this->data.boolData.t.staticClutterRemoval ? "yes" : "no") << "\n";
        if (this->data.cfar.boolData.t.enabled) {
            saveFile << "\n[" << STR_SEC_CFAR << "]\n";
            saveFile << STR_KEY_rangeGuard << " = " << this->data.cfar.intData.t.rangeGuard << "\n";
            saveFile << STR_KEY_rangeReference << " = " << this->data.cfar.intData.t.rangeReference << "\n";
            saveFile << STR_KEY_rangeAlpha << " = " << precision_control(this->data.cfar.floatData.t.rangeAlpha, 2) << "\n";
            saveFile << STR_KEY_rangeBias_dB << " = " << precision_control(this->data.cfar.floatData.t.rangeBias_dB, 2) << "\n";
            saveFile << STR_KEY_dopplerGuard << " = " << this->data.cfar.intData.t.dopplerGuard << "\n";
            saveFile << STR_KEY_dopplerReference << " = " << this->data.cfar.intData.t.dopplerReference << "\n";
            saveFile << STR_KEY_dopplerAlpha << " = " << precision_control(this->data.cfar.floatData.t.dopplerAlpha, 2) << "\n";
            saveFile << STR_KEY_dopplerBias_dB << " = " << precision_control(this->data.cfar.floatData.t.dopplerBias_dB, 2) << "\n";
        }
        saveFile.close();
    }

    void ChirpParameterHandler::load_cfg(const std::string& loadFileName) {
        if (loadFileName.empty()) {
            return;
        }
        this->loadErrorFlag = CP_ERR;
        bool ret = this->configParser.ReadConfig(loadFileName);
        if (!ret) {
            this->errMsg = this->strErr[errFileReadErr];
            return;
        }
        std::string tempStr;
        // basic part
        for (int i = 0;i < CFG_BASIC_LINES;i++) {
            tempStr = this->configParser.ReadString(STR_SEC_CHIRP, this->keyStringsBasic[i].c_str(), "");
            if (tempStr.empty()) {this->errMsg = this->strErr[errFileReadErr]; return;}
            switch (this->strRefTypeMap[this->keyStringsBasic[i]]) {
                case typeInt:
                    this->data.intData.v[this->strRefIdxMap[this->keyStringsBasic[i]]] = std::stoi(tempStr);
                    break;
                case typeFloat:
                    this->data.floatData.v[this->strRefIdxMap[this->keyStringsBasic[i]]] = std::stof(tempStr);
                    break;
                case typeBool:
                    this->data.boolData.v[this->strRefIdxMap[this->keyStringsBasic[i]]] = tempStr == "yes";
                    break;
                default:
                    break;
            }
        }
        // recalculate bandwidth
        if (this->slopePriority == spSlopeFirst) {
            this->data.floatData.t.slope_MHzus = this->data.floatData.t.bandWidth_MHz / this->data.floatData.t.rampTime_us;
        }
        // cfar part
        tempStr = this->configParser.ReadString(STR_SEC_CFAR, STR_KEY_rangeGuard, "");
        if (!tempStr.empty()) {
            for (int i = 0;i < CFG_CFAR_LINES;i++) {
                tempStr = this->configParser.ReadString(STR_SEC_CFAR, this->keyStringsCfar[i].c_str(), "");
                if (tempStr.empty()) {this->errMsg = this->strErr[errFileReadErr]; return;}
                switch (this->strRefTypeMap[this->keyStringsCfar[i]]) {
                    case typeInt:
                        this->data.cfar.intData.v[this->strRefIdxMap[this->keyStringsCfar[i]]] = std::stoi(tempStr);
                        break;
                    case typeFloat:
                        this->data.cfar.floatData.v[this->strRefIdxMap[this->keyStringsCfar[i]]] = std::stof(tempStr);
                        break;
                    default:
                        break;
                }
            }
            this->data.cfar.boolData.t.enabled = true;
        }
        this->compute_and_validate();
        this->loadErrorFlag = CP_OK;
    }

    ChirpParameterHandler::~ChirpParameterHandler() = default;

} // chirpParameters
