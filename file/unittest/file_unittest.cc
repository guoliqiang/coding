#include "file/public/file.h"
#include "base/public/string_util.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

namespace file {

TEST(File, FuzzyFind) {
  std::string pattern = "/tmp/*.cc";
  std::vector<std::string> files;
  file::File::FuzzyFind(pattern, &files);
  LOG(INFO) << JoinVector(files);
}

}  // namespace file
