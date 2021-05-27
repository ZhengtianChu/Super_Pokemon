#include "ScoreState.h"
#include <iostream>
#include <fstream>
#include <unordered_map>

using namespace std;

void ScoreState::Handle(Context* c)
{
	cout << "High Score Board!" << endl;
}

ScoreState::ScoreState(Scyzc3Engine* pEngine):
m_iOffset(0),
m_engine(pEngine),
m_filterScaling(0, 0, m_engine),
m_filterTranslation(0, 0, &m_filterScaling)
{
	image = ImageManager::loadImage("images/ScoreBoardBackground.png", true);
}

ScoreState::~ScoreState()
{
	cout << "delete ScoreState" << endl;
}

void ScoreState::virtSetupBackgroundBuffer()
{
	m_engine->fillBackground(0);

	m_engine->getBackgroundSurface()->setDrawPointsFilter(&m_filterTranslation);
	m_engine->getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);

	readScore();

	copyAllBackgroundBuffer();

	m_engine->lockBackgroundForDrawing();

	m_engine->copyBackgroundPixels(0, 0, 1300, 800);

	image.renderImage(m_engine->getBackgroundSurface(), 0, 0, 0, 0, image.getWidth(), image.getHeight());

	m_engine->unlockBackgroundForDrawing();


}

void ScoreState::virtMouseDown(int iButton, int iX, int iY)
{

	int iOldCentreX = m_engine->convertClickedToVirtualPixelXPosition(m_engine->getWindowWidth() / 2);
	int iOldCentreY = m_engine->convertClickedToVirtualPixelYPosition(m_engine->getWindowHeight() / 2);

	switch (iButton)
	{
	case 1:
		break;
	case 2:
		m_filterScaling.compress();
		break;
	case 3:
		m_filterScaling.stretch();
		break;
	}

	int iNewCentreX = m_engine->convertClickedToVirtualPixelXPosition(m_engine->getWindowWidth() / 2);
	int iNewCentreY = m_engine->convertClickedToVirtualPixelYPosition(m_engine->getWindowHeight() / 2);

	m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
	m_engine->virtSetupBackgroundBuffer();

	m_engine->redrawDisplay();
}

void ScoreState::virtMouseWheel(int x, int y, int which, int timestamp)
{
	int iOldCentreY = m_engine->convertClickedToVirtualPixelYPosition(m_engine->getWindowHeight() / 2);

	if (y < 0 && iOldCentreY < 1200)
		m_filterTranslation.changeOffset(0, -50);
	else if (y > 0 && iOldCentreY > 400)
		m_filterTranslation.changeOffset(0, 50);
	m_engine->redrawDisplay();
}

void ScoreState::virtMouseMoved(int iX, int iY)
{
	int iOldCentreX = m_engine->convertClickedToVirtualPixelXPosition(m_engine->getWindowWidth() / 2);

	if (iX > iOldCentreX + 600 && iOldCentreX < 1950)
		m_filterTranslation.changeOffset(-50, 0);
	else if (iX < iOldCentreX - 600 && iOldCentreX > 650)
		m_filterTranslation.changeOffset(50, 0);

	m_engine->redrawDisplay();
}


void ScoreState::virtDrawStringsOnTop()
{
	SimpleImage board = ImageManager::loadImage("images/board.png", true);
	board.renderImage(m_engine->getForegroundSurface(), 0, 0, 300, 100, board.getWidth(), board.getHeight());

	char buf[10];
	for (int i = 0; i < 5; i++)
	{
		m_engine->drawForegroundString(480, 170 + i * 130, nameList[i].data(), 0x000000, m_engine->getFont("resources/Cornerstone Regular.ttf", 40));
		sprintf(buf, "%d", scoreList[i]);
		m_engine->drawForegroundString(760, 170 + i * 130, buf, 0x000000, m_engine->getFont("resources/Cornerstone Regular.ttf", 40));
	}

	m_engine->drawForegroundString(550, 30, "High Score", 0xFFFFFF, m_engine->getFont("resources/FFF_Tusj.ttf", 50));

	m_engine->drawForegroundString(1050, 700, "Player again? [y][n]", 0xFFFFFF, m_engine->getFont("resources/Cornerstone Regular.ttf", 20));
	m_engine->redrawDisplay();
}

void ScoreState::virtKeyDown(int iKeyCode)
{
	int iOldCentreX = m_engine->convertClickedToVirtualPixelXPosition(m_engine->getWindowWidth() / 2);
	int iOldCentreY = m_engine->convertClickedToVirtualPixelYPosition(m_engine->getWindowHeight() / 2);


	
	if (iKeyCode == SDLK_n)
		m_engine->setExitWithCode(0);
	else if (iKeyCode == SDLK_y)
	{
		m_engine->destroyOldObjects(true);
		m_engine->setRunTime(m_engine->getModifiedTime() / 1000.0);
		if (!m_engine->isPaused())	m_engine->pause();
		m_engine->StateSwitch(0);
	}
	else if (iKeyCode == SDLK_SPACE)
	{
		m_filterTranslation.setOffset(0, 0);
		m_engine->redrawDisplay();
	}
	else if (iOldCentreX >= 670 && iKeyCode == SDLK_LEFT)
	{
		m_filterTranslation.changeOffset(30, 0);
		m_engine->redrawDisplay();
	}
	else if (iOldCentreX < 1930 && iKeyCode == SDLK_RIGHT)
	{
		m_filterTranslation.changeOffset(-30, 0);
		m_engine->redrawDisplay();
	}
	else if (iOldCentreY >= 420 && iKeyCode == SDLK_UP)
	{
		m_filterTranslation.changeOffset(0, 30);
		m_engine->redrawDisplay();
	}
	else if (iOldCentreY <= 1180 && iKeyCode == SDLK_DOWN)
	{
		m_filterTranslation.changeOffset(0, -30);
		m_engine->redrawDisplay();
	}

}

template <class T>
vector<pair<T, int>> sortMap(unordered_map<T, int> dict)
{
	vector<pair<T, int>> vtMap;
	for (auto it = dict.begin(); it != dict.end(); it++)
		vtMap.push_back(make_pair(it->first, it->second));

	sort(vtMap.begin(), vtMap.end(),
		[](const pair<T, int>& x, const pair<T, int>& y) -> int {
			return x.second > y.second;
		});

	return vtMap;
}



void ScoreState::readScore()
{
	unordered_map<string, int> dict;
	string tempStr;
	int tempNum;
	ifstream file;
	file.open("resources/score.txt", ios::in);
	if (!file.is_open()) {
		cout << "File failed to open!" << endl;
		return;
	}
	while (!file.eof())
	{
		file >> tempStr >> tempNum;

		if (dict[tempStr]) {
			if (dict[tempStr] < tempNum) { dict[tempStr] = tempNum; }
		} else{ dict[tempStr] = tempNum; }
	}

	vector<pair<string, int>> vtMap;
	vtMap = sortMap(dict);
	//for (auto it = dict.begin(); it != dict.end(); it++)
	//	vtMap.push_back(make_pair(it->first, it->second));

	//sort(vtMap.begin(), vtMap.end(),
	//	[](const pair<string, int>& x, const pair<string, int>& y) -> int {
	//		return x.second > y.second;
	//	});
	

	int i = 0;

	for (auto it = vtMap.begin(); it != vtMap.end(); it++)
	{
		if (i < 5) {
			nameList[i] = it->first;
			scoreList[i] = it->second;
			i++;
		}
		else break;
	}

	vtMap.clear();
	file.close();
}

void ScoreState::virtMainLoopDoBeforeUpdate()
{
	m_iOffset = (m_iOffset + 1) % m_engine->getWindowWidth();
	virtSetupBackgroundBuffer(); 
	m_engine->redrawDisplay();

}

void ScoreState::copyAllBackgroundBuffer()
{
	DrawingSurface* m_pBackgroundSurface = m_engine->getBackgroundSurface();
	DrawingSurface* m_pForegroundSurface = m_engine->getForegroundSurface();

	m_pForegroundSurface->copyRectangleFrom(m_pBackgroundSurface, 0, 0, m_engine->getWindowWidth(), m_engine->getWindowHeight(), m_iOffset, 0 );
	m_pForegroundSurface->copyRectangleFrom(m_pBackgroundSurface, m_engine->getWindowWidth() - m_iOffset, 0 , m_engine->getWindowWidth(), m_engine->getWindowHeight(), m_iOffset - m_engine->getWindowWidth(), 0);
}

