// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "2D_Character/CPP_Interact_BPI.h"
#include "CPP_Door.generated.h"

UCLASS()
class PROGRAMMINGFINAL_API ACPP_Door : public AActor, public ICPP_Interact_BPI
{
	GENERATED_BODY()
	
public:	
	
	ACPP_Door();

	/**
	 * PlayerInteract_Implementation
	 *
	 * Called when the player interacts with this actor.
	 * Implementation of the ICPP_Interact_BPI interface function.
	 */
	
	virtual void PlayerInteract_Implementation() override;

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

	// Static mesh representing the door.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Door;

	// Box component used to detect when the player is near the door.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* PlayerDetectBox;

	// Set default dimensions for the box collision, 
	// aswell as edit these dimensions anywhere and set it in the box collision tab.
	UPROPERTY(EditAnywhere, Category = "PlayerDetectBox")
	FVector CollisionScale = FVector(1.0f, 1.0f, 1.0f);

	// How far the door moves when opening.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDistance = 200.0f;

	// Speed at which the door moves.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 150.0f;

	// Boolean to track if the door is currently open.
	bool bIsOpen = false;

	// Boolean to track if the door is currently moving (opening or closing).
	bool bIsMoving = false;

	// Initial closed position of the door.
	FVector ClosedLocation;

	// Target open position of the door.
	FVector OpenLocation;

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
