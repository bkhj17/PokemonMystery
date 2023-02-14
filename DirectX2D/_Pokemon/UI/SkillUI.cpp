#include "Framework.h"
#include "SkillUI.h"
#include "PokemonUIManager.h"

SkillUI::SkillUI()
	: UIWindow({ WIN_WIDTH * 0.6f, WIN_HEIGHT * 0.6f }, {WIN_WIDTH * 0.4f, WIN_HEIGHT * 0.6f})
{
	UpdateWorld();

	maxCursor = 5;
}

void SkillUI::Update()
{
	__super::Update();
	if (!isActive)
		return;


	if (KEY_DOWN('Z')) {
		if (cursor == CLOSE_CURSOR) {

		}
	}
}
