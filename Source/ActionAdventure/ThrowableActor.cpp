// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "ThrowableActor.h"

#include "ActionAdventureCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Hearing.h"

// Sets default values
AThrowableActor::AThrowableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	SetRootComponent(MeshComp);
	
}

// Called when the game starts or when spawned
void AThrowableActor::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AThrowableActor::ActorHasHit);
	
}

// Called every frame
void AThrowableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowableActor::LaunchOnSpawn(TObjectPtr<AActionAdventureCharacter> ActorThatSpawned)
{
	if (ActorThatSpawned)
	{
		const FVector DirectionToThrow = UKismetMathLibrary::GetDirectionUnitVector(ActorThatSpawned->GetActorLocation(), GetActorLocation());
		const FVector NewLaunchVelocity = DirectionToThrow * SpeedToThrow;
		MeshComp->AddImpulse(NewLaunchVelocity);
	}
}

void AThrowableActor::ActorHasHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bHasHit)
	{
		bHasHit = true;
		if (SoundOnHit)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundOnHit, Hit.Location);
			const TObjectPtr<ACharacter> PlayerCharacter =  UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			UAISense_Hearing::ReportNoiseEvent(GetWorld(), Hit.Location, 1.f, this);
		}
	}
}

