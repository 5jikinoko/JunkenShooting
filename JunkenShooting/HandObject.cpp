#include "HandObject.h"

HandObject::HandObject(Vec2 position_, int hand_) :
	position(position_),
	state(hand_)
{

};

void HandObject::drawHand(const int Size, const Color& BackGroundColor) const {

	if (Rock <= state && state < Hit) {
		Circle(position, Size).draw(BackGroundColor);
	}
	switch (state) {
	case Rock: TextureAsset(U"RockTexture").resized(Size).drawAt(position); break;
	case Paper: TextureAsset(U"PaperTexture").resized(Size).drawAt(position); break;
	case Scissors: TextureAsset(U"ScissorsTexture").resized(Size).drawAt(position); break;
	default: TextureAsset(U"ExplosionTexture").resized(Size * 4).drawAt(position); break;
	}
}

Result JunkenResult(const Hand me, const Hand you) {
	int x = ((int)(me - you + 3)) % 3;
	return static_cast<Result>(x);
}
