#pragma once

#include "lib7842/api/other/taskWrapper.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "okapi/api/util/logging.hpp"

namespace lib7842 {
class CustomOdometry : public Odometry, public TaskWrapper {
public:
  /**
   * CustomOdometry.
   *
   * @param imodel         The chassis model for reading sensors.
   * @param ichassisScales The chassis dimensions.
   * @param ilogger        The logger
   */
  CustomOdometry(
    const std::shared_ptr<ChassisModel>& imodel,
    const ChassisScales& ichassisScales,
    const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  virtual ~CustomOdometry() = default;

  /**
   * Returns the current state.
   */
  virtual const State& getState() const;

  /**
   * Sets a new state to be the current state.
   */
  virtual void setState(const State& istate);

  /**
   * Resets state to {0, 0, 0}
   */
  virtual void resetState();

  /**
   * Resets sensors and state
   */
  virtual void reset();

  /**
   * Sets the drive and turn scales.
   */
  void setScales(const ChassisScales& ichassisScales) override;

  /**
   * Do one odometry step.
   */
  void step() override;

  /**
   * @return The internal ChassisModel.
   */
  std::shared_ptr<ReadOnlyChassisModel> getModel() override;

  /**
   * @return The internal ChassisScales.
   */
  virtual const ChassisScales& getScales() const;

  /**
   * Odometry calculation loop
   */
  void loop() override;

private:
  OdomState getState(const StateMode& imode) const override;
  void setState(const OdomState& istate, const StateMode& imode) override;

protected:
  std::shared_ptr<ChassisModel> model {nullptr};

  ChassisScales chassisScales;
  double chassisWidth;
  double middleDistance;

  std::shared_ptr<Logger> logger {nullptr};

  State state;
  std::valarray<std::int32_t> lastTicks {0, 0, 0};
};
} // namespace lib7842
