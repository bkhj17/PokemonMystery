#include "Framework.h"
#include "RectCollider.h"

RectCollider::RectCollider(Vector2 size)
    : size(size)
{
    type = Type::RECT;

    Vector2 halfSize = size * 0.5f;
    vertices.emplace_back(-halfSize.x, halfSize.y);
    vertices.emplace_back(halfSize.x, halfSize.y);
    vertices.emplace_back(halfSize.x, -halfSize.y);
    vertices.emplace_back(-halfSize.x, -halfSize.y);
    vertices.emplace_back(-halfSize.x, halfSize.y);

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexPos), vertices.size());
}

bool RectCollider::IsPointCollision(Vector2 point)
{

    Matrix invWorld = XMMatrixInverse(nullptr, world);

    point *= invWorld;

    Vector2 half = size * 0.5f;

    return (abs(point.x) < half.x && abs(point.y) < half.y);
}

bool RectCollider::IsRectCollision(RectCollider* rect, Vector2* overlap)
{
    if (overlap != nullptr)
        return IsAABB(rect, overlap);
    
    return IsOBB(rect);
}

bool RectCollider::IsCircleCollision(CircleCollider* circle)
{
    auto obb = GetObb();

    auto direction = obb.position - circle->GlobalPos();
    auto d = abs(Dot(obb.axis[0], direction));

    float x = abs(Dot(direction, obb.axis[0]));
    if (x > obb.halfSize.x + circle->Radius()) return false;

    float y = abs(Dot(direction, obb.axis[1]));
    if (y > obb.halfSize.y + circle->Radius()) return false;

    if (x < obb.halfSize.x || y < obb.halfSize.y) return true;

    Vector2 temp = Vector2(x, y) - obb.halfSize;
    return (temp.Length() < circle->Radius());
}

Vector2 RectCollider::LeftTop()
{
    Vector2 edge(-size.x * 0.5f, size.y * 0.5f);

    return edge * world;
}

Vector2 RectCollider::LeftBottom()
{
    Vector2 edge(-size.x * 0.5f, -size.y * 0.5f);

    return edge * world;
}

Vector2 RectCollider::RightTop()
{
    Vector2 edge(size.x * 0.5f, size.y * 0.5f);

    return edge * world;
}

Vector2 RectCollider::RightBottom()
{
    Vector2 edge(size.x * 0.5f, -size.y * 0.5f);

    return edge * world;
}

float RectCollider::L()
{
    float minLeft = min(LeftTop().x, LeftBottom().x);
    float minRight = min(RightTop().x, RightBottom().x);

    return min(minLeft, minRight);
}

float RectCollider::R()
{
    float maxLeft = max(LeftTop().x, LeftBottom().x);
    float maxRight = max(RightTop().x, RightBottom().x);

    return max(maxLeft, maxRight);
}

float RectCollider::T()
{
    float maxLeft = max(LeftTop().y, RightTop().y);
    float maxRight = max(LeftBottom().y, RightBottom().y);

    return max(maxLeft, maxRight);
}

float RectCollider::B()
{
    float minLeft = min(LeftTop().y, RightTop().y);
    float minRight = min(LeftBottom().y, RightBottom().y);

    return min(minLeft, minRight);
}

RectCollider::ObbDesc RectCollider::GetObb()
{
    obbDesc.position = GlobalPos();
    obbDesc.halfSize = Half();

    obbDesc.axis[0] = Right();
    obbDesc.axis[1] = Up();

    return obbDesc;
}

bool RectCollider::IsAABB(RectCollider* rect, Vector2* overlap)
{
    Vector2 halfSize = Size() * 0.5f;

    float minRight = min(R(), rect->R());
    float maxLeft = max(L(), rect->L());
    float minTop = min(T(), rect->T());
    float maxBottom = max(B(), rect->B());

    if (overlap) {
        overlap->x = minRight - maxLeft;
        overlap->y = minTop - maxBottom;
    }

    return (minRight > maxLeft && minTop > maxBottom);
}

bool RectCollider::IsOBB(RectCollider* rect)
{
    ObbDesc obbA = GetObb();
    ObbDesc obbB = rect->GetObb();

    if (IsSeperate(obbA.axis[0], obbA, obbB)) return false;
    if (IsSeperate(obbA.axis[1], obbA, obbB)) return false;
    if (IsSeperate(obbB.axis[0], obbA, obbB)) return false;
    if (IsSeperate(obbB.axis[1], obbA, obbB)) return false;
    return true;
}

bool RectCollider::IsSeperate(Vector2 seperateAxis, ObbDesc box1, ObbDesc box2)
{
    float d = abs(Dot(seperateAxis, box1.position - box2.position));

    Vector2 right = box1.axis[0] * box1.halfSize.x;
    Vector2 up = box1.axis[1] * box1.halfSize.y;

    float a = abs(Dot(seperateAxis, right) + abs(Dot(seperateAxis, up)));

    right = box2.axis[0] * box2.halfSize.x;
    up = box2.axis[1] * box2.halfSize.y;

    float b = abs(Dot(seperateAxis, right) + abs(Dot(seperateAxis, up)));

    return a+b < d;
}
