#ifndef _OPERATE_LAYER_H_
#define _OPERATE_LAYER_H_

#include <cocos2d.h>

class Hero;
class OperateLayer : public cocos2d::Layer
{
public:
	OperateLayer();
	~OperateLayer();

	virtual bool init();

	CREATE_FUNC(OperateLayer);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

	CC_SYNTHESIZE(Hero*, m_pHero, Hero);

private:
	void showJoystick(cocos2d::Point pos);
	void hideJoystick();
	void updateJoystick(cocos2d::Point direction, float distance);

	cocos2d::Sprite *m_pJoystick;
	cocos2d::Sprite *m_pJoystickBg;

};

#endif