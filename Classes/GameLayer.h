#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include <cocos2d.h>
#include "OperateDelegate.h"

class BaseSprite;
class GameLayer : public cocos2d::Layer, public OperateDelegate
{
public:
	GameLayer();
	~GameLayer();

	virtual bool init();
	CREATE_FUNC(GameLayer);

	void onWalk(cocos2d::Point direction, float distance);
	void onAttack();
	void onStop();

	void update(float dt);

private:
	cocos2d::TMXTiledMap *m_pTiledMap;
	BaseSprite *m_pHero;
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