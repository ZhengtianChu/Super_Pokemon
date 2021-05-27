#include "ChooseState.h"
#include "Scyzc3Engine.h"
#include <ostream>
#include "Bgm.h"
using namespace std;

void ChooseState::Handle(Context* c)
{
	cout << "ChooseState Starts!" << endl;
}

ChooseState::ChooseState(Scyzc3Engine* pEngine)
{
	m_engine = pEngine;
	image = ImageManager::loadImage("images/choose.png", true);
	mapImage = ImageManager::loadImage("images/map1.png", true);

	//if (!m_engine->isPaused()) m_engine->pause();
}

ChooseState::~ChooseState()
{
	cout << "delete choosestate" << endl;
}

void ChooseState::virtSetupBackgroundBuffer()
{
	m_engine->setLoad(0);
	m_engine->fillBackground(0xFFFFFF);

	m_engine->lockBackgroundForDrawing();
	m_engine->lockForegroundForDrawing();
	image.renderImage(m_engine->getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());


	SimpleImage volume = ImageManager::loadImage("images/volume1.png", true);
	volume.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 100, 620,
		volume.getWidth(), volume.getHeight());
	
}

void ChooseState::virtMouseDown(int iButton, int iX, int iY)
{
	printf("Mouse clicked at %d %d\n", iX, iY);

	if (iButton == SDL_BUTTON_LEFT)
	{
		if (iX > 740 && iX < 1040 && iY > 250 && iY < 330) {
			flag1++;
		}
		if (iX > 740 && iX < 1040 && iY > 400 && iY < 450) {
			flag4++;
		}
		if (iX > 100 && iX < 200 && iY > 700 && iY < 760) {
			m_engine->StateSwitch(0);
		}
		if (iX > 1200 && iX < 1300 && iY > 700 && iY < 760) {
			m_engine->StateSwitch(1);
		}
		if (iX > 720 && iX < 880 && iY > 580 && iY < 620) {
			flag2++;
		}
		if (iX > 920 && iX < 1090 && iY > 580 && iY < 620) {
			flag3++;
		}
		if (iX > 100 && iX < 200 && iY > 620 && iY < 670) {
			if (Mix_PausedMusic() == 0)
			{
				Bgm::pauseMusic();
			}
			else if (Mix_PausedMusic() == 1)
			{
				Bgm::resumeMusic();
			}
		}
	}
}

int ChooseState::virtInitialiseObjects()
{
	m_engine->drawableObjectsChanged();
	m_engine->destroyOldObjects(true);
	return 0;
}

void ChooseState::virtDrawStringsOnTop()
{
	char buf[128];
	if (flag1 % 3 == 0) {
		m_engine->drawForegroundString(750, 300, "<<   I Love CPP   >>", 0xffffff, NULL);
		mapImage = ImageManager::loadImage("images/map1.png", true);
		m_engine->setMapId(0);
	}
		
	if (flag1 % 3 == 1) {
		m_engine->drawForegroundString(750, 300, "<<      Maze      >>", 0xffffff, NULL);
		mapImage = ImageManager::loadImage("images/map2.png", true);
		m_engine->setMapId(1);
	}
	
	if (flag1 % 3 == 2) {
		m_engine->drawForegroundString(750, 300, "<<   BattleField  >>", 0xffffff, NULL);
		mapImage = ImageManager::loadImage("images/map3.png", true);
		m_engine->setMapId(2);
	}
		
	if (flag2 % 4 == 0) {
		pImage1 = ImageManager::loadImage("images/pikaquImage.png", true);
		m_engine->setName1("pikaqu");
	}

	if (flag2 % 4 == 1) {
		pImage1 = ImageManager::loadImage("images/flashbirdImage.png", true);
		m_engine->setName1("flashbird");
	}

	if (flag2 % 4 == 2) {
		pImage1 = ImageManager::loadImage("images/ElectabuzzImage.png", true);
		m_engine->setName1("Electabuzz");
	}

	if (flag2 % 4 == 3) {
		pImage1 = ImageManager::loadImage("images/flashEveeImage.png", true);
		m_engine->setName1("flashEvee");
	}

	if (flag3 % 4 == 0) {
		pImage2 = ImageManager::loadImage("images/fireHorseImage.png", true);
		m_engine->setName2("fireHorse");
	}

	if (flag3 % 4 == 1) {
		pImage2 = ImageManager::loadImage("images/CharmanderImage.png", true);
		m_engine->setName2("Charmander");
	}

	if (flag3 % 4 == 2) {
		pImage2 = ImageManager::loadImage("images/duckImage.png", true);
		m_engine->setName2 ("duck");
	}

	if (flag3 % 4 == 3) {
		pImage2 = ImageManager::loadImage("images/fireEveeImage.png", true);
		m_engine->setName2("fireEvee");
	}

	if (flag4 % 3 == 0) {
		m_engine->drawForegroundString(750, 420, "<<      easy      >>", 0xffffff, NULL);
		m_engine->setLevel(0);
	}

	if (flag4 % 3 == 1) {
		m_engine->drawForegroundString(750, 420, "<<     normal     >>", 0xffffff, NULL);
		m_engine->setLevel(1);
	}

	if (flag4 % 3 == 2) {
		m_engine->drawForegroundString(750, 420, "<<      hard      >>", 0xffffff, NULL);
		m_engine->setLevel(2);
	}

	mapImage.renderImage(m_engine->getBackgroundSurface(), 0, 0, 700, 100,
		mapImage.getWidth(), mapImage.getHeight());

	pImage1.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 750, 550,
		pImage1.getWidth(), pImage1.getHeight());

	pImage2.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 950, 550,
		pImage2.getWidth(), pImage2.getHeight());

	m_engine->drawForegroundString(100, 700, "Return", 0xffffff, NULL);
	m_engine->drawForegroundString(720, 590, "<<        >>", 0xffffff, NULL);
	m_engine->drawForegroundString(920, 590, "<<        >>", 0xffffff, NULL);
	m_engine->drawForegroundString(1200, 700, "enter", 0xffffff, NULL);
	m_engine->drawForegroundString(750, 500, "Player1", 0xffffff, NULL);
	m_engine->drawForegroundString(950, 500, "Player2", 0xffffff, NULL);
	m_engine->drawForegroundString(500, 180, "Map", 0xffffff, m_engine->getFont("resources/FFF_Tusj.ttf", 40));
	m_engine->drawForegroundString(480, 400, "Level", 0xffffff, m_engine->getFont("resources/FFF_Tusj.ttf", 40));
	m_engine->drawForegroundString(450, 555, "Character", 0xffffff, m_engine->getFont("resources/FFF_Tusj.ttf", 40));

	m_engine->redrawDisplay();
}

void ChooseState::virtKeyDown(int ikeyDown) {
	if (ikeyDown == SDLK_ESCAPE) {
		m_engine->StateSwitch(0);
	}
	else if (ikeyDown == SDLK_a || ikeyDown == SDLK_d)
	{
		flag2++;
	}
	else if (ikeyDown == SDLK_LEFT || ikeyDown == SDLK_RIGHT)
	{
		flag3++;
	}
	else if (ikeyDown == SDLK_RETURN) {
		m_engine->StateSwitch(1);
	}
}

void ChooseState::virtMainLoopDoBeforeUpdate()
{
	m_engine->lockAndSetupBackground();
	m_engine->lockBackgroundForDrawing();
	virtDrawStringsOnTop();
	m_engine->unlockBackgroundForDrawing();
}

void ChooseState::copyAllBackgroundBuffer()
{
	m_engine->copyBackgroundPixels(0, 0, 1300, 800);
}