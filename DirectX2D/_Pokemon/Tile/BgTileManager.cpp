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
    tileGrid[DOWN | RIGHT | RIGHT_DOWN] = { 0, 0 };
    tileGrid[DOWN | RIGHT | RIGHT_DOWN | LEFT | LEFT_DOWN] = { 1, 0 };
    tileGrid[DOWN | LEFT | LEFT_DOWN] = { 2, 0 };
    
    tileGrid[UP | RIGHT_UP | RIGHT | DOWN | RIGHT_DOWN] = { 0, 1 };
    tileGrid[ALL] = { 1, 1 };
    tileGrid[LEFT_UP | UP | LEFT | LEFT_DOWN | DOWN] = { 2, 1 };

    tileGrid[UP | RIGHT_UP | RIGHT] = { 0, 2 };
    tileGrid[LEFT_UP | UP | RIGHT_UP | LEFT | RIGHT] = { 1, 2 };
    tileGrid[LEFT_UP | UP | LEFT] = { 2, 2 };
    
    tileGrid[RIGHT | DOWN] = { 0, 3 };
    tileGrid[LEFT | RIGHT] = { 1, 3 };
    tileGrid[LEFT | DOWN] = { 2, 3 };

    tileGrid[UP | DOWN] = { 0, 4 };
    tileGrid[NONE] = { 1, 4 };

    tileGrid[RIGHT | UP] = { 0, 5 };
    tileGrid[LEFT | UP] = { 2, 5 };

    tileGrid[DOWN] = { 1, 6 };

    tileGrid[RIGHT] = { 0, 7 };
    tileGrid[RIGHT | LEFT | UP | DOWN] = { 1, 7 };
    tileGrid[LEFT] = { 2, 7 };

    tileGrid[UP] = { 1, 8 };

    tileGrid[LEFT | DOWN | RIGHT] = { 1, 9 };

    tileGrid[UP | RIGHT | DOWN] = { 0, 10 };
    tileGrid[UP | LEFT | DOWN] = { 2, 10 };

    tileGrid[LEFT | UP | RIGHT] = { 1, 11 };

    tileGrid[ALL & ~(LEFT_DOWN | RIGHT_DOWN)] = { 1, 12 };

    tileGrid[ALL & ~(RIGHT_UP | RIGHT_DOWN)] = { 0, 13 };
    tileGrid[ALL & ~(LEFT_UP | LEFT_DOWN)] = { 2, 13 };

    tileGrid[ALL & ~(LEFT_UP | RIGHT_UP)] = { 1, 14 };

    tileGrid[ALL & ~RIGHT_DOWN] = { 0, 15 };
    tileGrid[ALL & ~LEFT_DOWN] = { 1, 15 };

    tileGrid[ALL & ~RIGHT_UP] = { 0, 16 };
    tileGrid[ALL & ~LEFT_UP] = { 1, 16 };

    tileGrid[UP | RIGHT_UP | RIGHT | DOWN] = { 0, 17 };
    tileGrid[UP | LEFT_UP | LEFT | DOWN] = { 1, 17 };

    tileGrid[UP | RIGHT_DOWN | RIGHT | DOWN] = { 0, 18 };
    tileGrid[UP | LEFT_DOWN | LEFT | DOWN] = { 1, 18 };
    
    tileGrid[LEFT | RIGHT | LEFT_DOWN | DOWN] = { 0, 19 };
    tileGrid[LEFT | RIGHT | DOWN | RIGHT_DOWN] = { 1, 19 };

    tileGrid[UP | LEFT_UP | RIGHT | LEFT] = { 0, 20 };
    tileGrid[RIGHT_UP | UP | RIGHT | LEFT] = { 1, 20 };

    tileGrid[ALL & ~(LEFT_UP | RIGHT_UP | LEFT_DOWN)] = { 0, 21 };
    tileGrid[ALL & ~(LEFT_UP | RIGHT_UP | RIGHT_DOWN)] = { 1, 21 };

    tileGrid[ALL & ~(LEFT_UP | LEFT_DOWN | RIGHT_DOWN)] = { 0, 22 };
    tileGrid[ALL & ~(RIGHT_UP | LEFT_DOWN | RIGHT_DOWN)] = { 1, 22 };

    tileGrid[ALL & ~(LEFT_UP | RIGHT_DOWN)] = { 0, 23 };
    tileGrid[ALL & ~(RIGHT_UP | LEFT_DOWN)] = { 1, 23 };
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
    vcheck.reserve(12);
    //5
    vcheck.push_back({ DOWN | RIGHT | RIGHT_DOWN | LEFT | LEFT_DOWN, UP });
    vcheck.push_back({ DOWN | RIGHT | RIGHT_DOWN | RIGHT_UP | UP, LEFT });
    vcheck.push_back({ DOWN | UP | LEFT_UP | LEFT | LEFT_DOWN, RIGHT });
    vcheck.push_back({ UP | RIGHT | RIGHT_UP | LEFT | LEFT_UP, DOWN });
    //4
    vcheck.push_back({ UP | LEFT | RIGHT | LEFT_UP, DOWN | RIGHT_UP });
    vcheck.push_back({ UP | LEFT | RIGHT | RIGHT_UP, DOWN | LEFT_UP });
    vcheck.push_back({ DOWN | LEFT | RIGHT | LEFT_DOWN, DOWN | RIGHT_DOWN });
    vcheck.push_back({ DOWN | LEFT | RIGHT | RIGHT_DOWN, DOWN | LEFT_DOWN });

    vcheck.push_back({ UP | DOWN | RIGHT | RIGHT_UP, LEFT | RIGHT_DOWN });
    vcheck.push_back({ UP | DOWN | LEFT | LEFT_UP, RIGHT |LEFT_DOWN });
    vcheck.push_back({ UP | DOWN | LEFT | LEFT_DOWN, RIGHT | LEFT_UP});
    vcheck.push_back({ UP | DOWN | RIGHT | RIGHT_DOWN, LEFT | RIGHT_UP });
    //3
    vcheck.push_back({ DOWN | RIGHT | RIGHT_DOWN, UP | LEFT });
    vcheck.push_back({ DOWN | LEFT | LEFT_DOWN, UP | RIGHT });
    vcheck.push_back({ UP | RIGHT | RIGHT_UP , LEFT | DOWN });
    vcheck.push_back({ UP | LEFT | LEFT_UP, DOWN | RIGHT });
    //2
    vcheck.push_back({ UP | DOWN, LEFT | RIGHT });
    vcheck.push_back({ LEFT | RIGHT,  UP | DOWN});
    vcheck.push_back({ DOWN, UP | RIGHT | LEFT });
    vcheck.push_back({ LEFT, DOWN | RIGHT | UP });
    vcheck.push_back({ RIGHT, DOWN | UP | LEFT });
    vcheck.push_back({ UP, DOWN | RIGHT | LEFT });
}

void BgTileManager::InitRoomCheck()
{

    //방 판별기
    roomCheck.reserve(4);
    roomCheck.push_back({ LEFT | UP | LEFT_UP, 0 });
    roomCheck.push_back({ LEFT | DOWN | LEFT_DOWN, 0 });
    roomCheck.push_back({ RIGHT | UP | RIGHT_UP, 0 });
    roomCheck.push_back({ RIGHT | DOWN | RIGHT_DOWN, 0 });
}

BgTileManager::TileType BgTileManager::GetTileType(Texture* texture)
{
    if (texture == wallTexture)
        return WALL;
    if (texture == waterTexture)
        return WATER;
    if (texture == landTexture)
        return LAND;

    return WALL;
}

bool BgTileManager::CheckMovable(int tileFlag, int dirX, int dirY)
{
    GridFlag dirFlag = NONE;
    if (dirX == -1) {
        switch (dirY) {
        case -1: dirFlag = LEFT_DOWN; break;
        case 0: dirFlag = LEFT; break;
        case 1: dirFlag = LEFT_UP; break;
        }
    }
    if (dirX == 0) {
        switch (dirY) {
        case -1: dirFlag = DOWN; break;
        case 0: dirFlag = NONE; break;
        case 1: dirFlag = UP; break;
        }
    }
    if (dirX == 1) {
        switch (dirY) {
        case -1: dirFlag = RIGHT_DOWN; break;
        case 0: dirFlag = RIGHT; break;
        case 1: dirFlag = RIGHT_UP; break;
        }
    }

    if (dirFlag == NONE)
        return false;

    switch (dirFlag)
    {
    case BgTileManager::LEFT_UP:    return (tileFlag & (LEFT_UP | LEFT | UP)) == (LEFT_UP | LEFT | UP);
    case BgTileManager::UP:         return (tileFlag & UP) == UP;
    case BgTileManager::RIGHT_UP:   return (tileFlag & (RIGHT_UP | RIGHT | UP)) == (RIGHT_UP | RIGHT | UP);
    case BgTileManager::LEFT:       return (tileFlag & LEFT) == LEFT;
    case BgTileManager::RIGHT:      return (tileFlag & RIGHT) == RIGHT;
    case BgTileManager::LEFT_DOWN:  return (tileFlag & (LEFT_DOWN | LEFT | DOWN)) == (LEFT_DOWN | LEFT | DOWN);
    case BgTileManager::DOWN:       return (tileFlag & DOWN) == DOWN;
    case BgTileManager::RIGHT_DOWN: return (tileFlag & (RIGHT_DOWN | RIGHT | DOWN)) == (RIGHT_DOWN | RIGHT | DOWN);
    default: return false;
    }
}
