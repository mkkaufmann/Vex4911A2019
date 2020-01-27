#ifndef UIHELPER_HPP
#define UIHELPER_HPP
#include "../pros/apix.h"
#include "ui.hpp"
class UIHelper {
public:
  static void initialize();

  static void updateDisplay(int l, int r, int b, double x, double y,
                            double theta);

private:
};
#endif
