#include <QApplication>

#include "view/smartcalc.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::PolishModel model;
  s21::CalcController controller(&model);
  s21::SmartCalc view(&controller);
  view.show();
  return app.exec();
}
