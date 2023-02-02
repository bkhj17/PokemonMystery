#pragma once

class DungeonTileMap : public GameTileMap
{
public:
    DungeonTileMap(string file);
    ~DungeonTileMap();
private:

    void SetGrid(int x, int y);

    void Load(string file) override;
};

