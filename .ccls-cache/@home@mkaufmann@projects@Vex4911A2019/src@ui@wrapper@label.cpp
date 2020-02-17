#include "ui/wrapper/label.hpp"

Label Label::create() { return Label(lv_label_create(lv_scr_act(), NULL)); }

Label Label::create(const Object &iparent) {
  return Label(lv_label_create(iparent.get(), NULL));
}

Label Label::create(const Object &iparent, const Label &icopy) {
  return Label(lv_label_create(iparent.get(), icopy.get()));
}

Label &Label::setText(const char *text) & {
  lv_label_set_text(object, text);
  return *this;
}

Label &&Label::setText(const char *text) && { return std::move(setText(text)); }

Label &Label::setArrayText(const char *array, uint16_t size) & {
  lv_label_set_array_text(object, array, size);
  return *this;
}

Label &&Label::setArrayText(const char *array, uint16_t size) && {
  return std::move(setArrayText(array, size));
}

Label &Label::setStaticText(const char *text) & {
  lv_label_set_static_text(object, text);
  return *this;
}

Label &&Label::setStaticText(const char *text) && {
  return std::move(setStaticText(text));
}
