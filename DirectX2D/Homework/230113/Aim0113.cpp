#include "Framework.h"
#include "Aim0113.h"

Aim0113::Aim0113()
    : Quad(L"Textures/Shooting/cursor.png")
{
}

void Aim0113::Update()
{
    UpdateWorld();
}

bool Aim0113::IsCollision(Quad* quad)
{
    if (!quad->Active())
        return false;

    float dist = (Pos() - quad->Pos()).Length();
    return (dist < Half().x + max(quad->Half().x, quad->Half().y));
}
