#pragma once
#include "abstractPath.hpp"
#include "simplePath.hpp"

namespace lib7842 {

class CompoundPath : public AbstractPath {

public:
  /**
   * Default Constructors
   */
  CompoundPath() = default;
  CompoundPath(const CompoundPath& ipath) = default;
  virtual ~CompoundPath() = default;

  explicit CompoundPath(const std::shared_ptr<AbstractPath>& ipath);

  CompoundPath& addPath(const std::shared_ptr<AbstractPath>& ipath);

  /**
   * Interpolate the path
   *
   * @param isteps how many points to interpolate per segment, from start (inclusive) to end (exclusive) of segment
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override;

protected:
  std::vector<std::shared_ptr<AbstractPath>> paths {};
}; // namespace lib7842

} // namespace lib7842