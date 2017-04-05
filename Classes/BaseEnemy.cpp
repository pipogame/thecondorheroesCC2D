#include "BaseEnemy.h"
#include "layer/GameScene.h"

BaseEnemy::BaseEnemy() :B2Skeleton()
{
	isDie = false;
}

BaseEnemy::~BaseEnemy()
{
}

BaseEnemy::BaseEnemy(spSkeletonData * data):B2Skeleton(data)
{
	isDie = false;
}

BaseEnemy::BaseEnemy(string jsonFile, string atlasFile, float scale):B2Skeleton(jsonFile, atlasFile, scale)
{
	isDie = false;
}

BaseEnemy * BaseEnemy::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}

BaseEnemy * BaseEnemy::create(spSkeletonData * data)
{
	auto skeleton = new BaseEnemy(data);
	//skeleton->initWithData(data);
	skeleton->update(1.0f);
	return skeleton;
}

void BaseEnemy::run()
{
}

void BaseEnemy::attack()
{
}

void BaseEnemy::die()
{
}

void BaseEnemy::updateMe(float dt)
{
	if (body != nullptr && body->GetType() == b2_staticBody) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}
}

void BaseEnemy::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
}

