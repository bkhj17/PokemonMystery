#include "Framework.h"
#include "Controller.h"
#include "../Unit/Unit.h"

Controller::~Controller()
{
	unit->SetController(nullptr);
}