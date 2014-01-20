#ifndef _COMMON_H_
#define _COMMON_H_

#include <cocos2d.h>

using namespace cocos2d;

Scene* createScene(Layer *pLayer)
{
	Scene *pRet = Scene::create();
	if(pLayer)
	{
		pRet->addChild(pLayer);
	}

	return pRet;
}

#endif