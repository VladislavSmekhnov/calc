#ifndef CPP3_SMARTCALC_V2_0_SMART_CALC_VIEW_H
#define CPP3_SMARTCALC_V2_0_SMART_CALC_VIEW_H

#include <QMainWindow>

#include "../controller/calc_controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SmartCalc;
}
QT_END_NAMESPACE

namespace s21 {
class SmartCalc : public QMainWindow {
  Q_OBJECT

 public:
  SmartCalc(CalcController *controller, QWidget *parent = nullptr);
  ~SmartCalc();

 private slots:
  // manipulation
  void on_calc_button_clicked();
  void on_clear_button_clicked();
  void on_left_bracket_button_clicked();
  void on_right_bracket_button_clicked();

  // functions
  void on_sin_button_clicked();
  void on_asin_button_clicked();
  void on_cos_button_clicked();
  void on_acos_button_clicked();
  void on_tan_button_clicked();
  void on_atan_button_clicked();
  void on_log_button_clicked();
  void on_ln_button_clicked();
  void on_sqrt_button_clicked();

  // operations
  void on_pow_button_clicked();
  void on_mod_button_clicked();
  void on_plus_button_clicked();
  void on_minus_button_clicked();
  void on_mult_button_clicked();
  void on_div_button_clicked();

  // numbers
  void on_dot_button_clicked();
  void on_num0_button_clicked();
  void on_num1_button_clicked();
  void on_num2_button_clicked();
  void on_num3_button_clicked();
  void on_num4_button_clicked();
  void on_num5_button_clicked();
  void on_num6_button_clicked();
  void on_num7_button_clicked();
  void on_num8_button_clicked();
  void on_num9_button_clicked();

  void on_expressionLineEdit_returnPressed();
  void on_expressionLineEdit_textChanged(const QString &arg1);
  void on_x_lineEdit_textEdited(const QString &arg1);
  void on_x_lineEdit_textChanged(const QString &arg1);
  void on_plot_button_clicked();
  void on_back_button_clicked();
  void on_x_axis_left_LE_textChanged(const QString &arg1);
  void on_x_axis_right_LE_textChanged(const QString &arg1);
  void on_y_axis_left_LE_textChanged(const QString &arg1);
  void on_y_axis_right_LE_textChanged(const QString &arg1);

 private:
  Ui::SmartCalc *ui;
  CalcController *controller_;

  bool valid_x(const QString &arg1);
};
}  // namespace s21
#endif  // CPP3_SMARTCALC_V2_0_SMART_CALC_VIEW_H
