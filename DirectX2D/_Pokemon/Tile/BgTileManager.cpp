#include "Framework.h"
#include "BgTileManager.h"

BgTileManager::BgTileManager()
{
    InitGridInfo();
    InitVCheck();
    InitRoomCheck();
}

void BgTileManager::InitGridInfo()
{
    int none        = 0;
    int all         = 0xFF;
    
    int leftUp      = 0x01;
    int up          = leftUp << 1;
    int rightUp     = leftUp << 2;
    int left        = leftUp << 3;
    int right       = leftUp << 4;
    int leftDown    = leftUp << 5;
    int down        = leftUp << 6;
    int rightDown   = leftUp << 7;
    
    tileGrid[down | right | rightDown] = { 0, 0 };
    tileGrid[down | right | rightDown | left | leftDown] = { 1, 0 };
    tileGrid[down | left | leftDown] = { 2, 0 };
    
    tileGrid[up | rightUp | right | down | rightDown] = { 0, 1 };
    tileGrid[all] = { 1, 1 };
    tileGrid[leftUp | up | left | leftDown | down] = { 2, 1 };

    tileGrid[up | rightUp | right] = { 0, 2 };
    tileGrid[leftUp | up | rightUp | left | right ] = { 1, 2 };
    tileGrid[leftUp | up | left ] = { 2, 2 };
    
    tileGrid[right | down] = { 0, 3 };
    tileGrid[left | right] = { 1, 3 };
    tileGrid[left | down] = { 2, 3 };

    tileGrid[up | down] = { 0, 4 };
    tileGrid[none] = { 1, 4 };

    tileGrid[right | up] = { 0, 5 };
    tileGrid[left | up] = { 2, 5 };

    tileGrid[down] = { 1, 6 };

    tileGrid[right] = { 0, 7 };
    tileGrid[right | left | up | down] = { 1, 7 };
    tileGrid[left] = { 2, 7 };

    tileGrid[up] = { 1, 8 };

    tileGrid[left | down | right] = { 1, 9 };

    tileGrid[up | right | down] = { 0, 10 };
    tileGrid[up | left | down] = { 2, 10 };

    tileGrid[left | up | right] = { 1, 11 };

    tileGrid[all & ~(leftDown | rightDown)] = { 1, 12 };

    tileGrid[all & ~rightUp & ~rightDown] = { 0, 13 };
    tileGrid[all & ~leftUp & ~leftDown] = { 2, 13 };

    tileGrid[all & ~leftUp & ~rightUp] = { 1, 14 };

    tileGrid[all & ~rightDown] = { 0, 15 };
    tileGrid[all & ~leftDown] = { 1, 15 };

    tileGrid[all & ~rightUp] = { 0, 16 };
    tileGrid[all & ~leftUp] = { 1, 16 };

    tileGrid[up | rightUp | right | down] = { 0, 17 };
    tileGrid[up | leftUp | left | down] = { 1, 17 };

    tileGrid[up | rightDown | right | down] = { 0, 18 };
    tileGrid[up | leftDown | left | down] = { 1, 18 };
    
    tileGrid[left | right | leftDown | down] = { 0, 19 };
    tileGrid[left | right | down | rightDown] = { 1, 19 };

    tileGrid[rightUp | up | right | left] = { 0, 20 };
    tileGrid[up | leftUp | right | left] = { 1, 20 };

    tileGrid[all & ~(leftUp | rightUp | leftDown)] = { 0, 21 };
    tileGrid[all & ~(leftUp | rightUp | rightDown)] = { 1, 21 };

    tileGrid[all & ~(leftUp | leftDown | rightDown)] = { 0, 22 };
    tileGrid[all & ~(rightUp | leftDown | rightDown)] = { 1, 22 };

    tileGrid[all & ~(leftUp | rightDown)] = { 0, 23 };
    tileGrid[all & ~(rightUp | leftDown)] = { 1, 23 };
}

void BgTileManager::SetTexture(wstring path)
{
    wallTexture = Texture::Add(path + L"Wall.png");
    landTexture = Texture::Add(path + L"Land.png");
    waterTexture = Texture::Add(path + L"Water.png");
}

POINT BgTileManager::GetGrid(int flag)
{
    if(tileGrid.find(flag) != tileGrid.end())
        return tileGrid[flag];
    

    for (auto& c : vcheck) {
        if (CheckGrid(flag, c.first, c.second))
            return tileGrid[c.first];
    }

    return tileGrid[0]; //디폴트
}

bool BgTileManager::IsRoom(int flag)
{
    for (auto& r : roomCheck) {
        if (CheckGrid(flag, r.first, r.second))
            return true;
    }
    return false;
}

bool BgTileManager::CheckGrid(const int& flag, int check, int uncheck)
{
    return ((flag & check) == check && (flag & ~uncheck));
}

void BgTileManager::InitVCheck()
{
    //판별
    int all = 0xFF;

    int leftUp = 0x01;
    int up = leftUp << 1;
    int rightUp = leftUp << 2;
    int left = leftUp << 3;
    int right = leftUp << 4;
    int leftDown = leftUp << 5;
    int down = leftUp << 6;
    int rightDown = leftUp << 7;

    vcheck.reserve(12);
    vcheck.push_back({ down | right | rightDown | left | leftDown, up });
    vcheck.push_back({ down | right | rightDown | rightUp | up, left });
    vcheck.push_back({ down | up | leftUp | left | leftDown, right });
    vcheck.push_back({ up | right | rightUp | left | leftUp, down });
    vcheck.push_back({ down | right | rightDown, up | left });
    vcheck.push_back({ down | left | leftDown, up | right });
    vcheck.push_back({ up | right | rightUp , left | down });
    vcheck.push_back({ up | left | leftUp, down | right });
    vcheck.push_back({ down, up | right | left });
    vcheck.push_back({ left, down | right | up });
    vcheck.push_back({ right, down | up | left });
    vcheck.push_back({ up, down | right | left });
}

void BgTileManager::InitRoomCheck()
{

    //방 판별기
    int all = 0xFF;

    int leftUp = 0x01;
    int up = leftUp << 1;
    int rightUp = leftUp << 2;
    int left = leftUp << 3;
    int right = leftUp << 4;
    int leftDown = leftUp << 5;
    int down = leftUp << 6;
    int rightDown = leftUp << 7;

    roomCheck.reserve(4);
    roomCheck.push_back({ left | up | leftUp, 0 });
    roomCheck.push_back({ left | down | leftDown, 0 });
    roomCheck.push_back({ right | up | rightUp, 0 });
    roomCheck.push_back({ right | down | rightDown, 0 });
}

BgTileManager::TileType BgTileManager::GetTileType(Texture* texture)
{
    if (texture == wallTexture)
        return WALL;
    if (texture == waterTexture)
        return WATER;
    if (texture == landTexture)
        return LAND;

    return NONE;
}
