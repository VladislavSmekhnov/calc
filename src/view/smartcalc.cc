#include "smartcalc.h"

#include <regex>

#include "ui_smartcalc.h"

namespace s21 {
SmartCalc::SmartCalc(s21::CalcController *controller, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SmartCalc), controller_(controller) {
  ui->setupUi(this);
  ui->plotWidget->xAxis->setRange(-10, 10);
  ui->plotWidget->yAxis->setRange(-10, 10);
}

SmartCalc::~SmartCalc() {
  ui->plotWidget->clearGraphs();
  delete ui;
}

void SmartCalc::on_calc_button_clicked() {
  QString x_str = ui->x_lineEdit->text();
  double x = 0;
  bool ok = true;
  if (x_str.length() > 0) {  // проверка на пустую строку X
    x = x_str.toDouble(&ok);
  }
  if (!ok) {
    ui->x_lineEdit->setText("Error");
  } else {
    // считаем выражение и чек ошибки
    std::string expression = ui->expressionLineEdit->text().toStdString();
    try {
      controller_->SetExpression(expression);
    } catch (std::exception ex) {
      ui->expressionLineEdit->setText("Ошибка в выражении");
      return;
    }

    try {
      double result = controller_->Calc(x);
      QString new_text = QString::number(result, 'g', 16);
      ui->expressionLineEdit->setText(new_text);
    } catch (const std::exception &ex) {
      ui->expressionLineEdit->setText("Ошибка в вычислениях");
    }
  }
}

void SmartCalc::on_expressionLineEdit_textChanged(const QString &arg1) {
  // чистка строки после Error
  int str_len = arg1.length();
  if (arg1.startsWith("Error") && str_len > 5)
    ui->expressionLineEdit->setText(arg1.right(str_len - 5));
}

void SmartCalc::on_x_lineEdit_textEdited(const QString &arg1) {
  // валидация невалидных символов X
  int str_len = arg1.length();
  if (!valid_x(arg1) && str_len > 0)
    ui->x_lineEdit->setText(arg1.left(str_len - 1));
}

void SmartCalc::on_x_lineEdit_textChanged(const QString &arg1) {
  int str_len = arg1.length();
  if (arg1.startsWith("Error") && str_len > 5)
    // if "Error": clear last 5 chars, +1 if is not valid char (0-9 & arithm)
    ui->x_lineEdit->setText(arg1.right(str_len - 5 - !valid_x(arg1.last(1))));
}

bool SmartCalc::valid_x(const QString &arg1) {
  static const std::regex r(R"(^[0-9\.\-\+]+$)");
  return std::regex_match(arg1.toStdString().data(), r);
}

void SmartCalc::on_plot_button_clicked() {
  std::string expression = ui->expressionLineEdit->text().toStdString();
  if (expression.find('x') != std::string::npos && expression.length() > 0) {
    try {
      controller_->SetExpression(expression);
    } catch (const std::exception &ex) {
      ui->expressionLineEdit->setText(ex.what());
      return;
    }

    // достаем границы графика и считаем шаг (на 10000 точек)
    double left = ui->x_axis_left_LE->text().toDouble(),
           right = ui->x_axis_right_LE->text().toDouble();
    double step = (right - left) / 10000;
    step = (step == 0) ? 1 : step;

    ui->plotWidget->clearGraphs();
    ui->plotWidget->addGraph();
    ui->plotWidget->graph()->setLineStyle(QCPGraph::lsNone);
    ui->plotWidget->graph()->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssDisc, 2));

    // добавляем на график существующие пары значений (x, y)
    while (left <= right) {
      try {
        double y = controller_->Calc(left);
        ui->plotWidget->graph()->addData(left, y);
      } catch (const std::exception &ex) {
      }
      left += step;
    }

    ui->plotWidget->replot();
    ui->stackedWidget->setCurrentIndex(1);
  }
}

void SmartCalc::on_x_axis_left_LE_textChanged(const QString &arg1) {
  // обновляем границы и перерисовываем график
  bool ok1, ok2;
  double left = arg1.toDouble(&ok1),
         right = ui->x_axis_right_LE->text().toDouble(&ok2);
  if (ok1 && ok2) {
    ui->plotWidget->xAxis->setRange(left, right);
  }
  on_plot_button_clicked();
}

void SmartCalc::on_x_axis_right_LE_textChanged(const QString &arg1) {
  // обновляем границы и перерисовываем график
  bool ok1, ok2;
  double left = ui->x_axis_left_LE->text().toDouble(&ok1),
         right = arg1.toDouble(&ok2);
  if (ok1 && ok2) {
    ui->plotWidget->xAxis->setRange(left, right);
  }
  on_plot_button_clicked();
}

void SmartCalc::on_y_axis_left_LE_textChanged(const QString &arg1) {
  // обновляем границы и перерисовываем график
  bool ok1, ok2;
  double left = arg1.toDouble(&ok1),
         right = ui->y_axis_right_LE->text().toDouble(&ok2);
  if (ok1 && ok2) {
    ui->plotWidget->yAxis->setRange(left, right);
  }
  on_plot_button_clicked();
}

void SmartCalc::on_y_axis_right_LE_textChanged(const QString &arg1) {
  // обновляем границы и перерисовываем график
  bool ok1, ok2;
  double left = ui->y_axis_left_LE->text().toDouble(&ok1),
         right = arg1.toDouble(&ok2);
  if (ok1 && ok2) ui->plotWidget->yAxis->setRange(left, right);
  on_plot_button_clicked();
}

void SmartCalc::on_back_button_clicked() {
  ui->plotWidget->clearGraphs();
  ui->stackedWidget->setCurrentIndex(0);
}

void SmartCalc::on_clear_button_clicked() { ui->expressionLineEdit->clear(); }

void SmartCalc::on_left_bracket_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('('));
}

void SmartCalc::on_right_bracket_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append(')'));
}

void SmartCalc::on_sin_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("sin("));
}

void SmartCalc::on_asin_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("asin("));
}

void SmartCalc::on_cos_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("cos("));
}

void SmartCalc::on_acos_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("acos("));
}

void SmartCalc::on_tan_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("tan("));
}

void SmartCalc::on_atan_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("atan("));
}

void SmartCalc::on_log_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("log("));
}

void SmartCalc::on_ln_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("ln("));
}

void SmartCalc::on_sqrt_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("sqrt("));
}

void SmartCalc::on_pow_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('^'));
}

void SmartCalc::on_mod_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append("mod"));
}

void SmartCalc::on_plus_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('+'));
}

void SmartCalc::on_minus_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('-'));
}

void SmartCalc::on_mult_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('*'));
}

void SmartCalc::on_div_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('/'));
}

void SmartCalc::on_dot_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('.'));
}

void SmartCalc::on_num0_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('0'));
}

void SmartCalc::on_num1_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('1'));
}

void SmartCalc::on_num2_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('2'));
}

void SmartCalc::on_num3_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('3'));
}

void SmartCalc::on_num4_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('4'));
}

void SmartCalc::on_num5_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('5'));
}

void SmartCalc::on_num6_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('6'));
}

void SmartCalc::on_num7_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('7'));
}

void SmartCalc::on_num8_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('8'));
}

void SmartCalc::on_num9_button_clicked() {
  QString expression = ui->expressionLineEdit->text();
  ui->expressionLineEdit->setText(expression.append('9'));
}

void SmartCalc::on_expressionLineEdit_returnPressed() {
  on_calc_button_clicked();
}
}  // namespace s21
