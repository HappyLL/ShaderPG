#pragma once
# include "GRef.h"
#define MAXINPUTNUM 5
class InputCommand: public GRef{
public:
	InputCommand();
	virtual ~InputCommand();
public:
	virtual void execute();
};

class InputKeyCommand : public InputCommand {
public:
	InputKeyCommand();
	~InputKeyCommand();
public:
	void execute();
};

class InputHelper: public GRef{
public:
	InputHelper();
	~InputHelper();	
public:
	void pushCommand(InputCommand *cmd);
	InputCommand* popCommand();
	void clearCommands();
	bool empty();
	bool full();
	void tick();
private:
	int _front, _end;
	InputCommand ** input_queue;
};