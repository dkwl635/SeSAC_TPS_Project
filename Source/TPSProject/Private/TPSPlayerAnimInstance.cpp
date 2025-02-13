// Fill out your copyright notice in the Description page of Project Settings.
#include "TPSPlayerAnimInstance.h"
#include "TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
void UTPSPlayerAnimInstance::NativeUpdateAnimation ( float DeltaSeconds )
{
	//Super::NativeUpdateAnimation ( DeltaSeconds );

	ATPSCharacter* player = Cast<ATPSCharacter> ( TryGetPawnOwner ( ) );
	if (!player) { return; }
	
	FVector playerVelocity = player->GetVelocity ( );
	
	//현제 캐릭터가 가는 앞 뒤 인지
	//GetActorForwardVector 기준으로 벡터 내적을 하여 계산
	FVector playerForward = player->GetActorForwardVector ( );
	Speed = FVector::DotProduct ( playerForward , playerVelocity );

	//현제 캐릭터가 가는 오른쪽 왼쪽 인지
	//GetActorRightVector 기준으로 벡터 내적을 하여 계산
	FVector playerRight = player->GetActorRightVector ( );
	Direction = FVector::DotProduct ( playerRight , playerVelocity );

	//UE_LOG ( LogTemp , Warning , TEXT ( "%f" ) , Speed );
	
	//현제 캐릭터가 공중에 있는지
	IsInAir = player->GetCharacterMovement ( )->IsFalling ( );



}

void UTPSPlayerAnimInstance::PlayAttackAnim ( )
{
	if (!AttackAnimMontage) return;
	Montage_Play ( AttackAnimMontage );
	
	
}
