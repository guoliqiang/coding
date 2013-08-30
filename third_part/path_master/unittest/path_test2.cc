// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include <vector>
#include <string>
#include <map>

#include "base/public/callback.h"
#include "file/public/file.h"

#include "../public/path_master.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

using path_master::PathMaster;
using path_master::ParserParam;
using parse_master::DomNode;

using namespace std;  // NOLINT
using namespace base;  // NOLINT

class SampleParser {
 public:
  explicit SampleParser(const vector<string>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
      result_[vec[i]] = false;
    }
  }
  ~SampleParser() {
  }

  bool HasPassAll() {
    int count = 0;
    map<string, bool>::iterator iter = result_.begin();
    while (iter != result_.end()) {
      if (iter->second == false) {
        LOG(INFO) << "error: " << endl << iter->first;
        count++;
      }
      iter++;
    }

    if (count == 0) {
      return true;
    } else {
      LOG(INFO) << "error: ( " << count
                << " / " << result_.size() << ")";
      return false;
    }
  }
  bool Parse(ParserParam *param) {
    //  该路经被触发了
    result_[*param->path()] = true;
    string content;
    param->node()->GetContent(&content);
    VLOG(1) << "content: " << content;
    return true;
  }

 private:
  map<string, bool> result_;
  DISALLOW_COPY_AND_ASSIGN(SampleParser);
};

bool test(const string& filename, const vector<string>& paths) {
  const string parser_name("test");

  PathMaster ypm;
  parse_master::ParseMaster pm;
  ypm.Init(&pm);

  SampleParser parser(paths);
  for (int i = 0; i < paths.size(); ++i) {
    ypm.RegistParser(parser_name, paths[i],
        NewPermanentCallback(&parser, &SampleParser::Parse));
  }

  string page_buff;
  file::File::ReadFileToString(filename, &page_buff);

  ypm.ResetCurrentParser(parser_name);
  pm.SetBuff(page_buff);
  pm.Parse();
  return parser.HasPassAll();
}

TEST(PathMasterUnittest, baidu_zhidao) {
  const string filename =
      "./testdata/zhidao1.html";
  vector<string> paths;
  //  以下包含非标准tag: <ca>
  //  问题描述的路径
  paths.push_back(
      "<div class=\"f14\" id=\"question_content\"><cd><pre>");

  paths.push_back("<html><body>");
  paths.push_back("<html><head><title>");
  paths.push_back("<html><head><meta>");
  //  以下这个却不能被触发，<cn>自定义
  paths.push_back("<div class=\"f14 p90 pl10 answer_content\">"
      "<span id=\"reply_content_531769369\"><cn><pre>");


  //  以下是测试绝对路径。
  //  最佳答案的时间
  paths.push_back("<html><body><div><div><div><div><div><div><div>"
                  "<div><p><span class=\"answer_time\">");
  paths.push_back("<body><div><div><div><div><div><div><div><div>"
                  "<p><span class=\"answer_time\">");
  paths.push_back("<div><div><div><div><div><div><div><div>"
                  "<p><span class=\"answer_time\">");
  paths.push_back("<div><div><div><div><div><div>"
                  "<p><span class=\"answer_time\">");
  paths.push_back("<div><div><div><div><div>"
                  "<p><span class=\"answer_time\">");
  paths.push_back("<div><div><div><div>"
                  "<p><span class=\"answer_time\">");
  paths.push_back("<div><div><div>"
                  "<p><span class=\"answer_time\">");
  paths.push_back("<div><div>"
                  "<p><span class=\"answer_time\">");
  paths.push_back("<div><p><span class=\"answer_time\">");

  //  以下成功
  paths.push_back("<p style><span class=\"answer_time\">");
  paths.push_back("<div class id=\"best_answer_info\" style align>"
                  "<p style><span class=\"answer_time\">");
  EXPECT_TRUE(test(filename, paths));
}

TEST(PathMasterUnittest, douban_movie) {
  const string filename =
      "./testdata/douban_movie1.html";
  vector<string> paths;
  //  “恶狗帮 Dog Pound”
  paths.push_back("<html><body><div id=\"wrapper\"><h1>");
  //  图片
  paths.push_back("<div class=\"related_info\">"
                  "<ul class><li><a href><img src>");
  //  "青春(193)  监狱(175)  法国(114)  剧情(72)  法国电影(70)
  //  英国(69)  2010(54)  加拿大(46)  "
  paths.push_back(
      "<html><body><div><div><div><div><div><div><div><a href>");
//  编剧
  paths.push_back("<html><body><div id=\"wrapper\">"
                  "<div id=\"content\">"
                  "<div><div><div><div><div><div><span><a href>");

  //  上映时间
  paths.push_back("<div id=\"info\"><div class=\"obmo\">");
  //  电影图片
  paths.push_back("<ul class><li><a href><img src>");
  //  简短评论时间
  paths.push_back("<div class=\"comment\"><h3 class><span class>");
//  简短评论的评论
  paths.push_back("<div class=\"article\"><div class>"
                  "<ul class=\"simple_list\"><li><div><p>");

  //  简短评论的评论者"[以梦为马]"
  paths.push_back("<h3><span class=\"fleft\"><a href>");

  //  第一个影评的内容
  paths.push_back("<div class=\"related_info\"><div><div>"
                  "<ul class style>"
                  "<li class=\"clst\"><div id>");

  EXPECT_TRUE(test(filename, paths));
}

TEST(PathMasterUnittest, dianping_shop) {
  const string filename =
      "./testdata/dianpingShop1.html";
  vector<string> paths;
  //  "锦绣净雅大酒店"
  paths.push_back("<html><body><form><div><div><div><div><div><div>"
                 "<div class=\"snamewrapper\"><h1><strong>");

  //  口味等分数
  paths.push_back("<html><body><form><div><div><div><div>"
                   "<p class=\"score\"><span class>");

  //  地址
  paths.push_back("<div class=\"objectiveinfo\"><address><p>");
  //  餐厅氛围
  paths.push_back("<div class=\"info\"><dl class=\"intro\">");
  paths.push_back("<div class=\"info\"><dl class=\"intro\"><dd>"
                  "<a class onclick href>");

//  第一条评论内容
  paths.push_back("<html><body><form><div class=\"main_w\">"
                  "<div class=\"content_a\">"
                  "<div><dl><dd><div id class>");

    //  第一条评论姓名
  paths.push_back("<html><body><form><div><div><div><dl><dt>"
                  "<cite><a class href>");
  EXPECT_TRUE(test(filename, paths));
}
