#pragma once

class UIWindow;
class PokemonUIManager : public Singleton<PokemonUIManager>
{
private:
	friend class Singleton;
	PokemonUIManager();
	~PokemonUIManager();

public:
	void Update();
	void Render();

	void OpenMenu();
	void CloseAll();
private:
	void OpenUI(string key);
	void CloseUI();

private:
	class StatusUI* statusUI;
	UIWindow* logUI = nullptr; 

	unordered_map<string, UIWindow*> totalUI;
	list<UIWindow*> openned;
};