#include "Framework.h"
#include "TextureScene.h"

TextureScene::TextureScene()
{
	plane = new Plane();
	plane->Pos() = { CENTER_X, CENTER_Y };
}

TextureScene::~TextureScene()
{
	delete plane;
}

void TextureScene::Update()
{
	plane->Update();
}

void TextureScene::Render()
{
	plane->Render();
}

void TextureScene::PostRender()
{
	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	ImGui::Text(fps.c_str());

	plane->RenderUI();
}
