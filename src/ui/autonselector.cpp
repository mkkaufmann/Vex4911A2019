#include "ui/autonselector.hpp"

namespace lib7842 {

AutonSelector::Color AutonSelector::color = AutonSelector::Color::RED;
AutonSelector::Auton AutonSelector::auton = AutonSelector::Auton::TEST;
const char *AutonSelector::colormap[3] = {"Red", "Blue", ""};
const char *AutonSelector::autonmap[10] = {
    "TEST", "one", "five", "six", "seven", "nine", "\n", "THREE", "FOUR", ""};
void AutonSelector::initialize() {
  initializeAuton();
  initializeColor();
}

void AutonSelector::render() {}

AutonSelector::Auton AutonSelector::getAuton() { return auton; }

AutonSelector::Color AutonSelector::getColor() { return color; }

std::string AutonSelector::getName() { return "AutonSelector"; }

void AutonSelector::initializeAuton() {
  autonSelector = ButtonMatrix::create(container);
  lv_style_copy(&autonReleased, &lv_style_pretty);
  autonReleased.body.main_color = LV_COLOR_WHITE;
  autonReleased.body.grad_color = LV_COLOR_WHITE;
  lv_style_copy(&autonBackground, &autonReleased);
  autonReleased.body.main_color = LV_COLOR_MAKE(168, 168, 168);
  autonSelector.setMap(autonmap)
      .setAction(autonAction)
      .setToggle(true, 2)
      .setStyle(LV_BTNM_STYLE_BG, &autonBackground)
      .setStyle(LV_BTNM_STYLE_BTN_PR, &autonReleased)
      .setStyle(LV_BTNM_STYLE_BTN_TGL_PR, &colorPressed)
      .setStyle(LV_BTNM_STYLE_BTN_TGL_REL, &colorPressed)
      .setStyle(LV_BTNM_STYLE_BTN_REL, &autonReleased)
      .setStyle(LV_BTNM_STYLE_BTN_INA, &autonReleased)
      .setPosition(0, LV_VER_RES / 3)
      .setSize(LV_HOR_RES, LV_VER_RES * 2 / 3);
}

void AutonSelector::initializeColor() {
  colorSelector = ButtonMatrix::create(container);
  lv_style_copy(&colorReleased, &autonReleased);
  lv_style_copy(&colorPressed, &colorReleased);
  colorPressed.body.main_color = LV_COLOR_MAKE(0, 131, 179);
  colorPressed.body.grad_color = LV_COLOR_MAKE(0, 131, 179);

  colorSelector.setMap(colormap)
      .setAction(colorAction)
      .setToggle(true, 0)
      .setStyle(LV_BTNM_STYLE_BG, &autonBackground)
      .setStyle(LV_BTNM_STYLE_BTN_PR, &colorReleased)
      .setStyle(LV_BTNM_STYLE_BTN_TGL_PR, &colorPressed)
      .setStyle(LV_BTNM_STYLE_BTN_TGL_REL, &colorPressed)
      .setStyle(LV_BTNM_STYLE_BTN_REL, &colorReleased)
      .setStyle(LV_BTNM_STYLE_BTN_INA, &colorReleased)
      .setSize(LV_HOR_RES, LV_VER_RES / 3);
}

/*
 * color selector
 */
lv_res_t AutonSelector::colorAction(lv_obj_t *btnm, const char *txt) {
  lv_style_t *style = lv_btnm_get_style(btnm, LV_BTNM_STYLE_BTN_TGL_REL);
  style->body.main_color = LV_COLOR_RED;
  if (strcmp(txt, "Red") == 0) {
    color = AutonSelector::Color::RED;
    style->body.main_color = LV_COLOR_MAKE(232, 89, 95);
    style->body.grad_color = LV_COLOR_MAKE(232, 89, 95);
  } else if (strcmp(txt, "Blue") == 0) {
    color = AutonSelector::Color::BLUE;
    style->body.main_color = LV_COLOR_MAKE(0, 131, 179);
    style->body.grad_color = LV_COLOR_MAKE(0, 131, 179);
  }
  return LV_RES_OK;
}

/**
 * auton selector
 */
lv_res_t AutonSelector::autonAction(lv_obj_t *btnm, const char *txt) {
  if (strcmp(txt, "one") == 0) {
    auton = AutonSelector::Auton::one;
  } else if (strcmp(txt, "THREE") == 0) {
    auton = AutonSelector::Auton::THREE;
  } else if (strcmp(txt, "FOUR") == 0) {
    auton = AutonSelector::Auton::FOUR;
  } else if (strcmp(txt, "five") == 0) {
    auton = AutonSelector::Auton::five;
  } else if (strcmp(txt, "six") == 0) {
    auton = AutonSelector::Auton::six;
  } else if (strcmp(txt, "seven") == 0) {
    auton = AutonSelector::Auton::seven;
  } else if (strcmp(txt, "nine") == 0) {
    auton = AutonSelector::Auton::nine;
  } else if (strcmp(txt, "TEST") == 0) {
    auton = AutonSelector::Auton::TEST;
  }

  return LV_RES_OK;
}

} // namespace lib7842
