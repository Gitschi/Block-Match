#include "GameOver.h"

GameOver::GameOver(GameManager* _gameManager)
{
	gameManager = _gameManager;

	img = Texture(L"Assets/Images/GameOver.png");
}

// Update is called every frame
void GameOver::update()
{
	// Returns to first level
	if (Input::MouseR.clicked)
	{
		gameManager->init();
		gameManager->setGameState(0);
	}
}

// Draw to the screen
void GameOver::draw()
{
	if(gameManager->getGameState() != 11) { return; }

	img.resize(1280, 768).draw(0,0, Alpha(150));
}
