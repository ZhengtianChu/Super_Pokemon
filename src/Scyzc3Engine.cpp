#include "header.h"
#include "Scyzc3Engine.h"
#include "Scyzc3Object.h"
#include "ImageManager.h"
#include "State.h"
#include "RunningState.h"
#include "StartUpState.h"
#include "HelpState.h"
#include "ChooseState.h"
#include "SaveState.h"
#include "LoadState.h"
#include "WinState.h"
#include "ScoreState.h"
#include "SDL_mixer.h"
using namespace std;
Scyzc3Engine::Scyzc3Engine() 
{
	this->pause();
	bgm = new Bgm();
	states[0] = new StartUpState(this);
	states[1] = new RunningState(this);
	states[2] = new HelpState(this);
	states[3] = new ChooseState(this);
	states[4] = new SaveState(this);
	states[5] = new LoadState(this);
	states[6] = new WinState(this);
	states[7] = new ScoreState(this);

	currentState = states[0];

}

Scyzc3Engine::~Scyzc3Engine()
{
	delete (StartUpState*) states[0];
	delete (RunningState*) states[1];
	delete (HelpState*) states[2];
	delete (ChooseState*)states[3];
	delete (SaveState*)states[4];
	delete (LoadState*)states[5];
	delete (WinState*)states[6];
	delete (ScoreState*)states[7];
	delete bgm;
}

void Scyzc3Engine::virtSetupBackgroundBuffer()
{
	currentState->virtSetupBackgroundBuffer();

}


void Scyzc3Engine::virtMouseDown(int iButton, int iX, int iY)
{
	currentState->virtMouseDown(iButton, iX, iY);
}


int Scyzc3Engine::virtInitialiseObjects()
{
	currentState->virtInitialiseObjects();
	return 0;
}


void Scyzc3Engine::virtDrawStringsOnTop()
{
	currentState->virtDrawStringsOnTop();
}

void Scyzc3Engine::StateSwitch(int stateID)
{
	currentState = states[stateID];
	lockAndSetupBackground();
	redrawDisplay();
}

void Scyzc3Engine::virtKeyDown(int iKeyCode)
{
	currentState->virtKeyDown(iKeyCode);
}

void Scyzc3Engine::virtMainLoopDoBeforeUpdate()
{
	currentState->virtMainLoopDoBeforeUpdate();
}

void Scyzc3Engine::copyAllBackgroundBuffer()
{
	currentState->copyAllBackgroundBuffer();
}

void Scyzc3Engine::virtMouseWheel(int x, int y, int which, int timestamp)
{
	currentState->virtMouseWheel(x, y, which, timestamp);
}

void Scyzc3Engine::virtMouseMoved( int iX, int iY)
{
	currentState->virtMouseMoved(iX, iY);
}



