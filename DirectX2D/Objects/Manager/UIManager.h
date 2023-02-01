#pragma once
class UIManager : public Singleton<UIManager>
{
private:
	friend class Singleton;
	UIManager();
	~UIManager();

public:
	void Update();
	void Render();

	void AddPanel(string key, wstring panelTextureFile, 
		wstring cancelBtnTextureFile = L"Textures/UI/Cancel.png");

	void Show(string key);
	void Hide(string key);

	void HideFront();
private:
	void SelectUI();

private:
	map<string, Panel*> panels;
	list<Panel*> popupList;
};

