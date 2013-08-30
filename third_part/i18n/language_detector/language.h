// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#ifndef UTIL_I18N_LANGUAGE_H_
#define UTIL_I18N_LANGUAGE_H_

//namespace i18n {

enum Language {
  UNKNOWN_LANGUAGE = 0,
  EN,  // 英语,

  // ************************
  // now handled
  // ************************
  ZH_CN,  // 中文(中华人民共和国)
  ZH_TW,  // 中文(中国台湾)
  JA,     // 日语
  // ************************
  // now un-handled
  // ************************
  AF,     // 南非荷兰语
  SQ,     // 阿尔巴尼亚语
  AR_SA,  // 阿拉伯语(沙特阿拉伯)
  AR_IQ,  // 阿拉伯语(伊拉克)
  AR_EG,  // 阿拉伯语(埃及)
  AR_LY,  // 阿拉伯语(利比亚)
  AR_DZ,  // 阿拉伯语(阿尔及利亚)
  AR_MA,  // 阿拉伯语(摩洛哥)
  AR_TN,  // 阿拉伯语(突尼斯)
  AR_OM,  // 阿拉伯语(阿曼)
  AR_YE,  // 阿拉伯语(也门)
  AR_SY,  // 阿拉伯语(叙利亚)
  AR_JO,  // 阿拉伯语(约旦)
  AR_LB,  // 阿拉伯语(黎巴嫩)
  AR_KW,  // 阿拉伯语(科威特)
  AR_AE,  // 阿拉伯语(阿拉伯联合酋长国)
  AR_BH,  // 阿拉伯语(巴林)
  AR_QA,  // 阿拉伯语(卡塔尔)
  EU,  // 巴斯克语
  BG,  // 保加利亚语
  BE,  // 贝劳语
  CA,  // 加泰罗尼亚语
  ZH_SG,  // 中文(新加坡)
  ZH_HK,  // 中文(中国香港特别行政区)
  HR,  // 克罗地亚语
  CS,  // 捷克语
  DA,  // 丹麦语
  NL,  // 荷兰语(标准)
  NL_BE,  // 荷兰语(比利时)
  EN_US,  // 英语(美国)
  EN_GB,  // 英语(英国)
  EN_AU,  // 英语(澳大利亚)
  EN_CA,  // 英语(加拿大)
  EN_NZ,  // 英语(新西兰)
  EN_IE,  // 英语(爱尔兰)
  EN_ZA,  // 英语(南非)
  EN_JM,  // 英语(牙买加)
  EN_BZ,  // 英语(伯利兹)
  EN_TT,  // 英语(特立尼达)
  ET,  // 爱沙尼亚语
  FO,  // 法罗语
  FA,  // 波斯语
  FI,  // 芬兰语
  FR,  // 法语(标准)
  FR_BE,  // 法语(比利时)
  FR_CA,  // 法语(加拿大)
  FR_CH,  // 法语(瑞士)
  FR_LU,  // 法语(卢森堡)
  GD,  // 盖尔语(苏格兰)
  GD_IE,  // 盖尔语(爱尔兰)
  DE,  // 德语(标准)
  DE_CH,  // 德语(瑞士)
  DE_AT,  // 德语(奥地利)
  DE_LU,  // 德语(卢森堡)
  DE_LI,  // 德语(列支敦士登)
  EL,  // 希腊语
  HE,  // 希伯来语
  HI,  // 北印度语
  HU,  // 匈牙利语
  IS,  // 冰岛语
  IN,  // 印度尼西亚语
  IT,  // 意大利语(标准)
  IT_CH,  // 意大利语(瑞士)
  KO,  // 朝鲜语
  LV,  // 拉脱维亚语
  LT,  // 立陶宛语
  MK,  // 马其顿语
  MS,  // 马来西亚语
  MT,  // 马耳他语
  NO,  // 挪威语
  PL,  // 波兰语
  PT_BR,  // 葡萄牙语(巴西)
  PT,  // 葡萄牙语(葡萄牙)
  RM,  // 拉丁语系
  RO,  // 罗马尼亚语
  RO_MO,  // 罗马尼亚语(摩尔达维亚)
  RU,  // 俄语
  RU_MO,  // 俄语(摩尔达维亚)
  SZ,  // 萨摩斯语(拉普兰)
  SR,  // 塞尔维亚语
  SK,  // 斯洛伐克语
  SL,  // 斯洛文尼亚语
  SB,  // 索布语
  ES,  // 西班牙语
  ES_MX,  // 西班牙语(墨西哥)
  ES_GT,  // 西班牙语(危地马拉)
  ES_CR,  // 西班牙语(哥斯达黎加)
  ES_PA,  // 西班牙语(巴拿马)
  ES_DO,  // 西班牙语(多米尼加共和国)
  ES_VE,  // 西班牙语(委内瑞拉)
  ES_CO,  // 西班牙语(哥伦比亚)
  ES_PE,  // 西班牙语(秘鲁)
  ES_AR,  // 西班牙语(阿根廷)
  ES_EC,  // 西班牙语(厄瓜多尔)
  ES_CL,  // 西班牙语(智利)
  ES_UY,  // 西班牙语(乌拉圭)
  ES_PY,  // 西班牙语(巴拉圭)
  ES_BO,  // 西班牙语(玻利维亚)
  ES_SV,  // 西班牙语(萨尔瓦多)
  ES_HN,  // 西班牙语(洪都拉斯)
  ES_NI,  // 西班牙语(尼加拉瓜)
  ES_PR,  // 西班牙语(波多黎各)
  SX,  // 苏图语
  SV,  // 瑞典语
  SV_FI,  // 瑞典语(芬兰)
  TH,  // 泰语TS 汤加语
  TN,  // 瓦纳语
  TR,  // 土耳其语
  UK,  // 乌克兰语
  UR,  // 乌尔都语
  VE,  // 文达语
  VI,  // 越南语
  XH,  // 科萨语
  JI,  // 依地语
  ZU,  // 祖鲁语

  LANGUAGE_VERSION_COUNT,
};

//}  // namespace i18n
#endif  // UTIL_I18N_LANGUAGE_H_
