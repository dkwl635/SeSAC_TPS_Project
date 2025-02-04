// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"


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

