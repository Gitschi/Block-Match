# include "SoundManager.h"

SoundManager::SoundManager()
{
	// Stores music in an array
	music[0] = Sound(L"Assets/BGM/nosound.wav");
	music[1] = Sound(L"Assets/BGM/BgmLoop.wav");
	music[2] = Sound(L"Assets/BGM/BgmLoopAlt.wav");

	// Stores sfx in array
	soundEffects[0] = Sound(L"Assets/SE/BlockHit.mp3");
	soundEffects[1] = Sound(L"Assets/SE/BlockHitAlt.mp3");
	soundEffects[2] = Sound(L"Assets/SE/GameOver.mp3");
	soundEffects[3] = Sound(L"Assets/SE/StageClear.mp3");
	soundEffects[4] = Sound(L"Assets/SE/StepNormal.mp3");
	soundEffects[5] = Sound(L"Assets/SE/StepStone.mp3");
	soundEffects[6] = Sound(L"Assets/SE/Wind.mp3");

	// Set bgm on startup
	currentBGM = -1;
}

// Play background music
void SoundManager::playMusic(int choice)
{
	float Vol[5] = {0.5, 0.5, 0.5, 0.5, 0.5};

	// Returns if current bgm is same as requested
	if (currentBGM == choice)
	{
		return;
	}

	if (currentBGM != -1)
	{
		music[currentBGM].stop();
		music[currentBGM].setVolume(0);
	}

	music[choice].setVolume(Vol[choice]);
	music[choice].setLoop(true);
	music[choice].play();
	currentBGM = choice;
}

// Plays sound effect
void SoundManager::playSound(int choice)
{
	float Vol[10] = {0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f};

	// Check if effect is already playing
	if (soundEffects[choice].isPlaying())
	{
		soundEffects[choice].stop();
		soundEffects[choice].setVolume(0);
	}

	soundEffects[choice].setVolume(Vol[choice]);
	soundEffects[choice].playMulti();
}
