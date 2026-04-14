// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "CPP_Flamethrower.generated.h"

UCLASS()
class PROGRAMMINGFINAL_API ACPP_Flamethrower : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_Flamethrower();

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

	// The static mesh representing the flamethrower's chimney
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Chimney;

	// Box component used to detect player overlaps
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* PlayerDetectBox;

	// Set default dimensions for the box collision, 
	// aswell as edit these dimensions anywhere and set it in the box collision tab.
	UPROPERTY(EditAnywhere, Category = "PlayerDetectBox")
	FVector CollisionScale = FVector(1.0f, 1.0f, 1.0f);

	// Niagara component for fire VFX
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	UNiagaraComponent* Fire;

	// The target actor to teleport the player to when hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	AActor* TeleportTarget;

	// Widget to display on game over
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	// Widget for level UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelUIWidgetClass;

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

	/**
	 * OnBoxOverlapEnd
	 *
	 * Called when another actor ends overlapping the box collision component.
	 * Can be used to stop effects or reset states when the player leaves the overlap area.
	 *
	 * @param OverlappedComp - The component that triggered the end overlap.
	 * @param OtherActor - The other actor involved in the overlap.
	 * @param OtherComp - The specific component of the other actor.
	 * @param OtherBodyIndex - Body index for multi-body objects.
	 */
	UFUNCTION()
	void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
