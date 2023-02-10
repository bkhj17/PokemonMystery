#pragma once
#include "../Object/DungeonObject.h"

class Unit;
class EffectObject : public DungeonObject
{
public:
	//이펙트 발생 주체
	//이펙트 애니메이션
	//어떤 효과인지도 알아야 한다
	//쓴놈 맞는 놈 해서 유닛 둘은 필요할 거다
	//피격대상 판별여부도 필요할 것이다
	EffectObject(
		Unit* unit,
		Effect* effect, 
		function<bool(Unit*, Unit*)> condition, 
		function<void(Unit*, Unit*)> event,
		POINT startPoint
	);
	virtual ~EffectObject();


	virtual void Update() override;
	virtual void Render() override;

	virtual bool IsActing() override;
private:
	Unit* unit;								//이펙트 발생 주체
	//발생시킨 스킬도 필요할까? 자식으로 스킬 이펙트 오브젝트를 만드는 게 나을까?
	//여기서는 좌표와 이동을 지닌 
	Effect* effect;							//출력할 이펙트 애니메이션 : 이펙트 매니저에서 처리?
	function<bool(Unit*, Unit*)> condition;	//효과 적용 조건 : 타겟 판별용
	function<void(Unit*, Unit*)> hitEvent;	//효과 함수
};