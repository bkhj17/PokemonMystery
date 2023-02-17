#include "Framework.h"

Controller::~Controller()
{
	unit->SetController(nullptr);
}