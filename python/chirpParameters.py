import configparser
from dataclasses import dataclass

CP_ERR = False
CP_OK = True

SP_SLOPE_FIRST = 0
SP_BANDWIDTH_FIRST = 1

LANG_ZH_CN = 0
LANG_EN_US = 1

LANGUAGE_DIC = {LANG_ZH_CN: "zh_CN.cfg", LANG_EN_US: "en_US.cfg", }


@dataclass
class StrErrIdxEnum:
    errDutyCycleGT100: int = 0
    errADCPointsGTLimit: int = 1
    errChirpLoopsGTLimit: int = 2
    errRangeFFTSizeGTLimit: int = 3
    errDopplerFFTSizeGTLimit: int = 4
    errADCPointsLTLimit: int = 5
    errChirpLoopsLTLimit: int = 6
    errRangeFFTSizeLTLimit: int = 7
    errDopplerFFTSizeLTLimit: int = 8
    errADCDelayTooLong: int = 9
    errADCSampleTimeTooLong: int = 10
    errRangeFFTSIzeNotPower2: int = 11
    errRangeFFTSizeLTADCPoints: int = 12
    errDopplerFFTSIzeNotPower2: int = 13
    errDopplerFFTSizeLTChirpLoops: int = 14
    errNoErr: int = 15
    errFileReadErr: int = 16
    errStrNums: int = 17


# for save and load .cfg file
@dataclass
class CfarParameterString:
    cfarRangeGuard: str = "cfarRangeGuard"
    cfarRangeReference: str = "cfarRangeReference"
    cfarRangeAlpha: str = "cfarRangeAlpha"
    cfarRangeBias_dB: str = "cfarRangeBias_dB"
    cfarDopplerGuard: str = "cfarDopplerGuard"
    cfarDopplerReference: str = "cfarDopplerReference"
    cfarDopplerAlpha: str = "cfarDopplerAlpha"
    cfarDopplerBias_dB: str = "cfarDopplerBias_dB"
    cfarEnabled: str = "cfarEnabled"


@dataclass
class ChirpParameterString(CfarParameterString):
    startFrequency_MHz: str = "startFrequency_MHz"
    bandWidth_MHz: str = "bandWidth_MHz"
    idleTime_us: str = "idleTime_us"
    rampTime_us: str = "rampTime_us"
    periodicity_ms: str = "periodicity_ms"
    ADCPoints: str = "ADCPoints"
    sampleRate_ksps: str = "sampleRate_ksps"
    ADCDelay_us: str = "ADCDelay_us"
    chirpLoops: str = "chirpLoops"
    iqSample: str = "iqSample"
    antTDM: str = "antTDM"
    rx: str = "rx"
    rangeFFTSize: str = "rangeFFTSize"
    dopplerFFTSize: str = "dopplerFFTSize"
    minADCPoints: str = "minADCPoints"
    minChirpLoops: str = "minChirpLoops"
    minRangeFFTSize: str = "minRangeFFTSize"
    minDopplerFFTSize: str = "minDopplerFFTSize"
    maxADCPoints: str = "maxADCPoints"
    maxChirpLoops: str = "maxChirpLoops"
    maxRangeFFTSize: str = "maxRangeFFTSize"
    maxDopplerFFTSize: str = "maxDopplerFFTSize"
    staticClutterRemoval: str = "staticClutterRemoval"


@dataclass
class CfarParameterData:
    cfarRangeGuard: int
    cfarRangeReference: int
    cfarRangeAlpha: float
    cfarRangeBias_dB: float
    cfarDopplerGuard: int
    cfarDopplerReference: int
    cfarDopplerAlpha: float
    cfarDopplerBias_dB: float
    cfarEnabled: bool


@dataclass
class ChirpParameterData(CfarParameterData):
    # front-end parameters
    startFrequency_MHz: float
    bandWidth_MHz: float
    idleTime_us: float
    rampTime_us: float
    periodicity_ms: float
    ADCPoints: int
    sampleRate_ksps: float
    ADCDelay_us: float
    chirpLoops: int
    iqSample: bool
    antTDM: int
    rx: int

    # processing parameters
    rangeFFTSize: int
    dopplerFFTSize: int

    minADCPoints: int  # opt
    minChirpLoops: int  # opt
    minRangeFFTSize: int  # opt
    minDopplerFFTSize: int  # opt
    maxADCPoints: int  # opt
    maxChirpLoops: int  # opt
    maxRangeFFTSize: int  # opt
    maxDopplerFFTSize: int  # opt
    staticClutterRemoval: bool  # opt

    # performance parameters
    slope_MHzus: float
    ADCTime_us: float
    ADCBandWidth_MHz: float
    maxADCTime_us: float
    lambdaStart_mm: float
    lambdaCenter_mm: float
    dutyCycle_percent: float
    Tc_us: float
    TcTDM_us: float
    Tf_us: float

    dMax_m: float
    dRes_m: float
    dResCompute_m: float
    vMax_m_s: float
    vRes_m_s: float
    vResCompute_m_s: float
    dopplerSampleRate_sps: float
    oneMeterIF_kHz: float
    oneResBinIF_kHz: float
    oneResComputeBinIF_kHz: float
    maxIF_kHz: float
    f32radarCube_kB: float


class ChirpParameterHandler(ChirpParameterData):
    def __init__(self):
        self.errMsg = ""
        self.errFlag = CP_OK
        self.config = configparser.ConfigParser()
        self.config.optionxform = str
        self.slopePriority = SP_BANDWIDTH_FIRST

        self.languageCfgFileFolder = ""
        self.strErr = []

    @staticmethod
    def is2power(x: int) -> bool:
        return x > 0 and (x & (x - 1)) == 0

    def setLanguage(self, idx: int):
        lines = open(self.languageCfgFileFolder + "/" + LANGUAGE_DIC[idx], encoding='utf-8').readlines()
        self.strErr.clear()
        for i in range(int(StrErrIdxEnum.errStrNums)):
            self.strErr.append(lines[i][:-1])  # discard "\n"

    def set_default(self) -> None:
        self.minADCPoints = 0
        self.minChirpLoops = 0
        self.minRangeFFTSize = 0
        self.minDopplerFFTSize = 0
        self.maxADCPoints = 0  # unlimited
        self.maxChirpLoops = 0
        self.maxRangeFFTSize = 0
        self.maxDopplerFFTSize = 0
        self.staticClutterRemoval = True

        self.startFrequency_MHz = 77e3  # The expression of Scientific notation will automatically become a floating point number
        self.bandWidth_MHz = 3e3
        self.idleTime_us = 20.
        self.rampTime_us = 100.
        self.periodicity_ms = 10.
        self.ADCPoints = 256
        self.sampleRate_ksps = 3000.
        self.ADCDelay_us = 10.
        self.chirpLoops = 64
        self.iqSample = True
        self.antTDM = 1
        self.rx = 1

        self.rangeFFTSize = self.ADCPoints
        self.dopplerFFTSize = self.chirpLoops
        self.cfarEnabled = False
        self.compute_and_validate()

    def compute_and_validate(self) -> None:
        self.errFlag = CP_ERR
        if self.slopePriority == SP_SLOPE_FIRST:
            self.bandWidth_MHz = self.slope_MHzus * self.rampTime_us
        elif self.slopePriority == SP_BANDWIDTH_FIRST:
            self.slope_MHzus = self.bandWidth_MHz / self.rampTime_us
        self.lambdaStart_mm = 3e5 / self.startFrequency_MHz
        self.lambdaCenter_mm = 3e5 / (self.startFrequency_MHz + self.bandWidth_MHz / 2)

        self.Tc_us = self.idleTime_us + self.rampTime_us
        self.TcTDM_us = self.Tc_us * self.antTDM
        self.Tf_us = self.TcTDM_us * self.chirpLoops
        self.dutyCycle_percent = self.Tf_us / self.periodicity_ms * 1e-1
        self.maxADCTime_us = self.rampTime_us - self.ADCDelay_us
        self.ADCTime_us = self.ADCPoints / self.sampleRate_ksps * 1e3
        self.ADCBandWidth_MHz = self.slope_MHzus * self.ADCTime_us
        self.dMax_m = self.sampleRate_ksps * 15e-2 / self.slope_MHzus
        if not self.iqSample:
            self.dMax_m /= 2
        self.dRes_m = 1.5e2 / self.ADCBandWidth_MHz
        self.dResCompute_m = self.dMax_m / self.rangeFFTSize
        if not self.iqSample:
            self.dResCompute_m *= 2
        self.vMax_m_s = self.lambdaCenter_mm / 4 / self.TcTDM_us * 1e3
        self.vRes_m_s = self.lambdaCenter_mm / 2 / self.Tf_us * 1e3
        self.vResCompute_m_s = self.vMax_m_s / self.dopplerFFTSize * 2
        self.dopplerSampleRate_sps = 1 / self.TcTDM_us * 1e6
        self.oneMeterIF_kHz = 20 * self.slope_MHzus / 3
        self.oneResBinIF_kHz = self.oneMeterIF_kHz * self.dRes_m
        self.oneResComputeBinIF_kHz = self.oneMeterIF_kHz * self.dResCompute_m
        self.maxIF_kHz = self.oneMeterIF_kHz * self.dMax_m
        self.f32radarCube_kB = self.antTDM * self.rx * self.rangeFFTSize * self.dopplerFFTSize / 128.

        if self.dutyCycle_percent > 100:
            self.errMsg = self.strErr[StrErrIdxEnum.errDutyCycleGT100]
            return

        if self.maxADCPoints != 0:
            if self.ADCPoints > self.maxADCPoints:
                self.errMsg = self.strErr[StrErrIdxEnum.errADCPointsGTLimit]
                return

        if self.maxChirpLoops != 0:
            if self.chirpLoops > self.maxChirpLoops:
                self.errMsg = self.strErr[StrErrIdxEnum.errChirpLoopsGTLimit]
                return

        if self.maxRangeFFTSize != 0:
            if self.rangeFFTSize > self.maxRangeFFTSize:
                self.errMsg = self.strErr[StrErrIdxEnum.errRangeFFTSizeGTLimit]
                return

        if self.maxDopplerFFTSize != 0:
            if self.dopplerFFTSize > self.maxDopplerFFTSize:
                self.errMsg = self.strErr[StrErrIdxEnum.errDopplerFFTSizeGTLimit]
                return

        if self.ADCPoints < self.minADCPoints:
            self.errMsg = self.strErr[StrErrIdxEnum.errADCPointsLTLimit]
            return

        if self.chirpLoops < self.minChirpLoops:
            self.errMsg = self.strErr[StrErrIdxEnum.errChirpLoopsLTLimit]
            return

        if self.rangeFFTSize < self.minRangeFFTSize:
            self.errMsg = self.strErr[StrErrIdxEnum.errRangeFFTSizeLTLimit]
            return

        if self.dopplerFFTSize < self.minDopplerFFTSize:
            self.errMsg = self.strErr[StrErrIdxEnum.errDopplerFFTSizeLTLimit]
            return

        if self.maxADCTime_us < 0 or (self.ADCDelay_us + self.ADCTime_us > self.rampTime_us):
            self.errMsg = self.strErr[StrErrIdxEnum.errADCDelayTooLong]
            return

        if self.ADCTime_us > self.maxADCTime_us:
            self.errMsg = self.strErr[StrErrIdxEnum.errADCSampleTimeTooLong]
            return

        if not self.is2power(self.rangeFFTSize):
            self.errMsg = self.strErr[StrErrIdxEnum.errRangeFFTSIzeNotPower2]
            return

        if self.rangeFFTSize < self.ADCPoints:
            self.errMsg = self.strErr[StrErrIdxEnum.errRangeFFTSizeLTADCPoints]
            return

        if not self.is2power(self.dopplerFFTSize):
            self.errMsg = self.strErr[StrErrIdxEnum.errDopplerFFTSIzeNotPower2]
            return

        if self.dopplerFFTSize < self.chirpLoops:
            self.errMsg = self.strErr[StrErrIdxEnum.errDopplerFFTSizeLTChirpLoops]
            return

        self.errMsg = self.strErr[StrErrIdxEnum.errNoErr]
        self.errFlag = CP_OK

    def gen_description(self) -> str:
        s = "[chirp parameters]\n"
        s += ChirpParameterString.startFrequency_MHz + " = " + str("{:.2f}".format(self.startFrequency_MHz)) + "\n"
        s += ChirpParameterString.bandWidth_MHz + " = " + str("{:.2f}".format(self.bandWidth_MHz)) + "\n"
        s += ChirpParameterString.idleTime_us + " = " + str("{:.2f}".format(self.idleTime_us)) + "\n"
        s += ChirpParameterString.rampTime_us + " = " + str("{:.2f}".format(self.rampTime_us)) + "\n"
        s += ChirpParameterString.periodicity_ms + " = " + str("{:.2f}".format(self.periodicity_ms)) + "\n"
        s += ChirpParameterString.ADCPoints + " = " + str(self.ADCPoints) + "\n"
        s += ChirpParameterString.sampleRate_ksps + " = " + str("{:.2f}".format(self.sampleRate_ksps)) + "\n"
        s += ChirpParameterString.ADCDelay_us + " = " + str("{:.2f}".format(self.ADCDelay_us)) + "\n"
        s += ChirpParameterString.chirpLoops + " = " + str(self.chirpLoops) + "\n"
        s += ChirpParameterString.iqSample + " = " + ("yes" if self.iqSample else "no") + "\n"
        s += ChirpParameterString.antTDM + " = " + str(self.antTDM) + "\n"
        s += ChirpParameterString.rx + " = " + str(self.rx) + "\n"
        s += ChirpParameterString.rangeFFTSize + " = " + str(self.rangeFFTSize) + "\n"
        s += ChirpParameterString.dopplerFFTSize + " = " + str(self.dopplerFFTSize) + "\n"
        s += ChirpParameterString.minADCPoints + " = " + str(self.minADCPoints) + "\n"
        s += ChirpParameterString.minChirpLoops + " = " + str(self.minChirpLoops) + "\n"
        s += ChirpParameterString.minRangeFFTSize + " = " + str(self.minRangeFFTSize) + "\n"
        s += ChirpParameterString.minDopplerFFTSize + " = " + str(self.minDopplerFFTSize) + "\n"
        s += ChirpParameterString.maxADCPoints + " = " + str(self.maxADCPoints) + "\n"
        s += ChirpParameterString.maxChirpLoops + " = " + str(self.maxChirpLoops) + "\n"
        s += ChirpParameterString.maxRangeFFTSize + " = " + str(self.maxRangeFFTSize) + "\n"
        s += ChirpParameterString.maxDopplerFFTSize + " = " + str(self.maxDopplerFFTSize) + "\n"
        s += ChirpParameterString.staticClutterRemoval + " = " + ("yes" if self.staticClutterRemoval else "no") + "\n"
        if self.cfarEnabled:
            s += "[cfar parameters]\n"
            s += ChirpParameterString.cfarRangeGuard + " = " + str(self.cfarRangeGuard) + "\n"
            s += ChirpParameterString.cfarRangeReference + " = " + str(self.cfarRangeReference) + "\n"
            s += ChirpParameterString.cfarRangeAlpha + " = " + str("{:.2f}".format(self.cfarRangeAlpha)) + "\n"
            s += ChirpParameterString.cfarRangeBias_dB + " = " + str("{:.2f}".format(self.cfarRangeBias_dB)) + "\n"
            s += ChirpParameterString.cfarDopplerGuard + " = " + str(self.cfarDopplerGuard) + "\n"
            s += ChirpParameterString.cfarDopplerReference + " = " + str(self.cfarDopplerReference) + "\n"
            s += ChirpParameterString.cfarDopplerAlpha + " = " + str("{:.2f}".format(self.cfarDopplerAlpha)) + "\n"
            s += ChirpParameterString.cfarDopplerBias_dB + " = " + str("{:.2f}".format(self.cfarDopplerBias_dB)) + "\n"
        return s

    def save_cfg(self, directory: str) -> None:
        self.compute_and_validate()
        if not self.errFlag:
            return

        self.config["chirp parameters"] = {
            ChirpParameterString.startFrequency_MHz: str("{:.2f}".format(self.startFrequency_MHz)),
            ChirpParameterString.bandWidth_MHz: str("{:.2f}".format(self.bandWidth_MHz)),
            ChirpParameterString.idleTime_us: str("{:.2f}".format(self.idleTime_us)),
            ChirpParameterString.rampTime_us: str("{:.2f}".format(self.rampTime_us)),
            ChirpParameterString.periodicity_ms: str("{:.2f}".format(self.periodicity_ms)),
            ChirpParameterString.ADCPoints: str(self.ADCPoints),
            ChirpParameterString.sampleRate_ksps: str("{:.2f}".format(self.sampleRate_ksps)),
            ChirpParameterString.ADCDelay_us: str("{:.2f}".format(self.ADCDelay_us)),
            ChirpParameterString.chirpLoops: str(self.chirpLoops),
            ChirpParameterString.iqSample: "yes" if self.iqSample else "no",
            ChirpParameterString.antTDM: str(self.antTDM),
            ChirpParameterString.rx: str(self.rx),
            ChirpParameterString.rangeFFTSize: str(self.rangeFFTSize),
            ChirpParameterString.dopplerFFTSize: str(self.dopplerFFTSize),
            ChirpParameterString.minADCPoints: str(self.minADCPoints),
            ChirpParameterString.minChirpLoops: str(self.minChirpLoops),
            ChirpParameterString.minRangeFFTSize: str(self.minRangeFFTSize),
            ChirpParameterString.minDopplerFFTSize: str(self.minDopplerFFTSize),
            ChirpParameterString.maxADCPoints: str(self.maxADCPoints),
            ChirpParameterString.maxChirpLoops: str(self.maxChirpLoops),
            ChirpParameterString.maxRangeFFTSize: str(self.maxRangeFFTSize),
            ChirpParameterString.maxDopplerFFTSize: str(self.maxDopplerFFTSize),
            ChirpParameterString.staticClutterRemoval: "yes" if self.staticClutterRemoval else "no"
        }
        if self.cfarEnabled:
            self.config["cfar parameters"] = {
                ChirpParameterString.cfarRangeGuard: str(self.cfarRangeGuard),
                ChirpParameterString.cfarRangeReference: str(self.cfarRangeReference),
                ChirpParameterString.cfarRangeAlpha: str("{:.2f}".format(self.cfarRangeAlpha)),
                ChirpParameterString.cfarRangeBias_dB: str("{:.2f}".format(self.cfarRangeBias_dB)),
                ChirpParameterString.cfarDopplerGuard: str(self.cfarDopplerGuard),
                ChirpParameterString.cfarDopplerReference: str(self.cfarDopplerReference),
                ChirpParameterString.cfarDopplerAlpha: str("{:.2f}".format(self.cfarDopplerAlpha)),
                ChirpParameterString.cfarDopplerBias_dB: str("{:.2f}".format(self.cfarDopplerBias_dB))
            }
        with open(directory, 'w') as configfile:
            self.config.write(configfile)

    def load_cfg(self, directory: str) -> None:
        self.config.read(directory)
        sections = self.config.sections()
        for sec in sections:
            for key in self.config[sec]:
                try:
                    if type(self.__dict__[key]) is int:
                        self.__dict__[key] = int(self.config[sec][key])
                    elif type(self.__dict__[key]) is float:
                        self.__dict__[key] = float(self.config[sec][key])
                    elif type(self.__dict__[key]) is bool:
                        self.__dict__[key] = True if self.config[sec][key] == "yes" else False
                except KeyError:
                    self.errMsg = self.strErr[StrErrIdxEnum.errFileReadErr]
        self.compute_and_validate()
