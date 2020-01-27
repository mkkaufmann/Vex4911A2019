#ifndef ACTIONS_HPP
#define ACTIONS_HPP
#include <functional>
// collection of autonomous actions
class Actions {
public:
  static std::function<void()> nothingAction();
  static std::function<void()> parallelAction(std::function<void()> a,
                                              std::function<void()> b);

private:
};
#endif
