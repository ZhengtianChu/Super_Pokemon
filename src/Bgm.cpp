#include "Bgm.h"
#include "SDL_mixer.h"
#include "SDL.h"

Bgm::Bgm()
{
	sound2 = Mix_LoadWAV("resources/shoot.wav");
}

Bgm::~Bgm()
{
	Mix_FreeChunk(sound2);
}

void Bgm::playMusic(int id)
{
	Mix_Music* sound = NULL;
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	if (id == 1)
	{
		sound = Mix_LoadMUS("resources/bgm1.wav");
		Mix_PlayMusic(sound, 1);
	}
		
	else if (id == 2)
	{
		sound = Mix_LoadMUS("resources/victory.wav");
		Mix_PlayMusic(sound, 1);
	}
	
	else if (id == 3)
	{
		sound = Mix_LoadMUS("resources/minigame.wav");
		Mix_PlayMusic(sound, 1);
	}
	//SDL_Delay(10000);
}

void Bgm::stopMusic()
{
	Mix_CloseAudio();
	Mix_Quit();
}

void Bgm::pauseMusic()
{
	Mix_Pause(-1);
	Mix_PauseMusic();
}

void Bgm::pauseChunk()
{
	//Mix_FreeChunk(sound2);
	sound2 = NULL;
}

void Bgm::resumeMusic()
{
	Mix_Resume(-1);
	Mix_ResumeMusic();
}

void Bgm::createChunk()
{
	sound2 = Mix_LoadWAV("resources/shoot.wav");
}

void Bgm::playChunk()
{
	Mix_PlayChannel(-1, sound2, 0);
}