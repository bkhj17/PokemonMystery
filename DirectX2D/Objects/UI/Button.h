#pragma once

class Button : public Quad
{
protected:
	enum State {
		NONE, DOWN, OVER
	};

	const Float4 NONE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 1.0f };
	const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 1.0f };
public:
	Button(wstring textureFile);
	Button(Vector2 size);
	~Button();

	virtual void Update();
	virtual void UpdateWorld() override;
	virtual void Render();

	void SetEvent(Event event) { this->event = event; }
	void SetParamEvent(ParamEvent event) { 
		this->paramEvent = event; 
	}
	void SetObject(void * object) { 
		this->object = object; 
	}

	void ExecuteEvent();

	RectCollider* GetCollider() { return collider; }
protected:
	RectCollider* collider;

	State state = NONE;

	bool isDownCheck = false;

	Event event;
	ParamEvent paramEvent;
	void* object = nullptr;
};