#include "Framework.h"
#include "Scene0120.h"
#include "CardManager0120.h"
Scene0120::Scene0120()
{
	CardManager0120::Get()->InitTable(12);
}

Scene0120::~Scene0120()
{
	CardManager0120::Delete();
}

void Scene0120::Update()
{
	CardManager0120::Get()->Update();
}

void Scene0120::Render()
{
	CardManager0120::Get()->Render();
}

void Scene0120::PostRender()
{
}
