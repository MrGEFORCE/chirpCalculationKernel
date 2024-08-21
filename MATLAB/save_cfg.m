function [] = save_cfg(cph, dir)
    cph = compute_and_validate(cph);
    if cph.computeErrorFlag ~= cph.CP_OK
        return
    end

    saveFile = fopen(dir, "w");
    fprintf(saveFile, "[chirp parameters]\r\n");
    fprintf(saveFile, cph.chirpParameterString.startFrequency_MHz + " = " + num2str(cph.cp.startFrequency_MHz, '%.2f') + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.bandWidth_MHz + " = " + num2str(cph.cp.bandWidth_MHz, '%.2f') + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.idleTime_us + " = " + num2str(cph.cp.idleTime_us, '%.2f') + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.rampTime_us + " = " + num2str(cph.cp.rampTime_us, '%.2f') + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.periodicity_ms + " = " + num2str(cph.cp.periodicity_ms, '%.2f') + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.ADCPoints + " = " + num2str(cph.cp.ADCPoints) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.sampleRate_ksps + " = " + num2str(cph.cp.sampleRate_ksps, '%.2f') + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.ADCDelay_us + " = " + num2str(cph.cp.ADCDelay_us, '%.2f') + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.chirpLoops + " = " + num2str(cph.cp.chirpLoops) + "\r\n");
    if cph.cp.iqSample
        fprintf(saveFile, cph.chirpParameterString.iqSample + " = yes\r\n");
    else
        fprintf(saveFile, cph.chirpParameterString.iqSample + " = no\r\n");
    end
    fprintf(saveFile, cph.chirpParameterString.antTDM + " = " + num2str(cph.cp.antTDM) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.rx + " = " + num2str(cph.cp.rx) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.rangeFFTSize + " = " + num2str(cph.cp.rangeFFTSize) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.dopplerFFTSize + " = " + num2str(cph.cp.dopplerFFTSize) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.minADCPoints + " = " + num2str(cph.cp.minADCPoints) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.minChirpLoops + " = " + num2str(cph.cp.minChirpLoops) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.minRangeFFTSize + " = " + num2str(cph.cp.minRangeFFTSize) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.minDopplerFFTSize + " = " + num2str(cph.cp.minDopplerFFTSize) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.maxADCPoints + " = " + num2str(cph.cp.maxADCPoints) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.maxChirpLoops + " = " + num2str(cph.cp.maxChirpLoops) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.maxRangeFFTSize + " = " + num2str(cph.cp.maxRangeFFTSize) + "\r\n");
    fprintf(saveFile, cph.chirpParameterString.maxDopplerFFTSize + " = " + num2str(cph.cp.maxDopplerFFTSize) + "\r\n");
    if cph.cp.staticClutterRemoval
        fprintf(saveFile, cph.chirpParameterString.staticClutterRemoval + " = yes\r\n");
    else
        fprintf(saveFile, cph.chirpParameterString.staticClutterRemoval + " = no\r\n");
    end

    if cph.cp.cfar.cfarEnabled
        fprintf(saveFile, "[cfar parameters]\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarRangeGuard + " = " + num2str(cph.cp.cfar.cfarDopplerBias_dB) + "\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarRangeReference + " = " + num2str(cph.cp.cfar.cfarRangeReference) + "\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarRangeAlpha + " = " + num2str(cph.cp.cfar.cfarRangeAlpha, '%.2f') + "\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarRangeBias_dB + " = " + num2str(cph.cp.cfar.cfarRangeBias_dB, '%.2f') + "\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarDopplerGuard + " = " + num2str(cph.cp.cfar.cfarDopplerGuard) + "\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarDopplerReference + " = " + num2str(cph.cp.cfar.cfarDopplerReference) + "\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarDopplerAlpha + " = " + num2str(cph.cp.cfar.cfarDopplerAlpha, '%.2f') + "\r\n");
        fprintf(saveFile, cph.cfarParameterString.cfarDopplerBias_dB + " = " + num2str(cph.cp.cfar.cfarDopplerBias_dB, '%.2f') + "\r\n");
    end

    fclose(saveFile);
end