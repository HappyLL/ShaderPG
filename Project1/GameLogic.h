#pragma once
#include "GRef.h"
class GameLogic : public GRef{
public:
	GameLogic();
	~GameLogic();
public:
	void run();
	void update(double dt);
private:

};