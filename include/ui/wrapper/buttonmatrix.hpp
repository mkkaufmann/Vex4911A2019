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

  /**
   * Set a new map for buttons to be created according to
   * @param map pointer to a string array. The last string has to be "".
   *		Use "\n" to start a new line
   *		The first byte can be a control data:
   *		 - bit 7: always 1
   *		 - bit 6: always 0
   *		 - bit 5: inactive (disabled)
   *		 - bit 4: no repeat (on long press)
   *		 - bit 3: hidden
   *		 - bit 2..0: button relative width
   *		 Example: "\224abc": "abc" text with 4 width and no long press
   */
  ButtonMatrix& setMap(const char** map);

  /**
   * Set a new callback function for the buttons (it will be called when a button is released)
   * @param action pointer to a callback function
   */
  ButtonMatrix& setAction(lv_btnm_action_t action);

  /**
   * Enable or disable button toggling
   * @param enabled toggles when true, disables toggles when false
   * @param id index of the currently toggled button (ignored if enabled is false)
   */
  ButtonMatrix& setToggle(bool enabled, uint16_t id);

  /**
   * Set a style of a button matrix
   * @param type which style should be set
   * @param style pointer to a style
   */
  ButtonMatrix& setStyle(lv_btnm_style_t type, lv_style_t* style);

  /**
   * Set whether recoloring is enabled
   * @param enabled whether recoloring is enabled
   */
  ButtonMatrix& setRecolor(bool enabled);
};
