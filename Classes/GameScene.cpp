#include "GameScene.h"
#include "GameLayer.h"
#include "OperateLayer.h"

using namespace cocos2d;

cocos2d::Scene* createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition( Point(visibleSize.width / 2, visibleSize.height / 2) );
	edgeNode->setPhysicsBody(body);
	scene->addChild(edgeNode);

	auto gameLayer = GameLayer::create();
	gameLayer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(gameLayer, 0);

	auto operateLayer = OperateLayer::create();
	scene->addChild(operateLayer, 1);
	operateLayer->setOperateDelegate(gameLayer);
}


