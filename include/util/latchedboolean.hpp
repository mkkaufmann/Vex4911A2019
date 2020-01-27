#ifndef LATCHEDBOOLEAN_HPP
#define LATCHEDBOOLEAN_HPP
// returns true when provided inputs change from false to true
class LatchedBoolean {
public:
  LatchedBoolean();

  bool update(bool newValue);

private:
  bool mLast;
};
#endif
