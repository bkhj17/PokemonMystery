#include "Framework.h"
#include "Robot0119.h"

Robot0119::Robot0119()
	: Robot0118()
{
	SetPixelShader(L"Filter.hlsl");
	
	floatValueBuffer = new FloatValueBuffer;

	collider = new RectCollider({ 60.0f, 80.0f });
	collider->SetParent(this);
	collider->UpdateWorld();

	actions[curAction]->Start();
}

Robot0119::~Robot0119()
{
	delete floatValueBuffer;
	delete collider;
}

void Robot0119::Update()
{
	if (!isActive)
		return;

	SetAction(selected ? RUN : IDLE);
	
	actions[curAction]->Update();
	floatValueBuffer->Get()[0] = GetSize().x;
	floatValueBuffer->Get()[1] = GetSize().y;

	UpdateWorld();
	collider->UpdateWorld();
}

void Robot0119::Render()
{
	if (!isActive)
		return;

	floatValueBuffer->SetPS(2);

	__super::Render();
}

Vector2 Robot0119::GetSize()
{
	return actions[curAction]->GetCurFrameSize();
}
