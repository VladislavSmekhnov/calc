#ifndef CPP3_SMARTCALC_V2_0_SRC_CONTROLLER_CALC_CONTROLLER_H_
#define CPP3_SMARTCALC_V2_0_SRC_CONTROLLER_CALC_CONTROLLER_H_

#include "../model/polish_model.h"

namespace s21 {
class CalcController {
 public:
  explicit CalcController(PolishModel *model) : model_(model){};
  ~CalcController() = default;
  std::string GetExpression() const;
  void SetExpression(const std::string &expr);
  double Calc(double x);

 private:
  PolishModel *model_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_SRC_CONTROLLER_CALC_CONTROLLER_H_