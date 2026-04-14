// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "2D_Character/CPP_Interact_BPI.h"
#include "CPP_TestActor.generated.h"

/**
 * URotatingMovementComponent
 *
 * Forward declaration of the Rotating Movement Component class.
 * This component allows actors to rotate automatically based on a
 * specified rotation rate. Full definition is included via
 * #include "GameFramework/RotatingMovementComponent.h" in the .cpp file.
 */
class URotatingMovementComponent;

/**
 * ACPP_TestActor
 *
 * This actor is an interactive test object in the game world.
 * It implements the ICPP_Interact_BPI interface to allow player interaction.
 *
 * Components include:
 * - DefaultSceneRoot: Root component for the actor.
 * - MeshComponent: Visual representation of the actor.
 * - BoxCollision: Detects overlaps for interaction or gameplay logic.
 * - RotatingMovement: Optional rotation behavior for dynamic objects.
 *
 * The actor can be expanded with custom gameplay events triggered by overlaps
 * or player interaction.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_TestActor : public AActor, public ICPP_Interact_BPI
{
	GENERATED_BODY()
	
public:	
	
	/**
	 * ACPP_TestActor
	 *
	 * Sets default values for this actor's properties.
	 * Creates components for the root, mesh, collision box, and rotating movement.
	 * Sets up attachment relationships and initializes default parameters.
	 */
	ACPP_TestActor();

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

	// Adds the Default Scene Root as a component for the class, aswell as make it always visible.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	// Adds the Mesh Component to the class, aswell as make it always visible.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	// Adds a Box Collision as a component for the class, aswell as make it always visible.
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

	// Set default dimensions for the box collision, 
	// aswell as edit these dimensions anywhere and set it in the box collision tab.
	UPROPERTY(EditAnywhere, Category = "BoxCollision")
	FVector BoxCollisionScale = FVector(1.0f, 1.0f, 1.0f);

	// Adds a Rotating Movement as a component for the class, 
	// aswell as make it always visible, only editable in C++ and set it in the movement tab.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	URotatingMovementComponent* RotatingMovement;

	// Set default rate of rotaion in each axis for the class, 
	// aswell as edit these rates anywehre and set it in the movement tab.
	UPROPERTY(EditAnywhere, Category = "Movement")
	FRotator RotationRate = FRotator(0.0f, 0.0f, 0.0f);

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