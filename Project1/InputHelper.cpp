#include "InputHelper.h"
#include <iostream>

InputHelper::InputHelper()
{
	input_queue = new InputCommand*[MAXINPUTNUM];
	_front = 0;
	_end = 1;
}

InputHelper::~InputHelper()
{
	clearCommands();
	delete[]input_queue;
}

void InputHelper::pushCommand(InputCommand * cmd)
{
	if (full()) {
		std::cout << "Input queue is full" << std::endl;
		return;
	}
	input_queue[_end] = cmd;
	_end = (_end + 1) % MAXINPUTNUM;
}

InputCommand * InputHelper::popCommand()
{
	if (empty()) {
		std::cout << "Input queue is empty" << std::endl;
		return nullptr;
	}
	auto tmp = input_queue[_front];
	_front = (_front + 1) % MAXINPUTNUM;
	return tmp;
}

void InputHelper::clearCommands()
{
	while (!empty()) {
		auto cmd = popCommand();
		cmd->release();
	}
}

bool InputHelper::empty()
{
	return (_front + 1) % MAXINPUTNUM == _end;
}

bool InputHelper::full()
{
	return _front == _end;
}

void InputHelper::tick()
{
	while (!empty()) {
		auto cmd = popCommand();
		cmd->execute();
	}
	clearCommands();
}

InputCommand::InputCommand()
{
}

InputCommand::~InputCommand()
{
}

void InputCommand::execute()
{
}

InputKeyCommand::InputKeyCommand()
{
}

InputKeyCommand::~InputKeyCommand()
{
}

void InputKeyCommand::execute()
{
}
