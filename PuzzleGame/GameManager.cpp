#include "GameManager.h"

GameManager::GameManager(SoundManager* _soundManager)
{
	soundManager = _soundManager;

	// Set textures for block images for hovered state
	img[0] = Texture(L"Assets/Images/01.png");
	img[1] = Texture(L"Assets/Images/01.png");
	img[2] = Texture(L"Assets/Images/02.png");
	img[3] = Texture(L"Assets/Images/03.png");
	img[4] = Texture(L"Assets/Images/04.png");
	img[5] = Texture(L"Assets/Images/05.png");
	img[6] = Texture(L"Assets/Images/06.png");
	img[7] = Texture(L"Assets/Images/07.png");
	img[8] = Texture(L"Assets/Images/08.png");

	horizontalBlockPosition = float(-blockSize);
	verticalBlockPosition = 0;

	// Set number of block variations the first level starts with
	colorMode = 2; 

	init();
	initRecursion();
}

// Initialize block data
void GameManager::init()
{
	// Delete blocks in matrix
	for (int i = 0; i < columnAmount; i++)
	{
		for (int j = 0; j < rowAmount; j++)
		{
			blockMatrixContent[j][i] = 0; // Delete block
		}
	}

	// Create code for floor
	for (int i = 0; i < columnAmount; i++)
	{
		blockMatrixContent[rowAmount - 1][i] = -9;
	}

	//Create code for wall
	for (int i = 0; i < rowAmount; i++)
	{
		blockMatrixContent[i][0] = -9;
	}
}

// Get gamestate
int GameManager::getGameState()
{
	return gameState;
}

// Set gamestate
void GameManager::setGameState(int newGameState)
{
	gameState = newGameState;
}

// Get block content
int GameManager::getBlockContent(int column, int row)
{
	return blockMatrixContent[row][column];
}

// Set block content
void GameManager::setBlockContent(int column, int row, int newContent)
{
	blockMatrixContent[row][column] = newContent;
}

// Checks if the game should be over or not
bool GameManager::gameShouldNotBeOver()
{
	int result = false;

	for (int i = 1; i < columnAmount; i++)
	{
		for (int j = 0; j < (rowAmount -1); j++)
		{
			int n = blockMatrixContent[j][i];
			if (n > 0 && n < 9)
			{
				initRecursion();
				fillAllBlocks(blockMatrixCopy, blockFlagMatrix, i, j, n);

				if (areMatchingBlocksLeft())
				{
					result = true;
					break;
				}
			}
		}
	}

	return result != 0;
}

// Checks if there are still matching blocks left
bool GameManager::areMatchingBlocksLeft()
{
	int result = false;
	int counter = 0;

	// Increments counter if an erasable block can be found
	for (int i = 0; i < columnAmount; i++)
	{
		for (int j = 0; j < rowAmount; j++)
		{
			if (blockFlagMatrix[j][i] == 1)
			{
				counter++;
			}
		}
	}

	// if counter is bigger than one we still have matching blocks left
	if (counter > 1)
	{
		result = true;
	}

	return result != 0;
}

// Checks if there are no block left
bool GameManager::areAllBlocksDeleted()
{
	int result = false;

	for (int i = 0; i < columnAmount; i++)
	{
		for (int j = 0; j < rowAmount; j++)
		{
			int n = blockMatrixContent[j][i];
			if (n > 0 && n < 9)
			{
				result = 1;
				break;
			}
		}
	}

	return result != 0;
}

// Gets how many block types are used
int GameManager::getColorMode()
{
	return colorMode;
}

// Sets how many block types are used
void GameManager::setColorMode(int mode)
{
	colorMode = mode;
}

// Update is called every frame
void GameManager::update()
{
	if (!areAllBlocksDeleted())
	{
		soundManager->playMusic(0);
		soundManager->playSound(3);
		setGameState(10);
		return;
	}

	if (!gameShouldNotBeOver())
	{
		soundManager->playMusic(0);
		soundManager->playSound(2);
		setGameState(11);
		return;
	}

	// Get mouse coordinates
	mousePosHor = float(Mouse::Pos().x);
	mousePosVert = float(Mouse::Pos().y);

	// Convert mouse coordinates to matrix position
	int column = int((mousePosHor + blockSize) / blockSize);
	int row = int(mousePosVert / blockSize);
	int blockContent = blockMatrixContent[row][column];

	// Return for blocks which are not play colors
	if (blockContent <= 0 && blockContent >= 9) { return; }

	initRecursion();
	fillAllBlocks(blockMatrixCopy, blockFlagMatrix, column, row, blockContent);

	deletionCheck();
	deleteBlock();
}

// Check if needs to be deleted or not
void GameManager::deletionCheck() 
{
	int counter;
	counter = 0;
	for (int i = 0; i < columnAmount; i++)
	{
		for (int j = 0; j < rowAmount; j++)
		{
			if(blockFlagMatrix[j][i] == 1) { counter++; }
		}
	}

	if (counter == 1)
	{
		for (int i = 0; i < columnAmount; i++)
		{
			for (int j = 0; j < rowAmount; j++)
			{
				blockFlagMatrix[j][i] = 0;
			}
		}
	}
}

// Delete blocks
void GameManager::deleteBlock()
{
	// Only handle if mouse clicked and not in bottom bar area
	if (Input::MouseL.clicked && mousePosVert < (768 - blockSize))
	{
		for (int i = 0; i < columnAmount; i++)
		{
			for (int j = 0; j < rowAmount; j++)
			{
				if (blockFlagMatrix[j][i] == 1)
				{
					setGameState(3);
					blockMatrixContent[j][i] = 0;
					blockFlagMatrix[j][i] = 0;

					// SE: clear blocks
					soundManager->playSound(0);
				}
			}
		}
	}
}

void GameManager::fillAllBlocks(
	int matrixCopy[rowAmount][columnAmount],
	int flagMatrix[rowAmount][columnAmount],
	int column, int row, int newContent
)
{
	if (column < 0 || column > (columnAmount - 1) || row < 0 || row > (rowAmount - 1))
	{
		return;
	}
	else
	{
		matrixCopy[row][column] = -8;

		// Left
		if ((column - 1) >= 0 && matrixCopy[row][column - 1] == newContent)
		{
			fillAllBlocks(matrixCopy, flagMatrix, column - 1, row, newContent); 
		}

		// Right
		if (matrixCopy[row][column + 1] == newContent)
		{
			fillAllBlocks(matrixCopy, flagMatrix, column + 1, row, newContent);
		}

		// Up
		if (matrixCopy[row - 1][column] == newContent)
		{
			fillAllBlocks(matrixCopy, flagMatrix, column, row - 1, newContent);
		}

		// Down
		if (matrixCopy[row + 1][column] == newContent)
		{
			fillAllBlocks(matrixCopy, flagMatrix, column, row + 1, newContent);
		}

		// Line up
		flagMatrix[row][column] = 1;
	}
}

// Initializer for recursive function
void GameManager::initRecursion()
{
	for (int i = 0; i < columnAmount; i++)
	{
		for (int j = 0; j < rowAmount; j++)
		{
			blockMatrixCopy[j][i] = blockMatrixContent[j][i];
			blockFlagMatrix[j][i] = 0;
		}
	}
}

// Draw blocks
void GameManager::draw()
{
	for (int i = 0; i < columnAmount; i++)
	{
		for (int j = 0; j < rowAmount; j++)
		{
			int n = blockMatrixContent[j][i];
			int al = 255;

			// Lessen alpha on blocks
			if (blockFlagMatrix[j][i] == 1)
			{
				al = 50;
			}
			
			// Apply alpha
			if (n > 0)
			{
				img[n].resize(blockSize, blockSize)
					.draw(horizontalBlockPosition + i * blockSize, verticalBlockPosition + j * blockSize, Alpha(al));
			}
		}
	}
}