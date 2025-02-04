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


public: //입력
	UPROPERTY(EditDefaultsOnly , Category = "INPUT" )
	class UInputMappingContext* IMC_TPS;

	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_LookUp;

	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_Turn;

	//좌우 회전 입력 처리
	UFUNCTION()
	void LockUp ( const struct FInputActionValue& InputValue );
	UFUNCTION()
	void Turn ( const struct FInputActionValue& InputValue );
};
