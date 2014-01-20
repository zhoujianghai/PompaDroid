#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <cocos2d.h>

class GameLayer;
class OperateLayer;

class GameScene : public cocos2d::Scene
{
public:
	GameScene();
	~GameScene();

	virtual bool init();

	CREATE_FUNC(GameScene);

	CC_SYNTHESIZE(GameLayer*, m_pGameLayer, GameLayer);
	CC_SYNTHESIZE(OperateLayer*, m_pOperateLayer, OperateLayer);
};


#endif