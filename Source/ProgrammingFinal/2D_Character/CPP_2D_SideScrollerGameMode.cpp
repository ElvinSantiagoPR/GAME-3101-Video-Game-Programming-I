// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "2D_Character/CPP_2D_SideScrollerGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"


/**
 * RespawnPlayer
 *
 * Spawns a new pawn at the RespawnActor's location and makes the specified
 * PlayerController possess it. This effectively "respawns" the player.
 *
 * @param PC - The PlayerController that will control the new pawn.
 */
void ACPP_2D_SideScrollerGameMode::RespawnPlayer(APlayerController* PC) {

    // Ensure we have a valid PlayerController, RespawnActor, and DefaultPawnClass
    if (!PC || !RespawnActor) return;
    if (!DefaultPawnClass) return;

    // Spawn a new pawn of the default class at the RespawnActor's location
    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(
        // Pawn class to spawn
        DefaultPawnClass,
        // Location to spawn at
        RespawnActor->RespawnLocation,
        // No rotation
        FRotator::ZeroRotator
    );

    // If the pawn was successfully spawned, possess it with the PlayerController
    if (NewPawn)
    {
        PC->Possess(NewPawn);
    }
}