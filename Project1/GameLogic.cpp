#pragma once
#include "GameLogic.h"
#include <iostream>

GameLogic::GameLogic()
{
}

GameLogic::~GameLogic()
{
}

void GameLogic::run()
{
}

void GameLogic::update(double dt) {
	std::cout << "[GameLogic][update] dt is " << dt << std::endl;
}
