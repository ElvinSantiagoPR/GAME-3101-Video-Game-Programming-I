// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CPP_Projectile.generated.h"


/**
 * ACPP_Projectile
 *
 * Represents a projectile that moves forward at a set speed and detects the player.
 * Can teleport the player to a designated target actor upon collision.
 *
 * Components:
 * - DefaultSceneRoot: Root scene component for attaching other components.
 * - Bullet: Visual mesh representing the projectile.
 * - PlayerDetectSphere: Sphere collision used to detect player overlaps.
 *
 * Properties:
 * - CollisionScale: Scale factor for the player detection sphere.
 * - Speed: Movement speed of the projectile.
 * - LifeSpan: Time before the projectile destroys itself automatically.
 * - TeleportTarget: Actor to teleport the player to when hit.
 *
 * Functions:
 * - OnBoxOverlapBegin: Handles player detection and teleport logic.
 * - Tick: Updates projectile movement each frame.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPP_Projectile();

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

	// Static mesh representing the projectile.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Bullet;

	// Sphere collision used to detect player overlaps.
	UPROPERTY(VisibleAnywhere)
	USphereComponent* PlayerDetectSphere;

	// Scale factor for the sphere collision component.
	UPROPERTY(EditAnywhere, Category = "PlayerDetectSphere")
	float CollisionScale = 1.0f;

	// Movement speed of the projectile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float Speed = 500.0f;

	// Lifespan of the projectile before automatic destruction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float LifeSpan = 3.0f;

	// Actor to teleport the player to upon collision.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	AActor* TeleportTarget;

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
