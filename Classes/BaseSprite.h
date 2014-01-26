#ifndef _BASE_SPRITE_H_
#define _BASE_SPRITE_H_

#include <cocos2d.h>

typedef enum {
	ACTION_STATE_NONE = 0,
	ACTION_STATE_IDLE,
	ACTION_STATE_WALK,
	ACTION_STATE_ATTACK,
	ACTION_STATE_HURT,
	ACTION_STATE_DEAD,
	ACTION_STATE_REMOVE,
}ActionState;

typedef struct _BoundingBox
{
	cocos2d::Rect actual;
	cocos2d::Rect original;
}BoundingBox;

class BaseSprite : public cocos2d::Sprite
{
public:
	BaseSprite();
	~BaseSprite();

	void runIdleAction();
	void runWalkAction();
	void runAttackAction();
	void runHurtAction();
	void removeSprite();
	void runDeadAction();

	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pIdleAction, IdleAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pWalkAction, WalkAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pAttackAction, AttackAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pHurtAction, HurtAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pDeadAction, DeadAction);

	CC_SYNTHESIZE(ActionState, m_currActionState, CurrActionState);
	CC_SYNTHESIZE(cocos2d::Point, m_fVelocity, Velocity);
	CC_SYNTHESIZE(unsigned int, m_hp, HP);
	CC_SYNTHESIZE(unsigned int, m_attack, Attack);

	CC_SYNTHESIZE(BoundingBox, m_bodyBox, BodyBox);
	CC_SYNTHESIZE(BoundingBox, m_hitBox, HitBox);

	virtual void setPosition(const cocos2d::Point &position);

	virtual void onDead();

	virtual bool isLive();

	cocos2d::CallFunc* createDeadCallbackFunc();
	cocos2d::CallFunc* createIdleCallbackFunc();

	std::function<void(void)> onDeadCallback;

	std::function<void(void)> attack;

protected:
	static cocos2d::Animation* createAnimation(const char* formatStr, int frameCount, int fps);


	BoundingBox createBoundingBox(cocos2d::Point origin, cocos2d::Size size);
	void updateBoxes();

private:
	bool changeState(ActionState actionState);
};

#endif