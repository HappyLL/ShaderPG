#include <iostream>
#include "GEngine.h"

int main() {
	GEngine::getInstance()->run();
	GEngine::getInstance()->destroy();
	return 0;
}