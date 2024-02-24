#include "polish_model.h"

namespace s21 {
std::string PolishModel::getExpression() const { return expr_; }

void PolishModel::setExpression(const std::string &expr) {
  if (expr_.empty() || expr_.compare(expr) != 0) {
    expr_ = std::move(expr);
  }
  generatePolish();
}

void PolishModel::generatePolish() {
  std::string reduced = reduceFunctions();
  polish_ = reducedToPolish(reduced);
}

std::string PolishModel::reduceFunctions() {
  std::string reduced;
  std::string funcs[] = {"mod",  "sin",  "cos", "tan", "asin",
                         "acos", "atan", "ln",  "log", "sqrt"};
  char short_funcs[] = {'%', 's', 'c', 't', 'i', 'o', 'a', 'n', 'g', 'q'};
  for (size_t i = 0; i < expr_.length(); i++) {
    char current = expr_[i];
    if (current == ' ') continue;
    if (strchr("msctal", current)) {
      std::string sub = expr_.substr(i);
      for (int j = 0; j < 10; j++) {
        if (sub.find(funcs[j]) == 0) {
          reduced.append(1, short_funcs[j]);
          i += funcs[j].length() - 1;
          break;
        }
      }
      if (expr_[i + 1] != '(' && reduced[reduced.length() - 1] != '%') {
        throw std::invalid_argument("Некорректная конфигурация скобок");
      }
    } else {
      if (!isValidChar(current)) {
        throw std::invalid_argument("Некорректные символы");
      }
      reduced.append(1, current);
    }
  }
  return reduced;
}

std::string PolishModel::reducedToPolish(const std::string &reduced) {
  std::string polish;
  char prev = '\0';
  for (size_t i = 0; i < reduced.length(); i++) {
    char current = reduced[i];
    if (isdigit(current)) {
      size_t pos = 0;
      std::stod(reduced.substr(i), &pos);
      polish.append(reduced.substr(i, pos));
      polish.append(" ");
      i += pos - 1;
    } else if (current == 'x') {
      polish.append("x ");
    } else if (current == '(') {
      oper_stack_.push(current);
    } else if (isOperator(current)) {
      if (isOperator(prev) || prev == '(') {
        current = replaceUnarArifm(current);
      }
      fromStackUpToPriority(polish, current);
    } else if (current == ')') {
      fromStackUpToBracket(polish);
    } else {
      throw std::invalid_argument("Некорректная последовательность операторов");
    }
    prev = current;
  }

  while (!oper_stack_.empty()) {
    char oper = oper_stack_.top();
    oper_stack_.pop();
    if (oper == '(' || oper == ')') {
      throw std::invalid_argument("Некорректная конфигурация скобок");
    }
    polish.append(1, oper);
    polish.append(" ");
  }

  return polish;
}

void PolishModel::fromStackUpToBracket(std::string &polish) {
  char stack_operator = '\0';
  while (!oper_stack_.empty() && stack_operator != '(') {
    stack_operator = oper_stack_.top();
    if (stack_operator != '(') {
      stack_operator = oper_stack_.top();
      oper_stack_.pop();
      polish.append(1, stack_operator);
      polish.append(" ");
    }
  }
  if (stack_operator != '(') {
    throw std::invalid_argument("Некорректная конфигурация скобок");
  }
  if (!oper_stack_.empty()) oper_stack_.pop();

  if (!oper_stack_.empty()) {
    stack_operator = oper_stack_.top();
    if (getOperatorPriority(stack_operator) == 4) {
      polish.append(1, oper_stack_.top());
      polish.append(" ");
      oper_stack_.pop();
    }
  }
}

void PolishModel::fromStackUpToPriority(std::string &polish, char cur_oper) {
  bool oper_greater = true;
  while (!oper_stack_.empty() && oper_greater) {
    char stack_operator = oper_stack_.top();
    int cur_priority = getOperatorPriority(cur_oper);
    int stack_priority = getOperatorPriority(stack_operator);
    bool isLeftAssociate = getOperatorPriority(cur_oper) < 3;
    oper_greater = (stack_priority > cur_priority) ||
                   (stack_priority == cur_priority && isLeftAssociate);
    if (oper_greater) {
      polish.append(1, oper_stack_.top());
      polish.append(" ");
      oper_stack_.pop();
    }
  }
  oper_stack_.push(cur_oper);
}

int PolishModel::isOperator(char c) noexcept {
  return getOperatorPriority(c) > 0;
}

int PolishModel::getOperatorPriority(char c) noexcept {
  int res = 0;
  if (strchr("sctioangq~#", c)) res = 4;
  if (strchr("^", c)) res = 3;
  if (strchr("*/%", c)) res = 2;
  if (strchr("+-", c)) res = 1;
  return res;
}

char PolishModel::replaceUnarArifm(char c) noexcept {
  if (c == '-') return '~';
  if (c == '+') return '#';
  return c;
}

bool PolishModel::isValidChar(char c) {
  const char *vars = "x+-*/^().e ";
  return isdigit(c) || strchr(vars, c);
}

double PolishModel::CalcResult(double x) {
  for (size_t i = 0; i < polish_.length(); i++) {
    char current = polish_[i];
    if (isdigit(current)) {
      size_t pos = 0;
      double number = std::stod(polish_.substr(i), &pos);
      i += pos - 1;
      val_stack_.push(number);
    } else if (current == 'x') {
      val_stack_.push(x);
    } else if (isOperator(current)) {
      calcPolishOperator(current);
    }
  }

  double result = 0;
  if (!val_stack_.empty()) {
    result = val_stack_.top();
    val_stack_.pop();
  }
  return result;
}

void PolishModel::calcPolishOperator(char oper) {
  // Проверяем унарные операторы
  if (getOperatorPriority(oper) == 4) {
    // Проверяем, если есть 1 число - берем его, если нет - задаем 0
    if (val_stack_.empty()) {
      throw std::runtime_error("Ошибка в вычислениях");
    }
    double num = val_stack_.top();
    val_stack_.pop();
    num = calcByUnarFunc(num, oper);
    val_stack_.push(num);
  } else {
    // Достаем последние 2 числа и применяем оператор
    if (val_stack_.empty()) {
      throw std::runtime_error("Ошибка в вычислениях");
    }
    double num2 = val_stack_.top();
    val_stack_.pop();
    if (val_stack_.empty()) {
      throw std::runtime_error("Ошибка в вычислениях");
    }
    double num1 = val_stack_.top();
    val_stack_.pop();
    num1 = calcByOperator(num1, num2, oper);
    val_stack_.push(num1);
  }
}

double PolishModel::calcByUnarFunc(double num, char oper) {
  double result;
  if (oper == 's')
    result = sin(num);
  else if (oper == 'c')
    result = cos(num);
  else if (oper == 't')
    result = tan(num);
  else if (oper == 'i')
    result = asin(num);
  else if (oper == 'o')
    result = acos(num);
  else if (oper == 'a')
    result = asin(num / sqrt(1 + num * num));
  else if (oper == 'n')
    result = log(num);
  else if (oper == 'g')
    result = log10(num);
  else if (oper == 'q')
    result = sqrt(num);
  else if (oper == '~')
    result = -num;
  else if (oper == '#')
    result = num;
  else
    throw std::runtime_error("Некорректный оператор");

  if (std::isnan(result) || std::isinf(result)) {
    throw std::runtime_error("Некорректное значение");
  }
  return result;
}

double PolishModel::calcByOperator(double num1, double num2, char oper) {
  double result;

  if (oper == '+')
    result = num1 + num2;
  else if (oper == '-')
    result = num1 - num2;
  else if (oper == '*')
    result = num1 * num2;
  else if (oper == '/')
    result = num1 / num2;
  else if (oper == '%')
    result = fmod(num1, num2);
  else if (oper == '^')
    result = pow(num1, num2);
  else
    throw std::runtime_error("Некорректный оператор");

  if (std::isnan(result) || std::isinf(result)) {
    throw std::runtime_error("Некорректное значение");
  }

  return result;
}
}  // namespace s21
