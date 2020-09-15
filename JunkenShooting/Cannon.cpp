#include "Cannon.h"

Cannon::Cannon() :
	position(Scene::Width() / 2, Scene::Height() - (CannonSize / 4))
{

}

void Cannon::update() {
	if (KeyS.down()) {
		shots.addShot(Vec2(position.x, 600), Rock);
	}
	if (KeyZ.down()) {
		shots.addShot(Vec2(position.x, 600), Scissors);
	}
	if (KeyX.down()) {
		shots.addShot(Vec2(position.x, 600), Paper);
	}
	if (KeyLeft.pressed()) {
		if (position.x >= 0) {
			position.x -= Speed * Scene::DeltaTime();
		}
	}
	if (KeyRight.pressed()) {
		if (position.x <= Scene::Width()) {
			position.x += Speed * Scene::DeltaTime();
		}
	}
	shots.update();
}
void Cannon::draw() const {
	shots.draw();
	TextureAsset(U"CannonTexture").resized(CannonSize).drawAt(position);
}
