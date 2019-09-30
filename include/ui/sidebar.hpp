#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP
#include "ui.hpp"
#include "pros/apix.h"
class Sidebar{
public:
  static void initialize();

  static void show();

  static void update();

  static void hide();

private:
    static bool visible;
    static bool initialized;
    static lv_btn_state_t odomLastState;
    static lv_btn_state_t pidLastState;
    static int currentMenu;

    static const int NONE;
    static const int ODOMETRY;
    static const int PID;

    static lv_obj_t * button_container;
    static lv_obj_t * odom_button;
    static lv_obj_t * odom_button_label;
    static lv_obj_t * pid_button;
    static lv_obj_t * pid_button_label;
};
#endif
