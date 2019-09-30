#include "sidebar.hpp"
#include "odometry.hpp"
#include "pidtuner.hpp"

lv_obj_t * Sidebar::button_container = lv_cont_create(lv_scr_act(), NULL);
lv_obj_t * Sidebar::odom_button =lv_btn_create(Sidebar::button_container,NULL);
lv_obj_t * Sidebar::odom_button_label =lv_label_create(Sidebar::odom_button,NULL);
lv_obj_t * Sidebar::pid_button =lv_btn_create(Sidebar::button_container,NULL);
lv_obj_t * Sidebar::pid_button_label =lv_label_create(Sidebar::pid_button,NULL);

const int Sidebar::NONE = 0;
const int Sidebar::ODOMETRY = 1;
const int Sidebar::PID = 2;
int Sidebar::currentMenu = Sidebar::NONE;

lv_btn_state_t Sidebar::odomLastState = LV_BTN_STATE_TGL_PR;
lv_btn_state_t Sidebar::pidLastState = LV_BTN_STATE_TGL_PR;


bool Sidebar::visible = false;
bool Sidebar::initialized = false;

void Sidebar::initialize(){
  visible = true;
  initialized = true;

  int button_cont_width=80;

  lv_obj_set_pos(button_container, 400, 0);
  lv_obj_set_size(button_container, button_cont_width, 240);

  lv_obj_set_size(odom_button, button_cont_width, 80);
  lv_btn_set_toggle(odom_button, true);
  lv_label_set_text(odom_button_label, "ODOM");

  lv_obj_set_pos(pid_button, 0, 80);
  lv_obj_set_size(pid_button, button_cont_width, 80);
  lv_btn_set_toggle(pid_button, true);
  lv_label_set_text(pid_button_label, "PID");

  odomLastState = lv_btn_get_state(odom_button);

  lv_label_set_text(odom_button_label, "ODOM");
}

void Sidebar::update(){
  //odom button
  lv_btn_state_t odom_state = lv_btn_get_state(odom_button);
  if(odom_state != odomLastState){
    if(odom_state == LV_BTN_STATE_TGL_REL){
      Odometry::hide();
      currentMenu = NONE;
    }else if(odom_state == LV_BTN_STATE_TGL_PR){
      switch(currentMenu){
        case PID:
          PIDTuner::hide();
          lv_btn_set_state(pid_button, LV_BTN_STATE_TGL_REL);
          pidLastState = LV_BTN_STATE_TGL_REL;
      }
      Odometry::show();
      currentMenu = ODOMETRY;
    }
    odomLastState = odom_state;
  }
  //pid button
  lv_btn_state_t pid_state = lv_btn_get_state(pid_button);
  if(pid_state != pidLastState){
    if(pid_state == LV_BTN_STATE_TGL_REL){
      PIDTuner::hide();
      currentMenu = NONE;
    }else if(pid_state == LV_BTN_STATE_TGL_PR){
      switch(currentMenu){
        case ODOMETRY:
          Odometry::hide();
          lv_btn_set_state(odom_button, LV_BTN_STATE_TGL_REL);
          odomLastState = LV_BTN_STATE_TGL_REL;
      }
      PIDTuner::show();
      currentMenu = PID;
    }
    pidLastState = pid_state;
  }
}

void Sidebar::hide(){
  if(visible && initialized){
    visible = false;
    lv_obj_set_hidden(button_container, true);
  }
}

void Sidebar::show(){
  if(!visible && initialized){
    visible = true;
    lv_obj_set_hidden(button_container, false);
  }
}
