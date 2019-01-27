#include "StageClear.h"

StageClear::StageClear(SoundManager* _soundManager, GameManager* _gameManager)
{
	gameManager = _gameManager;
	soundManager = _soundManager;

	img = Texture(L"Assets/Images/StageClear.png");
}

// Update function which is called every frame this component is active
void StageClear::update()
{
	// Restart stage when right mouse clicked on stage clear screen
	if (Input::MouseR.clicked)
	{
		int n = gameManager->getColorMode();
		n++;

		if (n > 8)
		{
			n = 8;
		}

		gameManager->setColorMode(n);
		gameManager->setGameState(0);
	}
}

// Draws overlay image
void StageClear::draw()
{
	if(gameManager->getGameState() != 10) { return; }

	img.resize(1280, 768).draw(0,0);
}
