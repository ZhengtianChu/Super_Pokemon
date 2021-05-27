#include "StartUpState.h"
#include "Scyzc3Engine.h"
#include "ExampleDragableObjects.h"
#include "Scyzc3Object.h"
#include <iostream>
#include <ctime>
#include "Bgm.h"

using namespace std;

void StartUpState::Handle(Context* c)
{
	cout << "Start Game!" << endl;
}

StartUpState::StartUpState(Scyzc3Engine* pEngine):
	rotator(0.0),
	shifter1(0,0),
	m_engine(pEngine),
	m_filterScaling(0, 0, m_engine),
	m_filterTranslation(1, 5, &m_filterScaling)
{
	image = ImageManager::loadImage("images/background.png", true);
	m_engine->setMapId(0);
	Bgm::playMusic(1);
}

StartUpState::~StartUpState()
{
	cout << "delete StartState" << endl;
}


void StartUpState::virtSetupBackgroundBuffer()
{

	m_engine->fillBackground(0xffffff);
	virtInitialiseObjects();
	m_engine->lockBackgroundForDrawing();

	m_engine->copyBackgroundPixels(0, 0, m_engine->getWindowWidth(), m_engine->getWindowHeight());
	image.setTransparencyColour(0xFF0000);

	image.renderImage(m_engine->getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());



	m_engine->unlockBackgroundForDrawing();
	
}

void StartUpState::virtMouseDown(int iButton, int iX, int iY)
{
	printf("Mouse clicked at %d %d\n", iX, iY);

	if (iButton == SDL_BUTTON_LEFT) {
		if (iX > 150 && iX < 250 && iY > 700 && iY < 750) {
			m_engine->setMapId(0);
			m_engine->setLoad(0);
			m_engine->setRunTime(m_engine->getModifiedTime() / 1000.0);
			m_engine->StateSwitch(1);
		}
		else if (iX > 400 && iX < 500 && iY > 700 && iY < 750) {
		
			m_engine->setAllObjectsVisible(false);
			m_engine->StateSwitch(3);
		}
		else if (iX > 680 && iX < 900 && iY > 700 && iY < 750) {
			m_engine->setAllObjectsVisible(false);
			m_engine->StateSwitch(5);
		}
		else if (iX > 1000 && iX < 1100 && iY > 700 && iY < 750) {
			m_engine->setExitWithCode(0);
		}
		else if (iX > 1200 && iX < 1250 && iY > 180 && iY < 230) {
			m_engine->setAllObjectsVisible(false);
			m_engine->StateSwitch(7);
		}

	}
}

int StartUpState::virtInitialiseObjects()
{
	if(m_engine->isPaused())	m_engine->unpause();

	m_engine->destroyOldObjects(true);

	m_engine->createObjectArray(2);

	ob = new MappableImageObject(&rotator, 1200, 180, m_engine, "images/ball.png", false);
	ob2 = new MappableImageObject(&shifter1, 300, 180, m_engine, "images/name.png", false);

	ob->setTransparencyColour(ob->getPixelColour(0, 0));
	ob2->setTransparencyColour(ob->getPixelColour(0, 0));

	m_engine->storeObjectInArray(0, ob);
	m_engine->storeObjectInArray(1, ob2);

	return 0;
}
 
void StartUpState::virtDrawStringsOnTop()
{
	auto_ptr<time_t> now(new time_t(time(0)));
	char* dt = ctime(now.get());

	m_engine->drawForegroundString(150, 700, "Start", 0, m_engine->getFont("resources/FFF_Tusj.ttf", 40));
	m_engine->drawForegroundString(400, 700, "Setting", 0, m_engine->getFont("resources/FFF_Tusj.ttf", 40));
	m_engine->drawForegroundString(680, 700, "Load Game", 0, m_engine->getFont("resources/FFF_Tusj.ttf", 40));
	m_engine->drawForegroundString(1000, 700, "Quit", 0, m_engine->getFont("resources/FFF_Tusj.ttf", 40));
	m_engine->drawForegroundString(150, 350, "SUPER POKEMON", 0, m_engine->getFont("resources/FFF_Tusj.ttf", 100));
	m_engine->drawForegroundString(1000, 750, dt, 0, m_engine->getFont("resources/FFF_Tusj.ttf", 20));
	m_engine->redrawDisplay();
}

void StartUpState::virtMainLoopDoBeforeUpdate()
{
	flag++;
	int iOffset = m_engine->getModifiedTime() / 5;
	rotator.setRotation((double)iOffset / 100.0);

	if (flag % 100 > 50)
		shifter1.setXShift((double)iOffset);
	else
		shifter1.setXShift(-(double)iOffset);

	m_iOffset = (m_iOffset + 1) % m_engine->getWindowWidth();
	virtSetupBackgroundBuffer();


	m_engine->redrawDisplay();

}


void StartUpState::copyAllBackgroundBuffer()
{
	DrawingSurface* m_pBackgroundSurface = m_engine->getBackgroundSurface();
	DrawingSurface* m_pForegroundSurface = m_engine->getForegroundSurface();

	m_pForegroundSurface->copyRectangleFrom(m_pBackgroundSurface, 0, 0, m_engine->getWindowWidth(), m_engine->getWindowHeight(), m_iOffset, 0);
	m_pForegroundSurface->copyRectangleFrom(m_pBackgroundSurface, m_engine->getWindowWidth() - m_iOffset, 0, m_engine->getWindowWidth(), m_engine->getWindowHeight(), m_iOffset - m_engine->getWindowWidth(), 0);
}

bool StartUpState::mapCoordinates(double& x, double& y, const SimpleImage& image)
{
	return true;
}




