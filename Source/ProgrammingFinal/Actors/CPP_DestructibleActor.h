// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_DestructibleActor.generated.h"

/**
 * ACPP_DestructibleActor
 *
 * Represents a destructible object in the game world, such as a crate or box.
 * Can be destroyed by player actions (attacks, projectiles, etc.).
 *
 * Components:
 * - DefaultSceneRoot: Root scene component for attaching other components.
 * - Box: Visual representation of the destructible object.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_DestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ACPP_DestructibleActor();

protected:
	/**
	 * BeginPlay
	 *
	 * Called when the game starts or when the actor is spawned.
	 * Used to initialize any logic that should run at the start of gameplay.
	 */
	virtual void BeginPlay() override;

	// Default root component for attaching other components.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	//Static mesh component representing the destructible box.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Box;
};