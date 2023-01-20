#pragma once
struct Vertex
{
    Float3 pos;
    Float2 uv;

    Vertex() : pos({0.0f, 0.0f, 0.0f}), uv({0.0f, 0.0f}) {}
    Vertex(float x, float y, float u, float v) 
        : pos({ x, y, 0.0f }), uv({u, v}) {}
};

struct VertexColor
{
    Float3 pos;
    Float4 color;

    VertexColor() 
        : pos({ 0.0f, 0.0f, 0.0f })
        , color(1.0f, 1.0f, 1.0f, 1.0f) {}
    VertexColor(float x, float y, float r, float g, float b) 
        : pos({ x, y, 0.0f }) 
        , color(r, g, b, 1.0f) {}
};

struct VertexPos //정점 : 3차원 공간에서의 한 점
{
    Float3 pos;

    VertexPos() : pos({ 0.0f, 0.0f, 0.0f }) {}
    VertexPos(float x, float y) : pos({ x, y, 0.0f }) {}
};
