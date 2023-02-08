#include "Framework.h"
#include "ItemDataManager.h"

ItemDataManager::ItemDataManager()
{
	Load("TextData/ItemData.csv");
}

ItemDataManager::~ItemDataManager()
{
}

void ItemDataManager::Load(string fileName)
{

	ifstream ifs;
	ifs.open(fileName);
	if (ifs.fail())
		return;

	bool isFirstLine = true;
	while (!ifs.eof()) {
		string tmp;
		getline(ifs, tmp);

		if (isFirstLine) {
			isFirstLine = false;
			continue;
		}

		if (tmp.empty())
			break;

		vector<string> v = SplitString(tmp, ",");

		ItemData data;
		data.key = v[0];
		data.name = v[1];

		data.textureFile = ChangeWString(v[2]);
		data.info = v[3];

		if (v.size() >= 5) {
			vector<string> vEvent = SplitString(v[4], " ");
			vector<string> vParam = SplitString(v[5], " ");
			for (int i = 0; i < vEvent.size(); i++) {
				data.useEvents.push_back(vEvent[i]);
				data.useParams.push_back(vParam[i]);
			}
		}
		datas[data.key] = data;
	}


}
