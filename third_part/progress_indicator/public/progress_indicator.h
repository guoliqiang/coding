// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#ifndef UTIL_PROGRESS_INDICATOR_PROGRESS_INDICATOR_H_
#define UTIL_PROGRESS_INDICATOR_PROGRESS_INDICATOR_H_

#include <sys/ioctl.h>
#include <stdio.h>
#include <string>
#include "base/public/flags.h"
#include "base/public/time.h"
#include "base/public/string_util.h"

namespace util {

using std::string;

DECLARE_bool(disable_progress_indicator);

template <class T>
class ProgressIndicator {
  private:
    static const uint32 SPEED_TEST_INTERVAL = 1;
    static const uint32 MIN_PERCENTAGE_BAR_WIDTH = 20;
  public:
    ProgressIndicator() {
      Init();
    }
    explicit ProgressIndicator(T max) {
      Init();
      max_ = max;
    }

    void Init() {
      value_ = 0;
      max_ = 0;
      is_terminal_ = false;
      output_fp_ = stdout;
      description_ = NULL;
      unit_ = NULL;
      finished_ = false;
      last_value_ = 0;
      start_time_ = base::Time::Now();
      last_time_ = start_time_;
      UpdateWidth();
      speed_ = 0.0f;
    }
    void SetValue(T value) {
      base::Time now = base::Time::Now();
      uint32 age = (now - last_time_).InSeconds();
      if (age >= SPEED_TEST_INTERVAL) {
        speed_ = static_cast<double>(value - last_value_)
          / (now - last_time_).InSecondsF();
        last_time_ = now;
        last_value_ = value;
      }
      value_ = value;
      ShowProgress();
    }
    void SetMax(T max) {
      max_ = max;
    }

    void SetDescription(const char *description, const char * unit = "") {
      description_ = description;
      unit_ = unit;
    }

    void SetOutput(FILE *fp) {
      output_fp_= fp;
    }

    void Finish() {
      if (!finished_) {
        finished_ = true;
        end_time_ = base::Time::Now();
        ShowStatusLine();
        fprintf(output_fp_, "\n");
        fflush(output_fp_);
      }
    }

  private:
    static string ToString(int t) {
      return StringPrintf("%d", t);
    }
    static string ToString(uint32 t) {
      return StringPrintf("%u", t);
    }
    static string ToString(uint64 t) {
      return StringPrintf("%lu", t);
    }
    static string ToString(double t) {
      return StringPrintf("%f", t);
    }

    void ShowStatusLine() {
      string left = Description();
      string right;
      string mid = "";
      right = StringPrintf("%s %s %s",
          Percentage().c_str(), Speed().c_str(), ETA().c_str());

      int32 mid_len = width_ - left.size() - right.size();
      if (mid_len <= 0) {
        mid_len = 1;
      }

      if (HasMax() && mid_len > MIN_PERCENTAGE_BAR_WIDTH) {
        double percentage = 1.0 * value_ / max_;
        if (percentage > 1.0) {
          percentage = 1.0;
        }
        int32 arrow_width = (mid_len - 2) * percentage - 1;
        if (arrow_width <=0) {
          arrow_width = 0;
        }
        mid = StringPrintf("[%s>%s]",
            string(arrow_width, '=').c_str(),
            string(mid_len - arrow_width - 3, ' ').c_str());
      } else {
        mid = string(mid_len, ' ');
      }

      if (HasMax()) {
        fprintf(output_fp_, "%s%s%s",
            left.c_str(),
            mid.c_str(),
            right.c_str());
      } else {
        fprintf(output_fp_, "%s%s%s",
            left.c_str(),
            right.c_str(),
            mid.c_str());
      }
    }

    void UpdateWidth() {
      struct winsize w;
      if (ioctl(0, TIOCGWINSZ, &w) >= 0) {
        width_ = w.ws_col;
        is_terminal_ = true;
      } else {
        is_terminal_ = false;
        width_ = 80;
      }
    }

    const char * Unit() {
      return unit_ ? unit_ : "";
    }

    const std::string Percentage() {
      if (HasMax()) {
        return StringPrintf("(%.02f%%)", 100.0 * value_ / max_);
      } else {
        return ToString(value_) + Unit();
      }
    }

    const char * Description() {
      return description_ ? description_ : "";
    }

    std::string Speed() {
      if (finished_) {
        return StringPrintf("%.02f%s/s",
            static_cast<double>(value_) / (end_time_ - start_time_)
            .InSecondsF(),
            Unit());
      } else {
        return StringPrintf("%.02f%s/s", speed_, Unit());
      }
    }

    std::string ETA() {
      if (finished_) {
        uint32 elapsed_seconds  = (end_time_ - start_time_).InSeconds();
        return StringPrintf("%02d:%02d:%02d Used",
            elapsed_seconds / 3600,
            (elapsed_seconds % 3600) / 60,
            elapsed_seconds % 60);
      } else if (!HasMax()) {
        return std::string("");
      } else {
        uint32 eta = base::TimeDelta::FromSeconds(
            static_cast<double>(max_ - value_)/speed_).InSeconds();
        return StringPrintf("%02d:%02d:%02d ETA",
            eta / 3600, (eta % 3600) / 60, eta % 60);
      }
    }


    void ShowProgress() {
      UpdateWidth();
      if (IsEnabled() && !IsLast()) {
        ShowStatusLine();
      }
      if (IsLast()) {
        Finish();
      } else {
        fprintf(output_fp_, "\r");
        fflush(output_fp_);
      }
    }

    bool HasMax() {
      return max_ > 0;
    }

    bool IsLast() {
      return HasMax() && value_ == max_;
    }

    bool IsEnabled() {
      return !FLAGS_disable_progress_indicator && is_terminal_;
    }
    base::Time last_time_;
    base::Time start_time_;
    base::Time end_time_;
    bool finished_;
    const char * description_;
    const char * unit_;
    double speed_;
    FILE* output_fp_;
    T last_value_;
    T max_;
    T value_;
    uint32 width_;
    bool is_terminal_;
};
}  // namespace util

#endif  // UTIL_PROGRESS_INDICATOR_PROGRESS_INDICATOR_H_
