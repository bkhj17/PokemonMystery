#include "Framework.h"
#include "Stair.h"

Stair::Stair(Data data, Vector2 size)
	: Tile(data, size)
{
	tag = "Stair";
}
