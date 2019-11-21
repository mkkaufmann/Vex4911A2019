#include "ui/autonselector.hpp"

namespace lib7842 {

void AutonSelector::initialize() {
  initializeAuton();
  initializeColor();
}

void AutonSelector::render() {
    if (!hasWarnedRender) {
      hasWarnedRender = true;
      LOG_WARN_S("AutonSelector::render: auton not attached");
    }
}

std::string AutonSelector::getName() {
  return "AutonSelector";
}

AutonSelector& AutonSelector::attachAuton(const std::shared_ptr<Auton>& iauton) {
  auton = iauton;
  return *this;
}

void AutonSelector::initializeAuton() {

  /**
  * Field
  */
  lv_obj_t* field = lv_obj_create(container, NULL);
  fieldDim = std::min(lv_obj_get_width(container), lv_obj_get_height(container));
  lv_obj_set_size(field, fieldDim, fieldDim);
  lv_obj_align(field, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);

  /**
   * Field Style
   */
  lv_style_copy(&fStyle, &cStyle);
  fStyle.body.main_color = LV_COLOR_WHITE;
  fStyle.body.grad_color = LV_COLOR_WHITE;
  lv_obj_set_style(field, &fStyle);

  /**
   * Tile Styles
   */
  lv_style_copy(&gry, &lv_style_plain);
  gry.body.main_color = LV_COLOR_HEX(0x828F8F);
  gry.body.grad_color = LV_COLOR_HEX(0x828F8F);
  gry.body.border.width = 1;
  gry.body.radius = 0;
  gry.body.border.color = LV_COLOR_WHITE;

  lv_style_copy(&red, &gry);
  red.body.main_color = LV_COLOR_HEX(0xD42630);
  red.body.grad_color = LV_COLOR_HEX(0xD42630);
  lv_style_copy(&blu, &gry);
  blu.body.main_color = LV_COLOR_HEX(0x0077C9);
  blu.body.grad_color = LV_COLOR_HEX(0x0077C9);

  /**
   * Tile Layout
   */
  lv_style_t* tileData[6][6] = {{&gry, &red, &gry, &gry, &blu, &gry}, //
                                {&red, &gry, &gry, &gry, &gry, &blu}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}};

  double tileDim = fieldDim / 6; // tile dimention

  /**
   * Create tile matrix, register callbacks, assign each tile an ID
   */
  for (size_t y = 0; y < 6; y++) {
    for (size_t x = 0; x < 6; x++) {
      lv_obj_t* tileObj = lv_btn_create(field, NULL);
      lv_obj_set_pos(tileObj, x * tileDim, y * tileDim);
      lv_obj_set_size(tileObj, tileDim, tileDim);
      lv_obj_set_free_num(tileObj, y * 6 + x);
      lv_obj_set_free_ptr(tileObj, this);
      lv_btn_set_toggle(tileObj, false);
      lv_btn_set_style(tileObj, LV_BTN_STYLE_PR, tileData[y][x]);
      lv_btn_set_style(tileObj, LV_BTN_STYLE_REL, tileData[y][x]);
    }
  }

  /**
   * Robot point using lvgl led
   */
  led = lv_led_create(field, NULL);
  lv_led_on(led);
  lv_obj_set_size(led, fieldDim / 15.0, fieldDim / 15.0);

  lv_style_copy(&ledStyle, &lv_style_plain);
  ledStyle.body.radius = LV_RADIUS_CIRCLE;
  ledStyle.body.main_color = themeColor;
  ledStyle.body.grad_color = themeColor;
  ledStyle.body.border.color = LV_COLOR_WHITE;
  ledStyle.body.border.width = 2;
  ledStyle.body.border.opa = LV_OPA_100;
  lv_obj_set_style(led, &ledStyle);

  /**
   * Robot line
   */
  line = lv_line_create(field, NULL);
  lv_line_set_points(line, linePoints.data(), linePoints.size());
  lv_obj_set_pos(line, 0, 0);

  lineLength = fieldDim / 6;

  lv_style_copy(&lineStyle, &lv_style_plain);
  lineStyle.line.width = 3;
  lineStyle.line.opa = LV_OPA_100;
  lineStyle.line.color = themeColor;
  lv_obj_set_style(line, &lineStyle);
}

void AutonSelector::initializeColor() {
  statusLabel = lv_label_create(container, NULL);

  lv_style_copy(&textStyle, &lv_style_plain);
  textStyle.text.color = LV_COLOR_WHITE;
  textStyle.text.opa = LV_OPA_100;
  lv_obj_set_style(statusLabel, &textStyle);

  lv_label_set_text(statusLabel, "No Odom Provided");

  lv_obj_align(
    statusLabel, container, LV_ALIGN_CENTER,
    -lv_obj_get_width(container) / 2.0 + (lv_obj_get_width(container) - fieldDim) / 2.0, 0);
}

/**
 * Sets odom state when tile is pressed
 * Decodes tile ID to find position
 */
lv_res_t AutonSelector::autonAction(lv_obj_t* btnm, const char * txt) {
  return LV_RES_OK;
}

/**
 * Reset Sensors and Position
 */
lv_res_t AutonSelector::colorAction(lv_obj_t* btnm, const char * txt) {
  return LV_RES_OK;
}

} // namespace lib7842
