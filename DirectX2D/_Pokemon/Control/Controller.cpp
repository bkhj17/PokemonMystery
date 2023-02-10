#include "Framework.h"
#include "Controller.h"
#include "../Unit/Unit.h"

Controller::~Controller()
{
	unit->SetController(nullptr);
}

void Controller::UseSkill(int i)
{
	//유닛이 i번 스킬을 사용
}
