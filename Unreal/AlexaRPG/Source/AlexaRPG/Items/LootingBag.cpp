// Fill out your copyright notice in the Description page of Project Settings.


#include "AlexaRPG/Items/LootingBag.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "AlexaRPG/Characters/PlayableCharacter.h"


ALootingBag::ALootingBag() : AItem() {

	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Game/Infinity_Blade_Assets/Meshes/Props/SM_TreasureBags02.SM_TreasureBags02"));

	if (VisualAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(VisualAsset.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}


	HitBox->SetRelativeScale3D(FVector(0.3f, 0.25f, 0.45f));
	HitBox->SetRelativeLocation(FVector(0, 0, 14));
	ConstructorHelpers::FClassFinder<UUserWidget> UIFinder(TEXT("/Game/AlexaRPG/Blueprints/UI/LootingUI"));
	succeded = UIFinder.Succeeded();
	if (succeded) {
		LootUIClass = UIFinder.Class;
	}
}

bool ALootingBag::Pick(ARPGCharacter* character) {
	if(succeded){
		APlayableCharacter* player = Cast<APlayableCharacter>(character);
		if (player != nullptr) {

			player->Loots = this;
			UUserWidget* lootWidget = CreateWidget<UUserWidget>(GetWorld(), LootUIClass);


			lootWidget->AddToViewport();


			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();


			PlayerController->bShowMouseCursor = true;
			PlayerController->SetPause(true);
			return true;
		}
	}
	return false;
}

void ALootingBag::AddCoins(int amount) {
	if (amount < 0) return;
	Coins += amount;
}

bool ALootingBag::AddItem(AItem* item) {
	if (Items.Contains(item)) return false;
	if (Items.Num() == MaxItems) return false;
	Items.Add(item);
	return true;

}

AItem* ALootingBag::GetItem(int pos) {
	if (Items.IsValidIndex(pos))
		return Items[pos];
	return nullptr;
}


void ALootingBag::removeItem(AItem* item) {
	if (Items.Contains(item)) {
		Items.Remove(item);
	}
}