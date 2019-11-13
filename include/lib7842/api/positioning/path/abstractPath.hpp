#pragma once

namespace lib7842 {

// Forward Declare
class SimplePath;

/**
 * Interface Path Class
 */
class AbstractPath {

public:
  /**
   * Default Constructors
   */
  AbstractPath() = default;
  AbstractPath(const AbstractPath& ipath) = default;
  virtual ~AbstractPath() = default;

  /**
   * Interpolate the path
   *
   * @param isteps how many points to interpolate per segment, from start (inclusive) to end (exclusive) of segment
   * @return generated path
   */
  virtual SimplePath generate(const int isteps = 1) const = 0;
};

} // namespace lib7842