// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_Interact_BPI.generated.h"

/**
 * UCPP_Interact_BPI
 *
 * This interface allows any Actor or Object to define custom interaction behavior.
 * By marking it as Blueprintable, both C++ and Blueprint classes can implement it.
 *
 * Classes that implement this interface can respond to player interaction events,
 * such as pressing an interact key, opening doors, picking up items, etc.
 */
UINTERFACE(Blueprintable)
class PROGRAMMINGFINAL_API UCPP_Interact_BPI : public UInterface
{
	GENERATED_BODY()
};

/**
 * ICPP_Interact_BPI
 *
 * Interface function definitions for interaction behavior.
 * Any class implementing this interface must provide logic for PlayerInteract(),
 * either in C++ or overridden in Blueprint using the BlueprintNativeEvent system.
 */
class PROGRAMMINGFINAL_API ICPP_Interact_BPI
{
	GENERATED_BODY()

public:
	/**
	 * PlayerInteract
	 *
	 * Called when the player interacts with an object that implements this interface.
	 * Classes using this interface can define custom interaction behavior
	 * such as opening doors, picking up items, activating switches, etc.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void PlayerInteract();
};