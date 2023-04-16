// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMaster.generated.h"

UCLASS()
class ACTIONADVENTURE_API AWeaponMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponMaster();

	UPROPERTY(VisibleAnywhere, Category = "SetUp")
	 USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	FText WeaponName;

	// Base damage per shot
	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	float DamagePerShot;

	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	float FireDistance;

	// Total ammo the weapon can hold per clip
	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	int32 MagazineSize;

	// Total ammo available to the player Clip + Reserve 
	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	int32 TotalAmmo;

	// Ammo currently in the clip
	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	int32 CurrentAmmoInClip;

	// Time in seconds the weapon takes to reload
	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	float ReloadTime;

	// Whether or not the player has this weapon available to them
	UPROPERTY(EditDefaultsOnly, Category = "SetUp")
	bool bPlayerHasThisWeapon;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Used for the player to get the firing distance of this weapon
	float GetFireDistance() const { return FireDistance; }

};
