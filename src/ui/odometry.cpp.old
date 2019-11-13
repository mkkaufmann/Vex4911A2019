#include "odometry.hpp"

lv_obj_t * Odometry::odom_container = lv_cont_create(lv_scr_act(), NULL);
lv_obj_t * Odometry::odom_l_enc_label = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_l_enc_val = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_r_enc_label = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_r_enc_val = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_b_enc_label = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_b_enc_val = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_x_label = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_x_val = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_y_label = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_y_val = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_theta_label = lv_label_create(Odometry::odom_container, NULL);
lv_obj_t * Odometry::odom_theta_val = lv_label_create(Odometry::odom_container, NULL);

bool Odometry::visible = false;
bool Odometry::initialized = false;

void Odometry::initialize(){
  visible = true;
  initialized = true;

  lv_obj_set_size(odom_container, 400, 240);

  lv_label_set_text(odom_l_enc_label, "L: ");
  lv_label_set_text(odom_r_enc_label, "R: ");
  lv_label_set_text(odom_b_enc_label, "B: ");
  lv_label_set_text(odom_x_label, "X: ");
  lv_label_set_text(odom_y_label, "Y: ");
  lv_label_set_text(odom_theta_label, "Theta: ");

  int l_enc_x = 3;
  int l_enc_y = 0;
  int r_enc_x = 3;
  int r_enc_y = 20;
  int b_enc_x = 3;
  int b_enc_y = 40;
  int x_x = 3;
  int x_y = 60;
  int y_x = 3;
  int y_y = 80;
  int theta_x = 3;
  int theta_y = 100;

  lv_obj_set_pos(odom_l_enc_label, l_enc_x, l_enc_y);
  lv_obj_set_pos(odom_r_enc_label, r_enc_x, r_enc_y);
  lv_obj_set_pos(odom_b_enc_label, b_enc_x, b_enc_y);
  lv_obj_set_pos(odom_x_label, x_x, x_y);
  lv_obj_set_pos(odom_y_label, y_x, y_y);
  lv_obj_set_pos(odom_theta_label, theta_x, theta_y);

  lv_obj_set_pos(odom_l_enc_val, l_enc_x + 15, l_enc_y);
  lv_obj_set_pos(odom_r_enc_val, r_enc_x + 15, r_enc_y);
  lv_obj_set_pos(odom_b_enc_val, b_enc_x + 15, b_enc_y);
  lv_obj_set_pos(odom_x_val, x_x + 17, x_y);
  lv_obj_set_pos(odom_y_val, y_x + 17, y_y);
  lv_obj_set_pos(odom_theta_val, theta_x + 54, theta_y);
}

void Odometry::update(int l, int r, int b, double x, double y, double theta){
  lv_label_set_text(odom_l_enc_val, &std::to_string(l)[0]);
  lv_label_set_text(odom_r_enc_val, &std::to_string(r)[0]);
  lv_label_set_text(odom_b_enc_val, &std::to_string(b)[0]);
  lv_label_set_text(odom_x_val, &std::to_string(x)[0]);
  lv_label_set_text(odom_y_val, &std::to_string(y)[0]);
  lv_label_set_text(odom_theta_val, &std::to_string(theta)[0]);
}

void Odometry::hide(){
  if(visible && initialized){
    visible = false;
    lv_obj_set_hidden(odom_container, true);
  }
}

void Odometry::show(){
  if(!visible && initialized){
    visible = true;
    lv_obj_set_hidden(odom_container, false);
  }
}

void Odometry::toggle(){
  if(initialized){
    if(visible){
      hide();
    }else{
      show();
    }
  }
}
