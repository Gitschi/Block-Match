#include "BlockManager.h"

BlockManager::BlockManager(SoundManager* _soundManager, GameManager* _gameManager)
{
	gameManager = _gameManager;
	soundManager = _soundManager;
	displayText = Font(12);

	// Set textures for block images standard state
	img[0] = Texture(L"Assets/Images/01.png");
	img[1] = Texture(L"Assets/Images/01.png");
	img[2] = Texture(L"Assets/Images/02.png");
	img[3] = Texture(L"Assets/Images/03.png");
	img[4] = Texture(L"Assets/Images/04.png");
	img[5] = Texture(L"Assets/Images/05.png");
	img[6] = Texture(L"Assets/Images/06.png");
	img[7] = Texture(L"Assets/Images/07.png");
	img[8] = Texture(L"Assets/Images/08.png");
}

// Request a full set of blocks
void BlockManager::requestFullBlockSet()
{
	// Prepare random number generator
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> ui(1, gameManager->getColorMode());

	// SE: Drop all blocks
	soundManager->playSound(6);

	// Increments through blocks
	for (int i = 0; i < MaxLength; ++i) {
		x[i] = float(blockSize * (i % (columnAmount - 1)));
		y[i] = float(-blockSize * (i / (columnAmount - 1)));
		horSpeed[i] = blockSpeed;
		vertSpeed[i] = blockSpeed;
		angle[i] = 90; // Drop Angle
		flag[i] = 1;
		charNum[i] = ui(gen); // Decide block type randomly
	}
}

// Drop individual blocks
void BlockManager::checkBlockVerticalDrop() 
{
	// Check for open spot upwards
	for (int i = 1; i < columnAmount; ++i)
	{
		for (int j = rowAmount - 1; j >= 0; --j)
		{
			if (gameManager->getBlockContent(i, j) == 0)
			{
				executeBlockVerticalDrop(i, j);
			}
		}
	}

	// Set game state
	gameManager->setGameState(4);
}

// Drop airborne blocks vertically
void BlockManager::executeBlockVerticalDrop(int column, int row)
{
	// Check upwards from clicked point
	for (int j = row; j >= 0; --j)
	{
		if (gameManager->getBlockContent(column, j) != 0)
		{
			for (int i = 0; i < MaxLength; ++i)
			{
				if (flag[i] == 0)
				{
					x[i] = float(column * blockSize - blockSize);
					y[i] = float(j * blockSize);
					horSpeed[i] = blockSpeed;
					vertSpeed[i] = blockSpeed;
					angle[i] = 90;
					flag[i] = 1;
					charNum[i] = gameManager->getBlockContent(column,j);

					gameManager->setBlockContent(column, j, 0);
					break;
				}
			}
		}
	}
}

// Request blocks to slide sideways
void BlockManager::requestBlockHorizontalSlide()
{
	xStart = 99;
	xEnd = 99;
	int counter;

	// Check from the left which column has an open spot
	for (int i = 1; i < columnAmount; i++)
	{
		counter = 0;
		
		// Vertical direction
		for (int j = 0; j < (rowAmount - 1); j++)
		{
			if (gameManager->getBlockContent(i, j) == 0)
			{
				counter++;
			}
		}
		if (counter == (rowAmount - 1))
		{
			xStart = i;
			break; 
		}
	}

	// Check from the right for a block that should slide
	for (int i = (columnAmount - 1); i >= 0; i--)
	{
		if (xEnd != 99) { break; }

		// Vertical Direction
		for (int j = 0; j < (rowAmount - 1); j++)
		{
			if (gameManager->getBlockContent(i, j) != 0)
			{
				xEnd = i;
				break;
			}
		}
	}

	// Slide block into an open slot if available
	if (xStart < xEnd)
	{
		prepareSpriteDataSlide();
		gameManager->setGameState(6);
	}
	// Change game state otherwise
	else
	{
		gameManager->setGameState(1);
	}
}

// Check for sprite data which needs to be sled to the left
void BlockManager::prepareSpriteDataSlide() 
{
	for (int i = xStart + 1; i <= xEnd; i++)
	{
		for (int j = 0; j < (rowAmount - 1); j++)
		{
			if (gameManager->getBlockContent(i, j) != 0)
			{
				executeSpriteDataSlide(j, i, gameManager->getBlockContent(i, j));
				gameManager->setBlockContent(i, j, 0);
			}
		}
	}
}

// Execute sliding sprite data to the left
void BlockManager::executeSpriteDataSlide(int row, int column, int blockContent)
{
	// Look for all empty sprite slots
	for (int i = 0; i < MaxLength; i++) 
	{
		// Only execute on empty slots
		if (flag[i] == 0)
		{
			flag[i] = 1;
			x[i] = float((column * blockSize) - blockSize); // Start x position
			y[i] = float(row * blockSize); // Start y position
			horSpeed[i] = blockSpeed/2; // x Speed
			vertSpeed[i] = blockSpeed/2; // y Speed
			angle[i] = 180; // Angle
			charNum[i] = blockContent; // New character number
			break; // Break out after finding one match
		}
	}
}

// Return weather or not blocks are falling
bool BlockManager::areBlocksFalling()
{
	bool result = false;

	for (int i = 0; i < MaxLength; ++i)
	{
		if (flag[i] == 1)
		{
			result = true;
			break;
		}
	}

	return result;
}

// Rewrite all blocks
void BlockManager::rewriteMatrix()
{
	for (int i = 0; i < MaxLength; i++)
	{
		if (flag[i] == 1)
		{
			flag[i] = 0;
			int Mx = ((int)x[i] + blockSize / 2) / blockSize + 1;
			int My = ((int)y[i] + blockSize / 2) / blockSize;
			gameManager->setBlockContent(Mx, My, charNum[i]); // Rewrite matrix data
		}
	}
}

// Update is run on every single frame
void BlockManager::update()
{
	checkBlockVerticalDrop();

	for (int i = 0; i < MaxLength; ++i)
	{
		if (flag[i] != 0)
		{
			x[i] += float(FrameTime * horSpeed[i] * cos(angle[i] * pi / 180.0));
			y[i] += float(FrameTime * vertSpeed[i] * sin(angle[i] * pi / 180.0));

			// Ignore blocks which are outside of the screen
			if (y[i] > 0)
			{
				// Convert mouse coordinate position to matrix value
				int column = int((x[i] + blockSize / 2 + blockSize) / blockSize);
				int row = int((y[i] + blockSize) / blockSize);

				int blockContent = gameManager->getBlockContent(column, row);

				// Rewrite matrix data
				if (blockContent != 0)
				{
					flag[i] = 0;
					gameManager->setBlockContent(column, row - 1, charNum[i]);
				}
			}
		}
	}
}

// Calculate horizontal sliding
void BlockManager::updateSlide()
{
	int column = 0, row = 0, blockContent = 0;
	for (int i = 0; i < MaxLength; ++i)
	{
		if (flag[i] != 0)
		{
			x[i] += float(FrameTime * horSpeed[i] * cos(angle[i] * pi / 180.0));
			y[i] += float(FrameTime * vertSpeed[i] * sin(angle[i] * pi / 180.0));

			// Ignore blocks outside of the screen
			if (angle[i] == 180)
			{
				column = int((x[i] / blockSize) + 1);
				row = int((y[i] + blockSize / 2) / blockSize);
				blockContent = gameManager->getBlockContent(column, row);

				// Rewrite affected blocks
				if (blockContent != 0)
				{
					rewriteMatrix(); 
				}
			}
		}
	}
}

// Draw Sprites to the screen
void BlockManager::draw()
{
	for (int i = 0; i < MaxLength; ++i)
	{
		if (flag[i] != 0)
		{
			img[charNum[i]].resize(blockSize, blockSize).draw(x[i], y[i]);
		}
	}
}
