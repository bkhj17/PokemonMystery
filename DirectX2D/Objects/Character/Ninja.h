#pragma once

class Clip;

class Ninja : public Transform
{
private:
	enum ActionType {
		IDLE, ATTACK
	};

public:
	Ninja();
	~Ninja();

	void Update();
	void Render();

private:
	void LoadClips();
	void LoadClip(ActionType type, string path, string file, bool isLoop, float speed);

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	
	MatrixBuffer* worldBuffer;

	map<ActionType, Clip*> clips;
	Clip* curClip = nullptr;
};

