// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
ASSProjectile::ASSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));

	CollisionComponent->SetSphereRadius(16.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Variant_Shooter/Blueprints/Pickups/Projectiles/Meshes/SM_FoamBullet.SM_FoamBullet'"));
	if (WeaponMeshAsset.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(WeaponMeshAsset.Object);
		ProjectileMesh->SetRelativeRotation(FRotator(0.0f, 90.f, 0.0f));
		ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// create the projectile movement component. No need to attach it because it's not a Scene Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->InitialSpeed = 12000.0f;
	ProjectileMovement->MaxSpeed = 12000.0f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void ASSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASSProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	// ignore if we've already hit something else
	if (bHit)
	{
		return;
	}

	bHit = true;
	PLLOG(Warning, TEXT("Hit"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->Deactivate();
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateLambda([this]() {
		this->Destroy();
		}), 10.f, false);
	
}

// Called every frame
void ASSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

