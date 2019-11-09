#pragma once
#include "object.hpp"

class LED : public Object {
public:
  /**
   * Wraps an LVGL led pointer
   */
  using Object::Object;

  /**
   * Returns the name of the object, used for logging
   */
  static constexpr const char* getName() {
    return "LED";
  }

  /**
   * Create LED with no parameters. Defaults parent to lv_scr_act() and copy to NULL
   */
  static LED create();

  /** 
   * Create LED with parent parameter and defaults copy to NULL
   *
   * @param iparent the parent object for this object to be a child of
   */
  static LED create(const Object& iparent);

  /** 
   * Create LED with both parent and copy parameters
   *
   * @param iparent the parent LED for this LED to be a child of
   * @param icopy   the LED for this LED to copy
   */
  static LED create(const Object& iparent, const LED& icopy);
};
