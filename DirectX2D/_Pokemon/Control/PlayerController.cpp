#include "Framework.h"
#include "PlayerController.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitManager.h"
#include "../Tile/DungeonTileMap.h"
#include "../UI/PokemonUIManager.h"

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

	if (KEY_DOWN('A')) {
		PokemonUIManager::Get()->OpenUI("Skill");
		return false;
	}

	if (KEY_DOWN('Z')) {
		unit->UseSkill(0);
		return true;
	}

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

	return false;
}