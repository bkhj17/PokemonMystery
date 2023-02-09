#include "Framework.h"
#include "TileScene.h"

TileScene::TileScene()
{
	sampleStartUV = { 0,0 };
	sampleEndUV = { 1, 1 };

	CreateSample(L"Textures/pokemon/ObjectTile/");
	editTileMap = new EditTileMap(34, 33);
	editTileMap->Pos() = { 30, 30 };

	float x = 0.33f;
	float y = 1.0f / 24.0f;

	sampleStartUV = { x, y * 4.0f };
	sampleEndUV = { x * 2.0f, y * 5.0f };
	CreateSample(L"Textures/pokemon/TinyWood/");

	editTileMap->SetBGUV(sampleStartUV, sampleEndUV);
	selectSample = new Quad(Vector2(SAMPLE_SIZE, SAMPLE_SIZE));
	selectSample->ModifyUV(sampleStartUV, sampleEndUV);
	
	char path[128];
	GetCurrentDirectoryA(sizeof(path), path);
	projectPath = path;
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
	//ImGui 창에 마우스가 안 올라갔다
	if (!ImGui::GetIO().WantCaptureMouse) {
		if (KEY_PRESS(VK_LBUTTON)) {
			bool sampleClicked = false;
			for (auto sampleBtn : sampleBtns)
				sampleClicked |= (sampleBtn->GetCollider()->IsPointCollision(mousePos));
			
			if (!sampleClicked && selectSample->GetTexture())
				editTileMap->ClickTile(selectSample, selectType);
		}
		if (KEY_DOWN(VK_RBUTTON)) {
			editTileMap->DeleteObjTile();
		}
	}

	if (KEY_PRESS(VK_SPACE))
		selectSample->SetTexture(nullptr);
	

	//9 * 8 카메라

	if (KEY_DOWN(VK_F1))
		editTileMap->Save("Textures/Tile/Tile.map");
	if (KEY_DOWN(VK_F2))
		editTileMap->Load("Textures/Tile/Tile.map");

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

	editTileMap->Render();

}

void TileScene::PostRender()
{
	for (auto button : sampleBtns)
		button->PostRender();

	if (selectSample->GetTexture())
		selectSample->PostRender();

	const char* list[] = { "BG", "OBJ" };
	ImGui::ListBox("Type", (int*)&selectType, list, 2);

	Save();
	Load();
}

void TileScene::ClickSampleBtn(void* sampleBtn)
{
	Button* button = (Button*)sampleBtn;

	if (button) {
		selectSample->SetTexture(button->GetTexture());
		if (selectType == Tile::BG) {
			selectSample->ModifyUV(sampleStartUV, sampleEndUV);
		}
		else {
			selectSample->ModifyUV(Vector2(), Vector2(1.0f, 1.0f));
		}

	}
}

void TileScene::CreateSample(wstring path)
{
	WIN32_FIND_DATA findData;

	wstring fileSet = path + +L"*png";
	HANDLE handle = FindFirstFile(fileSet.c_str(), &findData);

	bool result = true;
	wstring fileName;

	UINT width = 10;
	Vector2 size(SAMPLE_SIZE, SAMPLE_SIZE);
	Vector2 startPos(WIN_WIDTH - size.x, WIN_HEIGHT - size.y);

	while (result) {
		fileName = path;
		fileName += findData.cFileName;
		result = FindNextFile(handle, &findData);

		UINT index = sampleBtns.size();
		Vector2 pos(index % width, index / width);

		Button* button = new Button(size);
		button->SetTexture(Texture::Add(fileName));
		button->ModifyUV(sampleStartUV, sampleEndUV);
		button->Pos() = startPos - (pos * size);
		button->SetParamEvent(bind(&TileScene::ClickSampleBtn, this, placeholders::_1));
		button->SetObject(button);

		sampleBtns.push_back(button);
	}
}

void TileScene::Save()
{
	if (ImGui::Button("MapSave"))
		DIALOG->OpenDialog("Save", "SaveFile", ".map", ".");

	if (DIALOG->Display("Save")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());

			editTileMap->Save(file);
		}

		DIALOG->Close();
	}
}

void TileScene::Load()
{
	if (ImGui::Button("MapLoad"))
		DIALOG->OpenDialog("Load", "LoadFile", ".map", ".");

	if (DIALOG->Display("Load")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());

			editTileMap->Load(file);
		}

		DIALOG->Close();
	}

	editTileMap->SetBGUV(sampleStartUV, sampleEndUV);
}
