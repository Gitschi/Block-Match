#pragma once
#include <iostream>
#include <Siv3D.hpp>
#include "GameManager.h"
#include "SoundManager.h"
using namespace std;

// Handles dropping and sliding of blocks
class BlockManager
{
private:
	GameManager* gameManager;
	SoundManager* soundManager;

	// Size of each block in pixels
	const int blockSize = 32;

	// Number of columns (Add one to compensate for wall)
	const static int columnAmount = 40 + 1;

	// Number of rows
	const static int rowAmount = 24;

	// Maximum length of arrays
	const static int MaxLength = (columnAmount - 1)*(rowAmount - 1);

	// Time of a single frame 16.6ms
	const float FrameTime = 1.0f / 60.0f;

	// Value of pi
	const float pi = 3.141592f;

	// Amount of speed the blocks will fall and slide at
	const float blockSpeed = 500;

	// Font to display text
	Font displayText;

	// Array for block images in non-hovered state
	Texture img[10];

	// Initialization of block values
	float	x[MaxLength] = { 0 };
	float	y[MaxLength] = { 0 };
	float	horSpeed[MaxLength] = { 0 };
	float	vertSpeed[MaxLength] = { 0 };
	float	angle[MaxLength] = { 0 };
	int		flag[MaxLength] = { 0 };
	int		charNum[MaxLength] = { 0 };

	// Horizontal start and end value
	int xStart, xEnd;

public:
	BlockManager(SoundManager* _soundManager, GameManager* _gameManager);

	void draw();
	void update();
	void updateSlide();
	void requestFullBlockSet();
	void checkBlockVerticalDrop();
	void executeBlockVerticalDrop(int column, int row);
	void requestBlockHorizontalSlide();
	void prepareSpriteDataSlide();
	void executeSpriteDataSlide(int row, int column, int blockContent);
	void rewriteMatrix();
	bool areBlocksFalling();
};