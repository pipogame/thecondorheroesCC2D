﻿#include "CollisionListener.h"
#include "BaseHero.h"
#include "BaseEnemy.h"
#include "Coin.h"

CollisionListener::CollisionListener() {

}

CollisionListener::~CollisionListener()
{

}

void CollisionListener::BeginContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	//// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];
	//worldManifold.

	if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR)
		) {

		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();
		auto hero = sA ? (BaseHero *)sA : (BaseHero *)sB;

		// neu sa la floor
		if (!sA) {
			auto dentaX = fabs(collidePoint.x - bodyB->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (bodyB->GetPosition().y < collidePoint.y || dentaX > radius / 2) {

			}
			else {
				hero->getCurrentState()->run(hero);
				hero->setNumberOfJump(2);
			}
		}
		else {
			auto dentaX = fabs(collidePoint.x - bodyA->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (bodyA->GetPosition().y < collidePoint.y || dentaX > radius / 2) {

			}
			else {
				hero->getCurrentState()->run(hero);
				hero->setNumberOfJump(2);
			}
		}

	}

	/*if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_WOODER) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_WOODER)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto wooder = sA->getTag() == TAG_ENEMY_WOODER ? (BaseEnemy *)sA : (BaseEnemy *)sB;
		wooder->die();
		
	}*/

	if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_COIN) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_COIN)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto coin = sA->getTag() == TAG_COIN ? (Coin *)sA : (Coin *)sB;
		coin->picked();

	}

	if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_WOODER && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_SWORD) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_WOODER && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();
		auto enemy = sA ? (BaseEnemy *)sA : (BaseEnemy *)sB;
		
		enemy->die();
	}
}


void CollisionListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];

	

	if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR)
		) {

		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();
		
		auto hero = sA ? (BaseHero *)sA : (BaseHero *)sB;

		if (!sA) {
			auto dentaX = fabs(collidePoint.x - bodyB->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (fabs(bodyB->GetPosition().y - radius * 0.99f) < collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
			}
		}
		else {
			auto dentaX = fabs(collidePoint.x - bodyA->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (fabs(bodyA->GetPosition().y - radius * 0.99f)< collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
			}
		}

	}
}

