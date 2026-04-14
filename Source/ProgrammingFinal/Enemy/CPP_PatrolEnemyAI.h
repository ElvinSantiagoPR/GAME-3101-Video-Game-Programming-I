// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CPP_PatrolEnemyAI.generated.h"

class ACharacter;

UCLASS()
class PROGRAMMINGFINAL_API ACPP_PatrolEnemyAI : public AAIController
{
	GENERATED_BODY()
	
public:
	ACPP_PatrolEnemyAI();

	/**
	 * Tick
	 *
	 * Called every frame. Handles updating AI logic such as movement or
	 * chasing the player.
	 *
	 * @param DeltaSeconds - Time elapsed since last frame.
	 */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * OnPossess
	 *
	 * Called when this AIController possesses a pawn.
	 * Used to cache references to the controlled character.
	 *
	 * @param InPawn - The pawn being possessed.
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * FlipDirection
	 *
	 * Reverses the AI's facing direction, used when patrolling.
	 * Updates the FacingDirection value and rotates the character.
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	void FlipDirection();

protected:

	// Cached reference to the character this AI controls.
	TObjectPtr<ACharacter> ControlledEnemy;

	// The current horizontal facing direction (1.0 = right, -1.0 = left)
	UPROPERTY(BlueprintReadOnly, Category = "AI|Patrol")
	float FacingDirection = 1.0f;

	// Speed multiplier for patrolling movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Patrol")
	float PatrolSpeed = 1.0f;

	// Maximum distance at which the AI will start chasing the player
	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "AI|Chase")
	float ChaseDistance = 150.0f;
};
