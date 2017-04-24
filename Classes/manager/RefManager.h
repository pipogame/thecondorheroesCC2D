#ifndef __REF_MANAGER_H__
#define __REF_MANAGER_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;


class RefManager
{
public:

	RefManager();
	~RefManager();

protected:
	CC_SYNTHESIZE_READONLY(bool, isFirstPlay, IsFirstPlay);

	CC_SYNTHESIZE_READONLY(int, selectedHero, SelectedHero);
	CC_SYNTHESIZE_READONLY(bool, isLockedHero, IsLockedHero);	// to try or smt

	CC_SYNTHESIZE_READONLY(int, currentStageUnlocked, CurrentStageUnlocked);
	CC_SYNTHESIZE_READONLY(int, currentMapUnLocked, CurrentMapUnLocked);
	CC_SYNTHESIZE_READONLY(int, currentLevel, CurrentLevel);
	CC_SYNTHESIZE_READONLY(int, currentScore, CurrentScore);

	CC_SYNTHESIZE_READONLY(int, currentHealth, CurrentHealth);

	CC_SYNTHESIZE_READONLY(int, anchorTime, AnchorTime);   //

	// life of game
	CC_SYNTHESIZE_READONLY(int, numberOfLife, NumberOfLife);   //
	CC_SYNTHESIZE_READONLY(int, goldExplored, GoldExplored);
	CC_SYNTHESIZE_READONLY(int, diamondBuy, DiamondBuy);


	CC_SYNTHESIZE_READONLY(float, durationSkill_1, DurationSkill_1);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill_1, CoolDownSkill_1);

	CC_SYNTHESIZE_READONLY(float, durationSkill_2, DurationSkill_2);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill_2, CoolDownSkill_2);

	CC_SYNTHESIZE_READONLY(float, durationSkill_3, DurationSkill_3);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill_3, CoolDownSkill_3);

	CC_SYNTHESIZE_READONLY(int, levelSkill_1, LevelSkill_1);
	CC_SYNTHESIZE_READONLY(int, levelSkill_2, LevelSkill_2);
	CC_SYNTHESIZE_READONLY(int, levelSkill_3, LevelSkill_3);

	CC_SYNTHESIZE_READONLY(int, bonusScore, BonusScore);
	CC_SYNTHESIZE_READONLY(int, bonusGold, BonusGold);

	CC_SYNTHESIZE_READONLY(int, numberItemHealth, NumberItemHealth);
	CC_SYNTHESIZE_READONLY(int, numberItemBird, NumberItemBird);
	CC_SYNTHESIZE_READONLY(int, numberItemMagnet, NumberItemMagnet);
	CC_SYNTHESIZE_READONLY(int, numberItemDoubleGold, NumberItemDoubleGold);
	CC_SYNTHESIZE_READONLY(int, numberItemCoolDown, NumberItemCoolDown);

private:
	UserDefault* ref;
	static RefManager *refManager;

public:
	static RefManager* getInstance();

	void pointToCurrentHero(int index);		// set props to get
	
	void setDoneFirstPlay();

	void unLockHero(int index);
	void increaseLevel();

	void increaseStateUnlocked();
	void setMapUnlocked(int index);
	
	// baseHP
	void setUpHealth(int health);
	void setDownHealth(int health);

	// set current score after increase level
	void setCurrentScoreAfterIncrease(int score);

	// set increase score after done one game
	void setUpScore(int score);
	
	// void decrease time duration and cooldown
	void decreaseDurationSkill_X(int skill_What, int percent);
	void decreaseCoolDownSkill_X(int skill_What, int percent);

	void increaseBonusScore(int value);
	void increaseBonusGold(int value);

	void resetAnchorTime();

	// up and down

	// eat to play
	void setUpLife(int life);   
	bool setDownLife(int life);
	
	void setUpGoldExplored(int gold);
	bool setDownGold(int gold);

	void setUpDiamondBuy(int diamond);
	bool setDownDiamond(int diamond);


	void increaseLevelSkill_1();
	void increaseLevelSkill_2();
	void increaseLevelSkill_3();

	void increaseNumberItemHealth();				void decreaseNumberItemHealth();
	void increaseNumberItemBird();					void decreaseNumberItemBird();
	void increaseNumberItemMagnet();				void decreaseNumberItemMagnet();
	void increaseNumberItemDoubleGold();			void decreaseNumberItemDoubleGold();
	void increaseNumberItemCoolDown();				void decreaseNumberItemCoolDown();

};

#define REF RefManager::getInstance()
// Ref
#define KEY_FIRST							"FirstPlay"
#define KEY_SELECTED_HERO					"SelectedHero"		// integer

#define KEY_CUR_STAGE_UNLOCKED				"CurrentStageUnlocked"
#define KEY_CUR_MAP_UNLOCKED				"CurrentMapUnlocked"

#define KEY_ANCHORTIME						"AnchorTime"

#define KEY_LIFE							"MyLife"
#define KEY_GOLD							"MyGold"
#define KEY_DIAMOND							"MyDiamond"


#define KEY_LOCKED_HERO_X					"LockedHero_"

// LEVEL CURRENTLY
#define KEY_LEVEL_HERO_X					"LevelHero_"		// integer (1->15)

#define KEY_HEALTH_HERO_X					"HealthHero_"		// baseHP

// BONUS
#define KEY_BONUS_SCORE_HERO_X				"BonusScoreHero_"
#define KEY_BONUS_GOLD_HERO_X				"BonusGoldHero_"


// SCORE CURRENTLY // ONCE LV UPGRADE, CALCULATE SCORE FOR CURRENT LEVEL (EX: Level 2, score 2500 -> Level 3, score 500)
#define KEY_SCORE_X							"ScoreHero_"		// follow level


// Define duration and cooldown time (if not define, get data from json)
#define KEY_DURATION_SKILL_1_HERO_X						"DurationSkill1_Hero_"
#define KEY_COOLDOWN_SKILL_1_HERO_X						"CoolDownSkill1_Hero_"

#define KEY_DURATION_SKILL_2_HERO_X						"DurationSkill2_Hero_"
#define KEY_COOLDOWN_SKILL_2_HERO_X						"CoolDownSkill2_Hero_"

#define KEY_DURATION_SKILL_3_HERO_X						"DurationSkill3_Hero_"
#define KEY_COOLDOWN_SKILL_3_HERO_X						"CoolDownSkill3_Hero_"

// from level skill we can calculate duration and cooldown
#define KEY_LEVEL_SKILL_1_HERO_X						"LevelSkill1_Hero_"
#define KEY_LEVEL_SKILL_2_HERO_X						"LevelSkill2_Hero_"
#define KEY_LEVEL_SKILL_3_HERO_X						"LevelSkill3_Hero_"


// ITEM HERE
#define NUMBER_OF_ITEM_HEALTH							"NumberOfItemHealth"
#define NUMBER_OF_ITEM_BIRD								"NumberOfItemBird"
#define NUMBER_OF_ITEM_MAGNET							"NumberOfItemMagnet"
#define NUMBER_OF_ITEM_DOUBLE_COIN						"NumberOfItemDoubleCoin"
#define NUMBER_OF_ITEM_COOL_DOWN						"NumberOfItemCoolDown"

#endif // __REF_MANAGER_H__