#ifndef SUBSYSTEM_HPP
#define SUBSYSTEM_HPP
//abstract subsystem class
class Subsystem{
public:
  virtual void in() = 0;
  virtual void out() = 0;
};
#endif
