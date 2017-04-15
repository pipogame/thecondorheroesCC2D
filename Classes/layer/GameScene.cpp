#include "layer/GameScene.h"
#include "SimpleAudioEngine.h"
#include "layer/MenuScene.h"
#include "layer/Hud.h"
#include "layer/LoadingLayer.h"
#include "manager/SkeletonManager.h"

Hud *hud;
LoadingLayer* loadingLayer;


Scene* GameScene::createScene(int map, int haveboss, int charId)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object

	loadingLayer = LoadingLayer::create();
	auto layer = GameScene::create(map, haveboss, charId);
	layer->setAnchorPoint(Point(0, 0));
	//layer->coinPool = new MyPool(100, TAG_COIN);
	layer->setName("gameLayer");

	hud = Hud::create();

	// add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(hud);
	scene->addChild(layer->blur);
	scene->addChild(loadingLayer);


	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init(int map, int haveboss, int charId)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	this->coinPool = MyPool::create(150, TAG_COIN);
	this->wooderPool = MyPool::create(20, TAG_ENEMY_WOODER);
	//isModeDebug = false;
	isModeDebug = true;
	indexOfNextMapBoss = -1;
	this->haveboss = haveboss;
	this->map = map;
	this->isWinGame = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	initB2World();
	cachePlist();
	// cache batchnode
	//auto tmp = Sprite::createWithSpriteFrameName("coin_01.png");
	batchNode = SpriteBatchNode::create("coin_01.png");
	this->addChild(batchNode);
	// tesst

	danceWithCamera();
	//cacheSkeleton();
	if (charId == 0) {
		createDuongQua("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas",
			Point(origin.x, visibleSize.height * 0.75f));
	}
	else {
		createCoLong("Animation/CoLong/CoLong.json", "Animation/CoLong/CoLong.atlas", Point(origin.x, visibleSize.height * 0.75f));
	}




	loadBackground(map);
	loadPosAndTag();
	createGroundBody();
	initLayerToAddAgent();
	this->charId = charId;



	createEagle(Point(hero->getB2Body()->GetPosition().x - visibleSize.width, visibleSize.height / 2));

	danceWithEffect();
	if (haveboss)
		creatBoss();


	//createCoint();
	return true;
}

GameScene * GameScene::create(int map, int haveboss, int charId)
{
	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(map, haveboss, charId))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}


void GameScene::createDuongQua(string path_Json, string path_Atlas, Point position)
{
	hero = DuongQua::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(position);

	addChild(hero, ZORDER_HERO);

	hero->initCirclePhysic(world, hero->getPosition());
	hero->addStuff();

	hero->getBloodScreen()->setPosition(follow->getPosition());
	addChild(hero->getBloodScreen(), ZORDER_SMT);
}

void GameScene::createCoLong(string path_Json, string path_Atlas, Point position)
{
	hero = CoLong::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(position);

	addChild(hero, ZORDER_HERO);

	hero->initCirclePhysic(world, hero->getPosition());
	hero->addStuff();

	hero->getBloodScreen()->setPosition(follow->getPosition());
	addChild(hero->getBloodScreen(), ZORDER_SMT);
}


void GameScene::createEagle(Point position)
{
	_aEagle = ChimDieu::create("Animation/ChimDieu/ChimDieu.json",
		"Animation/ChimDieu/ChimDieu.atlas", SCREEN_SIZE.height / 2048);
	_aEagle->initCirclePhysic(world, position);
	this->addChild(_aEagle, ZORDER_HERO);
	_aEagle->setStringHero(charId == 0 ? "DuongQua" : "CoLong");
}

void GameScene::onBegin()
{
	hud->addEvents();
	hud->getBtnCalling()->setEnabled(true);
	hud->getPauseItem()->setEnabled(true);

	touch_listener = EventListenerTouchOneByOne::create();
	key_listener = EventListenerKeyboard::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	key_listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);
	this->scheduleUpdate();
}

void GameScene::checkActiveButton()
{
	if (hero->getIsDriverEagle() || hero->getPositionY() < 0) return;
	if (hud->getBtnSkill_1()->getIsBlocked()) {		// 2 and 3 active
		if (currentButton == 2) {
			if (hero->getIsDoneDuration2()) {
				hero->killThemAll(listEnemyOccurInScreen);
				if (!hud->getBtnSkill_1()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_1()->setVisible(true);
				hud->getBtnSkill_1()->setIsBlocked(false);
			}

		}
		else if (currentButton == 3) {
			if (hero->getIsDoneDuration3()) {
				if (!hud->getBtnSkill_1()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_1()->setVisible(true);
				hud->getBtnSkill_1()->setIsBlocked(false);
			}
		}

	}

	if (hud->getBtnSkill_2()->getIsBlocked()) {		// 1 and 3 active
		if (currentButton == 1) {
			if (hero->getIsDoneDuration1()) {
				if (!hud->getBtnSkill_2()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_2()->setVisible(true);
				hud->getBtnSkill_2()->setIsBlocked(false);
			}
		}
		else if (currentButton == 3) {
			if (hero->getIsDoneDuration3()) {
				hero->killThemAll(listEnemyOccurInScreen);
				if (!hud->getBtnSkill_2()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_2()->setVisible(true);
				hud->getBtnSkill_2()->setIsBlocked(false);
			}
		}

	}

	if (hud->getBtnSkill_3()->getIsBlocked()) {		// 2 and 1 active
		if (currentButton == 2) {
			if (hero->getIsDoneDuration2()) {
				if (!hud->getBtnSkill_3()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_3()->setVisible(true);
				hud->getBtnSkill_3()->setIsBlocked(false);
			}

		}
		else if (currentButton == 1) {
			if (hero->getIsDoneDuration1()) {
				hero->killThemAll(listEnemyOccurInScreen);
				if (!hud->getBtnSkill_3()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_3()->setVisible(true);
				hud->getBtnSkill_3()->setIsBlocked(false);
			}
		}

	}

}

void GameScene::listener()
{
	if (hero->getB2Body() == nullptr)
		return;

	if (hud->getBtnAttack()->getIsActive() && !hud->getBtnAttack()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		hero->getFSM()->changeState(MAttack);


		hud->getBtnAttack()->setIsActive(false);
	}

	if (hud->getBtnSkill_1()->getIsActive() && !hud->getBtnSkill_1()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		currentButton = 1;

		hero->setIsDoneDuration1(false);
		hero->doCounterSkill1();

		hud->getBtnSkill_2()->setIsBlocked(true);
		hud->getBtnSkill_2()->setVisible(false);


		hud->getBtnSkill_3()->setIsBlocked(true);
		hud->getBtnSkill_3()->setVisible(false);


		hud->getBtnSkill_1()->setIsActive(false);
	}

	if (hud->getBtnSkill_2()->getIsActive() && !hud->getBtnSkill_2()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		currentButton = 2;

		hero->setIsDoneDuration2(false);
		hero->doCounterSkill2();

		hud->getBtnSkill_1()->setIsBlocked(true);
		hud->getBtnSkill_1()->setVisible(false);

		hud->getBtnSkill_3()->setIsBlocked(true);
		hud->getBtnSkill_3()->setVisible(false);


		hud->getBtnSkill_2()->setIsActive(false);
	}

	if (hud->getBtnSkill_3()->getIsActive() && !hud->getBtnSkill_3()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		currentButton = 3;

		hero->setIsDoneDuration3(false);
		hero->doCounterSkill3();

		// block
		hud->getBtnSkill_1()->setIsBlocked(true);
		hud->getBtnSkill_1()->setVisible(false);

		hud->getBtnSkill_2()->setIsBlocked(true);
		hud->getBtnSkill_2()->setVisible(false);

		hud->getBtnSkill_3()->setIsActive(false);
	}

}

void GameScene::heroGetOffEagle() {
	hud->showButton();
	hero->changeBodyCategoryBits(BITMASK_HERO);
	hero->setIsDriverEagle(false);
	hero->getB2Body()->SetTransform(b2Vec2(_aEagle->getB2Body()->GetPosition().x, _aEagle->getB2Body()->GetPosition().y - 100.0f / PTM_RATIO), 0.0f);
	hero->setVisible(true);
	hero->getB2Body()->SetGravityScale(1);
	hero->getFSM()->changeState(MLand);
	_aEagle->flyAway();
}

void GameScene::update(float dt)
{
	//log("thisPos : %f, %f", this->getPositionX(), this->getPositionY());

	/*creatEnemyWooderRT();
	creatEnemyToanChanStudentRT();
	creatEnemyToanChanStudent2RT();*/
	updateB2World(dt);
	listener();
	updateLayer();

	if (m_fMultiKillsCounterTime > 0.0f) {
		m_fMultiKillsCounterTime -= dt;
	}
	else {
		m_nMultiKills = 0;
	}

	checkActiveButton();

	_aEagle->updateMe(dt);
	if (hero->getIsDriverEagle()) {
		if (hero->getHealth() <= 0) {	// die
			hero->getB2Body()->SetGravityScale(1);
			hero->getB2Body()->SetLinearVelocity(b2Vec2(0, -hero->getJumpVel()));
			hero->setVisible(true);
			_aEagle->stopAllActions();
			_aEagle->flyAway();
		}
		else {
			hero->getB2Body()->SetTransform(b2Vec2(_aEagle->getB2Body()->GetPosition().x,
				_aEagle->getB2Body()->GetPosition().y - 100.0f / PTM_RATIO), 0.0f);
		}
	}

	hero->updateMe(dt);

	updateEnemy();
	//cleanMap();
	updateCamera();

	// fall down some hold
	if (hud->getBtnCalling()->isVisible() &&
		hero->getPositionY() + hero->getTrueRadiusOfHero() < 0 && hud->getBtnSkill_1()->getCoolDownSprite()->isVisible()) {
		hud->hideButton();
		hud->moveCallBirdToCenterScreen(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
		hero->setOnGround(false);
		hero->setVisible(false);
		hero->getB2Body()->SetGravityScale(0);
		hero->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

	}
	else if (!hud->getBtnCalling()->isVisible() &&
		hero->getPositionY() + hero->getTrueRadiusOfHero() * 3 < 0 && hud->getBtnSkill_1()->getCoolDownSprite()->isVisible()) {
		overGame();
	}

	if (hud->getBtnCallingHintDone()) {
		static float g_fTimeCounter = 0.0f;
		g_fTimeCounter += dt;
		if (g_fTimeCounter > 5.0f / 180) {
			bool _bResult = hud->callBirdCooldown();
			if (!_bResult) {
				hud->setBtnCallingHintDone(false);
				overGame();
			}
			g_fTimeCounter = 0.0f;
		}
	}


	if (haveboss) {
		if (hero->getPositionX() > tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width / 2 && indexOfNextMapBoss < 0) {
			createGroundForMapBoss();
			indexOfNextMapBoss = 1;
		}

		if (hero->getPositionX() > tmx_mapboss[indexOfNextMapBoss]->getPositionX() + SCREEN_SIZE.width && indexOfNextMapBoss >= 0) {
			TMXTiledMap* tmpmap;
			indexOfNextMapBoss == 1 ? tmpmap = tmx_mapboss[0] : tmpmap = tmx_mapboss[1];
			tmpmap->setPositionX(tmx_mapboss[indexOfNextMapBoss]->getPositionX() + tmx_mapboss[indexOfNextMapBoss]->getBoundingBox().size.width);
			indexOfNextMapBoss == 1 ? indexOfNextMapBoss = 0 : indexOfNextMapBoss = 1;
		}
	}


	if (hero->getBloodScreen()->isVisible())
		hero->getBloodScreen()->setPosition(follow->getPosition());

	updateHUD(dt);

}

void GameScene::initB2World()
{
	world = new b2World(b2Vec2(0, -SCREEN_SIZE.height * 10.0f / 3.0f / PTM_RATIO));

	// draw debug
	auto debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	if (isModeDebug)
		world->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;

	debugDraw->SetFlags(flags);

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);

	auto collisionListener = new CollisionListener();
	world->SetContactListener(collisionListener);
}

void GameScene::updateB2World(float dt)
{
	int positionIterations = 3;		// position
	int velocityIterations = 8;		// velocity

	world->Step(dt, velocityIterations, positionIterations);
}

void GameScene::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
	Layer::draw(renderer, transform, flags);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(GameScene::onDraw, this);
	renderer->addCommand(&_customCommand);

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GameScene::onDraw()
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");

	auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
	world->DrawDebugData();
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}


void GameScene::loadBackground(int map)
{
	// khoi tao map

	tmx_map = TMXTiledMap::create(StringUtils::format("Map/map1/map%d.tmx", map));
	tmx_map->setAnchorPoint(Point::ZERO);
	scaleOfMap = SCREEN_SIZE.height / tmx_map->getContentSize().height;
	tmx_map->setScale(scaleOfMap);

	tmx_map->setPosition(Point::ZERO);

	if (isModeDebug)
		tmx_map->setVisible(false);

	this->addChild(tmx_map, ZORDER_BG2);
	if (haveboss) {
		for (int i = 0; i < 2; i++) {
			tmx_mapboss[i] = TMXTiledMap::create("Map/map1/mapboss.tmx");
			tmx_mapboss[i]->setAnchorPoint(Point::ZERO);
			tmx_mapboss[i]->setScale(scaleOfMap);
			if (isModeDebug)
				tmx_mapboss[i]->setVisible(false);
		}

		tmx_mapboss[0]->setPosition(tmx_map->getPosition() + Vec2(tmx_map->getBoundingBox().size.width, 0));
		tmx_mapboss[1]->setPosition(tmx_mapboss[0]->getPosition() + Vec2(tmx_mapboss[0]->getBoundingBox().size.width, 0));
		this->addChild(tmx_mapboss[0], ZORDER_BG2);
		this->addChild(tmx_mapboss[1], ZORDER_BG2);
	}

	createInfiniteNode();
}

void GameScene::createInfiniteNode()
{
	background = InfiniteParallaxNode::create();

	auto bg1_1 = Sprite::createWithSpriteFrameName("bg1.png");
	//auto bg1_1 = Sprite::create("bg-4.png");
	bg1_1->setScale(SCREEN_SIZE.width / (bg1_1->getContentSize().width));
	//bg1_1->setScaleY(SCREEN_SIZE.height / bg1_1->getContentSize().height);
	bg1_1->setAnchorPoint(Point(0, 0.5f));

	auto bg1_2 = Sprite::createWithSpriteFrameName("bg1.png");
	//auto bg1_2 = Sprite::create("bg-4.png");
	bg1_2->setScale(SCREEN_SIZE.width / (bg1_2->getContentSize().width));
	//bg1_2->setScaleY(SCREEN_SIZE.height / bg1_2->getContentSize().height);
	bg1_2->setAnchorPoint(Point(0, 0.5f));



	auto bg2_1 = Sprite::createWithSpriteFrameName("bg2.png");
	bg2_1->setScale(SCREEN_SIZE.width / (bg2_1->getContentSize().width));
	//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
	bg2_1->setAnchorPoint(Point(0, 0.5f));

	auto bg2_2 = Sprite::createWithSpriteFrameName("bg2.png");

	bg2_2->setScale(SCREEN_SIZE.width / (bg2_2->getContentSize().width));
	//bg2_2->setScale(SCREEN_SIZE.width / (bg2_2->getContentSize().width / 2));

	//bg2_2->setScaleY(SCREEN_SIZE.height / bg2_2->getContentSize().height);
	bg2_2->setAnchorPoint(Point(0, 0.5f));


	/*auto bg3_1 = Sprite::create("Map/bg3.png");
	bg3_1->setScaleX(SCREEN_SIZE.width / bg3_1->getContentSize().width);
	bg3_1->setScaleY(SCREEN_SIZE.height / bg3_1->getContentSize().height);
	bg3_1->setAnchorPoint(Point(0, 0.5f));

	auto bg3_2 = Sprite::create("Map/bg3.png");
	bg3_2->setScaleX(SCREEN_SIZE.width / bg3_2->getContentSize().width);
	bg3_2->setScaleY(SCREEN_SIZE.height / bg3_2->getContentSize().height);
	bg3_2->setAnchorPoint(Point(0, 0.5f));*/


	background->addChild(bg1_1, 0, Vec2(0.5f, 1), Vec2(0, 0));
	background->addChild(bg1_2, 0, Vec2(0.5f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));

	background->addChild(bg2_1, 0, Vec2(0.7f, 1), Vec2(0, 0));
	background->addChild(bg2_2, 0, Vec2(0.7f, 1), Vec2(bg2_1->getBoundingBox().size.width, 0));
	//background->addChild(bg2_3, 0, Vec2(0.7f, 1), Vec2(bg2_1->getBoundingBox().size.width*2, 0));
	//background->addChild(bg3_1, 0, Vec2(1.5f, 1), Vec2(0, 0));
	//background->addChild(bg3_2, 0, Vec2(1.5f, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	background->setPosition(Point(-SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
	background->setAnchorPoint(Point(0, 0.5f));
	if (isModeDebug)
		background->setVisible(false);
	this->addChild(background, ZORDER_BG);

	background2 = InfiniteParallaxNode::create();

	auto bg3_1 = Sprite::create("Map/bg3.png");
	bg3_1->setScaleX(SCREEN_SIZE.width / (bg3_1->getContentSize().width));
	bg3_1->setScaleY(bg3_1->getScaleX()*0.7f);
	//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
	bg3_1->setAnchorPoint(Point(0, 0));

	auto bg3_2 = Sprite::create("Map/bg3.png");
	bg3_2->setScale(SCREEN_SIZE.width / (bg3_1->getContentSize().width));
	bg3_2->setScaleY(bg3_2->getScaleX()*0.7f);
	//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
	bg3_2->setAnchorPoint(Point(0, 0));

	background2->addChild(bg3_1, 0, Vec2(0.8f, 1), Vec2(0, 0));
	background2->addChild(bg3_2, 0, Vec2(0.8f, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	background2->setPosition(Point(0, 0));
	background2->setAnchorPoint(Point(0, 0.0f));
	this->addChild(background2, ZORDER_BG);
}

void GameScene::createGroundBody()
{
	auto groupGround = tmx_map->getObjectGroup("ground");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initGroundPhysic(world, pos, sizeOfBound);
	}

	auto groupUnderGround = tmx_map->getObjectGroup("under_ground");
	if (!groupUnderGround) return;
	for (auto child : groupUnderGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initUnderGroundPhysic(world, pos, sizeOfBound);
	}
}

void GameScene::createGroundForMapBoss()
{
	auto groupGround = tmx_mapboss[0]->getObjectGroup("ground");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap) + tmx_mapboss[0]->getPosition();
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		//Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initGroundPhysic(world, origin, Size(INT_MAX / 4, sizeOfBound.height));
	}

	auto groupUnderGround = tmx_mapboss[0]->getObjectGroup("under_ground");
	if (!groupGround) return;
	for (auto child : groupUnderGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap) + tmx_mapboss[0]->getPosition();
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		//Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initUnderGroundPhysic(world, origin, Size(INT_MAX / 4, sizeOfBound.height));
	}
}

void GameScene::creatEnemyWooder(Layer * layer, Vec2 pos)
{
	auto scaleOfWooder = (SCREEN_SIZE.height / 3.5) / 490; // 490 la height cua spine
														   //auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
														   //	"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
	auto enemy = (EnemyWooder*)wooderPool->getObject();
	//log("index wooder: %d", wooderPool->indexOfPool);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	layer->addChild(enemy, ZORDER_ENEMY);
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2.5f));
	enemy->changeBodyCategoryBits(BITMASK_WOODER);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);

	enemy->listener();
}

void GameScene::createEnemyToanChanStudent(Layer * layer, Vec2 pos)
{
	auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
														 /*auto enemy = EnemyToanChanStudent::create("Animation/Enemy_DeTuToanChan1/ToanChan1.json",
														 "Animation/Enemy_DeTuToanChan1/ToanChan1.atlas", scaleOfEnemy);*/
	auto enemy = EnemyToanChanStudent::create("Animation/Enemy_DeTuToanChan1/ToanChan1", scaleOfEnemy);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	layer->addChild(enemy, ZORDER_ENEMY);
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 4));
	enemy->changeBodyCategoryBits(BITMASK_TOANCHAN1);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	//enemy->genSplash();
	enemy->listener();
}

void GameScene::createEnemyToanChanStudent2(Layer * layer, Vec2 pos)
{
	auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
														 /*auto enemy = EnemyToanChanStudent2::create("Animation/Enemy_DeTuToanChan2/ToanChan2.json",
														 "Animation/Enemy_DeTuToanChan2/ToanChan2.atlas", scaleOfEnemy);*/
	auto enemy = EnemyToanChanStudent2::create("Animation/Enemy_DeTuToanChan2/ToanChan2", scaleOfEnemy);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	layer->addChild(enemy, ZORDER_ENEMY);
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
	enemy->changeBodyCategoryBits(BITMASK_TOANCHAN2);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	enemy->genSlash();
	enemy->listener();
	/*auto slash = enemy->getSlash();
	slash->initCirclePhysic(world, slash->getPosition());
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
	slash->getB2Body()->GetFixtureList()->SetSensor(true);*/
}

void GameScene::creatBoss()
{
	auto groupGround = tmx_map->getObjectGroup("boss");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 3.0f / 560; // 560 la height cua spine
		/*auto enemy = EnemyBoss1::create("Animation/Enemy_Boss1/Boss1.json",
			"Animation/Enemy_Boss1/Boss1.atlas", scaleOfEnemy);*/
		auto enemy = EnemyBoss1::create("Animation/Enemy_Boss1/Boss1.json",
			"Animation/Enemy_Boss1/Boss1.atlas", scaleOfEnemy);
		//auto enemy = EnemyBoss1::create("Effect/exxp.json", "Effect/exxp.atlas", scaleOfEnemy);
		enemy->setPosition(origin);
		//enemy->setVisible(false);
		this->addChild(enemy, ZORDER_ENEMY);
		enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 2));
		enemy->changeBodyCategoryBits(BITMASK_BOSS);
		enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
		//enemy->listener();
		enemy->createPool();
		enemy->creatHpSprite();
		enemy->listener();

		enemy->createCoinPool();
	}
}

//void GameScene::createCoint()
//{
//	createFormCoin("coin_tim", "Map/tim.tmx", "tim");
//	createFormCoin("coin_straight", "Map/straight.tmx", "straight");
//	createFormCoin("coin_parabol", "Map/parapol.tmx", "parapol");
//	createFormCoin("coin_square", "Map/square.tmx", "square");
//	createFormCoin("coin_zigzag", "Map/zigzag.tmx", "zigzag");
//	createFormCoin("coin_zigzag2", "Map/zigzag2.tmx", "zigzag2");
//	createFormCoin("coin_circle", "Map/circle.tmx", "circle");
//	createCointBag();
//	createCoinBullion();
//}

void GameScene::createCointBag(Layer *layer, Vec2 pos)
{

	auto scaleOfEnemy = SCREEN_SIZE.height / 5.0f / 123; //  la height cua spine
	auto coin = CoinBag::create("Gold_bag", scaleOfEnemy);
	coin->setPosition(pos);
	//enemy->setVisible(false);
	layer->addChild(coin, ZORDER_ENEMY);
	coin->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY()));
	coin->getB2Body()->SetType(b2_staticBody);
	coin->getB2Body()->GetFixtureList()->SetSensor(true);
	coin->changeBodyCategoryBits(BITMASK_COIN_BAG);
	coin->changeBodyMaskBits(BITMASK_SWORD);
	coin->update(0.0f);
	//enemy->listener();

}

void GameScene::createCoinBullion(Layer *layer, Vec2 pos)
{
	auto scaleOfEnemy = SCREEN_SIZE.height / 6.0f / 118; //  la height cua spine
	auto coin = CoinBullion::create("gold", scaleOfEnemy);
	coin->setPosition(pos);
	//enemy->setVisible(false);
	layer->addChild(coin, ZORDER_ENEMY);
	coin->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY()));
	coin->getB2Body()->SetType(b2_staticBody);
	coin->getB2Body()->GetFixtureList()->SetSensor(true);
	coin->changeBodyCategoryBits(BITMASK_COIN_BULLION);
	coin->changeBodyMaskBits(BITMASK_HERO);
	coin->update(0.0f);
}


void GameScene::createFormCoin(Layer* layer, Vec2 pos, string objectMap, string objectInForm, SpriteBatchNode* batchnode)
{
	auto tmx = TMXTiledMap::create(objectMap);
	auto groupCoin = tmx->getObjectGroup(objectInForm);
	for (auto c : groupCoin->getObjects()) {
		auto mObject2 = c.asValueMap();
		Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
		auto coin = (Coin*)coinPool->getObject();
		if (coin->getParent()) {
			coin->removeFromParentAndCleanup(false);
		}
		/*if (coin->getB2Body()) {
			world->DestroyBody(coin->getB2Body());
			coin->setB2Body(nullptr);
		}*/
		coin->setVisible(true);
		auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
		coin->setScale(scale);
		coin->setPosition(pos + origin2);
		coin->runAnimation();
		coin->world = world;
		layer->addChild(coin, ZORDER_ENEMY);
		/*coin->initCirclePhysic(world, Vec2(pos + origin2 + layer->getPosition()));
		coin->changeBodyCategoryBits(BITMASK_COIN);
		coin->changeBodyMaskBits(BITMASK_HERO);
		coin->getB2Body()->GetFixtureList()->SetSensor(true);*/
		//coin->runAnimation();
	}

}

void GameScene::danceWithEffect()
{
	EM->createWithFile(hero->getScale() / 3);
	addChild(EM->getSlashBreak(), 4);
	addChild(EM->getSmokeJumpX2(), 4);
	addChild(EM->getSmokeLanding(), 4);
	addChild(EM->getSmokeRun(), 4);
	addChild(EM->getReviveMe(), 4);
}

void GameScene::danceWithCamera()
{
	auto origin = Director::getInstance()->getVisibleOrigin();
	follow = Node::create();
	follow->setPosition(origin + SCREEN_SIZE / 2);
	this->addChild(follow);

	camera = Follow::create(follow);
	camera->setTarget(follow);
	runAction(camera);

	blur = LayerColor::create(Color4B(0, 0, 0, 170));
	blur->setVisible(false);
	left_corner = CCRectMake(0, 0, SCREEN_SIZE.width / 2, SCREEN_SIZE.height);
}

void GameScene::updateCharacterPoint()
{
	if (previousPercentPosition < 1.0f) {
		auto map_distance = tmx_map->getBoundingBox().size.width;
		auto currentPercent = hero->getPositionX() / map_distance;
		auto deltaPos = (currentPercent - previousPercentPosition) * hud->getDistanceBar()->getBoundingBox().size.width * 0.82f;
		hud->getCharacterPoint()->setPositionX(hud->getCharacterPoint()->getPositionX() + deltaPos);
		previousPercentPosition = currentPercent;
	}
}

void GameScene::initGroundPhysic(b2World * world, Point pos, Size size)
{
	b2Body * body;
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_FLOOR;
	fixtureDef.filter.maskBits = BITMASK_HERO | BITMASK_FLOOR | BITMASK_COIN;

	bodyDef.type = b2_staticBody;

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void GameScene::initUnderGroundPhysic(b2World * world, Point pos, Size size)
{
	b2Body * body;
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_UNDER_GROUND;
	fixtureDef.filter.maskBits = BITMASK_SWORD | BITMASK_UNDER_GROUND;

	bodyDef.type = b2_staticBody;

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}



//void GameScene::readWriteJson()
//{
//	/**
//	* test json
//	*/
//
//	Document heroJsonFile;
//	string herobuffer = FileUtils::getInstance()->getStringFromFile("Hero.json");
//	heroJsonFile.Parse(herobuffer.c_str());
//	assert(heroJsonFile.IsObject());
//	heroJsonFile["hero"][0]["level"].SetInt(2);
//	//log("doi tuong thu nhat:%d", heroJsonFile["hero"][0]["level"].GetInt());
//
//	StringBuffer buffer;
//	Writer<StringBuffer> writer(buffer);
//	heroJsonFile.Accept(writer);
//	const char* output = buffer.GetString();
//
//	FileUtils::getInstance()->writeStringToFile(output, "Hero.json");
//}

bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (left_corner.containsPoint(touch->getLocation())) {

		// cannot jump while attacking or being injured
		if (hero->getFSM()->currentState == MAttack || hero->getFSM()->currentState == MInjured ||
			hero->getFSM()->currentState == MDie || hero->getFSM()->currentState == MRevive ||
			hero->getPositionY() + hero->getTrueRadiusOfHero() < 0
			)

			return false;

		if (hero->getNumberOfJump() > 0) {
			hero->setNumberOfJump(hero->getNumberOfJump() - 1);
			auto currentVelX = hero->getB2Body()->GetLinearVelocity().x;
			hero->getB2Body()->SetLinearVelocity(b2Vec2(currentVelX, hero->getJumpVel()));

			if (hero->getFSM()->currentState == MLand && hero->getNumberOfJump() > 0) {
				EM->getSmokeJumpX2()->setPosition(hero->getPosition());
				EM->getSmokeJumpX2()->setVisible(true);
				EM->smokeJumpX2Ani();
			}


			if (hero->getNumberOfJump() == 1) {
				hero->getFSM()->changeState(MJump);
				hero->setOnGround(false);
			}
			if (hero->getNumberOfJump() == 0)
				hero->getFSM()->changeState(MDoubleJump);
		}

	}

	return false;

}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		pauseGame();
	}
}

void GameScene::updateEnemy()
{
	auto child = this->getChildren();
	for (int i = 0; i < child.size(); i++) {
		if (child.at(i)->getTag() > 100) {
			auto tmp = (BaseEnemy*)child.at(i);
			if (tmp->getTag() == TAG_BOSS) {
				if (tmp->getIsDie()) {
					this->isWinGame = true;
				}
				else {
					auto boss = (EnemyBoss1*)tmp;
					if (tmp->getB2Body()->GetLinearVelocity().x != 0)
					{
						boss->updateMe(hero);
					}

					else if (tmp->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width * 0.75f) {
						boss->updateMe(hero);

					}
					if (tmp->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width) {
						hero->updateAttackBossAsTarget(boss);
					}
				}
			}
		}
	}
	if (preLayer != nullptr) {
		auto childLayer1 = preLayer->getChildren();

		for (int i = 0; i < childLayer1.size(); i++) {
			auto agent = childLayer1.at(i);

			if (agent->getTag() >= 90) {
				if ((agent->getPosition() + preLayer->getPosition()).x < follow->getPositionX() + SCREEN_SIZE.width * 1 / 2
					&& (agent->getPositionX() + preLayer->getPositionX())>follow->getPositionX() - SCREEN_SIZE.width * 1 / 2) {
					auto tmp = (B2Skeleton*)(agent);
					tmp->updateMe(hero);
				}
			}
			else if (agent->getTag() >= 80 && agent->getTag() < 90) {
				if ((agent->getPosition() + preLayer->getPosition()).x < follow->getPositionX() + SCREEN_SIZE.width * 1 / 2.5f
					&& (agent->getPositionX() + preLayer->getPositionX())>follow->getPositionX() - SCREEN_SIZE.width * 1 / 2.5f) {
					auto tmp = (B2Sprite*)(agent);
					tmp->updateMe(hero);
				}
			}

		}
	}
	if (posLayer != nullptr) {
		auto childLayer2 = posLayer->getChildren();

		for (int i = 0; i < childLayer2.size(); i++) {
			auto agent = childLayer2.at(i);
			if ((agent->getPosition() + posLayer->getPosition()).x < follow->getPositionX() + SCREEN_SIZE.width * 1 / 2
				&& (agent->getPositionX() + posLayer->getPositionX())>follow->getPositionX() - SCREEN_SIZE.width * 1 / 2) {

				if (agent->getTag() >= 90) {
					auto tmp = (B2Skeleton*)(agent);
					tmp->updateMe(hero);
				}
				else if (agent->getTag() >= 80 && agent->getTag() < 90) {
					auto tmp = (B2Sprite*)(agent);
					tmp->updateMe(hero);
				}
			}
		}
	}

}

void GameScene::updateBoss()
{

}

void GameScene::updateHUD(float dt)
{
	if (hero->getCurrentRunDis() - hero->getPreRunDis() > 10.0f) {
		hero->setScore(hero->getScore() + 15);
		hero->setPreRunDis(hero->getCurrentRunDis());
	}

	if (previousGold != hero->getCoinExplored()) {
		hud->getLbMoney()->setString(StringUtils::format("%i", ++previousGold));
	}

	if (previousScore != hero->getScore()) {
		hud->getLbScore()->setString(StringUtils::format("%i", ++previousScore));
	}

	updateCharacterPoint();
}

void GameScene::updateMultiKills() {
	m_nMultiKills += 1;
	m_fMultiKillsCounterTime = 1.0f;
	if (m_nMultiKills > 1) {
		hud->updateMultiKills(m_nMultiKills);
		if (m_nMultiKills >= 5) m_nMultiKills = 5;
		hero->setScore(hero->getScore() + (m_nMultiKills - 1) * m_lastScore);
	}
}

void GameScene::updateBloodBar(int numberOfHealth, bool isVisible)
{
	if (numberOfHealth >= 0) {
		auto blood = (Sprite*)hud->getListBlood()->getObjectAtIndex(numberOfHealth);
		blood->setVisible(isVisible);
	}
}

void GameScene::updateCamera()
{

	if (_aEagle->getIsUp() && _aEagle->getPositionZ() < 100.0f) {
		_aEagle->setPositionZ(_aEagle->getPositionZ() + 5.0f);
		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() - 0.04f);
		//background->setPositionZ(background->getPositionZ() + 5);
	}
	if (_aEagle->getIsDown() && _aEagle->getPositionZ() > 0.0f) {
		_aEagle->setPositionZ(_aEagle->getPositionZ() - 5.0f);
		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() + 0.04f);
		//background->setPositionZ(background->getPositionZ() - 5);
	}
	if (!_aEagle->getIsUp()) {
		if (hero->getPositionY() > SCREEN_SIZE.height * 5 / 6) {
			background->setPositionY(hero->getPositionY() - SCREEN_SIZE.height * 2 / 6);
		}
		else
		{
			background->setPositionY(SCREEN_SIZE.height / 2);
		}

		/*if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
			if (!haveboss) {
				if (hero->getPositionX() < tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width)
					follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
				if (hero->getPositionX() > follow->getPositionX() + SCREEN_SIZE.width / 4) {
					this->winGame();
				}
			}
			else {
				follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
			}

		}*/
	}
	else {
		if (hero->getPositionY() > SCREEN_SIZE.height * 0.5f) {
			background->setPositionY(hero->getPositionY());
		}
		else
		{
			background->setPositionY(SCREEN_SIZE.height / 2);
		}


		//if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
		//	// ngungcamemra
		//	if (!haveboss) {
		//		if (hero->getPositionX() < (tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width))
		//			follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);

		//		if (hero->getPositionX() > follow->getPositionX()+SCREEN_SIZE.width/4) {
		//			this->winGame();
		//		}

		//	}
		//	else {
		//		follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
		//	}
		//	follow->setPositionY(background->getPositionY());
		//}
	}
	if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
		if (hero->getPositionX() > (tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width) && !haveboss)
			this->isWinGame = true;
		if (!isWinGame) {
			follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
		}
		else {
			if (hero->getPositionX() > follow->getPositionX() + SCREEN_SIZE.width / 2) {
				this->winGame();
			}
		}
	}
	background->updatePosition();
	background2->updatePosition();
	follow->setPositionY(background->getPositionY());
}

//void GameScene::cleanMap()
//{
//	for (auto body = world->GetBodyList(); ; body != NULL) {
//		log("a");
//		auto tmp = body;
//		body->GetNext();
//		if (tmp->GetPosition().y < 0) {
//			auto data = (Node*)tmp->GetUserData();
//			data->removeFromParentAndCleanup(true);
//			world->DestroyBody(tmp);
//		}
//	}
//}


void GameScene::cachePlist()
{
	/*SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item/coin.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/bg.plist");*/
}

void GameScene::cacheSkeleton()
{


	//auto scaleOfToanchan1 = SCREEN_SIZE.height / 5 / 401; // 401 la height cua spine
	//sr_toanchan1 = createSkeletonData("Animation/Enemy_DeTuToanChan1/ToanChan1.atlas", "Animation/Enemy_DeTuToanChan1/ToanChan1.json");

//	sr_wooder = createSkeletonData("Animation/Enemy_MocNhan/MocNhan.atlas","Animation/Enemy_MocNhan/MocNhan.json");
	//sr_toanchan1 = createSkeletonData("ToanChan1.atlas", "ToanChan1.json");

	//sr_wooder = createSkeletonData("MocNhan.atlas", "MocNhan.json");

}

void GameScene::shakeTheScreen()
{
	auto shake = MoveBy::create(0.01f, Vec2(0, -0.005f * SCREEN_SIZE.height));
	this->runAction(Sequence::create(shake, shake->reverse(), shake, shake->reverse(), nullptr));
}

void GameScene::reviveHero()
{
	resumeGame();
	hero->setDieHard(1);
	hero->setIsPriorInjured(false);
	hero->getBloodScreen()->setVisible(false);
	hero->setHealth(JSHERO->getBaseHP());
	for (int i = 0; i < hero->getHealth(); i++) {
		auto health = (Sprite*)hud->getListBlood()->getObjectAtIndex(i);
		health->setVisible(true);
	}

	hero->doDestroyBodies(world);

	auto killAll = CallFunc::create([&]() {
		hero->killThemAll(listEnemyOccurInScreen);
	});

	auto reviveUp = MoveBy::create(1.5f, Vec2(0, hero->getTrueRadiusOfHero() * 2.7f));
	hero->runAction(Sequence::create(reviveUp, killAll, nullptr));
	hero->getFSM()->changeState(MRevive);
}

void GameScene::callingBird()
{
	hero->changeBodyCategoryBits(BITMASK_WOODER);
	hero->getB2Body()->SetGravityScale(0);
	hero->setIsDriverEagle(true);
	hero->setOnGround(false);
	hero->getFSM()->changeState(MIdle);
	hero->setVisible(false);
	hud->setBtnCallingHintDone(false);
	_aEagle->setVisible(true);
	_aEagle->getB2Body()->SetTransform(hero->getB2Body()->GetPosition(), 0.0f);
	_aEagle->flyUp(b2Vec2(hero->getMoveVel(), 7.0f));

	auto _aEagleFlyDown = Sequence::create(DelayTime::create(4.0f), CallFunc::create([&]() {
		_aEagle->flyDown(b2Vec2(hero->getMoveVel(), -7.0f));
	}), nullptr);
	auto _aHeroGetOffEagle = Sequence::create(DelayTime::create(6.0f), CallFunc::create([&]() { heroGetOffEagle(); }), nullptr);
	_aEagle->runAction(_aEagleFlyDown);
	_aEagle->runAction(_aHeroGetOffEagle);
}

void GameScene::blurScreen()
{
	blur->setVisible(true);
}

void GameScene::pauseGame()
{
	blurScreen();
	// disable the menu pause item
	if (EM->getSmokeRun()->isVisible()) {
		EM->getSmokeRun()->pause();
	}

	if (hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	hud->pauseIfVisible();

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogPause::create();
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::dieGame()
{
	blurScreen();
	if (hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	hud->pauseIfVisible();

	if (EM->getSmokeRun()->isVisible()) {
		EM->getSmokeRun()->pause();
	}

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogRevive::create(++numberRevive);
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::overGame()
{
	if (!blur->isVisible())
		blurScreen();

	if (hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	hud->pauseIfVisible();

	if (EM->getSmokeRun()->isVisible()) {
		EM->getSmokeRun()->pause();
	}

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogOverGame::create(hero->getScore(), hero->getCoinExplored());
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::nextGame()
{
	++map;

	if (map < 3)
		Director::getInstance()->replaceScene(GameScene::createScene(map, haveboss, charId));
	else if (map == 3)
		Director::getInstance()->replaceScene(GameScene::createScene(map, true, charId));
	else
		Director::getInstance()->replaceScene(MenuLayer::createScene());
}

void GameScene::winGame()
{
	blurScreen();
	if (hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	hud->pauseIfVisible();
	if (EM->getSmokeRun()->isVisible()) {
		EM->getSmokeRun()->pause();
	}

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogStageClear::create(hero->getScore(), hero->getCoinExplored());
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::resumeGame()
{
	blur->setVisible(false);
	if (EM->getSmokeRun()->isVisible()) {
		EM->getSmokeRun()->resume();
	}

	hero->resume();
	if (hero->getIsDriverEagle())
		_aEagle->resume();

	dialogPause->removeFromParentAndCleanup(true);
	dialogPause = nullptr;

	hud->resumeIfVisible();
	hud->getPauseItem()->setEnabled(true);
	if (hud->getBtnCalling()->isVisible())
		hud->getBtnCalling()->setEnabled(true);


	this->resume();
}

void GameScene::restartGame()
{
	Director::getInstance()->replaceScene(GameScene::createScene(map, haveboss, charId));
}

void GameScene::loadPosAndTag()
{
	loadPosOfObjectInGroup("wooder", TAG_ENEMY_WOODER);
	loadPosOfObjectInGroup("toanchan_student", TAG_ENEMY_TOANCHAN1);
	loadPosOfObjectInGroup("toanchan_student2", TAG_ENEMY_TOANCHAN2);
	loadPosOfObjectInGroup("coin_bullion", TAG_COINBULLION);
	loadPosOfObjectInGroup("coin_bag", TAG_COINBAG);
	loadPosOfObjectInGroup("coin_parabol", TAG_COIN_PARABOL);
	loadPosOfObjectInGroup("coin_straight", TAG_COIN_STRAIGHT);
	loadPosOfObjectInGroup("coin_zigzag", TAG_COIN_ZIGZAG);
	loadPosOfObjectInGroup("coin_zigzag2", TAG_COIN_ZIGZAG2);
	loadPosOfObjectInGroup("coin_square", TAG_COIN_SQUARE);
	loadPosOfObjectInGroup("coin_circle", TAG_COIN_CIRCLE);
	loadPosOfObjectInGroup("coin_tim", TAG_COIN_TIM);
}

void GameScene::loadPosOfObjectInGroup(string nameOfGroup, float tag)
{
	auto groupGround = tmx_map->getObjectGroup(nameOfGroup);
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		MyData data(tag, origin.x, origin.y);
		listPosAndTag.push_back(data);
	}
}

void GameScene::initLayerToAddAgent()
{
	preLayer = Layer::create();
	posLayer = Layer::create();
	preLayer->setPosition(Vec2::ZERO);
	preLayer->setAnchorPoint(Point(0, 0));
	posLayer->setPosition(SCREEN_SIZE.width*1.5f, 0);
	posLayer->setAnchorPoint(Point(0, 0));
	this->addChild(preLayer, ZORDER_ENEMY);
	this->addChild(posLayer, ZORDER_ENEMY);
	createAgentOnLayer(preLayer);
	createAgentOnLayer(posLayer);

}

void GameScene::updateLayer()
{
	if (posLayer != nullptr) {
		if (posLayer->getPositionX() <= follow->getPositionX() - SCREEN_SIZE.width) {
			preLayer->removeAllChildrenWithCleanup(false);
			Layer *tmpLayer = preLayer;
			preLayer = posLayer;
			posLayer = tmpLayer;

			posLayer->setPosition(preLayer->getPositionX() + SCREEN_SIZE.width*1.5f, 0);
			if (posLayer->getPositionX() < tmx_map->getBoundingBox().size.width) {
				auto call = CCCallFunc::create([&]() {
					createAgentOnLayer(posLayer);
				});
				this->runAction(call);
			}
			else {
				posLayer->removeFromParentAndCleanup(true);
				posLayer = nullptr;
			}
		}
	}
}

void GameScene::createAgentOnLayer(Layer * layer)
{
	for (auto i : listPosAndTag) {
		//log("i.x = %f", i.x);
		if (i.x >= layer->getPositionX() && i.x < layer->getPositionX() + SCREEN_SIZE.width*1.5f) {
			creatAgentByMydata(layer, i);
		}
	}
}

void GameScene::creatAgentByMydata(Layer * layer, MyData data)
{
	switch (data.tag)
	{
	case TAG_ENEMY_WOODER: {
		//ok
		creatEnemyWooder(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		//log("wooder: %f, %f", layer->getPositionX(), layer->getPositionY());
		break;

	}
	case TAG_ENEMY_TOANCHAN1: {
		createEnemyToanChanStudent(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_TOANCHAN2: {
		createEnemyToanChanStudent2(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_COINBAG: {
		// ok
		createCointBag(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_COINBULLION: {

		createCoinBullion(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_COIN_PARABOL: {
		//ok
		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/parapol.tmx", "parapol", batchNode);
		break;
	}
	case TAG_COIN_STRAIGHT: {
		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/straight.tmx", "straight", batchNode);
		break;
	}
	case TAG_COIN_ZIGZAG: {
		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/zigzag.tmx", "zigzag", batchNode);
		break;
	}
	case TAG_COIN_ZIGZAG2: {
		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/zigzag2.tmx", "zigzag2", batchNode);
		break;
	}
	case TAG_COIN_SQUARE: {
		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/square.tmx", "square", batchNode);
		break;
	}
	case TAG_COIN_CIRCLE: {
		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/circle.tmx", "circle", batchNode);
		break;
	}
	case TAG_COIN_TIM: {
		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/tim.tmx", "tim", batchNode);
		break;
	}

	default:
		break;
	}
}

//void GameScene::createMapItem()
//{
//	hero->checkItem.insert(KEY_ITEM_MAGNET,100);
//	hero->checkItem.insert(KEY_ITEM_DOUPLE_COIN, 0);
//}
//
//void GameScene::updateMapItem()
//{
//	for (int i = KEY_ITEM_MAGNET; i < KEY_ITEM_DOUPLE_COIN; i++) {
//		int value = hero->checkItem.at(i);
//		if (value > 0) {
//			value--;
//			hero->checkItem.insert(i, value);
//		};
//	}
//}