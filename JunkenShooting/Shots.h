#pragma once
#include<Siv3D.hpp>
#include"Targets.h"
#include"HandObject.h"

class Shots
{
public:
	void update();
	void draw() const;
	void addShot(Vec2, Hand);
	std::vector<HandObject> bullets;
	const int ShotSize = 30;
private:
	const double Speed = 450.0;
};
