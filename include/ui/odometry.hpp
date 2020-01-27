#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP
#include "pros/apix.h"
#include "ui.hpp"
class Odometry {
public:
  static void initialize();

  static void show();

  static void update(int l, int r, int b, double x, double y, double theta);

  static void hide();

  static void toggle();

private:
  static bool initialized;
  static bool visible;

  static lv_obj_t *odom_container;
  static lv_obj_t *odom_l_enc_label;
  static lv_obj_t *odom_l_enc_val;
  static lv_obj_t *odom_r_enc_label;
  static lv_obj_t *odom_r_enc_val;
  static lv_obj_t *odom_b_enc_label;
  static lv_obj_t *odom_b_enc_val;
  static lv_obj_t *odom_x_label;
  static lv_obj_t *odom_x_val;
  static lv_obj_t *odom_y_label;
  static lv_obj_t *odom_y_val;
  static lv_obj_t *odom_theta_label;
  static lv_obj_t *odom_theta_val;
};
#endif
