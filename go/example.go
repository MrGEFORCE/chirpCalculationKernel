package main

func main() {
	instance := ChirpParametersHandlerT{}
	instance.Init()
	instance.languageCfgFileFolder = "../locales"
	instance.SetLanguage(langEnUSIdx)
	instance.SetToDefault()
	instance.SaveCfg("./test.cfg")
	instance.LoadCfg("./test.cfg")
}
