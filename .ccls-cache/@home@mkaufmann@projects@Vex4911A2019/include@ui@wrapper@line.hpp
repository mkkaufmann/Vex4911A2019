#pragma once
#include "object.hpp"

class Line : public Object {
public:
  /**
   * Wraps an LVGL line pointer
   */
  using Object::Object;

  /**
   * Returns the name of the object, used for logging
   */
  static constexpr const char* getName() {
    return "Line";
  }

  /**
   * Create Line with no parameters. Defaults parent to lv_scr_act() and copy to NULL
   */
  static Line create();

  /** 
   * Create Line with parent parameter and defaults copy to NULL
   *
   * @param iparent the parent object for this object to be a child of
   */
  static Line create(const Object& iparent);

  /** 
   * Create Line with both parent and copy parameters
   *
   * @param iparent the parent Line for this Line to be a child of
   * @param icopy   the Line for this Line to copy
   */
  static Line create(const Object& iparent, const Line& icopy);

  /**
   * Set an array of points for the line to connect
   * @param points an array of points (cannot be a local variable that is destroyed)
   * @param numPoints number of points in points
   */
  Line& setPoints(const lv_point_t* points, uint16_t numPoints) &;
  Line&& setPoints(const lv_point_t* points, uint16_t numPoints) &&;

};
