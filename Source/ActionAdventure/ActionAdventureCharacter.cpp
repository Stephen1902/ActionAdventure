// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionAdventureCharacter.h"

#include "EnemyCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractiveBase.h"
#include "ThrowableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AActionAdventureCharacter

AActionAdventureCharacter::AActionAdventureCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkingMovementSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkingMovementSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create an aiming camera
	AimingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimingCamera"));
	AimingCamera->SetupAttachment(CameraBoom);
	AimingCamera->bUsePawnControlRotation = false;  // Camera does not rotate relative to arm

	// For the player to hold things in their left hand
	//LeftHandComp = CreateDefaultSubobject<ULeftHandComponent>(TEXT("Left Hand Comp"));
	//LeftHandComp->SetupAttachment(GetMesh(), TEXT("hand_l"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AActionAdventureCharacter::AddCoinToTotal(int32 CoinNumToAddIn)
{
	CollectedCoins += CoinNumToAddIn;
	const FString StringToDisplay = "Player has " + FString::FromInt(CollectedCoins) + " coins.";
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, *StringToDisplay);
}

void AActionAdventureCharacter::SetCanInteract(TObjectPtr<AInteractiveBase> InteractClassIn, bool NewInteractIn)
{
	bCanInteract = NewInteractIn;
	
	if (bCanInteract)
	{
		InteractClass = InteractClassIn;
	}
	else
	{
		InteractClass = nullptr;
	}
}

void AActionAdventureCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Set that this character can crouch.  This is only for AI / Nav movement.  To avoid issues later.
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AActionAdventureCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AActionAdventureCharacter::Move);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AActionAdventureCharacter::RunStart);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AActionAdventureCharacter::RunEnd);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AActionAdventureCharacter::SetPlayerCrouch);
		
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AActionAdventureCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AActionAdventureCharacter::Interact);

		//Throwing
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &AActionAdventureCharacter::ThrowItem);

		//Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AActionAdventureCharacter::AimStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AActionAdventureCharacter::AimEnd);

		//Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AActionAdventureCharacter::Fire);
	}

}

void AActionAdventureCharacter::RemovePlayerTorch()
{
	if (InteractClassToSpawn)
	{
		InteractClassToSpawn->Destroy();
		InteractClassToSpawn = nullptr;
	}
}

void AActionAdventureCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		
	}
}

void AActionAdventureCharacter::RunStart(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningMovementSpeed;
	}
}

void AActionAdventureCharacter::RunEnd(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingMovementSpeed;
	}
}

void AActionAdventureCharacter::SetPlayerCrouch(const FInputActionValue& Value)
{
	if (!GetCharacterMovement()->IsCrouching())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AActionAdventureCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AActionAdventureCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractClass != nullptr)
	{
		const EHandItem ItemHand = InteractClass->GetHandToPlace();
		InteractClass->Destroy();
		InteractClass = nullptr;
		
		// Check for a valid item in the set up and check whether the item being collected is a left or right hand item
		if ((InteractiveItemLeftHand && ItemHand == EHandItem::EHI_Left) || (InteractiveItemRightHand && ItemHand == EHandItem::EHI_Right))
		{
			const FActorSpawnParameters SpawnParameters;
			const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			FName SocketToAttachTo;
			
			if (ItemHand == EHandItem::EHI_Left)
			{
				InteractClassToSpawn = GetWorld()->SpawnActor<AInteractiveBase>(InteractiveItemLeftHand, GetActorLocation(), GetActorRotation(), SpawnParameters);
				SocketToAttachTo = FName("TorchSocket");
				bIsHoldingTorch = true;
			}
			else
			{
				InteractClassToSpawn = GetWorld()->SpawnActor<AInteractiveBase>(InteractiveItemRightHand, GetActorLocation(), GetActorRotation(), SpawnParameters);
				SocketToAttachTo = FName("PistolSocket");
				bHasPistol = true;
			}
			
			InteractClassToSpawn->AttachToComponent(GetMesh(), TransformRules, SocketToAttachTo);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Something failed"));
		}


	}

}

void AActionAdventureCharacter::ThrowItem(const FInputActionValue& Value)
{
	if (ActorToThrow)
	{
		// Multiply the current forward vector by how many units the actor is to spawn to avoid issues with player character collision
		const FVector CurrentForwardVector = GetActorForwardVector() * 50.f;
		const FVector LocationToSpawnFrom = GetActorTransform().GetLocation() + CurrentForwardVector;
		const FActorSpawnParameters SpawnParameters;
		// Spawn the projectile
		const TObjectPtr<AThrowableActor> SpawnedThrowable = GetWorld()->SpawnActor<AThrowableActor>(ActorToThrow, LocationToSpawnFrom, GetActorRotation(), SpawnParameters);
		
		SpawnedThrowable->LaunchOnSpawn(this);
		
	}
}

void AActionAdventureCharacter::AimStart(const FInputActionValue& Value)
{
	if (bHasPistol)
	{
		bIsAiming = true;
		FollowCamera->Deactivate();
		AimingCamera->Activate();
		bUseControllerRotationYaw = true;
		bUseControllerRotationPitch = true;
	}
}

void AActionAdventureCharacter::AimEnd(const FInputActionValue& Value)
{
	if (bHasPistol)
	{
		bIsAiming = false;
		AimingCamera->Deactivate();
		FollowCamera->Activate();
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
	}
}

void AActionAdventureCharacter::Fire(const FInputActionValue& Value)
{
	// Can only fire if the player is holding the pistol
	if (bHasPistol)
	{
		// Check for a valid fire montage from BP
		if (FireMontageToPlay)
		{
			PlayAnimMontage(FireMontageToPlay, 1.0f, NAME_None);
		}

		FVector FireFromLocation;
		FRotator FireFromRotation;
		GetActorEyesViewPoint(FireFromLocation, FireFromRotation);
		
		// Try to get the socket at the end of the gun
		if (AInteractiveBase* BaseRef = Cast<AInteractiveBase>(InteractClassToSpawn))
		{
			if (UObject* SK_MeshComp = BaseRef->GetDefaultSubobjectByName(FName("Skeletal Mesh Comp")))
			{
				if (USkeletalMeshComponent* WeaponMesh = Cast<USkeletalMeshComponent>(SK_MeshComp))
				{
					FireFromLocation = WeaponMesh->GetSocketLocation("FiringSocket");
					FireFromRotation = WeaponMesh->GetSocketRotation("FiringSocket");
				}
			}
		}

		// Check if there is a valid sound to play and play it
		if (FireSoundToPlay)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSoundToPlay, FireFromLocation);
		}

		const FVector MeshCompRotationAsVector = FireFromRotation.Vector();
		
		const FVector FireToLocation = (MeshCompRotationAsVector * WeaponFireDistance) + FireFromLocation;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		// TODO Remove this line
		DrawDebugLine(GetWorld(), FireFromLocation, FireToLocation, FColor::Green, true);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, FireFromLocation, FireToLocation, ECC_Visibility, CollisionParams))
		{
			// Check if what was hit is simulating physics
			if (HitResult.GetComponent()->IsSimulatingPhysics())
			{
				// Calculate where the actor was hit and multiply it by the Weapon Power
				FVector ImpulseToApply = UKismetMathLibrary::GetDirectionUnitVector(HitResult.TraceStart, HitResult.TraceEnd) * WeaponPower;
				//  Apply a physics force to it
				HitResult.GetComponent()->AddImpulse(ImpulseToApply);
			}
		}
	}
}

