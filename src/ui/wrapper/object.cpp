#include "ui/wrapper/object.hpp"

Object::Object(lv_obj_t* iobject) : object(iobject) {}

Object Object::create() {
  return Object(lv_obj_create(ActiveScreen(), NULL));
}

Object Object::create(const Object& iparent) {
  return Object(lv_obj_create(iparent, NULL));
}

Object Object::create(const Object& iparent, const Object& icopy) {
  return Object(lv_obj_create(iparent, icopy));
}

Object Object::ActiveScreen() {
  return lv_scr_act();
}

Object Object::TopScreen() {
  return lv_layer_top();
}

Object Object::SystemScreen() {
  return lv_layer_sys();
}

void Object::remove() {
  lv_obj_del(object);
}

Object& Object::clean() {
  lv_obj_clean(object);
  return *this;
}

Object& Object::invalidate() {
  lv_obj_invalidate(object);
  return *this;
}

Object& Object::setPosition(lv_coord_t x, lv_coord_t y) {
  lv_obj_set_pos(object, x, y);
  return *this;
}

Object& Object::setX(lv_coord_t x) {
  lv_obj_set_x(object, x);
  return *this;
}

Object& Object::setY(lv_coord_t y) {
  lv_obj_set_x(object, y);
  return *this;
}

Object& Object::setSize(lv_coord_t width, lv_coord_t height) {
  lv_obj_set_size(object, width, height);
  return *this;
}

Object& Object::setWidth(lv_coord_t width) {
  lv_obj_set_width(object, width);
  return *this;
}

Object& Object::setHeight(lv_coord_t height) {
  lv_obj_set_height(object, height);
  return *this;
}

Object& Object::align(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) {
  lv_obj_align(object, base, alignment, xShift, yShift);
  return *this;
}

Object Object::getScreen() const {
  return lv_obj_get_screen(object);
}

Object Object::getParent() const {
  return lv_obj_get_parent(object);
}

lv_area_t Object::getPosition() const {
  lv_area_t pos;
  lv_obj_get_coords(object, &pos);
  return pos;
}

lv_coord_t Object::getX() const {
  return lv_obj_get_x(object);
}

lv_coord_t Object::getY() const {
  return lv_obj_get_y(object);
}

lv_coord_t Object::getWidth() const {
  return lv_obj_get_width(object);
}

lv_coord_t Object::getHeight() const {
  return lv_obj_get_height(object);
}

lv_style_t* Object::getStyle() const {
  return lv_obj_get_style(object);
}

lv_obj_t* Object::get() const {
  return object;
}

Object::operator lv_obj_t*() const {
  return object;
}
