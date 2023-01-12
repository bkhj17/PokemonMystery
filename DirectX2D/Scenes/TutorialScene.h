#pragma once
class TutorialScene : public Scene
{
public:
	TutorialScene();
	~TutorialScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	Rect* rect;
	MatrixBuffer* worldBuffer;
	MatrixBuffer* projectionBuffer;

	XMFLOAT4X4 worldMatrix;

	Vector2 pos;	
	Vector2 scale;		//배율
	float angle = 0.0f;
};