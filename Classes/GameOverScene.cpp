#include "GameOverScene.h"
#include "SceneManager.h"

using namespace cocos2d;

GameOverLayer::GameOverLayer()
	:m_pLabel(NULL)
{
	
}

GameOverLayer::~GameOverLayer()
{

}

bool GameOverLayer::init()
{
	bool ret = false;
	do
	{
		CC_BREAK_IF( !LayerColor::initWithColor( Color4B(255, 255, 255, 255)) );

		Size winSize = Director::getInstance()->getWinSize();
		this->m_pLabel = LabelTTF::create("Game Over!!!", "Artial", 32);
		this->m_pLabel->setPosition( Point(winSize.width / 2, winSize.height / 2) );
		this->m_pLabel->setColor( Color3B(0, 0, 0) );

		this->addChild(this->m_pLabel);
		this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create( CC_CALLBACK_0(GameOverLayer::restartGame, this)), NULL));

		ret = true;
	}while(false);

	return ret;
}


void GameOverLayer::restartGame()
{
	SceneManager::getInstance()->showScene(GAME_SCENE);
}

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverLayer::create();
	scene->addChild(layer);
	return scene;
}
