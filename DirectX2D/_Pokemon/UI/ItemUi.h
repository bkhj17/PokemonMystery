#pragma once
class ItemUi : public UIWindow
{
private:

	const int CLOSE_CURSOR = 10;

public:
	ItemUi();
	~ItemUi() = default;

	void Update() override;
	void PostRender() override;

private:
	float CursorX();
	float NameX();

	float LineY(int lineNum);
};

