#include "Shots.h"

//的に当たった弾を爆発状態に、的を消す
//的に当たった数を返す


//弾を移動させる
//画面外の玉は消す
//爆発して0.5経過した弾(爆破エフェクト)も消す
void Shots::update() {
	auto itr = bullets.begin();
	const double moving_distance = Speed * Scene::DeltaTime();
	const int timer = static_cast<int>((Scene::DeltaTime() * 1000));
	while (itr != bullets.end()) {
		if ((*itr).state <= Scissors) {
			(*itr).position.y -= moving_distance;
			if ((*itr).position.y <= -(ShotSize / 2)) {
				itr = bullets.erase(itr);
			}
			else {
				++itr;
			}
		}
		else {
			(*itr).state += timer;
			if ((*itr).state >= 500) {
				itr = bullets.erase(itr);
			}
			else {
				++itr;
			}
		}
	}
};

//
void Shots::draw() const {
	for (auto bullet : bullets) {
		bullet.drawHand(ShotSize, Palette::Yellow);
	}
}

//指定した場所を中心とする弾を生成
void Shots::addShot(Vec2 position, Hand h) {
	bullets.push_back(HandObject(position, h));
}
