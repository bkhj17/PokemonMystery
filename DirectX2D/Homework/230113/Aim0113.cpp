#include "Framework.h"
#include "Aim0113.h"

Aim0113::Aim0113()
    : Quad(L"Textures/Shooting/cursor.png")
{
}

void Aim0113::Update()
{
    __super::Update();
}

bool Aim0113::IsCollision(Quad* quad)
{
    if (!quad->IsActive())
        return false;

    float dist = (Pos() - quad->Pos()).Length();
    return (dist < Half().x + max(quad->Half().x, quad->Half().y));
}
