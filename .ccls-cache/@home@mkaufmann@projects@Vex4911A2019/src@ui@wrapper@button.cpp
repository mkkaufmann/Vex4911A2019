#include "ui/wrapper/button.hpp"

Button Button::create() { return Button(lv_btn_create(lv_scr_act(), NULL)); }

Button Button::create(const Object &iparent) {
  return Button(lv_btn_create(iparent.get(), NULL));
}

Button Button::create(const Object &iparent, const Button &icopy) {
  return Button(lv_btn_create(iparent.get(), icopy.get()));
}

Button &Button::setToggle(bool enabled) {
  lv_btn_set_toggle(object, enabled);
  return *this;
}

Button &Button::setState(lv_btn_state_t state) {
  lv_btn_set_state(object, state);
  return *this;
}

Button &Button::toggle() {
  lv_btn_toggle(object);
  return *this;
}

Button &Button::setAction(lv_btn_action_t type, lv_action_t action) {
  lv_btn_set_action(object, type, action);
  return *this;
}

Button &Button::setLayout(lv_layout_t layout) {
  lv_btn_set_layout(object, layout);
  return *this;
}

Button &Button::setFit(bool horizontal_enable, bool vertical_enable) {
  lv_btn_set_fit(object, horizontal_enable, vertical_enable);
  return *this;
}

Button &Button::setInkInTime(uint16_t time) {
  lv_btn_set_ink_in_time(object, time);
  return *this;
}

Button &Button::setInkWaitTime(uint16_t time) {
  lv_btn_set_ink_wait_time(object, time);
  return *this;
}

Button &Button::setInkOutTime(uint16_t time) {
  lv_btn_set_ink_out_time(object, time);
  return *this;
}

Button &Button::setStyle(lv_btn_style_t type, lv_style_t *style) {
  lv_btn_set_style(object, type, style);
  return *this;
}
