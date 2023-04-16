// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ActionAdventureCharacter.generated.h"


UCLASS(config=Game)
class AActionAdventureCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Aiming Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* AimingCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float WalkingMovementSpeed = 200.f;
	
	/** Movement speed when running*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float RunningMovementSpeed = 500.f;

	/** Montage to play when the player fires the pistol */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FireMontageToPlay;
	
	/** Sound effect for when the player fires */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> FireSoundToPlay;
	
public:
	AActionAdventureCharacter();
	
	void AddCoinToTotal(int32 CoinNumToAddIn);

	void SetPlayerDeath() { bIsDead = true;}

	void SetCanInteract(TObjectPtr<class AInteractiveBase> InteractClassIn, bool NewInteractIn);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool GetPlayerIsDead() const  { return bIsDead; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool GetIsHoldingTorch() { return bIsHoldingTorch; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RemovePlayerTorch();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
	bool GetIsAiming() const { return bIsAiming;}
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called when the running key is pressed */
	void RunStart(const FInputActionValue& Value);

	/** Called when the running key is released */
	void RunEnd(const FInputActionValue& Value);

	/** Called for crouching input */
	void SetPlayerCrouch(const FInputActionValue& Value);
	
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interact actions */
	void Interact(const FInputActionValue& Value);

	/** Called for throwing an item */
	void ThrowItem(const FInputActionValue& Value);

	/** Called when aiming the weapon */
	void AimStart(const FInputActionValue& Value);

	/** Called when stopping aiming */
	void AimEnd(const FInputActionValue& Value);

	/** Called when firing a weapon */
	void Fire(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	TSubclassOf<AInteractiveBase> InteractiveItemLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	TSubclassOf<AInteractiveBase> InteractiveItemRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	TSubclassOf<class AThrowableActor> ActorToThrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float WeaponFireDistance = 500.f;

	// The impulse to apply when the pistol hits something that is simulating physics
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float WeaponPower = 1000.f;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	int32 CollectedCoins = 0;
	bool bIsDead = false;
	bool bCanInteract = false;
	bool bIsHoldingTorch = false;
	bool bHasPistol = false;
	bool bIsAiming = false;

	TObjectPtr<AInteractiveBase> InteractClass = nullptr;
	TObjectPtr<AInteractiveBase> InteractClassToSpawn = nullptr;
	
};

