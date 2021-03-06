#pragma once
#ifndef __CO_LONG_H__
#define __CO_LONG_H__

#include "JSonHeroManager.h"
#include "BaseHero.h"
#include "Rada.h"
#include "DQ_TieuHonChuong.h"
#include "GameScene.h"


class CoLong : public BaseHero {
public:
	CoLong(string p_sJsonFile, string p_sAtlasFile, float p_fScale);
	static CoLong* create(string p_sJsonFile, string p_sAtlasFile, float p_fScale);

	void initCirclePhysic(b2World *world, Point pos);
	void addStuff();
	void createPool();

	void run();
	void normalJump();
	void doubleJump();
	void landing();
	void die();
	void attackNormal();
	void attackLanding();
	void injured();
	void listener();

	void stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3);
	void doDestroyBodies(b2World *world);

	void updateMe(float p_fDelta);
	void updateAttackBossAsTarget(BaseEnemy *p_pBoss);


protected:
	Rada *m_pRadaSkill1;
	Rada *m_pRadaSkill2;
	Rada *m_pRadaSkill3;
	void createRada(b2World *p_pWorld);

	// Skill 1
	CCArray *poolSkill1;
	int indexSkill1 = 0;
	list<TieuHonChuong *> m_lDocPhongCham;
	void createDocPhongCham(Point p_ptStartPoint, Point p_ptEndPoint);
	void doCounterSkill1();

	// Skill 2
	CCArray *poolSkill2;
	int indexSkill2 = 0;
	bool isAddedAll = false;
	void createNgocNuKiemPhap(Point p_ptPoint);
	void doCounterSkill2();

	// Skill 3
	void doCounterSkill3();

	//////////////////////////////////////////////////////////////////////////
	void createSlash();
	void runSlash();
	void runSlashLand();

	int keysoundSKill3;
	//////////////////////////////////////////////////////////////////////////
};
#endif // __CO_LONG_H__

