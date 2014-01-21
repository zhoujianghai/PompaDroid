#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include <cocos2d.h>
#include "OperateDelegate.h"

typedef enum
{
	SPRITE_TAG_HERO = 1,
	SPRITE_TAG_ENEMY
}SpriteTag;

class BaseSprite;
class Hero;
class GameLayer : public cocos2d::Layer, public OperateDelegate
{
public:
	GameLayer();
	~GameLayer();

	virtual bool init();
	CREATE_FUNC(GameLayer);

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

	void setPhysicsWorld(cocos2d::PhysicsWorld* pWorld)
	{
		this->m_pWorld = pWorld;
	};

	bool onContactBegin(cocos2d::EventCustom *pEvent, const cocos2d::PhysicsContact &contact);

private:
	cocos2d::TMXTiledMap *m_pTiledMap;
	Hero *m_pHero;
	cocos2d::Array *m_pEnemies;
	cocos2d::SpriteBatchNode *m_pSpriteNodes;
	cocos2d::Point m_heroVelocity;
	float m_fTileWidth;
	float m_fTileHeight;
	float m_fScreenWidth;
	float m_fScreenHeight;

	cocos2d::ProgressTimer *m_pBlood;
	cocos2d::ProgressTimer *m_pBloodBg;

	cocos2d::PhysicsWorld* m_pWorld;

};

#endif