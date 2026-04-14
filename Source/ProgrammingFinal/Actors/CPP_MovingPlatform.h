// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_MovingPlatform.generated.h"

/**
 * ACPP_MovingPlatform
 *
 * Represents a platform that moves back and forth along a defined axis.
 * Can be used to create dynamic level elements, obstacles, or puzzles.
 *
 * Components:
 * - DefaultSceneRoot: Root scene component for attaching other components.
 * - Platform: Visual mesh of the moving platform.
 *
 * Movement:
 * - Moves a specified distance (MoveDistance) at a defined speed (MoveSpeed).
 * - Switches direction automatically when reaching target locations.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_MovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPP_MovingPlatform();

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

	// Static mesh representing the platform itself. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Platform;

	// Distance the platform travels from its starting location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDistance = 200.0f;

	// Speed at which the platform moves.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 150.0f;

	// Tracks whether the platform is currently moving right or left.
	bool bIsGoingRight = true;

	// Determines whether the platform should currently be moving.
	bool bIsMoving = true;

	// Target location on the right side of the movement path.
	FVector RightLocation;

	// Target location on the left side of the movement path.
	FVector LeftLocation;

public:	
	
	/**
	 * Tick
	 *
	 * Called every frame. Handles moving the door smoothly
	 * from closed to open or back based on player interaction.
	 *
	 * @param DeltaTime - Time elapsed since last frame.
	 */
	virtual void Tick(float DeltaTime) override;

};
