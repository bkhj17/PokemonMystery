#pragma once

class ProgressBar0120 : public Quad
{

public:
	ProgressBar0120(wstring textureFile);
	~ProgressBar0120();

	void Render();

	void Init(float maxValue);

	float& GetCurValue() { return curValue; }
private:
	float maxValue = 0.0f, invMaxValue = 1.0f;
	float curValue = 0.0f;

	FloatValueBuffer* floatValueBuffer;
};

