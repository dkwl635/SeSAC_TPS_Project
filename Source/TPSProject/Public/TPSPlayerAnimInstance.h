// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UTPSPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//매 프레임 갱신되는 함수
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;
public:
	//플레이어 이동속도
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = TPSPlayerAnim )
	float Speed = 0.0f;
	//플레이가 공중에 있는지
	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite , Category = TPSPlayerAnim )
	bool IsInAir = false;

	//플레이가 공중에 있는지
	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite , Category = TPSPlayerAnim )
	bool IsCrouched = false;


	//플레이어 좌우 이동속도
	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite , Category = TPSPlayerAnim )
	float Direction = 0.0f;

	//재생할 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly , Category = TPSPlayerAnim )
	class UAnimMontage* AttackAnimMontage;

	//공격 애니메이션 재생 함수
	void PlayAttackAnim ( );

};
