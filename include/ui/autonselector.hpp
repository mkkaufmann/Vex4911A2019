#pragma once

#include "lib7842/api/other/utility.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "lib7842/api/gui/page.hpp"
#include "util/auton.hpp"
#include "ui/wrapper/object.hpp"
#include "ui/wrapper/buttonmatrix.hpp"
#include <functional>
#include <string>

namespace lib7842 {

class AutonSelector : public Page {

public:
  using Page::Page;

  void initialize() override;
  void render() override;

  static std::string getName();

  /**
   * Attach the odom for reading
   *
   * @param iodom The odometry
   */
  AutonSelector& attachAuton(const std::shared_ptr<Auton>& iodom);


protected:
  std::shared_ptr<Auton> auton {nullptr};

private:
  void initializeAuton();
  void initializeColor();

  // field
  double fieldDim = 0; // width and height of field container
  lv_style_t fStyle;

  // tile styles
  lv_style_t gry;
  lv_style_t red;
  lv_style_t blu;

  // robot point
  lv_obj_t* led {nullptr};
  lv_style_t ledStyle;

  // robot line
  lv_obj_t* line {nullptr};
  std::vector<lv_point_t> linePoints {lv_point_t {0, 0}, lv_point_t {0, 0}}; // line positions
  int lineLength = 0;
  lv_style_t lineStyle;

  // status label
  lv_obj_t* statusLabel {nullptr};
  lv_style_t textStyle;

  // reset button styles
  lv_style_t resetRel;
  lv_style_t resetPr;

  static lv_res_t autonAction(lv_obj_t*, const char *); // action when auton is pressed
  static lv_res_t colorAction(lv_obj_t*, const char *); // action when color button is pressed

  bool hasWarnedRender = false;
};

} // namespace lib7842
