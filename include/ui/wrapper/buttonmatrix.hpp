#pragma once
#include "object.hpp"

class ButtonMatrix : public Object {
public:
  /**
   * Wraps an LVGL btnm pointer
   */
  using Object::Object;

  /**
   * Returns the name of the object, used for logging
   */
  static constexpr const char* getName() {
    return "ButtonMatrix";
  }

  /**
   * Create ButtonMatrix with no parameters. Defaults parent to lv_scr_act() and copy to NULL
   */
  static ButtonMatrix create();

  /** 
   * Create ButtonMatrix with parent parameter and defaults copy to NULL
   *
   * @param iparent the parent object for this object to be a child of
   */
  static ButtonMatrix create(const Object& iparent);

  /** 
   * Create ButtonMatrix with both parent and copy parameters
   *
   * @param iparent the parent ButtonMatrix for this ButtonMatrix to be a child of
   * @param icopy   the ButtonMatrix for this ButtonMatrix to copy
   */
  static ButtonMatrix create(const Object& iparent, const ButtonMatrix& icopy);
};
