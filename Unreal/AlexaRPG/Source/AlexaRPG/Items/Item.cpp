// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "AlexaRPG/Characters/PlayableCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AItem::AItem()
{
	//Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetCollisionProfileName("OverlapAll");

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));



	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));


	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassFinder(TEXT("/Game/AlexaRPG/Blueprints/UI/InteractionPopup"));
	if (MenuClassFinder.Succeeded()) {
		InteractWidget->SetWidgetClass(MenuClassFinder.Class);
	}

	InteractWidget->SetVisibility(false);

	RootComponent = SkeletalMesh;
	HitBox->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(RootComponent);
	InteractWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetCollisionProfileName("NoCollision");
	SkeletalMesh->SetCollisionProfileName("NoCollision");
}

void AItem::Tick(float DeltaTime) {
	RotateWidget();
}

/////	Interactions	/////

void AItem::Use(ARPGCharacter* itemOwner) {
	UseImplementation(itemOwner);
	itemOwner->inventory->RemoveItem(this);
	this->Destroy();
}

void AItem::ChangeVisibility(bool visibility) {
	SkeletalMesh->SetVisibility(visibility);
	StaticMesh->SetVisibility(visibility);
	if (visibility) {
		HitBox->SetCollisionProfileName("OverlapAll");
	}
	else {
		HitBox->SetCollisionProfileName("NoCollision");
	}
}


void AItem::showInteraction(bool isShown) {
	if (!IsPickable) isShown = false;
	InteractWidget->SetVisibility(isShown);
}

bool AItem::Pick(ARPGCharacter* character) {
	if (IsPickable) {
		return character->inventory->AddItem(this);
	}
	return false;
}

void AItem::RotateWidget() {
	InteractWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()));
}

/////////////////////////////////

FString AItem::toString() {
	return name;
}