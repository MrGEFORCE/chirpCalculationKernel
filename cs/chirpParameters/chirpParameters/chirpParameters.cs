using System.Collections.Specialized;
using System.Text;
using SharpConfig;

namespace chirpParameters;

public enum StatusE
{
    CpErr,
    CpOk
}

public enum LanguageTypeE
{
    LangZhCnIdx,
    LangEnUsIdx
}

public enum SlopePriorityE
{
    SpSlopeFirst,
    SpBandwidthFirst
}

public enum StrErrE
{
    ErrDutyCycleGt100,
    ErrAdcPointsGtLimit,
    ErrChirpLoopsGtLimit,
    ErrRangeFftSizeGtLimit,
    ErrDopplerFftSizeGtLimit,
    ErrAdcPointsLtLimit,
    ErrChirpLoopsLtLimit,
    ErrRangeFftSizeLtLimit,
    ErrDopplerFftSizeLtLimit,
    ErrAdcDelayTooLong,
    ErrAdcSampleTimeTooLong,
    ErrRangeFftSIzeNotPower2,
    ErrRangeFftSizeLtAdcPoints,
    ErrDopplerFftSIzeNotPower2,
    ErrDopplerFftSizeLtChirpLoops,
    ErrNoErr,
    ErrFileReadErr,
    ErrStrNums
}

public struct CfarParameterString
{
    public string CfarRangeGuard;
    public string CfarRangeReference;
    public string CfarRangeAlpha;
    public string CfarRangeBiasDb;
    public string CfarDopplerGuard;
    public string CfarDopplerReference;
    public string CfarDopplerAlpha;
    public string CfarDopplerBiasDb;
    public string CfarEnabled;

    public void Init()
    {
        CfarRangeGuard = "cfarRangeGuard";
        CfarRangeReference = "cfarRangeReference";
        CfarRangeAlpha = "cfarRangeAlpha";
        CfarRangeBiasDb = "cfarRangeBias_dB";
        CfarDopplerGuard = "cfarDopplerGuard";
        CfarDopplerReference = "cfarDopplerReference";
        CfarDopplerAlpha = "cfarDopplerAlpha";
        CfarDopplerBiasDb = "cfarDopplerBias_dB";
        CfarEnabled = "cfarEnabled";
    }
}

public struct ChirpParameterString
{
    public string StartFrequencyMHz;
    public string BandWidthMHz;
    public string IdleTimeUs;
    public string RampTimeUs;
    public string PeriodicityMs;
    public string AdcPoints;
    public string SampleRateKsps;
    public string AdcDelayUs;
    public string ChirpLoops;
    public string IqSample;
    public string AntTdm;
    public string Rx;
    public string RangeFftSize;
    public string DopplerFftSize;
    public string MinAdcPoints;
    public string MinChirpLoops;
    public string MinRangeFftSize;
    public string MinDopplerFftSize;
    public string MaxAdcPoints;
    public string MaxChirpLoops;
    public string MaxRangeFftSize;
    public string MaxDopplerFftSize;
    public string StaticClutterRemoval;
    public CfarParameterString Cfar;

    public void Init()
    {
        StartFrequencyMHz = "startFrequency_MHz";
        BandWidthMHz = "bandWidth_MHz";
        IdleTimeUs = "idleTime_us";
        RampTimeUs = "rampTime_us";
        PeriodicityMs = "periodicity_ms";
        AdcPoints = "ADCPoints";
        SampleRateKsps = "sampleRate_ksps";
        AdcDelayUs = "ADCDelay_us";
        ChirpLoops = "chirpLoops";
        IqSample = "iqSample";
        AntTdm = "antTDM";
        Rx = "rx";
        RangeFftSize = "rangeFFTSize";
        DopplerFftSize = "dopplerFFTSize";
        MinAdcPoints = "minADCPoints";
        MinChirpLoops = "minChirpLoops";
        MinRangeFftSize = "minRangeFFTSize";
        MinDopplerFftSize = "minDopplerFFTSize";
        MaxAdcPoints = "maxADCPoints";
        MaxChirpLoops = "maxChirpLoops";
        MaxRangeFftSize = "maxRangeFFTSize";
        MaxDopplerFftSize = "maxDopplerFFTSize";
        StaticClutterRemoval = "staticClutterRemoval";
        Cfar.Init();
    }
}

public struct CfarParameterData
{
    public int CfarRangeGuard;
    public int CfarRangeReference;
    public float CfarRangeAlpha;
    public float CfarRangeBiasDb;
    public int CfarDopplerGuard;
    public int CfarDopplerReference;
    public float CfarDopplerAlpha;
    public float CfarDopplerBiasDb;
    public bool CfarEnabled;
}

public struct ChirpParametersData
{
    public CfarParameterData Cfar;
    public float StartFrequencyMHz;
    public float BandWidthMHz;
    public float IdleTimeUs;
    public float RampTimeUs;
    public float PeriodicityMs;
    public int AdcPoints;
    public float SampleRateKsps;
    public float AdcDelayUs;
    public int ChirpLoops;
    public bool IqSample;
    public int AntTdm;
    public int Rx;
    public int RangeFftSize;
    public int DopplerFftSize;
    public int MinAdcPoints;
    public int MinChirpLoops;
    public int MinRangeFftSize;
    public int MinDopplerFftSize;
    public int MaxAdcPoints;
    public int MaxChirpLoops;
    public int MaxRangeFftSize;
    public int MaxDopplerFftSize;
    public bool StaticClutterRemoval;
    public float SlopeMHzus;
    public float AdcTimeUus;
    public float AdcBandWidthMHz;
    public float MaxAdcTimeUs;
    public float LambdaStartMm;
    public float LambdaCenterMm;
    public float DutyCyclePercent;
    public float TcUs;
    public float TcTdmUs;
    public float TfUs;
    public float DMaxM;
    public float DResM;
    public float DResComputeM;
    public float VMaxMs;
    public float VResMs;
    public float VResComputeMs;
    public float DopplerSampleRateSps;
    public float OneMeterIfKHz;
    public float OneResBinIfKHz;
    public float OneResComputeBinIfKHz;
    public float MaxIfKHz;
    public float F32RadarCubeKb;
}

public class ChirpParametersHandler
{
    public LanguageTypeE LangIdx;
    public string LanguageCfgFileFolder = "";
    public string[] StrErr = new string[(int)StrErrE.ErrStrNums];

    public ChirpParameterString CpString;

    public ChirpParametersData Data;
    public string ErrMsg = "";
    public StatusE ErrFlag;
    public SlopePriorityE SlopePriority;

    public void Init()
    {
        ErrFlag = StatusE.CpOk;
        SlopePriority = SlopePriorityE.SpBandwidthFirst;
        CpString = new ChirpParameterString();
        CpString.Init();
    }

    public void set_language()
    {
        StreamReader langFile;
        switch (LangIdx)
        {
            case LanguageTypeE.LangZhCnIdx:
                langFile = new StreamReader(LanguageCfgFileFolder + "/zh_CN.cfg", Encoding.UTF8);
                break;
            case LanguageTypeE.LangEnUsIdx:
                langFile = new StreamReader(LanguageCfgFileFolder + "/en_US.cfg", Encoding.UTF8);
                break;
            default:
                langFile = new StreamReader(LanguageCfgFileFolder + "/en_US.cfg", Encoding.UTF8);
                break;
        }

        for (int i = 0; i < StrErr.Length; i++)
        {
            StrErr[i] = langFile.ReadLine();
        }

        langFile.Close();
    }

    public void set_to_default()
    {
        Data.MinAdcPoints = 0;
        Data.MinChirpLoops = 0;
        Data.MinRangeFftSize = 0;
        Data.MinDopplerFftSize = 0;
        Data.MaxAdcPoints = 0;
        Data.MaxChirpLoops = 0;
        Data.MaxRangeFftSize = 0;
        Data.MaxDopplerFftSize = 0;
        Data.StaticClutterRemoval = true;

        Data.StartFrequencyMHz = 77000f;
        Data.BandWidthMHz = 3000f;
        Data.IdleTimeUs = 20f;
        Data.RampTimeUs = 100f;
        Data.PeriodicityMs = 10f;
        Data.AdcPoints = 256;
        Data.SampleRateKsps = 3000f;
        Data.AdcDelayUs = 10f;
        Data.ChirpLoops = 64;
        Data.IqSample = true;
        Data.AntTdm = 1;
        Data.Rx = 1;

        Data.RangeFftSize = Data.AdcPoints;
        Data.DopplerFftSize = Data.ChirpLoops;
        Data.Cfar.CfarEnabled = false;
        compute_and_validate();
    }

    public void compute_and_validate()
    {
        ErrFlag = StatusE.CpErr;
        switch (SlopePriority)
        {
            case SlopePriorityE.SpSlopeFirst:
                Data.BandWidthMHz = Data.SlopeMHzus * Data.RampTimeUs;
                break;
            case SlopePriorityE.SpBandwidthFirst:
                Data.SlopeMHzus = Data.BandWidthMHz / Data.RampTimeUs;
                break;
            default:
                Data.SlopeMHzus = Data.BandWidthMHz / Data.RampTimeUs;
                break;
        }

        Data.LambdaStartMm = 3e5f / Data.StartFrequencyMHz;
        Data.LambdaCenterMm = 3e5f / (Data.StartFrequencyMHz + Data.BandWidthMHz / 2f);
        Data.TcUs = Data.IdleTimeUs + Data.RampTimeUs;
        Data.TcTdmUs = Data.TcUs * Data.AntTdm;
        Data.TfUs = Data.TcTdmUs * Data.ChirpLoops;
        Data.DutyCyclePercent = Data.TfUs / Data.PeriodicityMs * 0.1f;
        Data.MaxAdcTimeUs = Data.RampTimeUs - Data.AdcDelayUs;
        Data.AdcTimeUus = Data.AdcPoints / Data.SampleRateKsps * 1e3f;
        Data.AdcBandWidthMHz = Data.SlopeMHzus * Data.AdcTimeUus;
        Data.DMaxM = Data.SampleRateKsps * 15e-2f / Data.SlopeMHzus;
        if (!Data.IqSample)
        {
            Data.DMaxM /= 2f;
        }

        Data.DResM = 1.5e2f / Data.AdcBandWidthMHz;
        Data.DResComputeM = Data.DMaxM / Data.RangeFftSize;
        if (!Data.IqSample)
        {
            Data.DResComputeM *= 2f;
        }

        Data.VMaxMs = Data.LambdaCenterMm / 4f / Data.TcTdmUs * 1e3f;
        Data.VResMs = Data.LambdaCenterMm / 2f / Data.TfUs * 1e3f;
        Data.VResComputeMs = Data.VMaxMs / Data.DopplerFftSize * 2f;
        Data.DopplerSampleRateSps = 1f / Data.TcTdmUs * 1e6f;
        Data.OneMeterIfKHz = 20f * Data.SlopeMHzus / 3f;
        Data.OneResBinIfKHz = Data.OneMeterIfKHz * Data.DResM;
        Data.OneResComputeBinIfKHz = Data.OneMeterIfKHz * Data.DResComputeM;
        Data.MaxIfKHz = Data.OneMeterIfKHz * Data.DMaxM;
        Data.F32RadarCubeKb = Data.AntTdm * Data.Rx * Data.RangeFftSize * Data.DopplerFftSize / 128f;

        if (Data.DutyCyclePercent > 100f)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrDutyCycleGt100];
            return;
        }

        if (Data.MaxAdcPoints != 0)
        {
            if (Data.AdcPoints > Data.MaxAdcPoints)
            {
                ErrMsg = StrErr[(int)StrErrE.ErrAdcPointsGtLimit];
                return;
            }
        }

        if (Data.MaxChirpLoops != 0)
        {
            if (Data.ChirpLoops > Data.MaxChirpLoops)
            {
                ErrMsg = StrErr[(int)StrErrE.ErrChirpLoopsGtLimit];
                return;
            }
        }

        if (Data.MaxRangeFftSize != 0)
        {
            if (Data.RangeFftSize > Data.MaxRangeFftSize)
            {
                ErrMsg = StrErr[(int)StrErrE.ErrRangeFftSizeGtLimit];
                return;
            }
        }

        if (Data.MaxDopplerFftSize != 0)
        {
            if (Data.DopplerFftSize > Data.MaxDopplerFftSize)
            {
                ErrMsg = StrErr[(int)StrErrE.ErrDopplerFftSizeGtLimit];
                return;
            }
        }

        if (Data.AdcPoints < Data.MinAdcPoints)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrAdcPointsLtLimit];
            return;
        }

        if (Data.ChirpLoops < Data.MinChirpLoops)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrChirpLoopsLtLimit];
            return;
        }

        if (Data.RangeFftSize < Data.MinRangeFftSize)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrRangeFftSizeLtLimit];
            return;
        }

        if (Data.DopplerFftSize < Data.MinDopplerFftSize)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrDopplerFftSizeLtLimit];
            return;
        }

        if (Data.MaxAdcTimeUs < 0 || Data.AdcDelayUs + Data.AdcTimeUus > Data.RampTimeUs)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrAdcDelayTooLong];
            return;
        }

        if (Data.AdcTimeUus > Data.MaxAdcTimeUs)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrAdcSampleTimeTooLong];
            return;
        }

        if (!Is2Power(Data.RangeFftSize))
        {
            ErrMsg = StrErr[(int)StrErrE.ErrRangeFftSIzeNotPower2];
            return;
        }

        if (Data.RangeFftSize < Data.AdcPoints)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrRangeFftSizeLtAdcPoints];
            return;
        }

        if (!Is2Power(Data.DopplerFftSize))
        {
            ErrMsg = StrErr[(int)StrErrE.ErrDopplerFftSIzeNotPower2];
            return;
        }

        if (Data.DopplerFftSize < Data.ChirpLoops)
        {
            ErrMsg = StrErr[(int)StrErrE.ErrDopplerFftSizeLtChirpLoops];
            return;
        }

        ErrMsg = StrErr[(int)StrErrE.ErrNoErr];
        ErrFlag = StatusE.CpOk;
    }

    public void save_cfg(string dir)
    {
        compute_and_validate();
        if (ErrFlag == StatusE.CpErr)
        {
            return;
        }

        var saveFile = new Configuration();
        saveFile["chirp parameters"][CpString.StartFrequencyMHz].StringValue = Data.StartFrequencyMHz.ToString("f2");
        saveFile["chirp parameters"][CpString.BandWidthMHz].StringValue = Data.BandWidthMHz.ToString("f2");
        saveFile["chirp parameters"][CpString.IdleTimeUs].StringValue = Data.IdleTimeUs.ToString("f2");
        saveFile["chirp parameters"][CpString.RampTimeUs].StringValue = Data.RampTimeUs.ToString("f2");
        saveFile["chirp parameters"][CpString.PeriodicityMs].StringValue = Data.PeriodicityMs.ToString("f2");
        saveFile["chirp parameters"][CpString.AdcPoints].IntValue = Data.AdcPoints;
        saveFile["chirp parameters"][CpString.SampleRateKsps].StringValue = Data.SampleRateKsps.ToString("f2");
        saveFile["chirp parameters"][CpString.AdcDelayUs].StringValue = Data.AdcDelayUs.ToString("f2");
        saveFile["chirp parameters"][CpString.ChirpLoops].IntValue = Data.ChirpLoops;
        saveFile["chirp parameters"][CpString.IqSample].StringValue = Data.IqSample ? "yes" : "no";
        saveFile["chirp parameters"][CpString.AntTdm].IntValue = Data.AntTdm;
        saveFile["chirp parameters"][CpString.Rx].IntValue = Data.Rx;
        saveFile["chirp parameters"][CpString.RangeFftSize].IntValue = Data.RangeFftSize;
        saveFile["chirp parameters"][CpString.DopplerFftSize].IntValue = Data.DopplerFftSize;
        saveFile["chirp parameters"][CpString.MinAdcPoints].IntValue = Data.MinAdcPoints;
        saveFile["chirp parameters"][CpString.MinChirpLoops].IntValue = Data.MinChirpLoops;
        saveFile["chirp parameters"][CpString.MinRangeFftSize].IntValue = Data.MinRangeFftSize;
        saveFile["chirp parameters"][CpString.MinDopplerFftSize].IntValue = Data.MinDopplerFftSize;
        saveFile["chirp parameters"][CpString.MaxAdcPoints].IntValue = Data.MaxAdcPoints;
        saveFile["chirp parameters"][CpString.MaxChirpLoops].IntValue = Data.MaxChirpLoops;
        saveFile["chirp parameters"][CpString.MaxRangeFftSize].IntValue = Data.MaxRangeFftSize;
        saveFile["chirp parameters"][CpString.MaxDopplerFftSize].IntValue = Data.MaxDopplerFftSize;
        saveFile["chirp parameters"][CpString.StaticClutterRemoval].StringValue = Data.StaticClutterRemoval ? "yes" : "no";
        if (Data.Cfar.CfarEnabled)
        {
            saveFile["cfar parameters"][CpString.Cfar.CfarRangeGuard].IntValue = Data.Cfar.CfarRangeGuard;
            saveFile["cfar parameters"][CpString.Cfar.CfarRangeReference].IntValue = Data.Cfar.CfarRangeReference;
            saveFile["cfar parameters"][CpString.Cfar.CfarRangeAlpha].StringValue = Data.Cfar.CfarRangeAlpha.ToString("f2");
            saveFile["cfar parameters"][CpString.Cfar.CfarRangeBiasDb].StringValue = Data.Cfar.CfarRangeBiasDb.ToString("f2");
            saveFile["cfar parameters"][CpString.Cfar.CfarDopplerGuard].IntValue = Data.Cfar.CfarDopplerGuard;
            saveFile["cfar parameters"][CpString.Cfar.CfarDopplerReference].IntValue = Data.Cfar.CfarDopplerReference;
            saveFile["cfar parameters"][CpString.Cfar.CfarDopplerAlpha].StringValue = Data.Cfar.CfarDopplerAlpha.ToString("f2");
            saveFile["cfar parameters"][CpString.Cfar.CfarDopplerBiasDb].StringValue = Data.Cfar.CfarDopplerBiasDb.ToString("f2");
        }
        saveFile.SaveToFile(dir);
    }

    public void load_cfg(string dir)
    {
        var loadFile = Configuration.LoadFromFile(dir);
        Data.StartFrequencyMHz = loadFile["chirp parameters"][CpString.StartFrequencyMHz].GetValue<float>();
        Data.BandWidthMHz = loadFile["chirp parameters"][CpString.BandWidthMHz].GetValue<float>();
        Data.IdleTimeUs = loadFile["chirp parameters"][CpString.IdleTimeUs].GetValue<float>();
        Data.RampTimeUs = loadFile["chirp parameters"][CpString.RampTimeUs].GetValue<float>();
        Data.PeriodicityMs = loadFile["chirp parameters"][CpString.PeriodicityMs].GetValue<float>();
        Data.AdcPoints = loadFile["chirp parameters"][CpString.AdcPoints].GetValue<int>();
        Data.SampleRateKsps = loadFile["chirp parameters"][CpString.SampleRateKsps].GetValue<float>();
        Data.AdcDelayUs = loadFile["chirp parameters"][CpString.AdcDelayUs].GetValue<float>();
        Data.ChirpLoops = loadFile["chirp parameters"][CpString.ChirpLoops].GetValue<int>();
        Data.IqSample = loadFile["chirp parameters"][CpString.StartFrequencyMHz].GetValue<string>() == "yes";  // \r\n?
        Data.AntTdm = loadFile["chirp parameters"][CpString.AntTdm].GetValue<int>();
        Data.Rx = loadFile["chirp parameters"][CpString.Rx].GetValue<int>();
        Data.RangeFftSize = loadFile["chirp parameters"][CpString.RangeFftSize].GetValue<int>();
        Data.DopplerFftSize = loadFile["chirp parameters"][CpString.DopplerFftSize].GetValue<int>();
        Data.MinAdcPoints = loadFile["chirp parameters"][CpString.MinAdcPoints].GetValue<int>();
        Data.MinChirpLoops = loadFile["chirp parameters"][CpString.MinChirpLoops].GetValue<int>();
        Data.MinRangeFftSize = loadFile["chirp parameters"][CpString.MinRangeFftSize].GetValue<int>();
        Data.MinDopplerFftSize = loadFile["chirp parameters"][CpString.MinDopplerFftSize].GetValue<int>();
        Data.MaxAdcPoints = loadFile["chirp parameters"][CpString.MaxAdcPoints].GetValue<int>();
        Data.MaxChirpLoops = loadFile["chirp parameters"][CpString.MaxChirpLoops].GetValue<int>();
        Data.MaxRangeFftSize = loadFile["chirp parameters"][CpString.MaxRangeFftSize].GetValue<int>();
        Data.MaxDopplerFftSize = loadFile["chirp parameters"][CpString.MaxDopplerFftSize].GetValue<int>();
        Data.StaticClutterRemoval = loadFile["chirp parameters"][CpString.StaticClutterRemoval].GetValue<string>() == "yes";
        
        Data.Cfar.CfarEnabled = false;
        if (loadFile["cfar parameters"][CpString.Cfar.CfarRangeGuard].GetValue<string>().Length != 0)
        {
            Data.Cfar.CfarRangeGuard = loadFile["cfar parameters"][CpString.Cfar.CfarRangeGuard].GetValue<int>();
            Data.Cfar.CfarRangeReference = loadFile["cfar parameters"][CpString.Cfar.CfarRangeReference].GetValue<int>();
            Data.Cfar.CfarRangeAlpha = loadFile["cfar parameters"][CpString.Cfar.CfarRangeAlpha].GetValue<float>();
            Data.Cfar.CfarRangeBiasDb = loadFile["cfar parameters"][CpString.Cfar.CfarRangeBiasDb].GetValue<float>();
            Data.Cfar.CfarDopplerGuard = loadFile["cfar parameters"][CpString.Cfar.CfarDopplerGuard].GetValue<int>();
            Data.Cfar.CfarDopplerReference = loadFile["cfar parameters"][CpString.Cfar.CfarDopplerReference].GetValue<int>();
            Data.Cfar.CfarDopplerAlpha = loadFile["cfar parameters"][CpString.Cfar.CfarDopplerAlpha].GetValue<float>();
            Data.Cfar.CfarDopplerBiasDb = loadFile["cfar parameters"][CpString.Cfar.CfarDopplerBiasDb].GetValue<float>();
            Data.Cfar.CfarEnabled = true;
        }
        compute_and_validate();
    }

    private bool Is2Power(int x)
    {
        return x > 0 && (x & (x - 1)) == 0;
    }
}