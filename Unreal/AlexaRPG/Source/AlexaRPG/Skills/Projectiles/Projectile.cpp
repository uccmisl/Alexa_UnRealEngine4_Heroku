// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("Projectile Construct"));

	hitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));

	hitBox->SetCollisionProfileName("NoCollision");

	particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	RootComponent = particle;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMovementComponent->ProjectileGravityScale = 0;
	hitBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{

	Super::BeginPlay();
	if (hitBox != nullptr) {
		hitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectile::SetInitialDirection(const FVector& initialDirection) {
	ProjectileMovementComponent->Velocity = initialDirection.GetSafeNormal() * ProjectileMovementComponent->InitialSpeed;
	hitBox->SetCollisionProfileName("OverlapAll");
}

void AProjectile::applyEffects(ARPGCharacter* target) {
	applyEffectsEvent(target);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == Caster) return;
	UE_LOG(LogTemp, Warning, TEXT("ProjectileOverlap"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticle, GetActorLocation());
	ARPGCharacter* target = Cast<ARPGCharacter>(OtherActor);
	if(target!= nullptr){
		applyEffects(target);
	}
	Destroy();
}

