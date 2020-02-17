#pragma once
class Auton {
public:
  enum AutonSequence {
    SMALL_ZONE_ONE_CUBE,
    BIG_ZONE_ONE_CUBE,
    SMALL_ZONE_5STACK,
    BIG_ZONE_3STACK,
    BIG_ZONE_PUSH,
    BIG_ZONE_KNOCKER,
    TEST
  };

  enum Color { RED, BLUE };
  AutonSequence autonSequence;
  Color color;

private:
};
