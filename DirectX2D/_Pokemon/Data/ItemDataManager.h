#pragma once

struct ItemData {
    string key;
    string name;
    wstring textureFile;
    string info;
    vector<string> useEvents;
    vector<string> useParams;

    void Use(class Unit*);
};

class ItemDataManager :
    public Singleton<ItemDataManager>
{
private:
    friend class Singleton;
    ItemDataManager();
    ~ItemDataManager();

    void Load(string fileName);
    void Heal(void* ptr);

public:
    ItemData* GetData(string key) {
        if (datas.find(key) == datas.end())
            return nullptr;

        return &datas[key];
    }
private:
    unordered_map<string, ItemData> datas;
};

