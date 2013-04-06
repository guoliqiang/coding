#include <string>
#include <vector>
#include <set>
#ifndef THIRD_PART_TAGGER_PUBLIC_TEXT_PARSER_H
#define THIRD_PART_TAGGER_PUBLIC_TEXT_PARSER_H

class TextParser {
  public:
    // TODO(jason): lookup ISO and fill in proper language code
    enum LanguageCode {
      zh_chs,
      zh_cht,
    };

    virtual ~TextParser() {
    }

    virtual bool Parse(const std::string& in, std::string& out) = 0;

    // Parse MUST be a thread SAFE function to call
    // Return true when word breaking succeeded
    // If your word breaker do NOT support certain language return false
    virtual bool Parse(const std::string &text,
                       std::vector<std::string> * p_words,
                       LanguageCode lang = zh_chs,
                       bool skip_space = true) = 0;

    virtual void ImportUserWords(const std::set<std::string>& words) = 0;

    // this interface should be overrided by word breakers providing
    // the functionality of tagging, POS is one kind of tagging
    // It's not required for all word breakers to implement, cause it's
    // not used a lot.
    virtual bool Parse(const std::string &text,
               std::vector<std::string> * p_words,
               std::vector<std::string> * p_tags,
               LanguageCode lang = zh_chs,
               bool skip_space = true) {
      return false;
    }

    virtual void Parse(const std::string &text,
               std::vector<std::string>& p_words,
               std::vector<std::string>& p_tags,
               bool skip_space = true) {
    }
};
#endif   // THIRD_PART_TAGGER_PUBLIC_TEXT_PARSER_H
