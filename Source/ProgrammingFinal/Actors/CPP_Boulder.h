// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Boulder.generated.h"

/**
 * ACPP_Boulder
 *
 * This class represents a Boulder actor used in the game world.
 * The boulder includes a root scene component and a static mesh component
 * to visually represent the object.
 *
 * Future functionality such as physics, collision responses, or movement
 * can be added through additional components or overridden methods.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_Boulder : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * ACPP_Boulder
	 *
	 * Sets default values for this actor's properties and initializes
	 * components such as the scene root and mesh. Also configures
	 * physics, collision, and material settings for the boulder.
	 */
	ACPP_Boulder();

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
	UStaticMeshComponent* Boulder;
};