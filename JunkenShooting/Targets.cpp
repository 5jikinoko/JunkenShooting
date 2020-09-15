#include "Targets.h"

//的を落下させる
//的を生成したらtrue,生成してないならfalse
bool Targets::update() {
	const double moving_distance = Speed * Scene::DeltaTime();
	timer += Scene::DeltaTime();
	for (auto& target : Targets_) {
		target.position.y += moving_distance;
	}
	if (timer >= frequency) {
		timer = 0.0;
		return addTarget();
	}
	return false;
};

//地面に届いていた的があるなら消してtrueを返す
bool Targets::LandingCheack() {
	auto itr = Targets_.begin();
	while (itr != Targets_.end()) {
		if ((*itr).position.y + TargetSize / 2 >= Scene::Height()) {
			Targets_.erase(itr);
			return true;
		}
		else {
			++itr;
		}
	}
	return false;
};


void Targets::draw() const {
	for (auto target : Targets_) {
		target.drawHand(TargetSize, Palette::Red);
	}
}

bool Targets::isempty() const {
	return Targets_.empty();
}

//(ランダム, 0)の座標に新しく的を生成
//生成出来たらtrue、そうでないならflase
bool Targets::addTarget() {
	if (isstop == true) return false;
	Vec2 newP(Random<int>(Scene::Width() - TargetSize) + TargetSize / 2, 0);
	Targets_.push_back(HandObject(newP, Random<int>(static_cast<int>(Scissors))));
	return true;
}

void Targets::stop_add() {
	isstop = true;
}

void Targets::restart_add() {
	isstop = false;
}