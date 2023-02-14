#include "Framework.h"
#include "AnimObject.h"

AnimObject::AnimObject()
{
	colorBuffer = new ColorBuffer;
}

AnimObject::~AnimObject()
{
	for (auto& clip : clips)
		delete clip.second;
	delete colorBuffer;
}

void AnimObject::Update()
{
	if (curClip == nullptr)
		return;

	curClip->Update();
	UpdateWorld();
}

void AnimObject::Render()
{
	if (curClip == nullptr)
		return;

	GameObject::SetRender();
	colorBuffer->SetPS(0);

	curClip->Render();
}

void AnimObject::SetClip(int key)
{
	if (clips.find(key) == clips.end())
		return;

	if (curClip != clips[key]) {
		curClip = clips[key];
		curClip->Play();
	}
}
