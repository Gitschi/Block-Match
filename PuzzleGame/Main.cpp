#include "StageClear.h"
#include "GameOver.h"
#include "BlockManager.h"
#include "GameManager.h"
#include <Siv3D.hpp>

void Main()
{
	// Set window properties
	const Font font(12);
	Graphics::SetBackground(Palette::Mediumslateblue);
	Window::SetTitle(L"Puzzle Game");
	Window::Resize(1280, 768);

	// Instantiate classes
	SoundManager* soundManager = new SoundManager();
	GameManager* gameManager = new GameManager(soundManager);
	BlockManager* blockManager = new BlockManager(soundManager, gameManager);
	GameOver* gameOver = new GameOver(gameManager);
	StageClear* stageClear = new StageClear(soundManager, gameManager);

	// Set initial game state
	gameManager->setGameState(0);

	while (System::Update())
	{
		switch (gameManager->getGameState())
		{
			// Starting a level and dropping blocks
			case 0:
				blockManager->requestFullBlockSet();
				gameManager->setGameState(1);
				break;

			// After fallen to the ground and done sliding
			case 1:
				blockManager->update();
				if(!blockManager->areBlocksFalling())
				{
					gameManager->setGameState(2);
					soundManager->playMusic(1);
				}
				break;

			// While all blocks are grounded and player can make a choice
			case 2:
				gameManager->update();
				break;

			// Wait for blocks to reach the ground
			case 3:
				blockManager->checkBlockVerticalDrop();
				break;

			// Any block reaching the floor
			case 4:
				blockManager->update();
				if(!blockManager->areBlocksFalling())
				{
					gameManager->setGameState(5);
					soundManager->playSound(5);
				}
				break;

			// Done sliding in either direction
			case 5:
				blockManager->requestBlockHorizontalSlide();
				soundManager->playSound(5);
				break;

			// Slide blocks sideways
			case 6:
				blockManager->updateSlide();
				if (!blockManager->areBlocksFalling())
				{
					gameManager->setGameState(5);
				}
				break;

			// Stage Clear
			case 10:
				stageClear->update();
				break;

			// Game Over
			case 11:
				gameOver->update();
				break;

			default:
				break;
		}

		gameManager->draw();
		blockManager->draw();
		stageClear->draw();
		gameOver->draw();

		font(L"Puzzle Game").draw(10, 768 - 24);
		Circle(Mouse::Pos(), 5).draw({ 255, 0, 0, 100 });
	}
}
