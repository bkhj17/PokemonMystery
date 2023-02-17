#include "Framework.h"

string ClearData::GetDungeonName()
{
    if (dungeonData == nullptr) return "";
    return dungeonData->name;
}

string ClearData::GetPlayerName()
{
    if (playerData == nullptr) return "";
    return playerData->statusData.name;
}

int ClearData::GetPlayerLevel()
{
    if (playerData == nullptr) return 0;
    return playerData->level;
}
