#include "Framework.h"
#include "PlayerController.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitManager.h"
#include "../Tile/DungeonTileMap.h"

PlayerController::PlayerController()
{
	tag = "Player";
	controllerType = ControllerType::PLAYER;
}

PlayerController::PlayerController(Unit* unit)
	: Controller(unit)
{
	tag = "Player";
	controllerType = ControllerType::PLAYER;
}

bool PlayerController::SetCommand()
{
	if (unit->IsActing())
		return false;

	int dirX = 0, dirY = 0;
	if (KEY_PRESS(VK_UP)) ++dirY;
	if (KEY_PRESS(VK_DOWN)) --dirY;
	if (KEY_PRESS(VK_LEFT)) --dirX;
	if (KEY_PRESS(VK_RIGHT)) ++dirX;

	if (dirX != 0 || dirY != 0) {
		unit->SetDir(dirX, dirY);
		if (!KEY_PRESS(VK_LSHIFT)) {
			POINT curPoint = unit->GetPoint();
			if (!UnitManager::Get()->CheckMovablePoint(curPoint, dirX, dirY))
				return false;

			unit->SetMovePlan(dirX, dirY, 1);
			if(unit->IsMoving())
				return true;
		}
	}

	bool attackSelect = KEY_DOWN('Z') && KEY_PRESS(VK_LSHIFT);
	if (attackSelect) {
		//스킬 4번 사용
		return true;
	}


	return false;
}