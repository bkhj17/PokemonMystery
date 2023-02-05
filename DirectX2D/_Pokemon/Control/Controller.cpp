#include "Framework.h"
#include "Controller.h"
#include "../Unit/Unit.h"

Controller::Controller()
{
}

Controller::Controller(Unit* unit)
	: unit(unit)
{
}

Controller::~Controller()
{
}

void Controller::SetCommand()
{
	if (unit->IsActing())
		return;

	int dirX = 0, dirY = 0;
	if (KEY_PRESS(VK_UP)) ++dirY;
	if (KEY_PRESS(VK_DOWN)) --dirY;
	if (KEY_PRESS(VK_LEFT)) --dirX;
	if (KEY_PRESS(VK_RIGHT)) ++dirX;


	if (dirX != 0 || dirY != 0) {
		unit->SetDir(dirX, dirY);
		if(!KEY_PRESS(VK_LSHIFT))
			unit->SetMove(dirX, dirY);
	}
}