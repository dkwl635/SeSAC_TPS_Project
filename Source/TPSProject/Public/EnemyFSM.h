// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"


//FSM 사용을 위한 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle	UMETA(DisplayName = "대기" ) ,
	Move	UMETA ( DisplayName = "이동" ) ,
	Attack	UMETA ( DisplayName = "공격" ) ,
	Damage	UMETA ( DisplayName = "데미지" ) ,
	Die		UMETA ( DisplayName = "사망" )
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//상태 변수
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = FSM )
	EEnemyState mState = EEnemyState::Idle;
		
	//대기 상태
	void IdleStat ( );
	//이동 상태
	void MoveState ( );
	//공격 상태
	void AttackState ( );
	//피격 상태
	void DamageState ( );
	//죽음 상태
	void DieState ( );

public:
	//대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM )
	float IdleDelayTime = 2.0f;
	
	//경과 시간
	float currentTime = 0.0f;

	//타겟
	UPROPERTY(VisibleAnywhere , Category = FSM )
	class ATPSCharacter* target;

	//소유자 엑터
	UPROPERTY( )
	class AEnemy* me;

	//공격범위
	UPROPERTY(EditAnywhere, Category = FSM )
	float AttackRange = 150.f;

	//공격 대기 시간
	UPROPERTY ( EditAnywhere , Category = FSM )
	float AttackDelayTime = 2.0f;

	//데미지 피격 이벤트
	void OnDamageProcess ( );

	//체력
	UPROPERTY ( EditAnywhere , BlueprintReadWrite ,Category = FSM )
	int32 hp = 3;

	//피격 대기 시간
	UPROPERTY ( EditAnywhere ,  Category = FSM )
	float daamgeDelayTime = 2.0f;
	
	//아래로 사라지는 속도
	UPROPERTY(EditAnywhere , Category = FSM )
	float dieSpeed = 50.f;
};
