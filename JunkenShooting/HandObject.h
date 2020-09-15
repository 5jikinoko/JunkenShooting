#pragma once
#include<Siv3D.hpp>
enum Hand
{
	Rock = 0, Paper, Scissors, Hit,
};

enum Result
{
	Draw = 0, Win = 1, Lose = 2,
};

Result JunkenResult(Hand, Hand);


class HandObject
{

public:
	Vec2 position;
	int state;
	HandObject(Vec2, int);
	void drawHand(const int, const Color&) const;
};