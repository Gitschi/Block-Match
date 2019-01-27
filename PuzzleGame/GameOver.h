#pragma once
#include "GameManager.h"
#include <Siv3D.hpp>

class GameOver 
{
private:
	GameManager* gameManager;
	Texture img;

public:
	GameOver(GameManager* _gameManager);
	void update();
	void draw();
};