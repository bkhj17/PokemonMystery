#pragma once

class Environment :
    public Singleton<Environment>
{
private:
    friend class Singleton;
    Environment();
    ~Environment();


public:
    void CreateProjection();
    void CreateSamplerState();
    void CreateBlendState();

private:
    MatrixBuffer* projectionBuffer;

    ID3D11SamplerState* samplerState;
    ID3D11BlendState* blendState;
    
};