#include "GameScene.h"
#include "GameLayer.h"
#include "OperateLayer.h"

using namespace cocos2d;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	
	auto gameLayer = GameLayer::create();
	scene->addChild(gameLayer, 0);

	auto operateLayer = OperateLayer::create();
	scene->addChild(operateLayer, 1);
	operateLayer->setHero(gameLayer->getHero());

	return scene;
}


