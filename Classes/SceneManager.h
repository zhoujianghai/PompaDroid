#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <cocos2d.h>


typedef enum {
	GAME_SCENE = 0,
	GAME_OVER_SCENE = 1,
}SceneType;


class SceneManager
{
public:
	SceneManager(){};
	~SceneManager(){};

	static SceneManager* getInstance();

	void showScene(SceneType sceneType);

};

#endif
