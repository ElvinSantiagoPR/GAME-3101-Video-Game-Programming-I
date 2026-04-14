// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPP_RespawnActor.generated.h"

UCLASS()
class PROGRAMMINGFINAL_API ACPP_RespawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_RespawnActor();

	// Default root component for attaching other components.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	// Box component to define the respawn area or for visualization in the editor
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	// The world location where players should respawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	FVector RespawnLocation;


protected:
	
	/**
	 * BeginPlay
	 *
	 * Called when the game starts or when the actor is spawned.
	 * Used to initialize any logic that should run at the start of gameplay.
	 */
	virtual void BeginPlay() override;
};
