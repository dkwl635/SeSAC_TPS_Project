// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSProject.h"
#include "Kismet/GameplayStatics.h"
#include "TPSCharacter.h"
#include "Enemy.h"
#include "Bullet.h"
// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

#pragma region 타겟 및 소유자 셋팅
	//월드에서 ATPSCharacter 타겟 찾기
	//auto actor =Cast<ATPSCharacter>( UGameplayStatics::GetActorOfClass ( GetWorld ( ) , ABullet::StaticClass ( ) ));
	auto actor =Cast<ATPSCharacter>( UGameplayStatics::GetActorOfClass ( GetWorld ( ) , ATPSCharacter::StaticClass ( ) ));
	target = Cast<ATPSCharacter> ( actor );
	//만약 찾으면 Cast
	if (actor)
	{
		//
	}

	me = Cast<AEnemy> ( GetOwner ( ) );

#pragma endregion

	//
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString ( mState );
	GEngine->AddOnScreenDebugMessage ( 0 , 1.f , FColor::Red , logMsg );

	switch (mState)
	{
	case EEnemyState::Idle: { IdleStat ( ); } break;
	case EEnemyState::Move: { MoveState ( ); } break;
	case EEnemyState::Attack: { AttackState ( ); } break;
	case EEnemyState::Damage: { DamageState ( ); } break;
	case EEnemyState::Die: { DieState ( ); } break;
	default:
		break;
	}

	// ...
}

void UEnemyFSM::IdleStat ( )
{
	//시간이 흐르다가 
	currentTime += GetWorld ( )->DeltaTimeSeconds;
	//만약 경과 시간이 대기시간을 초기한다면
	if (currentTime > IdleDelayTime)
	{
		//이동 상태로 전환 시키고 싶다.
		mState = EEnemyState::Move;
		//경과시간 초기화
		currentTime = 0.0f;
	}

}

void UEnemyFSM::MoveState ( )
{
	FVector destination = target->GetActorLocation ( );
	FVector dir = destination - me->GetActorLocation ( );
	dir.Z = 0;

	me->AddMovementInput ( dir.GetSafeNormal() );

	//타겟 거리가 공격범위 안이면 공격상태로 변환
	//거리 체크
	if (dir.Size ( ) < AttackRange)
	{
		mState = EEnemyState::Attack;
	}

}

void UEnemyFSM::AttackState ( )
{
	//일정 시간 마다 공격하기
	currentTime += GetWorld ( )->DeltaTimeSeconds;

	if (currentTime > AttackDelayTime)
	{
		
		PRINT_LOG ( TEXT ( "ATTACK!!!!!!!!" ) );
		currentTime = 0.0f;
	}

	//타겟이 공격 사거리를 벗어나면 다시 이동 상태로
	FVector destination = target->GetActorLocation ( );
	float distance = FVector::Distance ( destination , me->GetActorLocation ( ) );
	if (distance > AttackRange)
	{
		mState = EEnemyState::Move;
	}


}

void UEnemyFSM::DamageState ( )
{
	currentTime += GetWorld ( )->GetDeltaSeconds ( );
	if (currentTime  > daamgeDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
	}

}

void UEnemyFSM::DieState ( )
{
	//계속 아래로 내려가고 싶다.
	FVector P0 = me->GetActorLocation ( );
	FVector vt = FVector::DownVector * dieSpeed * GetWorld ( )->GetDeltaSeconds();
	me->SetActorLocation ( P0 + vt );

	//만약 2미터 이상 내려 왔다면
	//제거
	if ((P0 + vt).Z < -200)
	{
		me->Destroy ( );
	}



}

void UEnemyFSM::OnDamageProcess ( )
{
	//hp 감소
	hp--;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		me->SetActorEnableCollision ( false );
		mState = EEnemyState::Die;
	}

}

