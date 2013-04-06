#include "../public/tagger.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main(int argc, char** argv) {

  IctTagger tg;
  if (!tg.Init()){
    return 0;
  }

  string user_words = "康庄镇@@ns;绿菜园合作社@@nt";
  tg.ImportUserDict(user_words);

  string str = "走进延庆县康庄镇小丰营村绿菜园合作社 的 温室中";
  vector<string> words;
  vector<string> tags;
  words.clear();
  tags.clear();
  tg.Parse(str, words, tags);
  for (vector<string>::size_type i = 0; i != words.size(); ++i) {
    cout << words[i] << "/" << tags[i] << " ";
  }
  cout << endl;

  tg.Exit();
  return 0;
}
