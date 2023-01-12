#include "Framework.h"
#include "Transform0112.h"

Transform0112::Transform0112()
{
	worldBuffer = new MatrixBuffer();
}

Transform0112::~Transform0112()
{
	delete worldBuffer;
}

Matrix Transform0112::GetWorldMatrix()
{
	Matrix S = XMMatrixScaling(scale.x, scale.y, 1.0f);
	Matrix R = XMMatrixRotationZ(angle);
	Matrix T = XMMatrixTranslation(pos.x, pos.y, 0.0f);

	return S * R * T;
}

void Transform0112::Set()
{
	worldBuffer->Set(GetWorldMatrix());
	worldBuffer->SetVS(0);
}
