#pragma once
#include<iostream>
#include "SDL_mixer.h"
#include "SDL.h"

class Bgm
{
public:
	Bgm();
	~Bgm();
	static void playMusic(int id);
	static void stopMusic();
	static void pauseMusic();
	static void resumeMusic();

	void pauseChunk();
	void playChunk();
	void createChunk();

private:
	Mix_Chunk* sound2 = Mix_LoadWAV("resources/shoot.wav");
};

