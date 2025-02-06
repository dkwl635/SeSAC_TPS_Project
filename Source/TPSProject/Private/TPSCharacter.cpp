// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkeletalMeshComponent.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


#pragma region  캐릭터 메쉬 / 위치, 회전 값 // Mesh에 마네킹 로드 하기

	//주소 : /Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'
	//파일 찾아오기
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'" ) );
	//로드가 성공했다면
	if (TempMesh.Succeeded ( ))
	{
		//로드한 메쉬 넣어주기
		GetMesh ( )->SetSkeletalMesh ( TempMesh.Object );
		//위치값 과 회전값 반영하기
		GetMesh ( )->SetRelativeLocationAndRotation ( FVector ( 0.0f , 0.0f , -90.0f ) , FRotator ( 0.0f , -90.0f , 0.0f ) );
	}
#pragma endregion 
#pragma region 카메라 // TPS 카메라 붙이기
	
	//SpringArm 붙이기
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent> ( TEXT ( "SpringArmComp" ) );
	SpringArmComp->SetupAttachment ( RootComponent );
	SpringArmComp->SetRelativeLocation ( FVector ( 0.0f , 60.0f , 0.0f ) );
	SpringArmComp->TargetArmLength = 300;

	//카메라 
	TpsCamComp = CreateDefaultSubobject<UCameraComponent> ( TEXT ( "TpsCamComp" ) );
	TpsCamComp->SetupAttachment ( SpringArmComp );

	//카메라 회전을 위한
	SpringArmComp->bUsePawnControlRotation = true;
	TpsCamComp->bUsePawnControlRotation = false;
	//캐릭터는 돌지 않도록
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

#pragma endregion
#pragma region 총  생성 / 메쉬 , 위치 조정
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent> ( TEXT ( "GunMeshComp" ) );
	//모델 리소스 연결
	GunMesh->SetupAttachment ( GetMesh ( ) );
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'" ) );
	//로드가 성공했다면
	if (TempGunMesh.Succeeded ( ))
	{
		//로드한 메쉬 넣어주기
		GunMesh->SetSkeletalMesh ( TempGunMesh.Object );
		//위치값 반영하기
		GunMesh->SetRelativeLocation ( FVector ( 0, 60.0f , 120.f ) );
	}
#pragma endregion
#pragma region  스나이퍼 생성 /메쉬 , 위치 조정
	//스나이퍼 컴포넌트 생성
	SniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "SniperMeshComp" ) );
	//컴포넌트 위치 조정
	SniperMeshComp->SetupAttachment ( GetMesh ( ) );
	//메쉬 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempSniperMesh ( TEXT ( "/Script/Engine.StaticMesh'/Game/SniperGun/sniper11.sniper11'" ) );
	//로드가 성공했다면
	if (TempSniperMesh.Succeeded ( ))
	{
		//로드한 메쉬 넣어주기
		SniperMeshComp->SetStaticMesh ( TempSniperMesh.Object );
		//총 위치, 크기 설정 및 옵션 설정
		SniperMeshComp->SetRelativeLocation ( FVector ( 0 , 50.f , 120.f ) );
		SniperMeshComp->SetRelativeScale3D ( FVector ( 0.15f ) );
	}

#pragma endregion


}


// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
#pragma region IMC_TPS 등록

	APlayerController* pc = Cast<APlayerController> ( Controller );
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( pc->GetLocalPlayer ( ) );
		if (subsystem)
		{
			subsystem->AddMappingContext ( IMC_TPS , 0 );
		}
	}
#pragma endregion

#pragma region  스나이퍼 UI 등록
	SniperUI = CreateWidget (GetWorld() , SinperUIFactory );
	//일반 조준 UI
	CrossHairUI = CreateWidget ( GetWorld ( ) , CrossHairUIFactory );
	CrossHairUI->AddToViewport ( );
#pragma endregion

	ChangeToSinperGun ( FInputActionValue() );
}


// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Direction = FTransform ( GetControlRotation ( ) ).TransformVector ( Direction );
	AddMovementInput ( Direction );

	/*
	//P = p0 + vt
	FVector p0 = GetActorLocation ( ); 
	FVector vt = Direction.GetSafeNormal() * WalkSpeed * DeltaTime;
	FVector P = p0 + vt;
	SetActorLocation ( P );
	Direction = FVector::ZeroVector;
	*/

	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

#pragma region 입력 Action 등록
	auto PlayerInput = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	if (PlayerInput)
	{
		PlayerInput->BindAction ( IA_LookUp , ETriggerEvent::Triggered , this , &ThisClass::LockUp );
		PlayerInput->BindAction ( IA_Turn , ETriggerEvent::Triggered , this , &ThisClass::Turn );
		PlayerInput->BindAction ( IA_PlayerMove , ETriggerEvent::Triggered , this , &ThisClass::PlayerMove );
		PlayerInput->BindAction ( IA_Jump , ETriggerEvent::Started , this , &ThisClass::InputJump );
		PlayerInput->BindAction ( IA_Fire , ETriggerEvent::Started , this , &ThisClass::InputFire );
		
		//총 교체 이벤트 바인딩
		PlayerInput->BindAction ( IA_GrenadeGun , ETriggerEvent::Started , this , &ThisClass::ChangeToGrenadeGun );
		PlayerInput->BindAction ( IA_SniperGun , ETriggerEvent::Started , this , &ThisClass::ChangeToSinperGun );

		//줌인 이벤트 바인딩
		PlayerInput->BindAction ( IA_Sniper , ETriggerEvent::Started , this , &ThisClass::SniperAim );
		PlayerInput->BindAction ( IA_Sniper , ETriggerEvent::Completed , this , &ThisClass::SniperAim );

	}


#pragma endregion

}

void ATPSCharacter::LockUp ( const FInputActionValue& InputValue )
{
	float value = InputValue.Get<float> ( );
	AddControllerPitchInput ( value );

}

void ATPSCharacter::Turn ( const FInputActionValue& InputValue )
{
	float value = InputValue.Get<float> ( );
	AddControllerYawInput ( value );
}

void ATPSCharacter::PlayerMove ( const FInputActionValue& InputValue )
{
	FVector2D value = InputValue.Get<FVector2D> ( );
	//상하 입력처리
	Direction.X = value.X;
	//좌우 입력처리
	Direction.Y = value.Y;
}

void ATPSCharacter::InputJump ( const FInputActionValue& InputValue )
{
	ACharacter::Jump ( );
}

void ATPSCharacter::InputFire ( const FInputActionValue& InputValue )
{
	//발사
	if (bUsingGrenadeGun) // 유탄 발사 시
	{
		FTransform firePosition = GunMesh->GetSocketTransform ( TEXT ( "FirePosition" ) );
		GetWorld ( )->SpawnActor<ABullet> ( BulletFactory , firePosition );
	}
	else				//스나이퍼 발사 시
	{
		//LineTrace 시작위치
		FVector startPos = TpsCamComp->GetComponentLocation ( );
		//LineTrace 끝 위치
		FVector endPos = startPos + TpsCamComp->GetForwardVector ( ) * 5000.0f;
		//LineTrace 충돌 정보를 담을 변수
		FHitResult hitInfo;
		//충돌 옵션 설정 변수
		FCollisionQueryParams params;

		//자기 자신 충돌 제외
		params.AddIgnoredActor ( this );
	
		//LineTrace를 이용한 충돌 검출
		bool bHit = GetWorld ( )->LineTraceSingleByChannel ( hitInfo ,startPos , endPos , ECollisionChannel::ECC_Visibility ,params );
		//LineTrace가 충돌 했을때
		if (bHit)
		{
			//충돌 처리 -> 충돌효과 표현
			FTransform BulletTrans;
			BulletTrans.SetLocation ( hitInfo.ImpactPoint );
		
			UGameplayStatics::SpawnEmitterAtLocation ( GetWorld ( ) , BulletEffectFactory , BulletTrans );
		
			auto hitComp = hitInfo.GetComponent ( );
			if (hitComp && hitComp->IsSimulatingPhysics ( ))
			{
				FVector dir = (endPos - startPos).GetSafeNormal ( );
				FVector force = dir * hitComp->GetMass ( ) * 500000;

				hitComp->AddForceAtLocation ( force , hitInfo.ImpactPoint );
			}
				
		}
	}


}

void ATPSCharacter::ChangeToGrenadeGun ( const FInputActionValue& InputValue )
{
	if (bSniperAim) return;
	
	bUsingGrenadeGun = true;

	SniperMeshComp->SetVisibility ( !bUsingGrenadeGun );
	GunMesh->SetVisibility ( bUsingGrenadeGun );

}

void ATPSCharacter::ChangeToSinperGun ( const FInputActionValue& InputValue )
{
	bUsingGrenadeGun = false;
	bUsingGrenadeGun = false;

	SniperMeshComp->SetVisibility ( !bUsingGrenadeGun );
	GunMesh->SetVisibility ( bUsingGrenadeGun );
}

void ATPSCharacter::SniperAim ( const FInputActionValue& InputValue )
{
	if (bUsingGrenadeGun) return;

	
	if (!bSniperAim)
	{
		//ON
		bSniperAim = true;
		//에임 UI
		CrossHairUI->RemoveFromParent ( );
		SniperUI->AddToViewport ( );
		//카메라 시야각 FOV(Field Of View) 설정
		TpsCamComp->SetFieldOfView ( 45.0f );
	}
	else
	{
		//Off
		bSniperAim = false;
		SniperUI->RemoveFromParent ( );
		CrossHairUI->AddToViewport ( );
		//기존 시야각 복구
		TpsCamComp->SetFieldOfView ( 90 );
	}

}

