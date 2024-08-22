class example
{
    static void Main(string[] args)
    {
        chirpParameters.ChirpParametersHandler instance = new chirpParameters.ChirpParametersHandler();
        instance.Init();
        instance.LanguageCfgFileFolder = "../../../../../../locales";
        // D:\workSpace\coding\mixed\chirpCalculationKernel\locales
        // D:\workSpace\coding\mixed\chirpCalculationKernel\cs\example\example\bin\Debug\net8.0
        instance.LangIdx = chirpParameters.LanguageTypeE.LangZhCnIdx;
        instance.set_language();
        instance.set_to_default();
        instance.save_cfg("../../../test.cfg");
        instance.load_cfg("../../../test.cfg");
    }
}
