// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 21:11:11
// File  : code.cc
// Brief :
// http://www.geeksforgeeks.org/find-the-longest-substring-with-k-unique-characters-in-a-given-string/
// https://leetcode.com/discuss/21929/share-my-c-solution
// http://blog.csdn.net/whuwangyi/article/details/42451289

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string LongestSubstrExactly(std::string str, int k) {
  std::string ans;
  std::map<char, int> t;
  for (int i = 0; i < str.size(); i++) t[str[i]] = 0;

  int cnt = 0;
  int b = 0;
  int e = 0;
  while (e < str.size()) {
    if (t[str[e]] == 0) cnt++;
    t[str[e]]++;

    while (cnt > k) {
      t[str[b]]--;
      if (t[str[b]] == 0) cnt--;
      b++;
    }

    if (cnt == k) {
      if (e - b + 1 > ans.size()) ans = str.substr(b, e - b + 1);
    }
    e++;
  }
  return ans;
}

std::string LongestSubstrMost(std::string str, int k) {
  std::string ans;
  std::map<char, int> t;
  for (int i = 0; i < str.size(); i++) t[str[i]] = 0;

  int cnt = 0;
  int b = 0;
  int e = 0;
  while (e < str.size()) {
    if (t[str[e]] == 0) cnt++;
    t[str[e]]++;

    while (cnt > k) {
      t[str[b]]--;
      if (t[str[b]] == 0) cnt--;
      b++;
    }

    if (e - b + 1 > ans.size()) ans = str.substr(b, e - b + 1);
    e++;
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

namespace NB {
#define MAX_CHARS 26
using namespace std;
 
// This function calculates number of unique characters
// using a associative array count[]. Returns true if
// no. of characters are less than required else returns
// false.
bool isValid(int count[], int k)
{
    int val = 0;
    for (int i=0; i<MAX_CHARS; i++)
        if (count[i] > 0)
            val++;
 
    // Return true if k is greater than or equal to val
    return (k >= val);
}
 
// Finds the maximum substring with exactly k unique chars
void kUniques(string s, int k)
{
    int u = 0; // number of unique characters
    int n = s.length();
 
    // Associative array to store the count of characters
    int count[MAX_CHARS];
    memset(count, 0, sizeof(count));
 
    // Traverse the string, Fills the associative array
    // count[] and count number of unique characters
    for (int i=0; i<n; i++)
    {
        if (count[s[i]-'a']==0)
            u++;
        count[s[i]-'a']++;
    }
 
    // If there are not enough unique characters, show
    // an error message.
    if (u < k)
    {
        cout << "Not enough unique characters";
        return;
    }
 
    // Otherwise take a window with first element in it.
    // start and end variables.
    int curr_start = 0, curr_end = 0;
 
    // Also initialize values for result longest window
    int max_window_size = 1, max_window_start = 0;
 
    // Initialize associative array count[] with zero
    memset(count, 0, sizeof(count));
 
    count[s[0]-'a']++;  // put the first character
 
    // Start from the second character and add
    // characters in window according to above
    // explanation
    for (int i=1; i<n; i++)
    {
        // Add the character 's[i]' to current window
        count[s[i]-'a']++;
        curr_end++;
 
        // If there are more than k unique characters in
        // current window, remove from left side
        while (!isValid(count, k))
        {
            count[s[curr_start]-'a']--;
            curr_start++;
        }
 
        // Update the max window size if required
        if (curr_end-curr_start+1 > max_window_size)
        {
            max_window_size = curr_end-curr_start+1;
            max_window_start = curr_start;
        }
    }
 
    cout << "Max sustring is : "
         << s.substr(max_window_start, max_window_size)
         << " with length " << max_window_size << endl;
}
}  // namespace NB

int main(int argc, char** argv) {
  LOG(INFO) << LongestSubstrExactly("aaaaaaaaaaabcef", 2);
  LOG(INFO) << LongestSubstrMost("aaaaaaaaaaa", 2);
  NB::kUniques("aaab", 2);
  return 0;
}
