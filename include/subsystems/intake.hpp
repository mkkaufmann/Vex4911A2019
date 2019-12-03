#pragma once
#include "subsystem.hpp"
class Intake : public Subsystem{
public:
	void in();
	void out();
	static Intake& getInstance();
};

