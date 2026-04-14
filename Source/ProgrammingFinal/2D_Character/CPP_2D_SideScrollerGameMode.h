// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Needed to reference the custom Respawn Actor class
#include "Actors/CPP_RespawnActor.h"
#include "CPP_2D_SideScrollerGameMode.generated.h"

/**
 * ACPP_2D_SideScrollerGameMode
 *
 * This class defines the custom GameMode for the 2D Side Scroller.
 * The GameMode is responsible for setting high-level rules of the game,
 * such as default pawn, HUD and player controller.
 *
 * Currently, this class does not override any functions, but it serves
 * as the foundation for managing game-specific behavior as the project grows.
 */
UCLASS()
class PROGRAMMINGFINAL_API ACPP_2D_SideScrollerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Reference to a Respawn Actor in the level, used to reset the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACPP_RespawnActor* RespawnActor;


	/**
	* Function to respawn a player at the RespawnActor's location
	* Takes the PlayerController of the player to respawn
	**/
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(class APlayerController* PC);
};
