#pragma once

#include "okapi/api/units/QArea.hpp"
#include "okapi/api/units/QLength.hpp"

namespace lib7842 {

using namespace okapi;

class Vector {
public:
  QLength x {0_in};
  QLength y {0_in};

  Vector() = default;
  Vector(const Vector& ipoint) = default;
  virtual ~Vector() = default;

  Vector(const QLength& ix, const QLength& iy);

  Vector operator+(const Vector& rhs) const;
  Vector operator-(const Vector& rhs) const;
  bool operator==(const Vector& rhs) const;
  bool operator!=(const Vector& rhs) const;

  QLength& at(const size_t& iindex);
  QLength& operator[](const size_t& iindex);

  const QLength& read(const size_t& iindex) const;

  Vector operator*(const double scalar) const;
  Vector operator/(const double scalar) const;

  static Vector normalize(const Vector& lhs);
  static Vector scalarMult(const Vector& lhs, const double scalar);
  static QArea dot(const Vector& lhs, const Vector& rhs);
  static QLength mag(const Vector& lhs);
  static QLength dist(const Vector& lhs, const Vector& rhs);
};

} // namespace lib7842