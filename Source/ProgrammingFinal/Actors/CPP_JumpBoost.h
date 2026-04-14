// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CPP_JumpBoost.generated.h"

UCLASS()
class PROGRAMMINGFINAL_API ACPP_JumpBoost : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_JumpBoost();

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

	// Visual representation of the jump boost orb.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Orb;

	// Sphere collision component for detecting overlapping player.
	UPROPERTY(VisibleAnywhere)
	USphereComponent* PlayerDetectSphere;

	// Scale of the sphere collision for player detection.
	UPROPERTY(EditAnywhere, Category = "PlayerDetectSphere")
	float CollisionScale = 1.0f;

	// Multiplier applied to the player's jump.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff Stats")
	float JumpMultiplier = 1.4f;

	// Duration in seconds that the jump boost effect lasts.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff Stats")
	float BuffDuration = 10.0f;

	// Delay before the jump boost orb respawns after being collected.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff Stats")
	float RespawnDelay = 20.0f;

	// Timer handle for managing the jump boost effect duration.
	FTimerHandle BuffTimerHandler;

	// Timer handle for managing the orb respawn.
	FTimerHandle RespawnTimerHandler;

	/**
	 * OnBoxOverlapBegin
	 *
	 * Called when another actor begins overlapping the box collision component.
	 * Checks if the overlapping actor is the player and logs debug messages.
	 *
	 * @param OverlappedComp - The component that triggered the overlap.
	 * @param OtherActor - The other actor involved in the overlap.
	 * @param OtherComp - The specific component of the other actor.
	 * @param OtherBodyIndex - Body index for multi-body objects.
	 * @param bFromSweep - True if the overlap was from a sweep movement.
	 * @param SweepResult - Hit result data if from sweep.
	 */
	UFUNCTION()
	void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
