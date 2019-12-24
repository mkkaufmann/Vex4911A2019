#include "ui/wrapper/line.hpp"

Line Line::create(){
		return Line(lv_line_create(lv_scr_act(), NULL));
}

Line Line::create(const Object& iparent){
		return Line(lv_line_create(iparent.get(), NULL));
}

Line Line::create(const Object& iparent, const Line& icopy){
		return Line(lv_line_create(iparent.get(), icopy.get()));
}

Line& Line::setPoints(const lv_point_t* points, uint16_t numPoints) & {
		lv_line_set_points(object, points, numPoints);
		return *this;
};

Line&& Line::setPoints(const lv_point_t* points, uint16_t numPoints) && {
		return std::move(setPoints(points, numPoints));
};
