// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AEnemy::AEnemy()
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

#pragma region Enemy FSM 컴포넌트 추가

	fsm = CreateDefaultSubobject<UEnemyFSM> ( TEXT ( "FSM" ) );

#pragma endregion

	GetCharacterMovement ( )->bOrientRotationToMovement = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

