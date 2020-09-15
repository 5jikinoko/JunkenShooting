#pragma once
#include<Siv3D.hpp>
#include"Shots.h"
class Cannon
{
public:
	Shots shots;
	Cannon();
	void update();
	void draw() const;
	const int CannonSize = 100;
private:
	Vec2 position;
	const double Speed = 450.0;
};
