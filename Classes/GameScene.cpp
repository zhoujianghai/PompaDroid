#include "GameScene.h"
#include "GameLayer.h"
#include "OperateLayer.h"

using namespace cocos2d;


GameScene::GameScene():
	m_pGameLayer(NULL),
	m_pOperateLayer(NULL)
{

}

GameScene::~GameScene()
{

}


bool GameScene::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF( !Scene::init() );
		m_pGameLayer = GameLayer::create();
		this->addChild(m_pGameLayer, 0);
		m_pOperateLayer = OperateLayer::create();
		this->addChild(m_pOperateLayer, 1);
		m_pOperateLayer->setOperateDelegate(m_pGameLayer);

		ret = true;
	}while(0);

	return ret;
}

