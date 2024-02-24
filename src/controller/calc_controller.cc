#include "calc_controller.h"

namespace s21 {
std::string CalcController::GetExpression() const {
  return model_->getExpression();
}

void CalcController::SetExpression(const std::string &expr) {
  model_->setExpression(expr);
}

double CalcController::Calc(double x) { return model_->CalcResult(x); }
}  // namespace s21