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

	UPROPERTY ( VisibleAnywhere, BlueprintReadOnly , Category = Camera )
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
	//총 관련
	UPROPERTY ( VisibleAnywhere , Category = GunMesh )
	class USkeletalMeshComponent* GunMesh;
	//총알
	UPROPERTY( EditDefaultsOnly , Category = BulletFactory )
	TSubclassOf<class ABullet> BulletFactory;
	//발사 입력
	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_Fire;
	//총알 발사 함수
	void InputFire (const struct FInputActionValue& InputValue );

public:
	//스나이퍼 총 관련
	UPROPERTY ( VisibleAnywhere , Category = GunMesh)
	class UStaticMeshComponent* SniperMeshComp;

public:
	//총 교체 인풋
	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_GrenadeGun;
	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_SniperGun;

	//총 여부
	bool bUsingGrenadeGun = true;

	//총 변경 함수
	void ChangeToGrenadeGun ( const struct FInputActionValue& InputValue );
	void ChangeToSinperGun ( const struct FInputActionValue& InputValue );

public: 
	//스나이퍼 조준 이벤트 관련
	UPROPERTY ( EditDefaultsOnly , Category = "INPUT" )
	class UInputAction* IA_Sniper;

	//줌인 처리
	void SniperAim ( const struct FInputActionValue& InputValue );

	//줌인 여부
	bool bSniperAim = false;

	//줌인 UI
	UPROPERTY(EditDefaultsOnly , Category = SniperUI )
	TSubclassOf<class UUserWidget> SinperUIFactory;

	UPROPERTY()
	class UUserWidget* SniperUI;

	//총알 파편 효과
	UPROPERTY ( EditAnywhere , Category = BulletEffect)
	class UParticleSystem* BulletEffectFactory;

public:
	//일반 조준 UI
	UPROPERTY ( EditDefaultsOnly , Category = SinperUI )
	TSubclassOf<class UUserWidget> CrossHairUIFactory;

	UPROPERTY ()
	class UUserWidget* CrossHairUI;

};
