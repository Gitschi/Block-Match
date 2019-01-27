#pragma once
#include "SoundManager.h"
#include <Siv3D.hpp>

// Display blocks
class GameManager
{
private:
	SoundManager* soundManager;

	// Size of each block in pixels
	const int blockSize = 32;

	// Number of columns (Add one to compensate for wall)
	const static int columnAmount = 40 + 1;

	// Number of rows
	const static int rowAmount = 24;

	// GameState; TODO: Replace with enum for clarity
	// 0 = Falling
	// 1 = Falling over, still sliding
	// 2 = Recursion
	// 3 = Slide
	int gameState;

	// Amount of block types in current game mode
	int colorMode;

	// Array of all block images in hovered state
	Texture img[10];

	// Position of selected block
	float horizontalBlockPosition;
	float verticalBlockPosition;

	// Holds mouse position
	float mousePosHor, mousePosVert;

	// Initialization of matrixes
	int blockFlagMatrix[rowAmount][columnAmount] = {{ 0 }};
	int blockMatrixCopy[rowAmount][columnAmount] = {{ 0 }};
	int blockMatrixContent[rowAmount][columnAmount] = {{ 0 }};
	
public:
	GameManager(SoundManager* _soundManager);

	int getGameState();
	void setGameState(int newGameState);
	int getBlockContent(int column, int row);
	void setBlockContent(int column, int row, int newContent);
	bool areMatchingBlocksLeft();
	void init();
	void draw();
	void update();
	void fillAllBlocks(int matrixCopy[rowAmount][columnAmount], int flagMatrix[rowAmount][columnAmount], int row, int column, int newContent);
	void initRecursion();
	void deletionCheck();
	void deleteBlock();
	bool areAllBlocksDeleted();
	bool gameShouldNotBeOver();
	int getColorMode();
	void setColorMode(int mode);
};