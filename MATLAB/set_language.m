function [cph] = set_language(cph, langIdx)
    if langIdx == cph.langZhCNIdx
        langFile = fopen(cph.languageCfgFileFolder + "/zh_CN.cfg", "r");
    elseif langIdx == cph.langEnUSIdx
        langFile = fopen(cph.languageCfgFileFolder + "/en_US.cfg", "r");
    end

    for i = 1:cph.errMsgIdx.errStrNums
        cph.strErr(i) = fgetl(langFile);
    end
end
