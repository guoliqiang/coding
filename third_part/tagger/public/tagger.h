#include <string>
#include <vector>
#include <set>
#include "../public/text_parser.h"
#include "../public/ictclas50.h"
#ifndef THIRD_PART_TAGGER_PUBLIC_TAGGER_H
#define THIRD_PART_TAGGER_PUBLIC_TAGGER_H

class IctTagger : public TextParser {
  public:
    ~IctTagger() {}
    IctTagger() {}
  public:
    // 功能：读取配置文件，加载词典等
    // 参数：conf_path，配置文件，及data文件夹所在路径
    bool Init(const std::string& conf_path = "../../../data/third_part/tagger/data/");

    // 功能：导入用户词典
    // 参数：words, 用户词汇字符串, 格式如下：
    //       1.词语与词性用‘@@’间隔；
    //       2.词与词之间用 半角‘;’间隔；
    //       3.词性可省略
    //       例如：“清华@@nr;分词;系统@@n;……;”，
    void ImportUserDict(const std::string& words);
    // 功能：导入用户词典文件
    // 参数：dict_file，用户词典路径名称，格式如下：
    //       1.词语与词性用‘@@’间隔。例如：“清华大学@@nr；
    //       2.一行一词；
    //       3.词性可省略
    void ImportUserDictFile(const std::string& dict_file);
    void ImportUserDictFiles(const std::vector<std::string>& dict_files);
    void ImportUserWords(const std::vector<std::string>& words);
    void ImportUserWords(const std::set<std::string>& words);

    bool SetPOSmap(int nPOSmap);
    int  SaveTheUsrDic();

    // parsing functions
    void Parse(const std::string& str,
               std::vector<std::string> &words,
               std::vector<std::string> &tags,
               bool skip_space = true
               );
    void Parse4NewWordDetection(const std::string& str,
                                std::vector<std::string> &words,
                                std::vector<std::string> &tags
                                );
    void ParseP(const std::string& str,
                std::vector<std::string> &words,
                std::vector<std::string> &tags
                );
    void ParseAspect(const std::string& str,
                     std::vector<std::string> &words,
                     std::vector<std::string> &tags
                     );
    bool Parse(const std::string& in, std::string& out);

    bool Parse(const std::string& str,
        std::vector<std::string>* words,
        LanguageCode lang = zh_chs,
        bool skip_space = true);

    bool Parse(const std::string& str,
        std::vector<std::string>* words,
        std::vector<std::string>* tags,
        LanguageCode lang = zh_chs,
        bool skip_space = true);

  public:
    void Exit();

  public:
    bool GetNgrams(const std::vector<std::string>& terms,
        int n,
        std::vector<std::string> &ngrams
        );
};
#endif  // THIRD_PART_TAGGER_PUBLIC_TAGGER_H
