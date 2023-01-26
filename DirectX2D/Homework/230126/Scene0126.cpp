#include "Framework.h"
#include "Scene0126.h"
#include "Object0126.h"

Scene0126::Scene0126()
{
	LoadMap(L"Textures/Tile/Tile.map");
	BulletManager::Get();
}

Scene0126::~Scene0126()
{
	for (auto tile : bgTiles)
		delete tile;

	for (auto obj : objects)
		delete obj;
}

void Scene0126::Update()
{
	for (auto tile : bgTiles)
		tile->UpdateWorld();
	
	Vector2 velocity = {};
	if (KEY_PRESS(VK_LEFT))	velocity.x -= 300.0f * DELTA;
	if (KEY_PRESS(VK_RIGHT)) velocity.x += 300.0f * DELTA;
	if (KEY_PRESS(VK_UP)) velocity.y += 300.0f * DELTA;
	if (KEY_PRESS(VK_DOWN)) velocity.y -= 300.0f * DELTA;

	objects[0]->Pos() += velocity;

	for (auto obj : objects) {
		if (obj == objects[0])
			continue;

		Vector2 overlap = {};
		if (objects[0]->GetCollider()->IsRectCollision(obj->GetCollider(), &overlap)) {
			//콜리전
		}
	}

	if (KEY_PRESS(VK_SPACE)) {
		BulletManager::Get()->Fire(objects[0]->GlobalPos(), Vector2(0.0f, 1.0f));
	}

	for (auto obj : objects)
		obj->Update();

	BulletManager::Get()->Update();
}

void Scene0126::Render()
{
	for (auto tile : bgTiles)
		tile->Render();
	for (auto obj : objects)
		obj->Render();

	BulletManager::Get()->Render();
}

void Scene0126::PostRender()
{
	objects[0]->RenderUI();
}

void Scene0126::LoadMap(wstring file)
{
	BinaryReader* reader = new BinaryReader(file);
	if (!reader->IsValid()) {
		wstring wstr = L"파일 열기 실패 : " + file;
		MessageBox(hWnd, wstr.c_str(), L"Error", MB_OK);
		return;
	}

	width = reader->UInt();
	height = reader->UInt();

	UINT size = reader->UInt();
	bgTiles.reserve(size);

	for (UINT i = 0; i < size; i++) {
		Tile::Data data;
		data.textureFile = reader->WString();
		data.pos.x = reader->Float();
		data.pos.y = reader->Float();
		data.pos += offset;
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		Quad* quad = new Quad(data.textureFile);
		quad->Pos() = data.pos;
		quad->Rot().z = data.angle;

		bgTiles.push_back(quad);
	}

	size = reader->UInt();
	for (int i = 0; i < size; i++) {
		//부르긴 다 불러야 함
		bool active = reader->Int();
		wstring textureFile = reader->WString();
		Vector2 pos;
		pos.x = reader->Float();
		pos.y = reader->Float();

		float angle = reader->Float();
		reader->Int();

		if (!active)
			continue;

		Object0126* obj = new Object0126(textureFile);
		obj->Pos() = pos + offset;
		obj->Rot().z = angle;
		obj->Update();
		objects.push_back(obj);
	}
}
