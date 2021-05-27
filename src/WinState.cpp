#include "WinState.h"
#include <iostream>
#include <fstream>
#include "Bgm.h"

using namespace std;

void WinState::Handle(Context* c)
{
	cout << "Win" << endl;
}

WinState::WinState(Scyzc3Engine* pEngine)
{
	m_engine = pEngine;

	image = ImageManager::loadImage("images/remind2.png", true);

}

void WinState::virtSetupBackgroundBuffer()
{
	Bgm::playMusic(2);
	if (m_engine->getWinId() == 1)
		username = " Player1";
	else username = " Player2";
}

void WinState::virtDrawStringsOnTop()
{
    image.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 500, 200,
	image.getWidth(), image.getHeight());

	m_engine->drawForegroundString(520, 310, username.data(), 0x000000, NULL);
	m_engine->redrawDisplay();
}

void WinState::virtKeyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_ESCAPE) {
		m_engine->setAllObjectsVisible(false);
		m_engine->StateSwitch(0);
	}
	else if ((iKeyCode >= 'A' && iKeyCode <= 'z') && username.size() <= 8)
	{
		username.push_back(iKeyCode);
	}
	else if (iKeyCode == SDLK_BACKSPACE)
	{
		if (username.size() > 1)
		{
			username.erase(username.end() - 1);
		}
	}
	else if (iKeyCode == SDLK_RETURN || iKeyCode == SDLK_KP_ENTER) {
		if (username != " ")
		{
			m_engine->destroyOldObjects(true);
			writeScore();
			m_engine->StateSwitch(7);
		}
	}
	m_engine->redrawDisplay();
}

void WinState::writeScore()
{
	ofstream file;
	file.open("resources/score.txt", ios::app);
	
	file << username << " " << m_engine->getWinScore() << "\n";

	file.close();
}

void WinState::copyAllBackgroundBuffer()
{
	m_engine->copyBackgroundPixels(0, 0, 1300, 800);
}