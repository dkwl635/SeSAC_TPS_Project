// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//필요한 컴포넌트 생성
	CollisionComp = CreateDefaultSubobject<USphereComponent> (TEXT( "CollisionComp" ));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent> ( TEXT ( "MovementComp" ) );

	//컴포넌트 위치 설정
	SetRootComponent ( CollisionComp );
	MeshComp->SetupAttachment ( RootComponent );

	//콜리전 셋팅
	CollisionComp->SetCollisionProfileName ( TEXT ( "BlockAll" ) );
	CollisionComp->SetSphereRadius ( 20.0f );

	//외관 컴포넌트 설정
	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	//부모 컴포넌트 지정
	MeshComp->SetRelativeScale3D ( FVector ( 0.5f ) );
	
	//이동 컴포넌트 설정
	MovementComp->SetUpdatedComponent ( CollisionComp );
	MovementComp->InitialSpeed = 5000.0f;				//초기 속도	
	MovementComp->MaxSpeed = 5000.0f;					//최고 속도
	MovementComp->bShouldBounce = true;					//반동 여부
	MovementComp->Bounciness = 0.3f;					//반동 값

	CollisionComp->OnComponentBeginOverlap.AddDynamic ( this , &ThisClass::tssestOP );
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle deathHandler;
	GetWorld ( )->GetTimerManager ( ).SetTimer ( deathHandler , this , &ThisClass::Die , 1.5f , false );
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die ( )
{
	Destroy ( );
}

void ABullet::tssestOP ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
}

