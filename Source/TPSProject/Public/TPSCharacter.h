// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

UCLASS()
class TPSPROJECT_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = Camera )
	class USpringArmComponent* SpringArmComp;

	UPROPERTY ( EditAnywhere , Category = Camera )
	class UCameraComponent* TpsCamComp;


public: 
	//입력
	UPROPERTY(EditDefaultsOnly , Category = "INPUT" )
	class UInputMappingContext* IMC_TPS;

	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_LookUp;

	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_Turn;

	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_PlayerMove;

	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_Jump;

	//좌우 회전 입력 처리
	void LockUp ( const struct FInputActionValue& InputValue );
	void Turn ( const struct FInputActionValue& InputValue );
	//플레이어 좌우입력 받아 이동 처리
	void PlayerMove ( const struct FInputActionValue& InputValue );
	//플레이어 점프 처리
	void InputJump ( const struct FInputActionValue& InputValue );

public:
	//이동 관련 변수
	UPROPERTY(EditAnywhere , Category = "PLAYER SETTING" )
	float WalkSpeed = 600.0f;
	FVector Direction = FVector::ZeroVector;
public:


};
