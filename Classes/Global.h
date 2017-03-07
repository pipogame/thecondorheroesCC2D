#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <string>
#include <vector>

USING_NS_CC;
using namespace std;


// define
#define PI								3.1415926
#define PTM_RATIO						32


// bitmask
#define BITMASK_HERO					0x0001
#define BITMASK_ENEMY					0x0002
#define BITMASK_FLOOR					0x0004
#define BITMASK_WOODER					0x0008
#define BITMASK_COIN					0x0010
#define BITMASK_ITEM					0x0020
#define BITMASK_SWORD					0x0040
#define BITMASK_BLINK					0x0080
#define BITMASK_BOMB_ENEMY				0x0100

// tag
#define TAG_HERO							50
#define TAG_FLOOR							51
#define TAG_COIN							52
#define TAG_BULLET_ENEMY					53
#define TAG_ITEM							54
#define TAG_BOMB							55
#define TAG_BLINK							56
#define TAG_BOMB_ENEMY						57
#define TAG_GAME							58


#define TAG_ENEMY_WOODER					101


// z-order
#define ZORDER_BG							0
#define ZORDER_ENEMY						1


#define PTM_RATIO							32
#define MAX_DYNAMIC_HUMAN_ENEMY				8
#define MAX_BULLET_HERO_POOL				30
#define MAX_BOMB_HERO_POOL					7
#define MAX_BULLET_SOLDIER_ENEMY_POOL		3

#define MAX_BULLET_AUTOGUN_POOL				2
#define MAX_BULLET_FORT_MINI_POOL			3
#define MAX_BULLET_FORT_POOL				6
#define MAX_BULLET_TANK_POOL				3
#define MAX_BULLET_HELICOPTER_POOL			2

#define	SOUND_BACKGROUND					"sound/theme_music.mp3"
#define SOUND_BULLET_NORMAL					"sound/normal bullet.mp3"
#define SOUND_BULLET_CIRCLE					"sound/F bullet.mp3"
#define SOUND_BULLET_SUPER					"sound/S Bullet.mp3"
#define SOUND_TANK_SHOOT					"sound/tank shoot .mp3"
#define SOUND_TANK_EXPLOSION				"sound/tank_explosion.mp3"
#define SOUND_PLANE_BOMB					"sound/plane_bomb.mp3"
#define SOUND_PLANE_DROP					"sound/plane_drop.mp3"

#define SOUND_TANK_MOVE						"sound/tank move.mp3"
#define SOUND_HELICOPTER					"sound/helicopter.mp3"
#define SOUND_MACHINE_GUN					"sound/machine gun.mp3"
#define SOUND_ENEMY_BULLET					"sound/enemy bullet.mp3"
#define SOUND_ENEMY_BOMB					"sound/enemy_bomb.mp3"

#define SOUND_CANON_SHOOT					"sound/cannon shoot.mp3"
#define SOUND_ENEMY_BOMB_EXPLOSION			"sound/enemy_bomb_explosion.mp3"
#define SOUND_BIG_FORT_SHOOT				"sound/boss_explosion.mp3"
#define SOUND_GET_ITEM						"sound/get item.mp3"
#define SOUND_MISSLE						"sound/missle.mp3"

#define SOUND_LOSE							"sound/sound_lose.mp3"
#define SOUND_WIN							"sound/Win.mp3"

#define SOUND_TRANSFORM						"sound/transform.mp3"
#define SOUND_TRANSFORM2					"sound/transform_2.mp3"


#endif // __GLOBAL_H__
