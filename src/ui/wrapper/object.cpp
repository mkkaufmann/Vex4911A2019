#include "ui/wrapper/object.hpp"

Object::Object(lv_obj_t* iobject) : object(iobject) {}

Object Object::create() {
  return Object(lv_obj_create(lv_scr_act(), NULL));
}

Object Object::create(const Object& iparent) {
  return Object(lv_obj_create(iparent.get(), NULL));
}

Object Object::create(const Object& iparent, const Object& icopy) {
  return Object(lv_obj_create(iparent.get(), icopy.get()));
}

void Object::remove() {
  lv_obj_del(object);
}

Object& Object::clean() & {
  lv_obj_clean(object);
  return *this;
}

Object&& Object::clean() && {
  return std::move(clean());
}

Object& Object::invalidate() & {
  lv_obj_invalidate(object);
  return *this;
}

Object&& Object::invalidate() && {
  return std::move(invalidate());
}

Object& Object::setParent(const Object& iparent) & {
  lv_obj_set_parent(object, iparent.get());
  return *this;
}

Object&& Object::setParent(const Object& iparent) && {
  return std::move(setParent(iparent));
}

Object& Object::setPosition(lv_coord_t x, lv_coord_t y) & {
  lv_obj_set_pos(object, x, y);
  return *this;
}

Object&& Object::setPosition(lv_coord_t x, lv_coord_t y) && {
  return std::move(setPosition(x, y));
}

Object& Object::setX(lv_coord_t x) & {
  lv_obj_set_x(object, x);
  return *this;
}

Object&& Object::setX(lv_coord_t x) && {
  return std::move(setX(x));
}

Object& Object::setY(lv_coord_t y) & {
  lv_obj_set_x(object, y);
  return *this;
}

Object&& Object::setY(lv_coord_t y) && {
  return std::move(setY(y));
}

Object& Object::setSize(lv_coord_t width, lv_coord_t height) & {
  lv_obj_set_size(object, width, height);
  return *this;
}

Object&& Object::setSize(lv_coord_t width, lv_coord_t height) && {
  return std::move(setSize(width, height));
}

Object& Object::setWidth(lv_coord_t width) & {
  lv_obj_set_width(object, width);
  return *this;
}

Object&& Object::setWidth(lv_coord_t width) && {
  return std::move(setWidth(width));
}

Object& Object::setHeight(lv_coord_t height) & {
  lv_obj_set_height(object, height);
  return *this;
}

Object&& Object::setHeight(lv_coord_t height) && {
  return std::move(setHeight(height));
}

Object& Object::align(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) & {
  lv_obj_align(object, base.get(), alignment, xShift, yShift);
  return *this;
}

Object&&
  Object::align(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) && {
  return std::move(align(base, alignment, xShift, yShift));
}

Object& Object::alignToParent(lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) & {
  lv_obj_align(object, lv_obj_get_parent(object), alignment, xShift, yShift);
  return *this;
}

Object&& Object::alignToParent(lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) && {
  return std::move(alignToParent(alignment, xShift, yShift));
}

Object&
  Object::alignOrigo(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) & {
  lv_obj_align_origo(object, base.get(), alignment, xShift, yShift);
  return *this;
}

Object&&
  Object::alignOrigo(const Object& base, lv_align_t alignment, lv_coord_t xShift, lv_coord_t yShift) && {
  return std::move(alignOrigo(base, alignment, xShift, yShift));
}

Object& Object::realign() & {
  lv_obj_realign(object);
  return *this;
}

Object&& Object::realign() && {
  return std::move(realign());
}

Object& Object::setAutoRealign(bool enabled) & {
  lv_obj_set_auto_realign(object, enabled);
  return *this;
};

Object&& Object::setAutoRealign(bool enabled) && {
  return std::move(setAutoRealign(enabled));
};

Object& Object::setStyle(lv_style_t* style) & {
		lv_obj_set_style(object, style);
		return *this;
}

Object&& Object::setStyle(lv_style_t* style) && {
		return std::move(setStyle(style));
}

lv_obj_t* Object::get() const {
  return object;
}
