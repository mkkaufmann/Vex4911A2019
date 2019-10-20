#ifndef UIHELPER_HPP
#define UIHELPER_HPP
#include "ui.hpp"
#include "../pros/apix.h"
class UIHelper{
public:

  static void initialize();

  static void updateDisplay(int l, int r, int b, double x, double y, double theta);

private:

};
#endif
