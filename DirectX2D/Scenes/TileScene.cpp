#include "Framework.h"
#include "TileScene.h"

TileScene::TileScene()
{
	CreateSample();
	editTileMap = new EditTileMap(10, 10);
	editTileMap->Pos() = { 80, 80 };

	selectSample = new Quad(Vector2(SAMPLE_SIZE, SAMPLE_SIZE));
}

TileScene::~TileScene()
{
	for (auto button : sampleBtns)
		delete button;

	delete editTileMap;
	delete selectSample;
}

void TileScene::Update()
{
	if (KEY_PRESS(VK_LBUTTON)) {
		if (selectSample->GetTexture())
			editTileMap->ClickTile(selectSample, Tile::BG);
	}
	if (KEY_PRESS(VK_RBUTTON)) {
		if (selectSample->GetTexture())
			editTileMap->ClickTile(selectSample, Tile::OBJ);
	}

	if (KEY_PRESS(VK_SPACE)) {
		selectSample->SetTexture(nullptr);
	}

	if (KEY_DOWN(VK_F1))
	{
		editTileMap->Save("Textures/Tile/Tile.map");
	}

	if (KEY_DOWN(VK_F2))
	{
		editTileMap->Load("Textures/Tile/Tile.map");
	}



	for (auto button : sampleBtns)
		button->Update();


	editTileMap->Update();

	if (selectSample->GetTexture()) {
		if (mouseWheel > 0.0f) {
			selectSample->Rot().z += XM_PIDIV2;

			if (selectSample->Rot().z > XM_2PI)
				selectSample->Rot().z = 0.0;
		}
		else if (mouseWheel < 0.0f) {
			selectSample->Rot().z -= XM_PIDIV2;

			if (selectSample->Rot().z < 0.0f)
				selectSample->Rot().z = XM_2PI;
		}

		selectSample->Pos() = mousePos;
		selectSample->UpdateWorld();
	}


}

void TileScene::Render()
{
	for (auto button : sampleBtns)
		button->Render();

	editTileMap->Render();

	if (selectSample->GetTexture()) {
		selectSample->Render();
	}
}

void TileScene::PostRender()
{
}

void TileScene::ClickSampleBtn(void* sampleBtn)
{
	Button* button = (Button*)sampleBtn;

	if(button)
		selectSample->SetTexture(button->GetTexture());
}

void TileScene::CreateSample()
{
	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(L"Textures/Tile/*png", &findData);

	bool result = true;
	wstring fileName;

	UINT width = 10;
	Vector2 size(SAMPLE_SIZE, SAMPLE_SIZE);
	Vector2 startPos(WIN_WIDTH - size.x, WIN_HEIGHT - size.y);

	while (result) {
		fileName = L"Textures/Tile/";
		fileName += findData.cFileName;
		result = FindNextFile(handle, &findData);

		UINT index = sampleBtns.size();
		Vector2 pos(index % width, index / width);

		Button* button = new Button(size);
		button->SetTexture(Texture::Add(fileName));
		button->Pos() = startPos - (pos * size);
		button->SetParamEvent(bind(&TileScene::ClickSampleBtn, this, placeholders::_1));
		button->SetObject(button);

		sampleBtns.push_back(button);
	}
}
