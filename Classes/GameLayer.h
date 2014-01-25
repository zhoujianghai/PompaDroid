#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include <cocos2d.h>

#define MIN_ENEMY_COUNT 5
#define PATH_BG_MUSIC "background-music-aac.wav"
#define PATH_HERO_HIT_EFFECT "pd_hit0.wav"
#define PATH_ENEMY_HIT_EFFECT "pd_hit1.wav"
#define PATH_HERO_DEAD_EFFECT "pd_herodeath.mp3"
#define PATH_ENEMY_DEAD_EFFECT "pd_botdeath.wav"
#define PATH_HERO_TALK_EFFECT "hero_talk.mp3"

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

	void exitApp(cocos2d::Object* pSender);

	CC_SYNTHESIZE_READONLY(Hero*, m_pHero, Hero);

	CREATE_FUNC(GameLayer);

private:
	cocos2d::TMXTiledMap *m_pTiledMap;
	float m_fTileWidth;
	float m_fTileHeight;

	float m_fScreenWidth;
	float m_fScreenHeight;
	cocos2d::Point m_origin;

	cocos2d::Array *m_pEnemies;
	cocos2d::SpriteBatchNode *m_pSpriteNodes;

	cocos2d::ProgressTimer *m_pBlood;
	cocos2d::ProgressTimer *m_pBloodBg;
	cocos2d::MenuItemImage *m_pCloseItem;
};

#endif