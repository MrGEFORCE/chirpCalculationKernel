#include "CFAR.h"

#include <cmath>
#include <fstream>
#include <cstring>


static float factorial(int n) {
    if (n == 0 || n == 1) {
        return 1.;
    } else {
        return (float) n * factorial(n - 1);
    }
}

static float comb_number(int n, int r) {
    return factorial(n) / factorial(r) / factorial(n - r);
}

void from_rate_to_alpha_ca(basicCfarData_t *config) {
    config->f.t.alpha = (float) config->N * (powf(config->falseAlarmRate, -1.f / (float) config->N) - 1.f);
}

void false_alarm_rate_ca(basicCfarData_t *config) {
    config->falseAlarmRate = powf((float) config->N / ((float) config->N + config->f.t.alpha), (float) config->N);
}

// ref:https://zhuanlan.zhihu.com/p/652220176
void false_alarm_rate_go(basicCfarData_t *config) {
    float p1 = powf(1.f + config->f.t.alpha / (float) config->i.t.ref, -(float) config->i.t.ref);
    float p21 = powf(2.f + config->f.t.alpha / (float) config->i.t.ref, -(float) config->i.t.ref);
    float p221, p222;
    float p22 = 0.f;
    for (int i = 0; i < config->i.t.ref; i++) {
        p221 = comb_number(config->i.t.ref - 1 + i, i);
        p222 = powf(2.f + config->f.t.alpha / (float) config->i.t.ref, -(float) i);
        p22 += p221 * p222;
    }
    config->falseAlarmRate = 2.f * (p1 - p21 * p22);
}

void false_alarm_rate_so(basicCfarData_t *config) {
    float p1 = powf(2.f + config->f.t.alpha / (float) config->i.t.ref, -(float) config->i.t.ref);
    float p21, p22;
    float p2 = 0.f;
    for (int i = 0; i < config->i.t.ref; i++) {
        p21 = comb_number(config->i.t.ref - 1 + i, i);
        p22 = powf(2.f + config->f.t.alpha / (float) config->i.t.ref, -(float) i);
        p2 += p21 * p22;
    }
    config->falseAlarmRate = 2 * p1 * p2;
}

CFAR::CFAR() {
    this->keyStringsCfarParams = std::vector<std::string>(7);
    this->keyStringsCfarParams[0] = STR_KEY_cfarParamEnabled;
    this->keyStringsCfarParams[1] = STR_KEY_cfarParamDim;
    this->keyStringsCfarParams[2] = STR_KEY_cfarParamType;
    this->keyStringsCfarParams[3] = STR_KEY_cfarParamGuard;
    this->keyStringsCfarParams[4] = STR_KEY_cfarParamReference;
    this->keyStringsCfarParams[5] = STR_KEY_cfarParamAlpha;
    this->keyStringsCfarParams[6] = STR_KEY_cfarParamBias;

    this->keyStringsCfarDims = std::vector<std::string>(cfarDimTotalNums);
    this->keyStringsCfarDims[0] = STR_KEY_cfarDimRange;
    this->keyStringsCfarDims[1] = STR_KEY_cfarDimDoppler;
    this->keyStringsCfarDims[2] = STR_KEY_cfarDimAzimuth;
    this->keyStringsCfarDims[3] = STR_KEY_cfarDimElevation;

    this->strRefIdxMap[STR_KEY_cfarParamEnabled] = enabledIdx;
    this->strRefIdxMap[STR_KEY_cfarParamDim] = dimIdx;
    this->strRefIdxMap[STR_KEY_cfarParamType] = typeIdx;
    this->strRefIdxMap[STR_KEY_cfarParamGuard] = guardIdx;
    this->strRefIdxMap[STR_KEY_cfarParamReference] = refIdx;
    this->strRefIdxMap[STR_KEY_cfarParamAlpha] = alphaIdx;
    this->strRefIdxMap[STR_KEY_cfarParamBias] = biasIdx;

    this->defaultCfarValue.b.t.enabled = false;
    this->defaultCfarValue.i.t.dim = dimRange;
    this->defaultCfarValue.i.t.type = CFAR_CA;
    this->defaultCfarValue.i.t.guard = 2;
    this->defaultCfarValue.i.t.ref = 4;
    this->defaultCfarValue.f.t.alpha = 1.2;
    this->defaultCfarValue.f.t.bias = 0.;
    this->defaultCfarValue.N = 13;
    this->defaultCfarValue.falseAlarmRate = 0.317;

    this->errFlag = CP_OK;
}

void CFAR::set_default() {
    for (int i = 0; i < cfarDimTotalNums; i++) {
        memcpy(&this->data[i], &this->defaultCfarValue, sizeof(basicCfarData_t));
        this->data[i].i.t.dim = (cfarDim_e) (i);
    }
}

void CFAR::set_single_default(cfarDim_e dim) {
    memcpy(&this->data[dim], &this->defaultCfarValue, sizeof(basicCfarData_t));
    this->data[dim].i.t.dim = dim;
}

void CFAR::cal() {
    for (int i = 0; i < cfarDimTotalNums; i++) {
        if (!this->data[i].b.t.enabled) {
            continue;
        }
        this->data[i].N = 2 * (this->data[i].i.t.guard + this->data[i].i.t.ref) + 1;
        switch (this->data[i].i.t.type) {
            case CFAR_CA:
                false_alarm_rate_ca(&this->data[i]);
                break;
            case CFAR_GO:
                false_alarm_rate_go(&this->data[i]);
                break;
            case CFAR_SO:
                false_alarm_rate_so(&this->data[i]);
                break;
            default:  // CA
                false_alarm_rate_ca(&this->data[i]);
                break;
        }
    }
}

bool CFAR::is_enabled() {
    for (int i = 0; i < cfarDimTotalNums; i++) {
        if (this->data[i].b.t.enabled) {
            return true;
        }
    }
    return false;
}

void CFAR::save(std::ofstream *saveFile) {
    *saveFile << "\n[" << STR_SEC_CFAR << "]\n";
    for (int i = 0; i < cfarDimTotalNums; i++) {
        if (!this->data[i].b.t.enabled) {
            memset(&this->data[i], 0, sizeof(basicCfarData_t));
        }
        *saveFile << this->keyStringsCfarDims[i] << STR_KEY_cfarParamEnabled << " = " << (this->data[i].b.t.enabled ? "yes" : "no") << "\n";
        *saveFile << this->keyStringsCfarDims[i] << STR_KEY_cfarParamDim << " = " << this->data[i].i.t.dim << "\n";
        *saveFile << this->keyStringsCfarDims[i] << STR_KEY_cfarParamType << " = " << this->data[i].i.t.type << "\n";
        *saveFile << this->keyStringsCfarDims[i] << STR_KEY_cfarParamGuard << " = " << this->data[i].i.t.guard << "\n";
        *saveFile << this->keyStringsCfarDims[i] << STR_KEY_cfarParamReference << " = " << this->data[i].i.t.ref << "\n";
        *saveFile << this->keyStringsCfarDims[i] << STR_KEY_cfarParamAlpha << " = " << this->data[i].f.t.alpha << "\n";
        *saveFile << this->keyStringsCfarDims[i] << STR_KEY_cfarParamBias << " = " << this->data[i].f.t.bias << "\n";
    }
}

void CFAR::load(config::RrConfig *cfg, std::string *text) {
    std::string tempStr, current_key;
    tempStr = cfg->ReadString(STR_SEC_CFAR, (std::string(STR_KEY_cfarDimRange) + std::string(STR_KEY_cfarParamEnabled)).c_str(), "");
    if (!tempStr.empty()) {
        for (int i = 0; i < cfarDimTotalNums; i++) {
            for (int j = 0; j < CFAR_SAVED_SETTING_NUMS; j++) {
                current_key = std::string(this->keyStringsCfarDims[i]) + std::string(this->keyStringsCfarParams[j]);
                tempStr = cfg->ReadString(STR_SEC_CFAR, current_key.c_str(), "");
                if (tempStr.empty()) {
                    this->errFlag = CP_ERR;
                    return;
                }
                switch (cfarDataTypeList[j]) {
                    case typeInt:
                        this->data[i].i.v[this->strRefIdxMap[this->keyStringsCfarParams[j]]] = std::stoi(tempStr);
                        break;
                    case typeFloat:
                        this->data[i].f.v[this->strRefIdxMap[this->keyStringsCfarParams[j]]] = std::stof(tempStr);
                        break;
                    case typeBool:
                        this->data[i].b.v[this->strRefIdxMap[this->keyStringsCfarParams[j]]] = tempStr == "yes";
                        break;
                    default:
                        break;
                }
            }
        }
    } else {  // no section, In order to be compatible with older versions
        for (int i = 0; i < cfarDimTotalNums; i++) {
            this->data[i].b.t.enabled = false;
        }
    }
    this->errFlag = CP_OK;
}

CFAR::~CFAR() = default;
