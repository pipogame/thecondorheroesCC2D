#ifndef __ITEM_H__
#define __ITEM_H__

#include "B2Sprite.h"

enum Item_type {
	HEALTH,
	MAGNET
};

class Item : public B2Sprite
{

public:
	Item();
	~Item();

	static Item* create(string frameName, Item_type type, Point pos);
	void initCirclePhysic(b2World *world, Point pos);

protected:
	CC_SYNTHESIZE(Item_type, typeItem, TypeItem);
	CC_SYNTHESIZE(Sprite*, bubble, Bubble);
	CC_SYNTHESIZE(float, alpha, Alpha);
	CC_SYNTHESIZE(bool, taken, Taken);

	void picked();
	void updateMe(BaseHero *hero);
	
};


#define DURATION_MAGNET 1000

#endif // __B2_SPRITE_H__