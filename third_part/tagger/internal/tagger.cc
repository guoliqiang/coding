#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "../public/tagger.h"
#include "../../../base/public/string_util.h"
using namespace std;


bool IctTagger::SetPOSmap(int nPOSmap) {
  // 设置词性标注集:
  //    0 计算所二级标注集
  //    1 计算所一级标注集
  //    2 北大二级标注集
  //    3 北大一级标注集)
  return ICTCLAS_SetPOSmap(0);
}


bool IctTagger::Init(const string& conf_path) {
  if(!ICTCLAS_Init(conf_path.c_str())) {
    cerr << "\tInit fails" << endl;
    return false;
  }
  SetPOSmap(0);
  return true;
}


void IctTagger::ImportUserDict(const string& words) {
  ICTCLAS_ImportUserDict(words.c_str(),
                         words.size(),
                         CODE_TYPE_UTF8);
}


void IctTagger::ImportUserDictFile(const string& dict) {
  ICTCLAS_ImportUserDictFile(dict.c_str(), CODE_TYPE_UTF8);
}


void IctTagger::ImportUserWords(const set<string>& words) {
  string words_str = "";
  set<string>::const_iterator it = words.begin();
  for (; it != words.end(); ++it) {
    string word;
    TrimString(*it, " \n\t\v\f\r;", &word);
    if (word.size() > 0) {
      words_str += (word + ";");
    }
  }
  ImportUserDict(words_str);
}


void IctTagger::ImportUserWords(const vector<string>& words) {
  string words_str = "";
  vector<string>::const_iterator it = words.begin();
  for (; it != words.end(); ++it) {
    string word;
    TrimString(*it, " \n\t\v\f\r;", &word);
    if (word.size() > 0) {
      words_str += (word + ";");
    }
  }
  ImportUserDict(words_str);
}


void IctTagger::ImportUserDictFiles(const vector<string>& dicts) {
  string words = "";
  vector<string>::const_iterator it = dicts.begin();
  for (; it != dicts.end(); ++it) {
    ifstream inf((*it).c_str());
    string line;
    while (getline(inf, line)) {
      TrimString(line, " \n\t\v\f\r;", &line);
      if (line.size() > 0) {
        words += (line + ";");
      }
    }
    inf.close();
  }
  ImportUserDict(words);
}


int IctTagger::SaveTheUsrDic() {
  return ICTCLAS_SaveTheUsrDic();
}

bool IctTagger::Parse(const std::string& in, std::string& out) {
  char* s = 0;
  s = (char*) malloc(in.size() * 6);
  ICTCLAS_ParagraphProcess(in.c_str(), in.size(), s, CODE_TYPE_UTF8);
  out = s;
  free(s);
  return true;
}


void IctTagger::Parse(const string& str,
    vector<string> &words,
    vector<string> &tags,
    bool skip_space) {
  if (str.size() == 0) return;
  words.clear();
  tags.clear();
  const char *s = str.c_str();
  int nPaLen = str.size();
  int cnt = 0;
  tagICTCLAS_Result *vec =
    ICTCLAS_ParagraphProcessA(s,nPaLen,cnt,CODE_TYPE_UTF8,true);
  for (int i = 0; i < cnt; ++i) {
    // cout<<vec[i].iStartPos<<endl;
    // cout << vec[i].iLength<<endl;
    // cout<<vec[i].szPOS<<endl;
    // cout<<vec[i].iPOS<<endl;
    // cout<<vec[i].iWordID<<endl;
    // cout<<vec[i].iWordType<<endl;
    // cout<<vec[i].iWeight<<endl<<endl;
    string word = str.substr(vec[i].iStartPos, vec[i].iLength);
    string tag = vec[i].szPOS;
    if (skip_space == true && word == " ") continue;
    words.push_back(word);
    tags.push_back(tag);
    // cout << word << "/" << tag <<endl;
  }
  ICTCLAS_ResultFree(vec);
}


void IctTagger::Parse4NewWordDetection(const string& str,
    vector<string> &words,
    vector<string> &tags) {
  words.clear();
  tags.clear();
  if (str.size() == 0) return;
  const char *s = str.c_str();
  int nPaLen = str.size();
  int cnt = 0;
  tagICTCLAS_Result *vec = ICTCLAS_ParagraphProcessA(s,nPaLen,cnt,CODE_TYPE_UTF8,true);
  for (int i = 0; i < cnt; ++i) {
    // cout<<vec[i].iStartPos<<endl;
    // cout << vec[i].iLength<<endl;
    // cout<<vec[i].szPOS<<endl;
    // cout<<vec[i].iPOS<<endl;
    // cout<<vec[i].iWordID<<endl;
    // cout<<vec[i].iWordType<<endl;
    // cout<<vec[i].iWeight<<endl<<endl;
    string word = str.substr(vec[i].iStartPos, vec[i].iLength);
    string tag = vec[i].szPOS;
    words.push_back(word);
    tags.push_back(tag);
  }
  ICTCLAS_ResultFree(vec);
}


/*
   void IctTagger::parseP(const string str, vector<string> &words, vector<string> &tags) {
   words.clear();
   tags.clear();
   if (str.size() == 0) return;
   const char *s = str.c_str();
   int nPaLen = str.size();
   int cnt = 0;
   tagICTCLAS_Result *vec = ICTCLAS_ParagraphProcessA(s,nPaLen,cnt,CODE_TYPE_UTF8,true);
   string word = str.substr(vec[0].iStartPos, vec[0].iLength);
   string tag = vec[0].szPOS;
   tag = tag.substr(0,1);
   string ph = tag;
   string ch;
   for (int i=1; i<cnt; i++) {
// cout<<vec[i].iStartPos<<endl;
// cout<<vec[i].iLength<<endl;
// cout<<vec[i].szPOS<<endl;
// cout<<vec[i].iPOS<<endl;
// cout<<vec[i].iWordID<<endl;
// cout<<vec[i].iWordType<<endl;
// cout<<vec[i].iWeight<<endl<<endl;
string w = str.substr(vec[i].iStartPos, vec[i].iLength);
string t = vec[i].szPOS;
t = t.substr(0, 1);
ch = t;

if (ch.compare(ph) == 0) {
word += w;
tag = t;
} else {
words.push_back(word);
tags.push_back(tag);
// cout << word << "/" << tag <<endl;
word = w;
tag = t;
}

if (i == cnt-1) {
words.push_back(word);
tags.push_back(tag);
// cout << word << "/" << tag <<endl;
}
ph = ch;
}
ICTCLAS_ResultFree(vec);
}
*/


void IctTagger::ParseP(const string& str,
    vector<string> &words,
    vector<string> &tags) {
  const char *s = str.c_str();
  int nPaLen = str.size();
  int cnt = 0;
  tagICTCLAS_Result *vec =
    ICTCLAS_ParagraphProcessA(s,nPaLen,cnt,CODE_TYPE_UTF8,true);
  if(vec == NULL) {
    return ;
  }
  string word = str.substr(vec[0].iStartPos, vec[0].iLength);
  string tag = vec[0].szPOS;
  string ph = tag;
  string ch;
  for (int i=1; i<cnt; i++) {
    // cout<<vec[i].iStartPos<<endl;
    // cout<<vec[i].iLength<<endl;
    // cout<<vec[i].szPOS<<endl;
    // cout<<vec[i].iPOS<<endl;
    // cout<<vec[i].iWordID<<endl;
    // cout<<vec[i].iWordType<<endl;
    // cout<<vec[i].iWeight<<endl<<endl;
    string w = str.substr(vec[i].iStartPos, vec[i].iLength);
    string t = vec[i].szPOS;
    ch = t;

    if (ch.compare(ph) == 0 && (ch.compare("t")==0)) {
      word += w;
      tag += t;
    } else {
      words.push_back(word);
      tags.push_back(tag);
      // cout << word << "/" << tag <<endl;
      word = w;
      tag = t;
    }

    if (i == cnt-1) {
      words.push_back(word);
      tags.push_back(tag);
      // cout << word << "/" << tag <<endl;
    }
    ph = ch;
  }
  ICTCLAS_ResultFree(vec);
}


void IctTagger::ParseAspect(const string& str,
    vector<string> &wordsOut,
    vector<string> &tagsOut) {
  wordsOut.clear();
  tagsOut.clear();
  if (str.size() == 0) return;

  vector<string> words;
  vector<string> tags;
  vector<int> des;
  words.clear();
  tags.clear();
  des.clear();

  const char *s = str.c_str();
  int nPaLen = str.size();
  int cnt = 0;
  tagICTCLAS_Result *vec =
    ICTCLAS_ParagraphProcessA(s,nPaLen,cnt,CODE_TYPE_UTF8,true);
  string word = str.substr(vec[0].iStartPos, vec[0].iLength);
  string tag = vec[0].szPOS;

  string ph = tag.substr(0, 1);
  string ch;
  int newIndex = 0;
  for (int i = 1; i < cnt; i++) {
    // cout<<vec[i].iStartPos<<endl;
    // cout<<vec[i].iLength<<endl;
    // cout<<vec[i].szPOS<<endl;
    // cout<<vec[i].iPOS<<endl;
    // cout<<vec[i].iWordID<<endl;
    // cout<<vec[i].iWordType<<endl;
    // cout<<vec[i].iWeight<<endl<<endl;
    string w = str.substr(vec[i].iStartPos, vec[i].iLength);
    string t = vec[i].szPOS;
    if (t == "vn" || t == "an" || t == "key") t = "n";
    ch = t.substr(0, 1);
    // cout << "ch: " << ch << "ph:" << ph << endl;

    if ( (ch.compare(ph) == 0 && (ch.compare("t")==0 || ch.compare("n")==0)) ||
        ((ph.compare(0, 1, "a") == 0 || ph == "m") && ch.compare(0, 1, "n") == 0)) {

      if ( ((ph.compare(0, 1, "a") == 0 || ph == "m") && ch.compare(0, 1, "n") == 0) ) {
        tag.clear();
      }
      word += w;
      tag = "n";
    } else {
      words.push_back(word);
      tags.push_back(tag);
      if (tag == "ude1") des.push_back(newIndex);
      newIndex++;
      // cout << word << "/" << tag <<endl;
      word = w;
      tag = t;
    }
    if (i == cnt - 1) {
      words.push_back(word);
      tags.push_back(tag);
      newIndex++;
      // cout << word << "/" << tag <<endl;
    }
    ph = ch;
  }
  ICTCLAS_ResultFree(vec);

  // des.clear();  // zzw
  wordsOut.clear();
  tagsOut.clear();
  vector<string>::size_type j = 0;
  vector<int>::size_type k = 0;
  for (; k < des.size(); ++k) {
    int deI = des[k];
    if (deI <= 0) continue;

    for (; j < deI - 1 && j < words.size(); j++) {
      wordsOut.push_back(words[j]);
      tagsOut.push_back(tags[j]);
    }

    if ( (tags[deI - 1].compare(0, 1, "n") == 0 || tags[deI - 1].compare(0, 1, "v") == 0)
        && tags[deI + 1].compare(0, 1, "n") == 0) {
      wordsOut.push_back(words[deI - 1] + words[deI] + words[deI + 1]);
      tagsOut.push_back("n");
      j = deI + 2;
    } else {
      wordsOut.push_back(words[deI - 1]);
      tagsOut.push_back(tags[deI - 1]);
      j = deI;
    }
  }
  for (vector<string>::size_type m = j; m < words.size(); m++) {
    wordsOut.push_back(words[m]);
    tagsOut.push_back(tags[m]);
  }
}


void IctTagger::Exit() {
  ICTCLAS_Exit();
}


bool GetNgrams(const std::vector<std::string>& terms,
    int n,
    vector<string> &results) {
  if (n <= terms.size()) {
    for (vector<string>::size_type i = 0; i <= terms.size() - n; i++) {
      string gram = "";
      for (vector<string>::size_type j = i; j < i + n; j++) {
        gram += terms[j];
      }
      results.push_back(gram);
    }
    return true;
  } else {
    return false;
    string gram = "";
    for (vector<string>::size_type j = 0; j < terms.size(); j++) {
      gram += terms[j];
    }
    results.push_back(gram);
  }
}


bool IctTagger::Parse(const std::string& str,
    std::vector<std::string> * words,
    LanguageCode lang,
    bool skip_space) {
  if (lang != zh_chs || str.size() == 0) return false;
  words->clear();
  const char *s = str.c_str();
  int nPaLen = str.size();
  int cnt = 0;
  tagICTCLAS_Result *vec =
    ICTCLAS_ParagraphProcessA(s, nPaLen, cnt, CODE_TYPE_UTF8, true);
  for (int i = 0; i < cnt; ++i) {
    string word = str.substr(vec[i].iStartPos, vec[i].iLength);
    if (skip_space == true && word == " ") continue;
    words->push_back(word);
  }
  ICTCLAS_ResultFree(vec);
  return true;
}


bool IctTagger::Parse(const std::string& str,
    std::vector<std::string> * words,
    std::vector<std::string> * tags,
    LanguageCode lang,
    bool skip_space) {
  if (lang != zh_chs || str.size() == 0) return false;
  words->clear();
  tags->clear();
  const char *s = str.c_str();
  int nPaLen = str.size();
  int cnt = 0;
  tagICTCLAS_Result *vec =
    ICTCLAS_ParagraphProcessA(s, nPaLen, cnt, CODE_TYPE_UTF8, true);
  for (int i = 0; i < cnt; ++i) {
    string word = str.substr(vec[i].iStartPos, vec[i].iLength);
    string tag = vec[i].szPOS;
    if (skip_space == true && word == " ") continue;
    words->push_back(word);
    tags->push_back(tag);
  }
  ICTCLAS_ResultFree(vec);
  return true;
}

