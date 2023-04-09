// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "EnemyCharacterBase.h"

#include "ActionAdventureCharacter.h"
#include "EnemyAIControllerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TimerManager.h"
#include "Engine/TargetPoint.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Info"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyCharacterBase::PerceptionUpdated);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), this);
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
/*
	if (TargetPoints.Num() > 0)
	{
		
	}

	EnemyController = Cast<AEnemyAIControllerBase>(this->GetController());
	if (EnemyController && TargetPoints.Num() > 0)
	{
		TimerDelegate.BindUFunction(this, "MoveToNextPoint", TargetPointID);
		MoveToNextPoint(0);
	}
	*/
}

void AEnemyCharacterBase::PerceptionUpdated(const TArray<AActor*>&FoundActors)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("I see you!"));
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FString StringToDisplay = FString::FromInt(TargetPointID);
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, *StringToDisplay);
}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacterBase::MoveToNextPoint(int32 TargetID)
{
	TargetPointID = TargetID;
	if (const ATargetPoint* CurrentTarget = Cast<ATargetPoint>(TargetPoints[TargetPointID]))
	{
		const FVector TargetPointLocation = CurrentTarget->GetActorLocation();
		EnemyController->MoveToLocation(TargetPointLocation);

		// Increment the target point to send the AI to the next one
		TargetPointID++;

		if (TargetPointID == TargetPoints.Num())
		{
			TargetPointID = 0;
		}
		
		GetWorldTimerManager().SetTimer(MoveTimerHandle, TimerDelegate, 2.0f, false);
	}
}

