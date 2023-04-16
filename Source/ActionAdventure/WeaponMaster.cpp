// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "WeaponMaster.h"

// Sets default values
AWeaponMaster::AWeaponMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(MeshComponent);

	WeaponName = FText::FromString("Pistol");
	DamagePerShot = 20.f;
	FireRate = 0.2f;
	FireDistance = 500.f;
	MagazineSize = 20;
	ReloadTime = 0.5f;
	bPlayerHasThisWeapon = false;
}

// Called when the game starts or when spawned
void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoInClip = MagazineSize;
	TotalAmmo = MagazineSize;	
}

// Called every frame
void AWeaponMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

