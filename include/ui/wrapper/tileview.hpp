#pragma once
#include "object.hpp"

class TileView : public Object {
public:
  /**
   * Wraps an LVGL tileveiw pointer
   */
  using Object::Object;

  /**
   * Returns the name of the object, used for logging
   */
  static constexpr const char *getName() { return "TileView"; }

  /**
   * Create TileView with no parameters. Defaults parent to lv_scr_act() and
   * copy to NULL
   */
  static TileView create();

  /**
   * Create TileView with parent parameter and defaults copy to NULL
   *
   * @param iparent the parent TileView for this TileView to be a child of
   */
  static TileView create(const Object &iparent);

  /**
   * Create TileView with both parent and copy parameters
   *
   * @param iparent the parent TileView for this TileView to be a child of
   * @param icopy   the TileView for this TileView to copy
   */
  static TileView create(const Object &iparent, const TileView &icopy);
};
