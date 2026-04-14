// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025


#include "Enemy/CPP_PatrolEnemyAI.h"
#include "GameFramework/Character.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include <PaperZDCharacter.h>

ACPP_PatrolEnemyAI::ACPP_PatrolEnemyAI() {

	// Enable Tick() so AI logic is updated every frame
	PrimaryActorTick.bCanEverTick = true;
}


void ACPP_PatrolEnemyAI::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	// Cast the possessed pawn to our PaperZDCharacter class and cache it
	ControlledEnemy = Cast<APaperZDCharacter>(InPawn);
}

void ACPP_PatrolEnemyAI::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (!ControlledEnemy) return;

	// Get a reference to the player character
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Player) {
		// Calculate horizontal distance to player
		float DistanceToPlayer = FMath::Abs(Player->GetActorLocation().X - ControlledEnemy->GetActorLocation().X);

		if (DistanceToPlayer <= ChaseDistance) {
			// Determine direction to player (-1 = left, 1 = right)
			float Direction = FMath::Sign(Player->GetActorLocation().X - ControlledEnemy->GetActorLocation().X);

			// Move towards the player
			ControlledEnemy->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Direction * PatrolSpeed);

			// Flip sprite if AI is facing the wrong direction
			if ((Direction > 0 && FacingDirection < 0) || (Direction < 0 && FacingDirection > 0)) {
				FlipDirection();
			}
		}
		else {
			// Patrol in the current facing direction if player is out of range
			ControlledEnemy->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), FacingDirection * PatrolSpeed);
		}

	}
	else {
		// Patrol in the current facing direction if no player exists
		ControlledEnemy->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), FacingDirection * PatrolSpeed);
	}
}

/**
	 * FlipDirection
	 *
	 * Reverses the AI's facing direction, used when patrolling.
	 * Updates the FacingDirection value and rotates the character.
	 */
void ACPP_PatrolEnemyAI::FlipDirection() {
	
	if (!ControlledEnemy) return;

	// Get the flipbook component used to display 2D animations
	UPaperFlipbookComponent* FlipComp = ControlledEnemy->GetComponentByClass<UPaperFlipbookComponent>();
	if (!FlipComp) return;

	// Toggle the Yaw rotation to flip the sprite horizontally
	if (FlipComp->GetRelativeRotation().Yaw == 180) {
		FlipComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else if (FlipComp->GetRelativeRotation().Yaw == 0) {
		FlipComp->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	
	// Reverse the facing direction for movement logic
	FacingDirection *= -1.0f;
}