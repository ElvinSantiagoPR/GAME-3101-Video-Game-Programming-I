// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPP_BoulderCheck.generated.h"

/**
 * ACPP_BoulderCheck
 *
 * This actor is used to detect when the boulder reaches a specific area.
 * It contains a collision box that listens for overlap events and a blocking
 * wall that becomes enabled or disabled based on the boulder’s position.
 *
 * This class can be expanded to trigger cutscenes, open doors, or
 * activate gameplay events when the boulder enters the detection zone.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_BoulderCheck : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * ACPP_BoulderCheck
	 *
	 * This actor is used to detect when the boulder reaches a specific area.
	 * It contains a collision box that listens for overlap events and a blocking
	 * wall that becomes enabled or disabled based on the boulder’s position.
	 *
	 * This class can be expanded to trigger cutscenes, open doors, or
	 * activate gameplay events when the boulder enters the detection zone.
	 */
	ACPP_BoulderCheck();

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

	// Mesh for the wall that blocks or unblocks based on boulder detection.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BlockingWall;

	// Collision box used to detect when the boulder enters the trigger area.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoulderDetectBox;

	// Adjustable scale for the collision box area in the editor.
	UPROPERTY(EditAnywhere, Category = "BoxCollision")
	FVector BoxCollisionScale = FVector(1.0f, 1.0f, 1.0f);

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