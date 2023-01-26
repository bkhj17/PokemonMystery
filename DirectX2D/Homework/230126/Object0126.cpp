#include "Framework.h"
#include "Object0126.h"

Object0126::Object0126(wstring file)
	: Quad(file)
{
	rectCollider = new RectCollider(GetSize());
	rectCollider->SetParent(this);
}

Object0126::~Object0126()
{
	delete rectCollider;
}

void Object0126::Update()
{



	UpdateWorld();
	rectCollider->UpdateWorld();
}

void Object0126::Render()
{
	__super::Render();

	rectCollider->Render();
}
