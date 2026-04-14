// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

// Core Unreal Engine headers.
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// Enhanced Input system headers (for handling modern input mappings and actions).
#include "InputActionValue.h"

// Generated header for this class.
#include "CPP_2D_PlayerController.generated.h"

// Forward declarations (reduces compile time).
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * ACPP_2D_PlayerController
 *
 * Custom Player Controller responsible for:
 *   - Applying an Enhanced Input Mapping Context.
 *   - Binding input actions (Run, Jump, Interact, Scan).
 *   - Passing movement / interaction commands to the possessed 2D character.
 *
 * This controller acts as the bridge between input actions and character behavior.
 */
UCLASS(abstract)
class PROGRAMMINGFINAL_API ACPP_2D_PlayerController : public APlayerController {
	GENERATED_BODY()
	
public:

	// The mapping context that holds all 2D input bindings.
	// "EditDefaultsOnly" allows it to be assigned only in BP or defaults.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character_Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CurrentMappingContext;
	
	// Input action for horizontal movement (A/D or Left/Right).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character_Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Run2D_IA;
	
	// Input action for jumping and double jumping.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character_Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Jump2D_IA;

	// Input action for interacting with world objects (buttons, pickups, NPCs).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character_Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Interract2D_IA;

	// Input action for scanning ahead of the character.
	// Used to detect and damage enemies within a distance.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character_Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Scan2D_IA;

	// Cached reference to the currently possessed character.
	// This avoids calling GetCharacter() repeatedly.
	TObjectPtr<ACharacter> CurrentCharacter;

	// How far the scan should reach when using the Scan action.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Scan)
	float ScanDistance = 500.0f;

	/**
	 * SetupInputComponent
	 *
	 * Called to bind functionality to input for this actor or pawn.
	 * Override this function to set up key bindings, mouse events,
	 * or other input events specific to this class.
	 */
	virtual void SetupInputComponent() override;

protected:

	/**
	 * OnPossess
	 *
	 * Called when the controller takes control of a pawn.
	 * Stores a reference to the possessed character and
	 * applies the Enhanced Input Mapping Context.
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * Move
	 *
	 * Handles left/right movement using Enhanced Input's axis value.
	 * Also flips the sprite horizontally to match movement direction.
	 */
	void Move(const FInputActionValue& Value);

	/**
	 * JumpStart
	 *
	 * Called when the jump input is pressed.
	 */
	void JumpStart();

	/**
	 * JumpEnd
	 *
	 * Called when the jump input is released.
	 */
	void JumpEnd();

	/**
	 * Interract
	 *
	 * Attempts to interact with any overlapping actor that
	 * implements the UCPP_Interact_BPI interface.
	 */
	void Interract();

	/**
	 * Attack (Scan)
	 *
	 * Performs a forward line trace based on the character's facing
	 * direction and checks if it hits an actor.
	 * Draws debug boxes to visualize the scan and hit point.
	 */
	void Attack();
};