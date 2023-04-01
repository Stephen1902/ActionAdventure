// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "SphereToMove.h"

#include "Components/SphereComponent.h"

// Sets default values
ASphereToMove::ASphereToMove()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Root Comp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetSphereRadius(64.f);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	MeshComp->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void ASphereToMove::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASphereToMove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

