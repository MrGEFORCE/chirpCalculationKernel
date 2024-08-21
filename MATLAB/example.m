clear;
clc;

ChirpParameterHandler = init();
ChirpParameterHandler.languageCfgFileFolder = "../locales";
ChirpParameterHandler = set_language(ChirpParameterHandler, ChirpParameterHandler.langEnUSIdx);
ChirpParameterHandler = set_to_default(ChirpParameterHandler);
% save_cfg(ChirpParameterHandler, "./test.cfg");
% ChirpParameterHandler = load_cfg(ChirpParameterHandler, "./test.cfg");
