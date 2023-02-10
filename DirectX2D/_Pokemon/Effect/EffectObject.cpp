#include "Framework.h"
#include "EffectObject.h"
#include "../Tile/DungeonTileMap.h"

EffectObject::EffectObject(Unit* unit, Effect* effect, function<bool(Unit*, Unit*)> condition, function<void(Unit*, Unit*)> event, POINT startPoint)
	: DungeonObject(Vector2(50.0f, 50.0f)), unit(unit), effect(effect), condition(condition), hitEvent(event)
{
	SetPoint(startPoint);
	effect->SetParent(this);	//이펙트 말고 애님오브젝트로 써야 하나?
	//루프할 수도 있어서 복잡하다
	//이펙트에 루프기능을 추가한다? 
	// 
	// 
	// 
	//출력은 어차피 이펙트가 할 것이기에 오브젝트 크기는 불필요할지도?
	//타일맵을 싱글톤으로 바꿔버릴까...
	//업데이트도 되는데...
	//콜라이더 크기를 정하는 값이다만 콜라이더 자체를 뺄 수도 있다
}

EffectObject::~EffectObject()
{
	delete effect;
}

void EffectObject::Update()
{
	//이펙트 적용 조건 : 이펙트 종료 시, 좌표 검사 시
	//이펙트 적용 범위 : 해당 좌표, 공개좌표 전체
	//이펙트 오브젝트가 이펙트 오브젝트를 생성할 수도 있다 - hitEvent에서	

	UpdateWorld();
}

void EffectObject::Render()
{
}

bool EffectObject::IsActing()
{
	//
	return false;
}