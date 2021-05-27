#ifndef _STATE_H_
#define _STATE_H_

#include "Scyzc3Engine.h"

class Context;
class State
{
public:
	virtual void virtSetupBackgroundBuffer() = 0;
	virtual void virtMouseDown(int iButton, int iX, int iY) = 0;
	virtual int virtInitialiseObjects() = 0;
	virtual void virtDrawStringsOnTop() = 0;
	virtual void Handle(Context* c) = 0;
	virtual void virtKeyDown(int ikeyDown) = 0;
	virtual void virtMainLoopDoBeforeUpdate() {};
	virtual void copyAllBackgroundBuffer() {};
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) {};
	virtual void virtMouseMoved(int iX, int iY) {};
};

class Context {
private:
	State* state;
public:
	Context(State* s) { state = s; }
	void Request() {state->Handle(this);}
	void SetState(State* s) { state = s; }
};


#endif