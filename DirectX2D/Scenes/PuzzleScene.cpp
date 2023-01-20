#include "Framework.h"
#include "PuzzleScene.h"

PuzzleScene::PuzzleScene()
{
	button = new Button(L"Textures/Kuuga.png");
	button->Pos() = { CENTER_X, CENTER_Y };
	button->Update();

	LoadTextures();
}

PuzzleScene::~PuzzleScene()
{
	delete button;
}

void PuzzleScene::Update()
{
	button->Update();
}

void PuzzleScene::Render()
{
	button->Render();
}

void PuzzleScene::PostRender()
{
}

void PuzzleScene::LoadTextures()
{
	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(L"Textures/Cards/*png", &findData);
	
	bool result = true;
	wstring fileName;
	while (result) {
		fileName = L"Textures/Cards/";
		fileName += findData.cFileName;

		cardTextures.push_back(fileName);

		result = FindNextFile(handle, &findData);
	}

}
