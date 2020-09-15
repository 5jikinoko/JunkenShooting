#pragma once
#include<Siv3D.hpp>
#include"HandObject.h"
#include"Shots.h"

class Targets
{
public:
	bool update();
	bool LandingCheack();
	void draw() const;
	bool isempty() const;
	const int TargetSize = 50;
	std::vector<HandObject> Targets_;
	void stop_add();
	void restart_add();
private:
	bool addTarget();
	const double Speed = 250.0;
	const double frequency = 1.0;
	double timer = 1.0;
	bool isstop = false;
};