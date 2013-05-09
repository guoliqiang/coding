// Copyright 2012 Jike Inc. All Rights Reserved.
// Author: Ji Jian (jijian@jike.com)
// Modifier: LiqiangGuo (guoliqiang@pku.edu.cn)
// Date  : 2012-01-29 09:57:21
// File  : ACAutomatonUnit.h
// Brief :

#ifndef  ACAUTOMATON_UNIT_H_
#define  ACAUTOMATON_UNIT_H_

#include <map>
#include <vector>
#include <string>
#include "../../../base/public/basictypes.h"
#include "../../../base/public/logging.h"
#include "./utils.h"
#include "../../../base/public/string_util.h"


static int kACLogLevel = 5;
using std::vector;
using std::map;
using std::basic_string;

namespace ac_automaton {
template <typename Key>
class ACAutomatonUnit {
  private:
    struct GotoNode {
      Key c;
      uint32 state;
    };

 public:
    ACAutomatonUnit();
    ~ACAutomatonUnit();
    // @param[in] : patterns - the input patterns
    bool Init(const std::vector<std::vector<Key> > &patterns);
    //
    void Reset();
    // @param[in] : text - the target text
    // @param[out] : matchedSets
    // @param[out] : matchedCountSets
    void Match(const std::vector<Key>  &text,
               std::map<std::vector<Key>, std::vector<size_t> >
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
    std::vector<std::vector<std::vector<Key> > > m_matchPatternTable;
};

template <typename Key>
ACAutomatonUnit<Key>::ACAutomatonUnit() : m_pChildrenCount(NULL),
                                          m_pChildrenAddr(NULL),
                                          m_pChildren(NULL),
                                          m_pFail(NULL),
                                          m_totalStates(0){}

//
template <typename Key>
ACAutomatonUnit<Key>::~ACAutomatonUnit() {
  Reset();
}

//
template <typename Key>
void ACAutomatonUnit<Key>::Reset() {
  SAFE_DELETE_ARRAY(m_pChildrenCount);
  SAFE_DELETE_ARRAY(m_pChildrenAddr);
  SAFE_DELETE_ARRAY(m_pChildren);
  SAFE_DELETE_ARRAY(m_pFail);
  m_totalStates = 0;
  m_matchPatternTable.clear();
}

//
template <typename Key>
bool ACAutomatonUnit<Key>::Init(const vector<vector<Key> > &patterns) {
  // initialize starting point, set up state zero with empty goto table
  vector<map<Key, uint32> > gotoTable;
  /*
   * [state] -> <char, state>
   *                    [0]
   *        |            |         |
   *     <char, 1>   <char, 2>  <char, 3>
   *       /             |           \
   *      [1]           [2]         [3]
   * */
  vector<vector<uint32> > matchSetTable;

  gotoTable.resize(1);
  m_matchPatternTable.resize(1);

  // generate goto table by STL types
  for (size_t i = 0; i < patterns.size(); ++i) {
    uint32 state = 0;
    const vector<Key> &pattern = patterns[i];
    for (size_t j = 0; j < pattern.size(); ++j) {
      Key c = pattern[j];
      if (gotoTable[state].find(c) == gotoTable[state].end()) {
        VLOG(kACLogLevel) << "state:" << state << " goto sate:"
                          << gotoTable.size() << " for value:" << c;
        gotoTable[state][c] = gotoTable.size();
        state = gotoTable.size();

        gotoTable.resize(gotoTable.size() + 1);
        matchSetTable.resize(matchSetTable.size() + 1);
        m_matchPatternTable.resize(m_matchPatternTable.size() + 1);
      } else {
        state = gotoTable[state][c];
      }
    }
    // now the new pattern is added end at state
    m_matchPatternTable[state].push_back(pattern);
    VLOG(kACLogLevel) << "state :" << state << " own pattern:"
                       << JoinVector(pattern, ',');
  }

  // convert STL types into build in types for better perf
  m_totalStates = gotoTable.size();
  VLOG(kACLogLevel) << "finally, the sate number is:" << m_totalStates;
  m_pChildrenCount = new (std::nothrow)uint16[m_totalStates];
  m_pChildrenAddr = new (std::nothrow)uint32[m_totalStates];
  m_pChildren = new (std::nothrow)GotoNode[m_totalStates];

  CHECK(m_pChildrenCount != NULL) << "memory is not enough!";
  CHECK(m_pChildrenAddr != NULL) << "memory is not enough!";
  CHECK(m_pChildren != NULL) << "memory is not enough!";

  uint32 edgeCount = 0;
  for (size_t i = 0; i < gotoTable.size(); ++i) {
    m_pChildrenCount[i] = static_cast<uint16>(gotoTable[i].size());
    m_pChildrenAddr[i] = edgeCount;
    typename map<Key, uint32>::const_iterator it;
    for (it = gotoTable[i].begin(); it != gotoTable[i].end(); ++it) {
      m_pChildren[edgeCount].c = it->first;
      m_pChildren[edgeCount].state = it->second;
      ++edgeCount;
    }
  }

  m_pFail = new (std::nothrow)uint32[m_totalStates];
  AutoDeleteArray<uint32> queue(new (std::nothrow)uint32[m_totalStates]);
  CHECK(m_pFail != NULL) << "memory is not enough";
  CHECK(queue.Valid()) << "memory is not enough";

  // calculate longest suffix transition(fail table)
  queue.Get()[0] = 0;
  m_pFail[0] = -1;
  for (size_t i = 0, j = 1; i < j; ++i) {
    uint32 state = queue.Get()[i];
    for (uint16 k = 0; k < m_pChildrenCount[state]; ++k) {
      Key c = m_pChildren[m_pChildrenAddr[state] + k].c;
      uint32 newState = m_pChildren[m_pChildrenAddr[state] + k].state;
      queue.Get()[j++] = newState;

      m_pFail[newState] = -1;
      uint32 oldState = m_pFail[state];
      while (m_pFail[newState] == -1) {
        if (oldState == -1) {
          m_pFail[newState] = 0;
          break;
        }
        // use signed integer to avoid under-flow
        int start = 0, end = m_pChildrenCount[oldState];
        GotoNode * array = m_pChildren + m_pChildrenAddr[oldState];
        // the goto state is in [start, end)
        while (start < end) {
          int mid = start + (end - start)/2;
          if (array[mid].c < c) {
            start = mid + 1;
          } else if (array[mid].c == c) {
            // this will be checked in outter loop, so no jump
            m_pFail[newState] = array[mid].state;
            VLOG(kACLogLevel) << "state:" << newState << " 's failed state is"
                              << array[mid].state;
            break;
          } else {
            end = mid;
          }
        }
        oldState = m_pFail[oldState];
      }
      m_matchPatternTable[newState].insert(
                  m_matchPatternTable[newState].end(),
                  m_matchPatternTable[m_pFail[newState]].begin(),
                  m_matchPatternTable[m_pFail[newState]].end());
    }
  }
  // set fail state of root to -1 is required for above logic
  m_pFail[0] = 0;
  return true;
}

template <typename Key>
void ACAutomatonUnit<Key>::Match(const vector<Key> &text,
                                 map<vector<Key>, vector<size_t> >
                                 * pMatchedPos) const {
  if (pMatchedPos != NULL) {
    pMatchedPos->clear();
  }
  uint32 state = 0;
  for (size_t i = 0; i < text.size(); ++i) {
    Key c = text[i];
    int start = 0, end = m_pChildrenCount[state];
    bool success = false;
    {
      GotoNode * array = m_pChildren + m_pChildrenAddr[state];
      while (start < end) {
        int mid = start + (end - start) / 2;
        if (array[mid].c < c) {
          start = mid + 1;
        } else if (array[mid].c == c) {
          success = true;
          state = array[mid].state;
          break;
        } else {
          end = mid;
        }
      }
    }
    if (success) {
      // generate matched patterns
      if (pMatchedPos) {
        for (size_t j = 0; j < m_matchPatternTable[state].size(); ++j) {
          const std::vector<Key> &str = m_matchPatternTable[state][j];
          if (pMatchedPos->find(str) == pMatchedPos->end()) {
            (*pMatchedPos)[str] = vector<size_t>();
          }
          (*pMatchedPos)[str].push_back(i + 1 - str.size());
        }
      }
    }
    if (!success && state != 0) {
      state = m_pFail[state];
      --i;
    }
  }
}
}  // namespace ac_automaton
#endif  // ACAUTOMATON_UNIT_H_

