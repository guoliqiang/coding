// Copyright 2012 Jike Inc. All Rights Reserved.
// Author: Ji Jian (jijian@jike.com)
// Modifier: LiqiangGuo (guoliqiang@pku.edu.cn)
// Date  : 2012-01-29 09:57:21
// File  : ACAutomaton.h
// Brief :
// sorting the patterns alphabetically will give a redudent automaton,
//  but the running time is not linear, so we encode the states first,
// then BFS the automaton to calculate the fail table and match action.
//
// I can build an automaton with goto table of every state for all input
// characters, it's fast, but will consume about 50 times more memory.
//
// For 10^5 rules, there're about 10^6 patterns, about 10^7 states,
// for each state the goto table will be 256*sizeof(uint32) = 1024, so total
// goto table will be consume 10G memory.
//
// Each document contains 2k characters on average, the linear time factor
// for binary search state at each step is 10, then for 200 document each core,
// there's 4M computation, which is quite small compared with rule testing.
//
// Finally, I chose memory saving method.
// The variable name is named after AC algorithm, for referrence, please check
// paper Efficient String Matching: An Aid to Bibliographic Search,
// by Alfred V. Aho and Margaret J. Corasick


#ifndef  ACAUTOMATON_H_
#define  ACAUTOMATON_H_

#include <map>
#include <vector>
#include <string>
#include "../../../base/public/basictypes.h"

namespace ac_automaton {
class ACAutomaton {
  private:
    struct GotoNode {
      uint16 c;
      uint32 state;
    };

 public:
    ACAutomaton();
    ~ACAutomaton();
    // @param[in] : patterns - the input patterns
    bool Init(const std::vector<std::basic_string<uint16> > &patterns);
    bool Init(const std::vector<std::string> &patterns);
    //
    void Reset();
    // @param[in] : text - the target text
    // @param[out] : matchedSets
    // @param[out] : matchedCountSets
    void Match(const std::basic_string<uint16> &text,
               std::map<std::basic_string<uint16>, std::vector<size_t> >
               *pMatchedPos) const;

    bool Match(const std::string &text,
               std::map<std::string, std::vector<size_t> >
               *pMatchedPos) const;
  private:
    // char is denoted by uint16, so at most 65536 children for each state
    uint32 m_totalStates;
    //
    uint16 * m_pChildrenCount;
    uint32 * m_pChildrenAddr;
    GotoNode * m_pChildren;
    // failed state pointer
    uint32 * m_pFail;
    //
    std::vector<std::vector<std::basic_string<uint16> > > m_matchPatternTable;
};
}  // namespace ac_automaton
#endif  // ACAUTOMATON_H_

