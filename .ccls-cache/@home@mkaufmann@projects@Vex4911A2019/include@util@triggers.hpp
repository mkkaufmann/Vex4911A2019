#ifndef TRIGGERS_HPP
#define TRIGGERS_HPP
#include <functional>
class Triggers{
public:
  static std::function<bool()> trueTrigger();
  static std::function<bool()> falseTrigger();
  static std::function<bool()> compoundAndTrigger(std::function<bool()> a, std::function<bool()> b);
  static std::function<bool()> compoundOrTrigger(std::function<bool()> a, std::function<bool()> b);
};
#endif
