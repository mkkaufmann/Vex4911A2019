#ifndef PID_HPP
#define PID_HPP
#include "ui.hpp"
#include "pros/apix.h"
class PIDTuner{
public:
  static void initialize();

  static void show();

  static void update();

  static void hide();

  static void toggle();

private:
  static bool initialized;
  static bool visible;

  static lv_obj_t * pid_container;
  static lv_obj_t * pid_label;
};
#endif
