#include "GameLayer.h"
#include "Hero.h"
#include "Enemy.h"

using namespace cocos2d;

GameLayer::GameLayer()
	:m_pTiledMap(NULL),
	m_pSpriteNodes(NULL),
	m_pHero(NULL),
	m_pEnemies(NULL),
	m_pBlood(NULL),
	m_pBloodBg(NULL)
{

}

GameLayer::~GameLayer()
{
	this->unscheduleUpdate();
	CC_SAFE_RELEASE(m_pEnemies);
}

bool GameLayer::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF( !Layer::init());

		Size winSize = Director::getInstance()->getWinSize();
		m_fScreenWidth = winSize.width;
		m_fScreenHeight = winSize.height;

		m_pTiledMap = TMXTiledMap::create("pd_tilemap.tmx");
		this->addChild(m_pTiledMap);
		Size tileSize = m_pTiledMap->getTileSize();
		m_fTileWidth = tileSize.width;
		m_fTileHeight = tileSize.height;

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pd_sprites.plist");
		m_pSpriteNodes = SpriteBatchNode::create("pd_sprites.pvr.ccz");
		this->addChild(m_pSpriteNodes);

		m_pHero = Hero::create();
		m_pHero->setPosition( Point(100, 100) );
		m_pHero->idle();
		m_pHero->setZOrder(m_fScreenHeight - m_pHero->getPositionY());



		Sprite *pBloodSprite = Sprite::create("blood.jpg");
		this->m_pBlood = ProgressTimer::create(pBloodSprite);
		this->m_pBlood->setType(ProgressTimer::Type::BAR);
		this->m_pBlood->setMidpoint(Point(0, 0));
		this->m_pBlood->setBarChangeRate(Point(1, 0));
		this->m_pBlood->setAnchorPoint(Point(0, 1));
		this->m_pBlood->setPosition(Point(2, winSize.height - 10));
		this->m_pBlood->setPercentage(50);
		this->m_pBlood->setScaleX(4.0f);

		this->m_pBloodBg = ProgressTimer::create(Sprite::create("bloodBg.jpg"));
		this->m_pBloodBg->setType(ProgressTimer::Type::BAR);
		this->m_pBloodBg->setMidpoint(Point(0, 0));
		this->m_pBloodBg->setBarChangeRate(Point(1, 0));
		this->m_pBloodBg->setAnchorPoint(Point(0, 1));
		this->m_pBloodBg->setPosition(Point(2, winSize.height - 10));
		this->m_pBloodBg->setPercentage(100);
		this->m_pBloodBg->setScaleX(4.0f);

		this->addChild(m_pBloodBg, 100);
		this->addChild(m_pBlood, 100);

		m_pSpriteNodes->addChild(m_pHero);

		const int enemyCount = 3;
		float x = 300;
		float y = 80;
		m_pEnemies = Array::createWithCapacity(enemyCount);
		m_pEnemies->retain();
		for(int i = 0; i < enemyCount; ++ i)
		{
			Enemy *pEnemy = Enemy::create();
			pEnemy->setPosition(Point(x + i * 100, y));
			pEnemy->setZOrder(m_fScreenHeight - pEnemy->getPositionY());
			pEnemy->idle();
			m_pEnemies->addObject(pEnemy);

			m_pSpriteNodes->addChild(pEnemy);
		}

		this->scheduleUpdate();

		ret = true;
	} while(0);

	return ret;
}


void GameLayer::onWalk(Point direction, float distance)
{
	m_pHero->setFlippedX(direction.x < 0 ? true : false);
	m_pHero->walk();

	m_heroVelocity = direction * (distance < 96 ? 1 : 2);
}

void GameLayer::onAttack()
{
	m_pHero->attack();
	
	if(m_pHero->getCurrActionState() == ACTION_STATE_ATTACK)
	{
		unsigned int count = m_pEnemies->count();
		Object *enemyObj = NULL;
		CCARRAY_FOREACH(m_pEnemies, enemyObj)
		{
			Enemy *pEnemy = (Enemy*)enemyObj;
			if(pEnemy->getCurrActionState() != ACTION_STATE_KNOCKOUT)
			{
				if(fabsf(m_pHero->getPosition().y - pEnemy->getPosition().y) < 10)
				{
					if(m_pHero->getBodyBox().actual.intersectsRect(pEnemy->getHitBox().actual))
					{
						pEnemy->hurt(m_pHero->getAttack());
					}
				}
			}
		}
	}

}

void GameLayer::onStop()
{
	m_pHero->idle();
}

void GameLayer::update(float dt)
{
	if(m_pHero->getCurrActionState() == ACTION_STATE_WALK)
	{
		float halfHeroFrameHeight = (m_pHero->getDisplayFrame()->getRect().size.height) / 2;
		Point expectP = m_pHero->getPosition() + m_heroVelocity;
		Point actualP = expectP;
		if(expectP.y < halfHeroFrameHeight || expectP.y > (m_fTileHeight * 3 + halfHeroFrameHeight) )
		{
			actualP.y = m_pHero->getPositionY();
		}
		float mapWidth = m_pTiledMap->getContentSize().width;
		float halfWinWidth = m_fScreenWidth / 2;
		float halfHeroFrameWidth = (m_pHero->getDisplayFrame()->getRect().size.width) / 2;
		if(expectP.x > halfWinWidth && expectP.x <= (mapWidth - halfWinWidth))
		{
			this->setPositionX(this->getPositionX() - m_heroVelocity.x);
		}else if(expectP.x < halfHeroFrameWidth || expectP.x >= mapWidth - halfHeroFrameWidth){
			actualP.x = m_pHero->getPositionX();
		}
		m_pHero->setPosition(actualP);
		m_pHero->setZOrder(m_fScreenHeight - m_pHero->getPositionY());
	}
}