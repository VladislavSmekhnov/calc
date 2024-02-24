#ifndef CPP3_SMARTCALC_V2_0_SRC_MODEL_POLISH_MODEL_H_
#define CPP3_SMARTCALC_V2_0_SRC_MODEL_POLISH_MODEL_H_

#include <cmath>
#include <cstring>
#include <stack>
#include <stdexcept>
#include <string>

namespace s21 {
class PolishModel {
 public:
  PolishModel() = default;
  ~PolishModel() = default;
  double CalcResult(double x = 0);
  std::string getExpression() const;
  void setExpression(const std::string &expr);

 private:
  std::string expr_;
  std::string polish_;
  std::stack<char> oper_stack_;
  std::stack<double> val_stack_;

  void generatePolish();
  std::string reduceFunctions();
  std::string reducedToPolish(const std::string &reduced);
  static int isOperator(char c) noexcept;
  static int getOperatorPriority(char c) noexcept;
  void fromStackUpToPriority(std::string &polish, char cur_oper);
  void fromStackUpToBracket(std::string &polish);
  static char replaceUnarArifm(char c) noexcept;
  static bool isValidChar(char c);
  void calcPolishOperator(char oper);
  static double calcByUnarFunc(double num, char oper);
  static double calcByOperator(double num1, double num2, char oper);
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_SRC_MODEL_POLISH_MODEL_H_