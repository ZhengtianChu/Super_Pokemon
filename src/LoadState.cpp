#include "LoadState.h"
#include <iostream>
#include <fstream>
using namespace std;


void LoadState::Handle(Context* c)
{
	cout << "load file state" << endl;
}

LoadState::LoadState(Scyzc3Engine* pEngine)
{
	m_engine = pEngine;
	image = ImageManager::loadImage("images/saveBackground.png", true);
}

LoadState::~LoadState()
{
	cout << "delete LoadState" << endl;
}

void LoadState::virtSetupBackgroundBuffer()
{
	m_engine->fillBackground(0xFFFFFF);

	image.renderImage(m_engine->getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());

	loadInfo(0);
	loadInfo(1);
	loadInfo(2);
}

void LoadState::virtMouseDown(int iButton, int iX, int iY)
{
	printf("Mouse clicked at %d %d\n", iX, iY);

	if (iButton == SDL_BUTTON_LEFT)
	{
		if (iX > 680 && iX < 1220 && iY > 70 && iY < 250) {
			if (mapId[0] < 0)	return;
			m_engine->setMapId(mapId[0]);
			m_engine->setLoad(1);
			m_engine->StateSwitch(1);
		}
		if (iX > 680 && iX < 1220 && iY > 300 && iY < 480) {
			if (mapId[1] < 0)	return;
			m_engine->setMapId(mapId[1]);
			m_engine->setLoad(2);
			m_engine->StateSwitch(1);
		}
		if (iX > 680 && iX < 1220 && iY > 540 && iY < 710) {
			if (mapId[2] < 0)	return;
			m_engine->setMapId(mapId[2]);
			m_engine->setLoad(3);
			m_engine->StateSwitch(1);
		}
		if (iX > 100 && iX < 200 && iY > 700 && iY < 760) {
			m_engine->StateSwitch(0);
		}
	}
}

int LoadState::virtInitialiseObjects()
{
	m_engine->destroyOldObjects(true);
	return 0;
}

void LoadState::virtDrawStringsOnTop()
{
	char buf[128], buf1[128], buf4[30], buf5[50], buf6[30], buf7[50], buf8[30], buf9[30], buf10[30];

	sprintf(buf, "Map: %d", mapId[0]);
	sprintf(buf4, "Map: %d", mapId[1]);
	sprintf(buf6, "Map: %d", mapId[2]);
	sprintf(buf8, "Level: %d", level[0]);
	sprintf(buf9, "Level: %d", level[1]);
	sprintf(buf10, "Level: %d", level[2]);
	sprintf(buf1, (char*)linee1.c_str());
	sprintf(buf5, (char*)linee2.c_str());
	sprintf(buf7, (char*)linee3.c_str());
	m_engine->drawForegroundString(750, 150, buf, 0xffffff, NULL);
	m_engine->drawForegroundString(750, 100, "Archive 1", 0xff0000, m_engine->getFont("resources/FFF_Tusj.ttf", 30));
	m_engine->drawForegroundString(750, 330, "Archive 2", 0xff0000, m_engine->getFont("resources/FFF_Tusj.ttf", 30));
	m_engine->drawForegroundString(750, 560, "Archive 3", 0xff0000, m_engine->getFont("resources/FFF_Tusj.ttf", 30));
	m_engine->drawForegroundString(750, 380, buf4, 0xffffff, NULL);
	m_engine->drawForegroundString(850, 150, buf8, 0xffffff, NULL);
	m_engine->drawForegroundString(850, 380, buf9, 0xffffff, NULL);
	m_engine->drawForegroundString(850, 610, buf10, 0xffffff, NULL);
	m_engine->drawForegroundString(750, 430, buf5, 0xffffff, NULL);
	
	m_engine->drawForegroundString(750, 610, buf6, 0xffffff, NULL);
	m_engine->drawForegroundString(750, 660, buf7, 0xffffff, NULL);
	m_engine->drawForegroundString(750, 200, buf1, 0xffffff, NULL);
	m_engine->drawForegroundString(100, 700, "Return", 0xffffff, NULL);
}

void LoadState::virtKeyDown(int ikeyDown)
{
	if (ikeyDown == SDLK_ESCAPE) {
		m_engine->StateSwitch(0);
	}
}

void LoadState::saveGame(int id)
{
}

void LoadState::loadInfo(int id)
{
	ifstream infoFile;
	string line;

	string str1, str2, str3, str4, str5;

	if (id == 0)	infoFile.open("resources/Archive1.txt", ios::in);
	else if (id == 1)	infoFile.open("resources/Archive2.txt", ios::in);
	else if (id == 2)	infoFile.open("resources/Archive3.txt", ios::in);
	else return;

	if (!infoFile.is_open()) {
		cout << "info cannot open" << endl;
		return;
	}

	infoFile >> mapId[id] >> level[id];

	if (infoFile.eof())
	{
		mapId[id] = -1;
		if (id == 0)	linee1 = "No Data";
		else if (id == 1)	linee2 = "No Data";
		else if (id == 2)	linee3 = "No Data";
		cout << "Empty file" << endl;
	}
	else
	{
		infoFile >> p1X[id] >> p1Y[id] >> hp1[id] >> s1[id] >> p2X[id] >> p2Y[id] >> hp2[id] >> s2[id] >> p3X[id] >> p3Y[id] >> hp3[id] >> p4X[id] >> p4Y[id] >> hp4[id];
		infoFile >> str1 >> str2 >> str3 >> str4 >> str5;
		if (id == 0)	linee1 = str1 + " " + str2 + " " + str3 + " " + str4 + " " + str5;
		else if (id == 1)	linee2 = str1 + " " + str2 + " " + str3 + " " + str4 + " " + str5;
		else if (id == 2)	linee3 = str1 + " " + str2 + " " + str3 + " " + str4 + " " + str5;
	}

	infoFile.close();
}

void LoadState::copyAllBackgroundBuffer()
{
	m_engine->copyBackgroundPixels(0, 0, 1300, 800);
}