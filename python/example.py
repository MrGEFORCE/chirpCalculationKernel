# only fot test, not engage in kernel
import chirpParameters


if __name__ == "__main__":
    instance = chirpParameters.ChirpParameterHandler()
    instance.languageCfgFileFolder = "../locales"
    instance.setLanguage(chirpParameters.LANG_ZH_CN)
    instance.set_default()
    instance.save_cfg("./test.cfg")
    instance.load_cfg("./test.cfg")
    print(instance.gen_description())
