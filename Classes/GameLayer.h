#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include <cocos2d.h>
#include "OperateDelegate.h"

#define MIN_ENEMY_COUNT 5

class BaseSprite;
class Hero;
class GameLayer : public cocos2d::Layer
{
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	void onHeroWalk(cocos2d::Point direction, float distance);
	void onHeroAttack();
	void onHeroStop();
	void onHeroDead(BaseSprite *pTarget);

	void onEnemyAttack(BaseSprite *pSprite);
	void onEnemyDead(BaseSprite *pTarget);

	void addEnemy();

	void update(float dt);
	void updateHero(float dt);
	void updateEnemies(float dt);

	CC_SYNTHESIZE_READONLY(Hero*, m_pHero, Hero);

	CREATE_FUNC(GameLayer);

private:
	cocos2d::TMXTiledMap *m_pTiledMap;
	cocos2d::Array *m_pEnemies;
	cocos2d::SpriteBatchNode *m_pSpriteNodes;
	cocos2d::Point m_heroVelocity;
	float m_fTileWidth;
	float m_fTileHeight;
	float m_fScreenWidth;
	float m_fScreenHeight;

	cocos2d::ProgressTimer *m_pBlood;
	cocos2d::ProgressTimer *m_pBloodBg;
};

#endif