#include "HelpState.h"
#include "Scyzc3Engine.h"
#include <ostream>
using namespace std;

void HelpState::Handle(Context* c)
{
	cout << "HelpState Starts!" << endl;
}

HelpState::HelpState(Scyzc3Engine* pEngine):
m_engine(pEngine)
{
	image = ImageManager::loadImage("images/keyTips.png", true);

	if (!m_engine->isPaused()) m_engine->pause();
}

HelpState::~HelpState()
{
	cout << "delete HelpState" << endl;
}

void HelpState::virtSetupBackgroundBuffer()
{
	m_engine->fillBackground(0xFFFFFF);
	image.renderImage(m_engine->getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());
}

void HelpState::virtMouseDown(int iButton, int iX, int iY)
{

	if (iButton == SDL_BUTTON_LEFT)
	{
		if (iX > 1100 && iX < 1200 && iY > 700 && iY < 760) {
			m_engine->StateSwitch(1);
		}
	}

}

int HelpState::virtInitialiseObjects()
{

	return 0;
}

void HelpState::virtDrawStringsOnTop()
{
	m_engine->drawForegroundString(1100, 700, "Return", 0xffffff, NULL);	
}

void HelpState::virtKeyDown(int ikeyDown) {
	if (ikeyDown == SDLK_ESCAPE) {
		m_engine->StateSwitch(1);
	}
}


void HelpState::copyAllBackgroundBuffer()
{
	m_engine->copyBackgroundPixels(0, 0, 1300, 800);
}