// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "2D_Character/CPP_2D_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/CPP_DestructibleActor.h"
#include "Actors/CPP_Turret.h"
#include "PaperZDCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Engine/Engine.h"
#include "Logging/LogMacros.h"
#include "DrawDebugHelpers.h"
#include "CPP_Interact_BPI.h"
#include "LogClass/ESS_LOG.h"


/**
 * SetupInputComponent
 * 
 * Binds Enhanced Input actions (Move, Jump, Interact, Scan)
 * to their corresponding handler functions.
 * Called automatically when the PlayerController is initialized.
 */
void ACPP_2D_PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	// Cast InputComponent to EnhancedInputComponent to allow action binding.
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(this->InputComponent);
	
	if (EnhancedInputComponent){
		
		// Bind movement input.
		EnhancedInputComponent->BindAction(Run2D_IA.Get(), ETriggerEvent::Triggered, this, &ACPP_2D_PlayerController::Move);

		// Bind jump pressed.
		EnhancedInputComponent->BindAction(Jump2D_IA.Get(), ETriggerEvent::Started, this, &ACPP_2D_PlayerController::JumpStart);

		// Bind jump released.
		EnhancedInputComponent->BindAction(Jump2D_IA.Get(), ETriggerEvent::Completed, this, &ACPP_2D_PlayerController::JumpEnd);

		// Bind scanning / attack action.
		EnhancedInputComponent->BindAction(Scan2D_IA.Get(), ETriggerEvent::Started, this, &ACPP_2D_PlayerController::Attack);

		// Bind interact input.
		EnhancedInputComponent->BindAction(Interract2D_IA.Get(), ETriggerEvent::Started, this, &ACPP_2D_PlayerController::Interract);
	}
}

/**
 * OnPossess
 *
 * Called when the controller takes control of a pawn.
 * Stores a reference to the possessed character and
 * applies the Enhanced Input Mapping Context.
 */
void ACPP_2D_PlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	// Cache the possessed character for movement/jump handling.
	this->CurrentCharacter = Cast<ACharacter>(InPawn);

	// Get the Enhanced Input subsystem for this local player.
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputLocalPlayerSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	// Add the mapping context so all input actions become active.
	if (InputLocalPlayerSubsystem) {
		InputLocalPlayerSubsystem->AddMappingContext(this->CurrentMappingContext.Get(), 0);
	}
}

/**
 * Move
 *
 * Handles left/right movement using Enhanced Input's axis value.
 * Also flips the sprite horizontally to match movement direction.
 */
void ACPP_2D_PlayerController::Move(const FInputActionValue& Value) {

	// Get input axis value (-1 = left, +1 = right).
	float MoveValue = Value.Get<float>();

	// Apply movement to character.
	this->CurrentCharacter->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveValue);

	// Flip sprite to match movement direction.
	if (MoveValue != 0.0f) {

		// Get sprite component (Paper2D).
		UPaperFlipbookComponent* Sprite = this->CurrentCharacter->FindComponentByClass<UPaperFlipbookComponent>();
		if (Sprite) {
			// Flip scale on X axis.
			FRotator CurrentRotation = Sprite->GetRelativeRotation();
			CurrentRotation.Yaw = (MoveValue > 0.0f) ? 0.0f : 180.0f;
			Sprite->SetRelativeRotation(CurrentRotation);
		}
	}
}

/**
 * JumpStart
 *
 * Called when the jump input is pressed.
 */
void ACPP_2D_PlayerController::JumpStart() {
	// Jump uses the character's jump velocity in CharacterMovementComponent
	this->CurrentCharacter->Jump();
}

/**
 * JumpEnd
 *
 * Called when the jump input is released.
 */
void ACPP_2D_PlayerController::JumpEnd() {
	// StopJumping allows for variable jump height
	this->CurrentCharacter->StopJumping();
}

/**
 * Interract
 *
 * Attempts to interact with any overlapping actor that
 * implements the UCPP_Interact_BPI interface.
 */
void ACPP_2D_PlayerController::Interract() {
	if (!CurrentCharacter) return;

	// Get list of actors overlapping the character.
	TArray<AActor*> OverlappingActors;
	CurrentCharacter->GetOverlappingActors(OverlappingActors);

	// Only proceed if something is overlapped.
	if (OverlappingActors.Num() > 0) {
		
		AActor* OverlappingActor = OverlappingActors[0];

		// Check if actor implements the interaction interface.
		if (OverlappingActor && OverlappingActor->GetClass()->ImplementsInterface(UCPP_Interact_BPI::StaticClass())) {

			// Execute interface function.
			ICPP_Interact_BPI::Execute_PlayerInteract(OverlappingActor);

			// Display debugging info.
			FString ActorName = OverlappingActor->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Interacted with: %s"), *ActorName));
			UE_LOG(LogTemp, Warning, TEXT("Interacted with : % s"), *ActorName);
		}
	}
}

/**
 * Attack (Scan)
 *
 * Performs a forward line trace based on the character's facing
 * direction and checks if it hits an actor.
 * Draws debug boxes to visualize the scan and hit point.
 */
void ACPP_2D_PlayerController::Attack() {
	if (!CurrentCharacter) return;

	UCharacterMovementComponent* MoveComp = CurrentCharacter->GetCharacterMovement();
	if (MoveComp && MoveComp->IsFalling()) return;

	// Start the scan at the character's location.
	FVector Start = CurrentCharacter->GetActorLocation();

	// Determine which direction the character is facing.
	UPaperFlipbookComponent* Sprite = CurrentCharacter->FindComponentByClass<UPaperFlipbookComponent>();
	float FacingDirection = (Sprite && Sprite->GetComponentScale().X > 0) ? 1.0f : -1.0f;

	// End point of the scan (forward or backward).
	FVector End = Start + FVector(FacingDirection * ScanDistance, 0.0f, 0.0f);

	// Result of the trace.
	FHitResult HitResult;

	// Trace parameters.
	FCollisionQueryParams TraceParams(FName(TEXT("ScanTrace")), true, CurrentCharacter);
	TraceParams.bReturnPhysicalMaterial = false;

	// Perform the line trace.	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	// If something was hit, log it and show a debug box.
	if (bHit && HitResult.GetActor()) {

		FString ActorName = HitResult.GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit Actor: %s"), *ActorName));
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *ActorName);
		DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(10.0f, 10.0f, 10.0f), FColor::Green, false, 1.0f, 0, 1.0f);

		// Handle destructible actor
		if (HitResult.GetActor()->IsA(ACPP_DestructibleActor::StaticClass())) {
			HitResult.GetActor()->Destroy();
		}

		// Handle turret damage		
		else if (HitResult.GetActor()->IsA(ACPP_Turret::StaticClass())) {
			
			FName VarName = FName("HP");
			FProperty* Property = HitResult.GetActor()->GetClass()->FindPropertyByName(VarName);

			if (Property) {
				if (FIntProperty* IntProp = CastField<FIntProperty>(Property)) {
					int32 Value = IntProp->GetPropertyValue_InContainer(HitResult.GetActor());

					IntProp->SetPropertyValue_InContainer(HitResult.GetActor(), Value -= 1);

					if (Value <= 0) {
						HitResult.GetActor()->Destroy();
					}
				}
			}
		}

		// Handle enemy (PaperZDCharacter) damage
		else if (HitResult.GetActor()->IsA(APaperZDCharacter::StaticClass())) {

			FName VarName = FName("HP");
			FProperty* Property = HitResult.GetActor()->GetClass()->FindPropertyByName(VarName);

			if (Property) {
				if (FIntProperty* IntProp = CastField<FIntProperty>(Property)) {
					int32 Value = IntProp->GetPropertyValue_InContainer(HitResult.GetActor());

					IntProp->SetPropertyValue_InContainer(HitResult.GetActor(), Value -= 1);

					if (Value <= 0) {
						HitResult.GetActor()->Destroy();
					}
				}
			}
		}
	}

	// Draw the full scan path (blue).
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f, 0, 1.0f);
}