#include "Framework.h"
#include "TileMap0130.h"

TileMap0130::TileMap0130(string file)
	: GameTileMap(file)
{
	SetUp();

}

void TileMap0130::PushObject(Collider* collider)
{
	for (auto obj : walls) {
		if (obj->GetCollider()->PushCollider(collider))
			if (pushEvent)
				pushEvent();
	}
}

void TileMap0130::ItemCollision(Collider* collider)
{
	for (auto obj : chips) {
		if (!obj->Active())
			continue;

		if (collider->IsCollision(obj->GetCollider())) {
			obj->SetActive(false);
			CheckItem();
		}
	}

	for (auto obj : powers) {
		if (!obj->Active())
			continue;

		if (collider->IsCollision(obj->GetCollider())) {
			obj->SetActive(false);
			Observer::Get()->ExecuteEvent("GainSuper");
		}
	}
}

void TileMap0130::GetNodes(vector<Node*>& nodes)
{

	for (auto tile : bgTiles) {
		Vector2 tilePos = tile->GlobalPos();
		Node* node = new Node(tilePos, nodes.size());

		for (auto obj : walls) {
			Vector2 objPos = obj->GlobalPos();

			if (objPos == tilePos)
				node->SetState(Node::OBSTACLE);
		}

		nodes.push_back(node);
	}


}

void TileMap0130::Render()
{
	for (auto bg : bgTiles)
		bg->Render();

	for (auto obj : walls) {
		obj->Render();
		obj->GetCollider()->Render();
	}

	for (auto obj : powers) {
		if (obj->Active())
			obj->Render();
	}

	for (auto obj : chips) {
		if (obj->Active())
			obj->Render();
	}
}

void TileMap0130::UpdateWorld()
{
	__super::UpdateWorld();
	for (auto bg : bgTiles)
		bg->UpdateWorld();

	for (auto obj : objTiles) {
		obj->UpdateWorld();
		obj->GetCollider()->UpdateWorld();
	}

}

void TileMap0130::SetUp()
{
	collideTable[L"Textures/Tile/path.png"] = WALL;
	collideTable[L"Textures/Tile/path_curve.png"] = WALL;
	collideTable[L"Textures/Tile/path_crossing.png"] = WALL;

	collideTable[L"Textures/Tile/barrel.png"] = CHIP;
	collideTable[L"Textures/Tile/tree.png"] = POWER;

	for (auto bg : bgTiles)
		bg->GetColor() = { 0.01f, 0.01f, 0.01f, 1.0f };

	numItem = 0;
	for (auto obj : objTiles) {
		wstring fileName = obj->GetTexture()->GetFile();

		if (collideTable.find(fileName) == collideTable.end())
			continue;

		switch (collideTable[fileName])
		{
		case WALL:
			obj->GetColor() = { 0.0f, 0.0f, 1.0f, 1.0f };
			walls.push_back(obj);
			break;
		case POWER:
			obj->GetColor() = { 1.0f, 0.5f, 0.0f, 1.0f };
			powers.push_back(obj);
			break;
		case CHIP:
			obj->GetColor() = { 1.0f, 1.0f, 0.0f, 1.0f };
			numItem++;
			chips.push_back(obj);
			break;
		}
	}

}

void TileMap0130::CheckItem()
{
	if (--numItem <= 0 && winEvent)
		winEvent();
}