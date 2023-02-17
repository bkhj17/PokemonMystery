#include "Framework.h"

MiniMap::MiniMap()
{
	quad = new Quad(Vector2(20.0f, 20.0f));
	gridBuffer = new IntValueBuffer;
	quad->SetPixelShader(L"MiniMapShader.hlsl");

	Observer::Get()->AddParamEvent("MoveEnd", bind(&MiniMap::UpdateMap, this));
	Observer::Get()->AddParamEvent("UnitDie", bind(&MiniMap::UpdateMap, this));
	Observer::Get()->AddEvent("ClearMiniMap", bind(&MiniMap::Clear, this));
}

MiniMap::~MiniMap()
{
	delete quad;
	delete gridBuffer;
}
void MiniMap::UpdateMap()
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (!tileMap)
		return;
	
	Unit* player = UnitManager::Get()->GetPlayer();
	if (player == nullptr)
		return;

	//�νĵ� Ÿ�� ����
	POINT curPoint = player->GetPoint();
	vector<pair<int, int>> detectPoints = tileMap->DetectableTiles(curPoint);
	for (const auto& detect : detectPoints)
		info[detect] = tileMap->GetBgTile({ detect.first, detect.second })->GetGridFlag();

	//������Ʈ ���� ����
	objInfo.clear();	//�ʱ�ȭ
	//�̹� ������ Ÿ��
	for (const auto& i : info) {
		//������Ʈ Ÿ��
		const pair<int, int>& point = i.first;
		DungeonObjTile* tile = ObjTileManager::Get()->GetTile(point.first, point.second);
		if (tile != nullptr) {
			if (tile->GetType() == "Stair")	objInfo[point] = STAIR;
			else if (tile->GetType() == "Refresh") objInfo[point] = REFRESH;
		}
		//������ ������Ʈ
		ItemObject* item = ItemObjectManager::Get()->ItemOnPoint({ point.first, point.second });
		if (item != nullptr)
			objInfo[point] = ITEM;
	}

	//�νĹ����� �ִ� ����
	for (const auto& detect : detectPoints) {
		//����
		Unit* unit = UnitManager::Get()->GetUnitOnPoint({ detect.first, detect.second });
		//���� ��ȿ���� ����
		if (unit == nullptr || !unit->Active() || unit->GetData()->curHp <= 0)
			continue;
		//���� ����
		if (unit->GetController()->GetTag() == "Player")
			objInfo[detect] = PLAYER;
		else if (unit->GetController()->GetTag() == "Enemy")
			objInfo[detect] = ENEMY;
	}
}

void MiniMap::PostRender()
{
	for (auto& i : info) {
		quad->Pos() = { i.first.first * quad->GetSize().x,  i.first.second * quad->GetSize().y };
		quad->UpdateWorld();
		gridBuffer->Get()[0] = i.second;
		
		if (objInfo.find(i.first) != objInfo.end()) {
			gridBuffer->Get()[1] = objInfo[i.first];
			switch (objInfo[i.first])
			{
			case MiniMap::PLAYER:
				quad->GetColor() = WHITE;
				break;
			case MiniMap::ENEMY:
				quad->GetColor() = RED;
				break;
			case MiniMap::STAIR:
			case MiniMap::REFRESH:
			case MiniMap::ITEM:
				quad->GetColor() = CYAN;
				break;
			}
		}
		else {
			gridBuffer->Get()[1] = NONE;
			quad->GetColor() = Float4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		gridBuffer->SetPS(5);
		quad->PostRender();
	}
}

void MiniMap::Clear()
{
	info.clear();
	objInfo.clear();
}
