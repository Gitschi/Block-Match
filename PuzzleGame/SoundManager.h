#pragma once
# include <Siv3D.hpp>

class SoundManager
{
private:
	// Array of background music
	Sound music[3];

	//Array of sound effects
	Sound soundEffects[7];

	//Number of currently playing bgm
	int currentBGM;

public:
	SoundManager();
	void playMusic(int choice);
	void playSound(int choice);
};