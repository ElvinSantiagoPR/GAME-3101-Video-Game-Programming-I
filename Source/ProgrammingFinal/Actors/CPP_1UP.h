// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CPP_1UP.generated.h"

/**
 * URotatingMovementComponent
 *
 * Forward declaration of the Rotating Movement Component class.
 * This component allows actors to rotate automatically based on a
 * specified rotation rate. Full definition is included via
 * #include "GameFramework/RotatingMovementComponent.h" in the .cpp file.
 */
class URotatingMovementComponent;

UCLASS()
class PROGRAMMINGFINAL_API ACPP_1UP : public AActor {
	GENERATED_BODY()
	
public:	
	
	ACPP_1UP();

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

	// Visual representation of the 1UP (healing item)
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HealCross;

	// Sphere component to detect player overlaps
	UPROPERTY(VisibleAnywhere)
	USphereComponent* PlayerDetectSphere;

	// Scale for the collision detection sphere
	UPROPERTY(EditAnywhere, Category = "PlayerDetectSphere")
	float CollisionScale = 1.0f;

	/**
	 * RotatingMovementComponent
	 *
	 * Component that automatically rotates the 1UP item.
	 * Used to make the 1UP visually stand out in the level.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URotatingMovementComponent* RotatingComponent;

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