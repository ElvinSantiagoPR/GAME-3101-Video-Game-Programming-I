// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Turret.generated.h"

/**
 * ACPP_Turret
 *
 * Represents a stationary turret that can detect the player within a certain range
 * and automatically fires projectiles at a defined fire rate.
 *
 * Components:
 * - DefaultSceneRoot: Root component for attaching other components.
 * - Turret: Visual mesh representing the turret.
 *
 * Properties:
 * - ProjectileClass: Type of projectile to spawn when firing.
 * - FireRate: Time interval between consecutive shots.
 * - AttackRange: Maximum distance at which the turret can detect and attack the player.
 * - HP: Health points of the turret; turret is destroyed when HP <= 0.
 *
 * Timers:
 * - ReloadTimer: Controls the firing interval.
 * - DespawnTimer: Optional timer to remove turret after destruction.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_Turret : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPP_Turret();

protected:

	/**
	 * BeginPlay
	 *
	 * Called when the game starts or when the actor is spawned.
	 * Used to initialize any logic that should run at the start of gameplay.
	 */
	virtual void BeginPlay() override;

	//Default root component for attaching other components.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	//Static mesh representing the turret.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Turret;

	//Class of projectile to spawn when firing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<AActor> ProjectileClass;

	//Time interval between consecutive shots.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float FireRate = 5.0f;

	//Maximum distance at which the turret can detect and attack the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float AttackRange = 1000.0f;

	//Health points of the turret. When HP reaches 0, turret is destroyed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	int32 HP = 3;

	/**
	 * Fire
	 *
	 * Spawns a projectile aimed at the player if they are within AttackRange.
	 * Logs firing events to both screen and output log for debugging purposes.
	 */
	void Fire();

	//Reference to the player actor for targeting.
	AActor* PlayerRef;
	
	//Timer handle to manage the firing interval.
	FTimerHandle ReloadTimer;

	//Timer handle for optional despawn or destruction logic.
	FTimerHandle DespawnTimer;
};
