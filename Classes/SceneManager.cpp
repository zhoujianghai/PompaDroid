#include "SceneManager.h"
#include "GameScene.h"
#include "GameOverScene.h"

using namespace cocos2d;

static SceneManager *s_pSceneManager = NULL;

SceneManager* SceneManager::getInstance()
{
	if(!s_pSceneManager)
	{
		s_pSceneManager = new SceneManager();
	}
	return s_pSceneManager;
}


void SceneManager::showScene(SceneType sceneType)
{
	Scene *pScene = NULL;
	switch(sceneType)
	{
	case GAME_SCENE:
		pScene = GameScene::createScene();
		break;
	case GAME_OVER_SCENE:
		pScene = GameOverScene::createScene();
		break;
	}

	if(pScene)
	{
		Director::getInstance()->replaceScene(pScene);
	}
}