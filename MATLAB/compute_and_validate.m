function [cph] = compute_and_validate(cph)
    cph.computeErrorFlag = cph.CP_ERR;
    if cph.slopePriority == 0
        cph.cp.bandWidth_MHz = cph.cp.slope_MHzus * cph.cp.rampTime_us;
    elseif cph.slopePriority == 1
        cph.cp.slope_MHzus = cph.cp.bandWidth_MHz / cph.cp.rampTime_us;
    end
    cph.cp.lambdaStart_mm = 3e5 / cph.cp.startFrequency_MHz;
    cph.cp.lambdaCenter_mm = 3e5 / (cph.cp.startFrequency_MHz + cph.cp.bandWidth_MHz / 2);
    cph.cp.Tc_us = cph.cp.idleTime_us + cph.cp.rampTime_us;
    cph.cp.TcTDM_us = cph.cp.Tc_us * cph.cp.antTDM;
    cph.cp.Tf_us = cph.cp.TcTDM_us * cph.cp.chirpLoops;
    cph.cp.dutyCycle_percent = cph.cp.Tf_us / cph.cp.periodicity_ms * 1e-1;
    cph.cp.maxADCTime_us = cph.cp.rampTime_us - cph.cp.ADCDelay_us;
    cph.cp.ADCTime_us = cph.cp.ADCPoints / cph.cp.sampleRate_ksps * 1e3;
    cph.cp.ADCBandWidth_MHz = cph.cp.slope_MHzus * cph.cp.ADCTime_us;
    cph.cp.dMax_m = cph.cp.sampleRate_ksps * 15e-2 / cph.cp.slope_MHzus;
    if ~cph.cp.iqSample
        cph.cp.dMax_m = cph.cp.dMax_m / 2;
    end
    cph.cp.dRes_m = 1.5e2 / cph.cp.ADCBandWidth_MHz;
    cph.cp.dResCompute_m = cph.cp.dMax_m / cph.cp.rangeFFTSize;
    if ~cph.cp.iqSample
        cph.cp.dResCompute_m = cph.cp.dResCompute_m * 2;
    end
    cph.cp.vMax_m_s = cph.cp.lambdaCenter_mm / 4 / cph.cp.TcTDM_us * 1e3;
    cph.cp.vRes_m_s = cph.cp.lambdaCenter_mm / 2 / cph.cp.Tf_us * 1e3;
    cph.cp.vResCompute_m_s = cph.cp.vMax_m_s / cph.cp.dopplerFFTSize * 2;
    cph.cp.dopplerSampleRate_sps = 1 / cph.cp.TcTDM_us * 1e6;
    cph.cp.oneMeterIF_kHz = 20 * cph.cp.slope_MHzus / 3;
    cph.cp.oneResBinIF_kHz = cph.cp.oneMeterIF_kHz * cph.cp.dRes_m;
    cph.cp.oneResComputeBinIF_kHz = cph.cp.oneMeterIF_kHz * cph.cp.dResCompute_m;
    cph.cp.maxIF_kHz = cph.cp.oneMeterIF_kHz * cph.cp.dMax_m;
    cph.cp.f32radarCube_kB = cph.cp.antTDM * cph.cp.rx * cph.cp.rangeFFTSize * cph.cp.dopplerFFTSize / 128;

    if cph.cp.dutyCycle_percent > 100
        cph.errMsg = cph.strErr(cph.errMsgIdx.errDutyCycleGT100);
        return
    end

    if cph.cp.maxADCPoints ~= 0
        if cph.cp.ADCPoints > cph.cp.maxADCPoints
            cph.errMsg = cph.strErr(cph.errMsgIdx.errADCPointsGTLimit);
            return
        end
    end

    if cph.cp.maxChirpLoops ~= 0
        if cph.cp.chirpLoops > cph.cp.maxChirpLoops
            cph.errMsg = cph.strErr(cph.errMsgIdx.errChirpLoopsGTLimit);
            return
        end
    end

    if cph.cp.maxRangeFFTSize ~= 0
        if cph.cp.rangeFFTSize > cph.cp.maxRangeFFTSize
            cph.errMsg = cph.strErr(cph.errMsgIdx.errRangeFFTSizeGTLimit);
            return
        end
    end

    if cph.cp.maxDopplerFFTSize ~= 0
        if cph.cp.dopplerFFTSize > cph.cp.maxDopplerFFTSize
            cph.errMsg = cph.strErr(cph.errMsgIdx.errDopplerFFTSizeGTLimit);
            return
        end
    end

    if cph.cp.ADCPoints < cph.cp.minADCPoints
        cph.errMsg = cph.strErr(cph.errMsgIdx.errADCPointsLTLimit);
        return
    end

    if cph.cp.chirpLoops < cph.cp.minChirpLoops
        cph.errMsg = cph.strErr(cph.errMsgIdx.errChirpLoopsLTLimit);
        return
    end

    if cph.cp.rangeFFTSize < cph.cp.minRangeFFTSize
        cph.errMsg = cph.strErr(cph.errMsgIdx.errRangeFFTSizeLTLimit);
        return
    end

    if cph.cp.dopplerFFTSize < cph.cp.minDopplerFFTSize
        cph.errMsg = cph.strErr(cph.errMsgIdx.errDopplerFFTSizeLTLimit);
        return
    end

    if ((cph.cp.maxADCTime_us < 0) || (cph.cp.ADCDelay_us + cph.cp.ADCTime_us > cph.cp.rampTime_us))
        cph.errMsg = cph.strErr(cph.errMsgIdx.errADCDelayTooLong);
        return
    end

    if cph.cp.ADCTime_us > cph.cp.maxADCTime_us
        cph.errMsg = cph.strErr(cph.errMsgIdx.errADCSampleTimeTooLong);
        return
    end

    if ~is2power(cph.cp.rangeFFTSize)
        cph.errMsg = cph.strErr(cph.errMsgIdx.errRangeFFTSIzeNotPower2);
        return
    end

    if cph.cp.rangeFFTSize < cph.cp.ADCPoints
        cph.errMsg = cph.strErr(cph.errMsgIdx.errRangeFFTSizeLTADCPoints);
        return
    end

    if ~is2power(cph.cp.dopplerFFTSize)
        cph.errMsg = cph.strErr(cph.errMsgIdx.errDopplerFFTSIzeNotPower2);
        return
    end

    if cph.cp.dopplerFFTSize < cph.cp.chirpLoops
        cph.errMsg = cph.strErr(cph.errMsgIdx.errDopplerFFTSizeLTChirpLoops);
        return
    end

    cph.errMsg = cph.strErr(cph.errMsgIdx.errNoErr);
    cph.computeErrorFlag = cph.CP_OK;
end

function [res] = is2power(x)
    if x > 0 && floor(x) == x
        res = bitand(x, x - 1) == 0;
    else
        res = false;
    end
end
