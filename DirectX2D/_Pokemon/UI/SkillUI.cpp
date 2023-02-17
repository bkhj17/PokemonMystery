#include "Framework.h"
#include "SkillUI.h"
#include "PokemonUIManager.h"
#include "../Data/SkillDataManager.h"
#include "../Skill/Skill.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitManager.h"

SkillUI::SkillUI()
	: UIWindow({ WIN_WIDTH * 0.6f, WIN_HEIGHT * 0.6f }, {WIN_WIDTH * 0.4f, WIN_HEIGHT * 0.6f})
{
	UpdateWorld();
	maxCursor = CLOSE_CURSOR+1;
}

void SkillUI::Update()
{
	if (!isActive)
		return;


	__super::Update();
	auto player = UnitManager::Get()->GetPlayer();
	vector<Skill*> playerSkills = player->GetSkills();

	//현재 커서에 해당하는 스킬이 유효하지 않다면 다음으로 넘긴다
	if (KEY_DOWN(VK_DOWN)) {
		while (cursor + 1 < playerSkills.size()
			&& (!playerSkills[cursor + 1] || !playerSkills[cursor + 1]->GetData())) {
			cursor++;
		}
	}
	if (KEY_DOWN(VK_UP)) {
		while (cursor + 1 < playerSkills.size()
			&& (!playerSkills[cursor + 1] || !playerSkills[cursor + 1]->GetData())) {
			
			if(--cursor < 0)
				cursor = CLOSE_CURSOR;
		}
	}
	//커서 위치 세팅
	cQuad->Pos() = { CursorX(), LineY(cursor) };
	cQuad->UpdateWorld();

	if (KEY_DOWN('Z')) {
		if (cursor == CLOSE_CURSOR) {
			//
			Close();
			return;
		}
		else {
			if (player->UseSkill(cursor + 1)) {
				Observer::Get()->ExecuteEvent("PlayerCommand");
				Close();
			}
		}
	}

}

void SkillUI::PostRender()
{
	__super::PostRender();
	RenderCursor();

	auto player = UnitManager::Get()->GetPlayer();
	vector<Skill*> playerSkills = player->GetSkills();

	//0은 기본공격이기 때문에 출력하지 않는다
	for (int i = 1; i < playerSkills.size(); i++) {
		if (playerSkills[i] != nullptr && playerSkills[i]->GetData() != nullptr) {
			Font::Get()->RenderTextLeft(playerSkills[i]->GetData()->name, Pos() + Vector2(NameX(), LineY(i - 1)));
			wstring ppStr = to_wstring(playerSkills[i]->GetCurPP()) + L" / " + to_wstring(playerSkills[i]->GetData()->maxPP);
			Font::Get()->RenderText(ppStr, Pos() + Vector2(PpX(), LineY(i - 1)));
		}
	}

	Font::Get()->RenderTextLeft(L"뒤로", Pos() + Vector2(-Half().x + 80.0f + cQuad->GetSize().x, LineY(4)));
}

float SkillUI::CursorX()
{
	return -Half().x + 80.0f;
}

float SkillUI::NameX()
{
	return -Half().x + 80.0f + cQuad->GetSize().x;
}

float SkillUI::PpX()
{
	return -Half().x + 600.0f + cQuad->GetSize().x;
}

float SkillUI::LineY(int lineNum)
{
	return Half().y - 60.0f - lineNum * (cQuad->GetSize().y + 20.0f);
}

