#include "Framework.h"
#include "StatusUI.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitManager.h"
#include "../Tile/DungeonTileMap.h"
#include "../Data/DungeonDataManager.h"
#include "HPProgressBar.h"

StatusUI::StatusUI()
{
	wstring numTextureFile = L"Textures/pokemon/UI/TopUINum.png";
	
	qB = new Quad(L"Textures/pokemon/UI/B.png");
	qB->SetParent(this);
	qB->Pos().x = -CENTER_X + WIN_WIDTH * 0.02f;

	floorFont = new ImageFont(numTextureFile, 0.0f);
	floorFont->SetParent(this);
	floorFont->SetAligned(ImageFont::CENTER);
	
	qF = new Quad(L"Textures/pokemon/UI/F.png");
	qF->SetParent(this);

	qLv = new Quad(L"Textures/pokemon/UI/Lv.png");
	qLv->SetParent(this);
	qLv->Pos().x = -CENTER_X + WIN_WIDTH * 0.15f;

	levelFont = new ImageFont(numTextureFile, 0.0f);
	levelFont->SetParent(this);
	levelFont->SetAligned(ImageFont::CENTER);

	qHP = new Quad(L"Textures/pokemon/UI/HP.png");
	qHP->SetParent(this);
	qHP->Pos().x = -CENTER_X + WIN_WIDTH * 0.28f;

	curHpFont = new ImageFont(numTextureFile, 0.0f);
	curHpFont->SetParent(this);
	curHpFont->SetAligned(ImageFont::RIGHT);
	curHpFont->Pos().x = -CENTER_X + WIN_WIDTH * 0.38f;

	qSlash = new Quad(L"Textures/pokemon/UI/Slash.png");
	qSlash->SetParent(this);

	maxHpFont = new ImageFont(numTextureFile, 0.0f);
	maxHpFont->SetParent(this);
	maxHpFont->SetAligned(ImageFont::LEFT);
	maxHpFont->Pos().x = -CENTER_X + WIN_WIDTH * 0.41f;

	progressBar = new HPProgressBar(L"Textures/pokemon/UI/HpHarFront.png", L"Textures/pokemon/UI/HpHarBack.png");
	progressBar->SetSize(Vector2(CENTER_X, 40.0f));
	progressBar->Pos().x = progressBar->Half().x;
	progressBar->SetParent(this);

	Observer::Get()->AddEvent("UpdateStatusUI", bind(&StatusUI::UpdateInfo, this));
}

StatusUI::~StatusUI()
{
	SAFE_DELETE(progressBar);
	SAFE_DELETE(curHpFont);
	SAFE_DELETE(maxHpFont);
	SAFE_DELETE(floorFont);
	SAFE_DELETE(levelFont);
	SAFE_DELETE(qB);
	SAFE_DELETE(qF);
	SAFE_DELETE(qLv);
	SAFE_DELETE(qHP);
	SAFE_DELETE(qSlash);
}

void StatusUI::UpdateInfo()
{
	UpdateFloorInfo();
	UpdatePlayerInfo();
}

void StatusUI::PostRender()
{
	progressBar->PostRender();
	floorFont->PostRender();
	levelFont->PostRender();
	curHpFont->PostRender();
	maxHpFont->PostRender();

	if(floor < 0)
		qB->PostRender();
	qF->PostRender();
	qLv->PostRender();
	qHP->PostRender();
	qSlash->PostRender();
}

void StatusUI::UpdateWorld()
{
	__super::UpdateWorld();

	qB->UpdateWorld();
	floorFont->Pos().x = qB->Pos().x + qB->Half().x + floorFont->GetSize().x * 0.5f;
	floorFont->Update();
	
	qF->Pos().x = floorFont->Pos().x + floorFont->GetSize().x * 0.5f + qF->Half().x;
	qF->UpdateWorld();

	qLv->UpdateWorld();
	levelFont->Pos().x = 10.0f + qLv->Pos().x + qLv->Half().x + levelFont->GetSize().x * 0.5f;
	levelFont->Update();

	qHP->UpdateWorld();
	curHpFont->Update();
	maxHpFont->Update();

	qSlash->Pos().x = (curHpFont->Pos().x + curHpFont->GetSize().x * 0.5f + maxHpFont->Pos().x) * 0.5f;
	qSlash->UpdateWorld();

	progressBar->UpdateWorld();
}

void StatusUI::UpdateFloorInfo()
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (tileMap == nullptr)
		return;

	floor = tileMap->GetFloorData()->floorNum;
	floorFont->SetValue(abs(floor));

	UpdateWorld();
}

void StatusUI::UpdatePlayerInfo()
{
	Unit* player = UnitManager::Get()->GetPlayer();
	if (player == nullptr)
		return;

	auto playerData = player->GetData();
	if (playerData == nullptr)
		return;

	playerLevel = playerData->level;
	levelFont->SetValue(playerLevel);

	playerCurHP = playerData->curHp;
	curHpFont->SetValue(playerCurHP);
	playerMaxHP = playerData->statusData.maxHp;
	maxHpFont->SetValue(playerMaxHP);

	float renderAmount = min(1.0f, (float)playerMaxHP / 200.0f);
	renderAmount = max(renderAmount, 0.2f);
	
	progressBar->SetRenderAmount(renderAmount);
	progressBar->SetAmount((float)playerCurHP / playerMaxHP);

	UpdateWorld();
}
