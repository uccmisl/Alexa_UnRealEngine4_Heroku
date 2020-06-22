// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SocketIOClientComponent.h"
#include "Characters/PlayableCharacter.h"
#include "AlexaSocket.generated.h"

UCLASS()
class ALEXARPG_API AAlexaSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlexaSocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//The socket that receive event from the Alexa Server
	USocketIOClientComponent* SIOClientComponent;

public:	
	//The player character to be able to open the inventory and cast spells
	UPROPERTY(EditAnywhere)
		APlayableCharacter* player;
};
