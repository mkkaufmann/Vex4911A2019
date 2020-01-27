#include "ui/wrapper/tileview.hpp"

TileView TileView::create() {
  return TileView(lv_tileview_create(lv_scr_act(), NULL));
}

TileView TileView::create(const Object &iparent) {
  return TileView(lv_tileview_create(iparent.get(), NULL));
}

TileView TileView::create(const Object &iparent, const TileView &icopy) {
  return TileView(lv_tileview_create(iparent.get(), icopy.get()));
}
