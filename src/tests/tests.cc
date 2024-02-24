#include <gtest/gtest.h>

#include <cmath>

#include "../model/polish_model.h"

using namespace s21;

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(CalcTests, Test1) {
  std::string exp = "2 ^ (15.25 mod 3) / sin(atan(9)) + ln(log(35 ^ sin(2)))";
  double correct = 1.5358628664;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult();
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test2) {
  std::string exp = "31.53288 * sqrt(8.512) - sin(-32 / (acos(0.132)))";
  double correct = 91.7453613234;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult();
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test3) {
  std::string exp = "-352.99912 / asin(-0.991) + (-13.001 * cos(log(33)))";
  double correct = 245.050763106;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult();
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test4) {
  std::string exp = "2 * afos(4)";

  EXPECT_ANY_THROW({
    PolishModel model;
    model.setExpression(exp);

    model.CalcResult();
  });
}

TEST(CalcTests, Test5) {
  std::string exp = "3213.534534";
  double correct = 3213.534534;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult();
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test6) {
  std::string exp = "2 * 3.15 / +1.575";
  double correct = 4;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult();
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test7) {
  std::string exp = "3 ^ x + sin(x) / sqrt(54 mod x)";
  double x = 11, correct = 177146.6837753309;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult(x);
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test8) {
  std::string exp = "x ^ sin(3) / log(x) + atan(x)";
  double x = 2.5512, correct = 4.0031675084;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult(x);
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test9) {
  std::string exp = "sin(cos(2 * log(35)) / sqrt(x ^ 2))";
  double x = 6.5, correct = -0.153022812;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult(x);
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test10) {
  std::string exp = "7.55 / (2*cos((35))";
  double x = 6.5;

  EXPECT_ANY_THROW({
    PolishModel model;
    model.setExpression(exp);

    model.CalcResult(x);
  });
}

TEST(CalcTests, Test11) {
  std::string exp = "((cos(2) / ln(3 / 1)) + 2 mod 3) - (2 * -x)";
  double x = 3.33, correct = 8.2812068253;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult(x);
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test12) {
  std::string exp = "ln(x ^ atan(acos(0.5))) / sqrt(x + 2 * (sin(asin(0.15))))";
  double x = 12, correct = 0.57280927214;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult(x);
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test13) {
  std::string exp = "ln(atan(acos(33))) / sqrt((sin(asin(1)))))";

  EXPECT_ANY_THROW({
    PolishModel model;
    model.setExpression(exp);

    model.CalcResult();
  });
}

TEST(CalcTests, Test14) {
  std::string exp = "tan(cos(-x * 2.5))";
  double x = 13, correct = 0.5051157645;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult(x);
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test15) {
  std::string exp = "sin(vag(3))";

  EXPECT_ANY_THROW({
    PolishModel model;
    model.setExpression(exp);

    model.CalcResult();
  });
}

TEST(CalcTests, Test16) {
  std::string exp = "sin(x) + ln(-1)";
  double x = 3;

  EXPECT_ANY_THROW({
    PolishModel model;
    model.setExpression(exp);

    model.CalcResult(x);
  });
}

TEST(CalcTests, Test17) {
  std::string exp = "cos(x) / sin(0)";
  double x = 3;

  EXPECT_ANY_THROW({
    PolishModel model;
    model.setExpression(exp);

    model.CalcResult(x);
  });
}

TEST(CalcTests, Test18) {
  std::string exp = "2 ^ 3 ^ cos(1)";
  double correct = 3.50754898464;

  PolishModel model;
  model.setExpression(exp);

  double res = model.CalcResult();
  EXPECT_EQ(fabs(res - correct) < 1e-7, true);
}

TEST(CalcTests, Test19) {
  PolishModel pm;
  std::string longExpression(253, '1');
  EXPECT_NO_THROW(pm.setExpression(longExpression + "+2"));
}