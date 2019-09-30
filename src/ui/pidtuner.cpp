#include "pidtuner.hpp"

lv_obj_t * PIDTuner::pid_container = lv_cont_create(lv_scr_act(), NULL);
lv_obj_t * PIDTuner::pid_label = lv_label_create(PIDTuner::pid_container, NULL);

bool PIDTuner::visible = false;
bool PIDTuner::initialized = false;

void PIDTuner::initialize(){
  visible = true;
  initialized = true;

  lv_obj_set_size(pid_container, 400, 240);

  lv_label_set_text(pid_label, "PID Tuner");

}

void PIDTuner::update(){

}

void PIDTuner::hide(){
  if(visible && initialized){
    visible = false;
    lv_obj_set_hidden(pid_container, true);
  }
}

void PIDTuner::show(){
  if(!visible && initialized){
    visible = true;
    lv_obj_set_hidden(pid_container, false);
  }
}

void PIDTuner::toggle(){
  if(initialized){
    if(visible){
      hide();
    }else{
      show();
    }
  }
}
