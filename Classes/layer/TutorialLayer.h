#ifndef __TUTORIAL_LAYER_H__
#define __TUTORIAL_LAYER_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


class TutorialLayer : public Layer
{
	
protected:
	int counter = 0;
	EventListenerTouchOneByOne* listener;
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

public:
	virtual bool init();
	static TutorialLayer* create();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};


class TutorialJump : public TutorialLayer
{

public:
	virtual bool init(int type);
	static TutorialJump* create(int type);
	bool onTouchBegan(Touch *touch, Event *unused_event);
};

class TutorialAttack : public TutorialLayer
{

public:
	virtual bool init();
	static TutorialAttack* create();
};

class TutorialSkill : public TutorialAttack
{

public:
	virtual bool init();
	static TutorialSkill* create();
};

class TutorialIntroBird : public TutorialLayer
{

public:
	virtual bool init();
	static TutorialIntroBird* create();
	bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif // __TUT_H__
