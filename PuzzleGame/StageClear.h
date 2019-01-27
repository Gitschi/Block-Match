#pragma once
#include "GameManager.h"
#include "SoundManager.h"
#include <Siv3D.hpp>

class StageClear
{
private:
	GameManager* gameManager;
	SoundManager* soundManager;

	Texture img;

public:
	StageClear(SoundManager* _soundManager, GameManager* _gameManager);
	void update();
	void draw();
};