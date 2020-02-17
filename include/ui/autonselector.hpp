#pragma once

#include "lib7842/api/gui/page.hpp"
#include "lib7842/api/other/utility.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "ui/wrapper/buttonmatrix.hpp"
#include "ui/wrapper/object.hpp"
#include "util/auton.hpp"
#include <functional>
#include <string>

namespace lib7842 {

class AutonSelector : public GUI::Page {

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
  AutonSelector &attachAuton(const std::shared_ptr<Auton> &iodom);

  enum Auton {
    one,
    five,
    six,
    seven,
    nine,
    THREE,
    FOUR,
    TEST
  };

  enum Color { RED, BLUE };

  static Auton getAuton();
  static Color getColor();

protected:
private:
  void initializeAuton();
  void initializeColor();

  static Color color;
  static Auton auton;

  ButtonMatrix colorSelector;
  lv_style_t colorReleased;
  lv_style_t colorPressed;
  ButtonMatrix autonSelector;
  lv_style_t autonBackground;
  lv_style_t autonReleased;

  static lv_res_t autonAction(lv_obj_t *,
                              const char *); // action when auton is pressed
  static lv_res_t
  colorAction(lv_obj_t *, const char *); // action when color button is pressed
  static const char *colormap[3];
  static const char *autonmap[10];
};

} // namespace lib7842
