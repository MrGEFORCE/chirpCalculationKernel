package main

import (
	"bufio"
	"fmt"
	"github.com/go-ini/ini"
	"os"
)

type statusE bool

const (
	CpErr = false
	CpOk  = true
)

type slopePriorityE int

const (
	spSlopeFirst = iota
	spBandwidthFirst
)

type languageTypeE int

const (
	langZhCNIdx = iota
	langEnUSIdx
)

type strErrE int

const (
	errDutyCycleGT100 = iota
	errADCPointsGTLimit
	errChirpLoopsGTLimit
	errRangeFFTSizeGTLimit
	errDopplerFFTSizeGTLimit
	errADCPointsLTLimit
	errChirpLoopsLTLimit
	errRangeFFTSizeLTLimit
	errDopplerFFTSizeLTLimit
	errADCDelayTooLong
	errADCSampleTimeTooLong
	errRangeFFTSIzeNotPower2
	errRangeFFTSizeLTADCPoints
	errDopplerFFTSIzeNotPower2
	errDopplerFFTSizeLTChirpLoops
	errNoErr
	errFileReadErr
	errStrNums
)

const (
	StrStartFrequencyMHz    = "startFrequency_MHz"
	StrBandWidthMHz         = "bandWidth_MHz"
	StrIdleTimeUs           = "idleTime_us"
	StrRampTimeUs           = "rampTime_us"
	StrPeriodicityMs        = "periodicity_ms"
	StrADCPoints            = "ADCPoints"
	StrSampleRateKsps       = "sampleRate_ksps"
	StrADCDelayUs           = "ADCDelay_us"
	StrChirpLoops           = "chirpLoops"
	StrIqSample             = "iqSample"
	StrAntTDM               = "antTDM"
	StrRx                   = "rx"
	StrRangeFFTSize         = "rangeFFTSize"
	StrDopplerFFTSize       = "dopplerFFTSize"
	StrMinADCPoints         = "minADCPoints"
	StrMinChirpLoops        = "minChirpLoops"
	StrMinRangeFFTSize      = "minRangeFFTSize"
	StrMinDopplerFFTSize    = "minDopplerFFTSize"
	StrMaxADCPoints         = "maxADCPoints"
	StrMaxChirpLoops        = "maxChirpLoops"
	StrMaxRangeFFTSize      = "maxRangeFFTSize"
	StrMaxDopplerFFTSize    = "maxDopplerFFTSize"
	StrStaticClutterRemoval = "staticClutterRemoval"

	StrCfarRangeGuard       = "cfarRangeGuard"
	StrCfarRangeReference   = "cfarRangeReference"
	StrCfarRangeAlpha       = "cfarRangeAlpha"
	StrCfarRangeBiasDb      = "cfarRangeBias_dB"
	StrCfarDopplerGuard     = "cfarDopplerGuard"
	StrCfarDopplerReference = "cfarDopplerReference"
	StrCfarDopplerAlpha     = "cfarDopplerAlpha"
	StrCfarDopplerBiasDb    = "cfarDopplerBias_dB"
	StrCfarEnabled          = "cfarEnabled"
)

type CfarParametersDataT struct {
	cfarRangeGuard       int
	cfarRangeReference   int
	cfarRangeAlpha       float32
	cfarRangeBiasDB      float32
	cfarDopplerGuard     int
	cfarDopplerReference int
	cfarDopplerAlpha     float32
	cfarDopplerBiasDB    float32
	cfarEnabled          bool
}

type ChirpParametersDataT struct {
	CfarParametersDataT   // inherit
	startFrequencyMhz     float32
	bandwidthMhz          float32
	idleTimeUs            float32
	rampTimeUs            float32
	periodicityMs         float32
	ADCPoints             int
	sampleRateKsps        float32
	ADCDelayUs            float32
	chirpLoops            int
	iqSample              bool
	antTDM                int
	rx                    int
	rangeFFTSize          int
	dopplerFFTSize        int
	minADCPoints          int
	minChirpLoops         int
	minRangeFFTSize       int
	minDopplerFFTSize     int
	maxADCPoints          int
	maxChirpLoops         int
	maxRangeFFTSize       int
	maxDopplerFFTSize     int
	staticClutterRemoval  bool
	slopeMhzus            float32
	ADCTimeUs             float32
	ADCBandWidthMHz       float32
	maxADCTimeUs          float32
	lambdaStartMm         float32
	lambdaCenterMm        float32
	dutyCyclePercent      float32
	TcUs                  float32
	TcTDMUs               float32
	TfUs                  float32
	dMaxM                 float32
	dResM                 float32
	dResComputeM          float32
	vMaxMs                float32
	vResMs                float32
	vResComputeMs         float32
	dopplerSampleRateSps  float32
	oneMeterIFKHz         float32
	oneResBinIFKHz        float32
	oneResComputeBinIFKHz float32
	maxIFKHz              float32
	f32radarCubeKB        float32
}

type ChirpParametersHandlerT struct {
	cp                    ChirpParametersDataT
	loadErrorFlag         statusE
	computeErrorFlag      statusE
	errMsg                string
	languageCfgFileFolder string
	strErr                []string
	slopePriority         slopePriorityE
}

func (cph *ChirpParametersHandlerT) Init() {
	cph.strErr = make([]string, 0, errStrNums)
	cph.computeErrorFlag = CpOk
	cph.slopePriority = spBandwidthFirst
}

func (cph *ChirpParametersHandlerT) SetLanguage(lt languageTypeE) {
	cph.loadErrorFlag = CpOk
	var langFile *os.File
	var err error
	switch lt {
	case langZhCNIdx:
		langFile, err = os.Open(cph.languageCfgFileFolder + "/zh_CN.cfg")
		break
	case langEnUSIdx:
		langFile, err = os.Open(cph.languageCfgFileFolder + "/en_US.cfg")
		break
	default:
		langFile, err = os.Open(cph.languageCfgFileFolder + "/en_US.cfg")
		break
	}
	if err != nil {
		cph.loadErrorFlag = CpErr
		cph.errMsg = cph.strErr[errFileReadErr]
		return
	}
	defer langFile.Close()
	scanner := bufio.NewScanner(langFile)
	for i := 0; i < errStrNums; i++ {
		scanner.Scan()
		cph.strErr = append(cph.strErr, scanner.Text())
	}
}

func (cph *ChirpParametersHandlerT) SetToDefault() {
	cph.cp.minADCPoints = 0
	cph.cp.minChirpLoops = 0
	cph.cp.minRangeFFTSize = 0
	cph.cp.minDopplerFFTSize = 0
	cph.cp.maxADCPoints = 0
	cph.cp.maxChirpLoops = 0
	cph.cp.maxRangeFFTSize = 0
	cph.cp.maxDopplerFFTSize = 0
	cph.cp.staticClutterRemoval = true

	cph.cp.startFrequencyMhz = 77e3
	cph.cp.bandwidthMhz = 3e3
	cph.cp.idleTimeUs = 20
	cph.cp.rampTimeUs = 100
	cph.cp.periodicityMs = 10
	cph.cp.ADCPoints = 256
	cph.cp.sampleRateKsps = 3000
	cph.cp.ADCDelayUs = 10
	cph.cp.chirpLoops = 64
	cph.cp.iqSample = true
	cph.cp.antTDM = 1
	cph.cp.rx = 1
	cph.cp.rangeFFTSize = cph.cp.ADCPoints
	cph.cp.dopplerFFTSize = cph.cp.chirpLoops
	cph.cp.cfarEnabled = false
	cph.ComputeAndValidate()
}

func (cph *ChirpParametersHandlerT) ComputeAndValidate() {
	cph.computeErrorFlag = CpErr
	if cph.slopePriority == spSlopeFirst {
		cph.cp.bandwidthMhz = cph.cp.slopeMhzus * cph.cp.rampTimeUs
	} else if cph.slopePriority == spBandwidthFirst {
		cph.cp.slopeMhzus = cph.cp.bandwidthMhz / cph.cp.rampTimeUs
	}
	cph.cp.lambdaStartMm = 3e5 / cph.cp.startFrequencyMhz
	cph.cp.lambdaCenterMm = 3e5 / (cph.cp.startFrequencyMhz + cph.cp.bandwidthMhz/2)
	cph.cp.TcUs = cph.cp.idleTimeUs + cph.cp.rampTimeUs
	cph.cp.TcTDMUs = cph.cp.TcUs * float32(cph.cp.antTDM)
	cph.cp.TfUs = cph.cp.TcTDMUs * float32(cph.cp.chirpLoops)
	cph.cp.dutyCyclePercent = cph.cp.TfUs / cph.cp.periodicityMs * 1e-1
	cph.cp.maxADCTimeUs = cph.cp.rampTimeUs - cph.cp.ADCDelayUs
	cph.cp.ADCTimeUs = float32(cph.cp.ADCPoints) / cph.cp.sampleRateKsps * 1e3
	cph.cp.ADCBandWidthMHz = cph.cp.slopeMhzus * cph.cp.ADCTimeUs
	cph.cp.dMaxM = cph.cp.sampleRateKsps * 15e-2 / cph.cp.slopeMhzus
	if !cph.cp.iqSample {
		cph.cp.dMaxM /= 2
	}
	cph.cp.dResM = 1.5e2 / cph.cp.ADCBandWidthMHz
	cph.cp.dResComputeM = cph.cp.dMaxM / float32(cph.cp.rangeFFTSize)
	if !cph.cp.iqSample {
		cph.cp.dResComputeM *= 2
	}
	cph.cp.vMaxMs = cph.cp.lambdaCenterMm / 4 / cph.cp.TcTDMUs * 1e3
	cph.cp.vResMs = cph.cp.lambdaCenterMm / 2 / cph.cp.TfUs * 1e3
	cph.cp.vResComputeMs = cph.cp.vMaxMs / float32(cph.cp.dopplerFFTSize) * 2
	cph.cp.dopplerSampleRateSps = 1 / cph.cp.TcTDMUs * 1e6
	cph.cp.oneMeterIFKHz = 20 * cph.cp.slopeMhzus / 3
	cph.cp.oneResBinIFKHz = cph.cp.oneMeterIFKHz * cph.cp.dResM
	cph.cp.oneResComputeBinIFKHz = cph.cp.oneMeterIFKHz * cph.cp.dResComputeM
	cph.cp.maxIFKHz = cph.cp.oneMeterIFKHz * cph.cp.dMaxM
	cph.cp.f32radarCubeKB = float32(cph.cp.antTDM*cph.cp.rx*cph.cp.rangeFFTSize*cph.cp.dopplerFFTSize) / 128

	if cph.cp.dutyCyclePercent > 100 {
		cph.errMsg = cph.strErr[errDutyCycleGT100]
		return
	}

	if cph.cp.maxADCPoints != 0 {
		if cph.cp.ADCPoints > cph.cp.maxADCPoints {
			cph.errMsg = cph.strErr[errADCPointsGTLimit]
			return
		}
	}

	if cph.cp.maxChirpLoops != 0 {
		if cph.cp.chirpLoops > cph.cp.maxChirpLoops {
			cph.errMsg = cph.strErr[errChirpLoopsGTLimit]
			return
		}
	}

	if cph.cp.maxRangeFFTSize != 0 {
		if cph.cp.rangeFFTSize > cph.cp.maxRangeFFTSize {
			cph.errMsg = cph.strErr[errRangeFFTSizeGTLimit]
			return
		}
	}

	if cph.cp.maxDopplerFFTSize != 0 {
		if cph.cp.dopplerFFTSize > cph.cp.maxDopplerFFTSize {
			cph.errMsg = cph.strErr[errDopplerFFTSizeGTLimit]
			return
		}
	}

	if cph.cp.ADCPoints < cph.cp.minADCPoints {
		cph.errMsg = cph.strErr[errADCPointsLTLimit]
		return
	}

	if cph.cp.chirpLoops < cph.cp.minChirpLoops {
		cph.errMsg = cph.strErr[errChirpLoopsLTLimit]
		return
	}

	if cph.cp.rangeFFTSize < cph.cp.minRangeFFTSize {
		cph.errMsg = cph.strErr[errRangeFFTSizeLTLimit]
		return
	}

	if cph.cp.dopplerFFTSize < cph.cp.minDopplerFFTSize {
		cph.errMsg = cph.strErr[errDopplerFFTSizeLTLimit]
		return
	}

	if cph.cp.maxADCTimeUs < 0 || cph.cp.ADCDelayUs+cph.cp.ADCTimeUs > cph.cp.rampTimeUs {
		cph.errMsg = cph.strErr[errADCDelayTooLong]
		return
	}

	if cph.cp.ADCTimeUs > cph.cp.maxADCTimeUs {
		cph.errMsg = cph.strErr[errADCSampleTimeTooLong]
		return
	}

	if !is2power(cph.cp.rangeFFTSize) {
		cph.errMsg = cph.strErr[errRangeFFTSIzeNotPower2]
		return
	}

	if cph.cp.rangeFFTSize < cph.cp.ADCPoints {
		cph.errMsg = cph.strErr[errRangeFFTSizeLTADCPoints]
		return
	}

	if !is2power(cph.cp.dopplerFFTSize) {
		cph.errMsg = cph.strErr[errDopplerFFTSIzeNotPower2]
		return
	}

	if cph.cp.dopplerFFTSize < cph.cp.chirpLoops {
		cph.errMsg = cph.strErr[errDopplerFFTSizeLTChirpLoops]
		return
	}

	cph.errMsg = cph.strErr[errNoErr]
	cph.computeErrorFlag = CpOk
}

func (cph *ChirpParametersHandlerT) SaveCfg(dir string) {
	cph.ComputeAndValidate()
	if cph.computeErrorFlag == CpErr {
		return
	}
	saveFile := ini.Empty()
	cpSec, _ := saveFile.NewSection("chirp parameters")
	cpSec.NewKey(StrStartFrequencyMHz, fmt.Sprintf("%.2f", cph.cp.startFrequencyMhz))
	cpSec.NewKey(StrBandWidthMHz, fmt.Sprintf("%.2f", cph.cp.bandwidthMhz))
	cpSec.NewKey(StrIdleTimeUs, fmt.Sprintf("%.2f", cph.cp.idleTimeUs))
	cpSec.NewKey(StrRampTimeUs, fmt.Sprintf("%.2f", cph.cp.rampTimeUs))
	cpSec.NewKey(StrPeriodicityMs, fmt.Sprintf("%.2f", cph.cp.periodicityMs))
	cpSec.NewKey(StrADCPoints, fmt.Sprintf("%d", cph.cp.ADCPoints))
	cpSec.NewKey(StrSampleRateKsps, fmt.Sprintf("%.2f", cph.cp.sampleRateKsps))
	cpSec.NewKey(StrADCDelayUs, fmt.Sprintf("%.2f", cph.cp.ADCDelayUs))
	cpSec.NewKey(StrChirpLoops, fmt.Sprintf("%d", cph.cp.chirpLoops))
	if cph.cp.iqSample {
		cpSec.NewKey(StrIqSample, "yes")
	} else {
		cpSec.NewKey(StrIqSample, "no")
	}
	cpSec.NewKey(StrAntTDM, fmt.Sprintf("%d", cph.cp.antTDM))
	cpSec.NewKey(StrRx, fmt.Sprintf("%d", cph.cp.rx))
	cpSec.NewKey(StrRangeFFTSize, fmt.Sprintf("%d", cph.cp.rangeFFTSize))
	cpSec.NewKey(StrDopplerFFTSize, fmt.Sprintf("%d", cph.cp.dopplerFFTSize))
	cpSec.NewKey(StrMinADCPoints, fmt.Sprintf("%d", cph.cp.minADCPoints))
	cpSec.NewKey(StrMinChirpLoops, fmt.Sprintf("%d", cph.cp.minChirpLoops))
	cpSec.NewKey(StrMinRangeFFTSize, fmt.Sprintf("%d", cph.cp.minRangeFFTSize))
	cpSec.NewKey(StrMinDopplerFFTSize, fmt.Sprintf("%d", cph.cp.minDopplerFFTSize))
	cpSec.NewKey(StrMaxADCPoints, fmt.Sprintf("%d", cph.cp.maxADCPoints))
	cpSec.NewKey(StrMaxChirpLoops, fmt.Sprintf("%d", cph.cp.maxChirpLoops))
	cpSec.NewKey(StrMaxRangeFFTSize, fmt.Sprintf("%d", cph.cp.maxRangeFFTSize))
	cpSec.NewKey(StrMaxDopplerFFTSize, fmt.Sprintf("%d", cph.cp.maxDopplerFFTSize))
	if cph.cp.staticClutterRemoval {
		cpSec.NewKey(StrStaticClutterRemoval, "yes")
	} else {
		cpSec.NewKey(StrStaticClutterRemoval, "no")
	}

	if cph.cp.cfarEnabled {
		cfSec, _ := saveFile.NewSection("cfar parameters")
		cfSec.NewKey(StrCfarRangeGuard, fmt.Sprintf("%d", cph.cp.cfarRangeGuard))
		cfSec.NewKey(StrCfarRangeReference, fmt.Sprintf("%df", cph.cp.cfarRangeReference))
		cfSec.NewKey(StrCfarRangeAlpha, fmt.Sprintf("%.2f", cph.cp.cfarRangeAlpha))
		cfSec.NewKey(StrCfarRangeBiasDb, fmt.Sprintf("%.2f", cph.cp.cfarRangeBiasDB))
		cfSec.NewKey(StrCfarDopplerGuard, fmt.Sprintf("%d", cph.cp.cfarDopplerGuard))
		cfSec.NewKey(StrCfarDopplerReference, fmt.Sprintf("%d", cph.cp.cfarDopplerReference))
		cfSec.NewKey(StrCfarDopplerAlpha, fmt.Sprintf("%.2f", cph.cp.cfarDopplerAlpha))
		cfSec.NewKey(StrCfarDopplerBiasDb, fmt.Sprintf("%.2f", cph.cp.cfarDopplerBiasDB))
	}
	saveFile.SaveTo(dir)
}

func (cph *ChirpParametersHandlerT) LoadCfg(dir string) {
	loadFile, err := ini.Load(dir)
	if err != nil {
		cph.loadErrorFlag = CpErr
		cph.errMsg = cph.strErr[errFileReadErr]
		return
	}
	var float64Temp float64
	var intTemp int
	var stringTemp string
	float64Temp, err = loadFile.Section("chirp parameters").Key(StrStartFrequencyMHz).Float64()
	cph.cp.startFrequencyMhz = float32(float64Temp)
	float64Temp, err = loadFile.Section("chirp parameters").Key(StrBandWidthMHz).Float64()
	cph.cp.bandwidthMhz = float32(float64Temp)
	float64Temp, err = loadFile.Section("chirp parameters").Key(StrIdleTimeUs).Float64()
	cph.cp.idleTimeUs = float32(float64Temp)
	float64Temp, err = loadFile.Section("chirp parameters").Key(StrRampTimeUs).Float64()
	cph.cp.rampTimeUs = float32(float64Temp)
	float64Temp, err = loadFile.Section("chirp parameters").Key(StrPeriodicityMs).Float64()
	cph.cp.periodicityMs = float32(float64Temp)
	intTemp, err = loadFile.Section("chirp parameters").Key(StrADCPoints).Int()
	cph.cp.ADCPoints = intTemp
	float64Temp, err = loadFile.Section("chirp parameters").Key(StrSampleRateKsps).Float64()
	cph.cp.sampleRateKsps = float32(float64Temp)
	float64Temp, err = loadFile.Section("chirp parameters").Key(StrADCDelayUs).Float64()
	cph.cp.ADCDelayUs = float32(float64Temp)
	intTemp, err = loadFile.Section("chirp parameters").Key(StrChirpLoops).Int()
	cph.cp.chirpLoops = intTemp
	stringTemp = loadFile.Section("chirp parameters").Key(StrIqSample).String()
	if stringTemp == "yes" {
		cph.cp.iqSample = true
	} else {
		cph.cp.iqSample = false
	}
	intTemp, err = loadFile.Section("chirp parameters").Key(StrAntTDM).Int()
	cph.cp.antTDM = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrRx).Int()
	cph.cp.rx = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrRangeFFTSize).Int()
	cph.cp.rangeFFTSize = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrDopplerFFTSize).Int()
	cph.cp.dopplerFFTSize = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMinADCPoints).Int()
	cph.cp.minADCPoints = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMinChirpLoops).Int()
	cph.cp.minChirpLoops = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMinRangeFFTSize).Int()
	cph.cp.minRangeFFTSize = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMinDopplerFFTSize).Int()
	cph.cp.minDopplerFFTSize = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMaxADCPoints).Int()
	cph.cp.maxADCPoints = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMaxChirpLoops).Int()
	cph.cp.maxChirpLoops = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMaxRangeFFTSize).Int()
	cph.cp.maxRangeFFTSize = intTemp
	intTemp, err = loadFile.Section("chirp parameters").Key(StrMaxDopplerFFTSize).Int()
	cph.cp.maxDopplerFFTSize = intTemp
	stringTemp = loadFile.Section("chirp parameters").Key(StrStaticClutterRemoval).String()
	if stringTemp == "yes" {
		cph.cp.staticClutterRemoval = true
	} else {
		cph.cp.staticClutterRemoval = false
	}

	cph.cp.cfarEnabled = false
	stringTemp = loadFile.Section("cfar parameters").Key(StrCfarRangeGuard).String()
	if len(stringTemp) > 0 {
		intTemp, err = loadFile.Section("cfar parameters").Key(StrCfarRangeGuard).Int()
		cph.cp.cfarRangeGuard = intTemp
		intTemp, err = loadFile.Section("cfar parameters").Key(StrCfarRangeReference).Int()
		cph.cp.cfarRangeReference = intTemp
		float64Temp, err = loadFile.Section("cfar parameters").Key(StrCfarRangeAlpha).Float64()
		cph.cp.cfarRangeAlpha = float32(float64Temp)
		float64Temp, err = loadFile.Section("cfar parameters").Key(StrCfarRangeBiasDb).Float64()
		cph.cp.cfarRangeBiasDB = float32(float64Temp)
		intTemp, err = loadFile.Section("cfar parameters").Key(StrCfarDopplerGuard).Int()
		cph.cp.cfarDopplerGuard = intTemp
		intTemp, err = loadFile.Section("cfar parameters").Key(StrCfarDopplerReference).Int()
		cph.cp.cfarDopplerReference = intTemp
		float64Temp, err = loadFile.Section("cfar parameters").Key(StrCfarDopplerAlpha).Float64()
		cph.cp.cfarDopplerAlpha = float32(float64Temp)
		float64Temp, err = loadFile.Section("cfar parameters").Key(StrCfarDopplerBiasDb).Float64()
		cph.cp.cfarDopplerBiasDB = float32(float64Temp)
		cph.cp.cfarEnabled = true
	}
	cph.ComputeAndValidate()
}

func is2power(x int) bool {
	return x > 0 && (x&(x-1)) == 0
}
