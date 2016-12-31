#include <gtest/gtest.h>
#include <iostream>
#include <dirent.h>
#include <sstream>
#include <fstream>

using namespace std;

class Solution {
 public:
  static void run(istream &is, ostream &os);
};

namespace {

typedef pair<stringstream *, stringstream *> TestData;

class Filesystem {
 public:
  static vector<TestData> ListDir() {
    vector<TestData> test_data;
    struct dirent *file;
    DIR *test_directory = opendir(".");
    while ((file = readdir(test_directory))) {
      string filename = file->d_name;
      if (is_test(filename)) {
        fstream f(filename);
        auto input_stream = CreateStream(f, [](const string &buffer) {
          return buffer == "EXPECTED:";
        });
        auto expected_stream = CreateStream(f, [](const string &) {
          return false;
        });
        f.close();
        test_data.emplace_back(input_stream, expected_stream);
      }
    }
    return test_data;
  }
 private:
  static stringstream * CreateStream(fstream &f,
                                     const function<bool(string&)> &predicate) {
    stringstream *s = new stringstream();
    string buffer;
    while(f >> buffer && !predicate(buffer))
      *s << buffer << " ";
    return s;
  }

  bool static is_test(const string &filename) {
    return filename.find("test") != string::npos && !opendir(filename.c_str());
  }
};

class ProblemTest: public ::testing::TestWithParam<TestData> { };

TEST_P(ProblemTest, Contest) {
  auto pair = GetParam();
  istream &input_stream = *pair.first;
  istream &expected_stream = *pair.second;
  stringstream output_stream;

  ASSERT_NO_THROW(Solution::run(input_stream, output_stream));

  string output_buffer, expected_buffer;
  while (output_stream >> output_buffer) {
    if (!(expected_stream >> expected_buffer))
      FAIL() << "The solution generates unnecessery output";
    try {
      long double output_value = stold(output_buffer);
      long double expected_value = stold(expected_buffer);
      EXPECT_DOUBLE_EQ(output_value, expected_value);
    } catch (exception &e) {
      EXPECT_EQ(output_buffer, expected_buffer);
    }
  }
}

INSTANTIATE_TEST_CASE_P(TestsEnumerating, ProblemTest,
                        ::testing::ValuesIn(Filesystem::ListDir()));

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


