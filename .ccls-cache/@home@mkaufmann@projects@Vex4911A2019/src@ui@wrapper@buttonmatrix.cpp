#include "ui/wrapper/buttonmatrix.hpp"

ButtonMatrix ButtonMatrix::create() {
  return ButtonMatrix(lv_btnm_create(lv_scr_act(), NULL));
}

ButtonMatrix ButtonMatrix::create(const Object &iparent) {
  return ButtonMatrix(lv_btnm_create(iparent.get(), NULL));
}

ButtonMatrix ButtonMatrix::create(const Object &iparent,
                                  const ButtonMatrix &icopy) {
  return ButtonMatrix(lv_btnm_create(iparent.get(), icopy.get()));
}

ButtonMatrix &ButtonMatrix::setMap(const char **map) {
  lv_btnm_set_map(object, map);
  return *this;
}

ButtonMatrix &ButtonMatrix::setAction(lv_btnm_action_t action) {
  lv_btnm_set_action(object, action);
  return *this;
}

ButtonMatrix &ButtonMatrix::setToggle(bool enabled, uint16_t id) {
  lv_btnm_set_toggle(object, enabled, id);
  return *this;
}

ButtonMatrix &ButtonMatrix::setStyle(lv_btnm_style_t type, lv_style_t *style) {
  lv_btnm_set_style(object, type, style);
  return *this;
}

ButtonMatrix &ButtonMatrix::setRecolor(bool enabled) {
  lv_btnm_set_recolor(object, enabled);
  return *this;
}
