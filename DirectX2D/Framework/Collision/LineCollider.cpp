#include "Framework.h"
#include "LineCollider.h"

LineCollider::LineCollider(Vector2 startPos, Vector2 endPos)
{
    vertices.emplace_back(startPos.x, startPos.y);
    vertices.emplace_back(endPos.x, endPos.y);

    vertexBuffer = new VertexBuffer(vertices.data(), (UINT)sizeof(VertexPos), (UINT)vertices.size());
}

bool LineCollider::IsPointCollision(Vector2 point)
{
    Vector2 v1 = endPos - startPos;
    Vector2 v2 = point - startPos;

    return v1.GetNormalized() == v2.GetNormalized() && v1.Length() > v2.Length();
}

bool LineCollider::IsRectCollision(RectCollider* rect, Vector2* overlap)
{



    return false;
}

bool LineCollider::IsCircleCollision(CircleCollider* circle)
{
    return false;
}

bool LineCollider::PushCollider(Collider* collider)
{
    return false;
}
