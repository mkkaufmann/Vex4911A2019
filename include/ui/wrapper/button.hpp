#pragma once
#include "object.hpp"

class Button : public Object {
public:
  /**
   * Wraps an LVGL button pointer
   */
  using Object::Object;

  /**
   * Returns the name of the object, used for logging
   */
  static constexpr const char *getName() { return "Button"; }

  /**
   * Create object with no parameters. Defaults parent to lv_scr_act() and copy
   * to NULL
   */
  static Button create();

  /**
   * Create object with parent parameter and defaults copy to NULL
   *
   * @param iparent the parent object for this object to be a child of
   */
  static Button create(const Object &iparent);

  /**
   * Create object with both parent and copy parameters
   *
   * @param iparent the parent object for this object to be a child of
   * @param icopy   the object for this object to copy
   */
  static Button create(const Object &iparent, const Button &icopy);

  /**
   * Enable the toggled states
   *
   * @param enabled if true, enable toggled states. If false, disable
   */
  Button &setToggle(bool enabled);

  /**
   * Set the state of the button
   *
   * @param state new state of the button
   */
  Button &setState(lv_btn_state_t state);

  /**
   * Toggle the state of the button
   */
  Button &toggle();

  /**
   * Set a function to call when a button event happens
   * @param type the type of event
   * @param action the action to perform
   */
  Button &setAction(lv_btn_action_t type, lv_action_t action);

  /**
   * Set the layout of a button
   *
   * @param layout the new layout of the button
   */
  Button &setLayout(lv_layout_t layout);

  /**
   * Enable the horizontal or vertical fit of a button
   * @param horizontal_enable enable horizontal fit if true
   * @param vertical_enable enable vertical fit if true
   */
  Button &setFit(bool horizontal_enable, bool vertical_enable);

  /**
   * Set time of the ink in effect
   * @param time the time of the ink animation
   */
  Button &setInkInTime(uint16_t time);

  /**
   * Set the wait time before the ink disappears
   * @param time the time of the ink animation
   */
  Button &setInkWaitTime(uint16_t time);

  /**
   * Set time of the ink out effect
   * @param time the time of the ink animation
   */
  Button &setInkOutTime(uint16_t time);

  /**
   * Set the style of a button
   *
   * @param type which style should be set
   * @param style style to be set
   */
  Button &setStyle(lv_btn_style_t type, lv_style_t *style);

  // GETTERS
};
