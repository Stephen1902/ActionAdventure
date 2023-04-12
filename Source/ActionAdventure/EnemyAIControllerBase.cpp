// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "EnemyAIControllerBase.h"

#include "ActionAdventureCharacter.h"
#include "EnemyCharacterBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIControllerBase::AEnemyAIControllerBase()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Info"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Info"));
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComp->ConfigureSense(*HearingConfig);
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIControllerBase::TargetPerceptionUpdated);
	AIPerceptionComp->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AEnemyAIControllerBase::TargetPerceptionInfoUpdated);
	
//	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, SightConfig->GetSenseImplementation(), GetControlledPawn());
//	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, HearingConfig->GetSenseImplementation(), GetControlledPawn());
}
void AEnemyAIControllerBase::TargetPerceptionUpdated(AActor* FoundActor, FAIStimulus Stimulus)
{
	if (!Stimulus.WasSuccessfullySensed())
	{
		bIsChasingTarget = false;		
	}
	else
	{
		// Check there is a valid pawn being controlled by this controller
		if (const AEnemyCharacterBase* ControlledPawn = Cast<AEnemyCharacterBase>(GetCharacter()))
		{
			bIsChasingTarget = true;

			//ControlledPawn->GetMovementComponent()->MaxWalkSpeed = ChasingSpeed;
			MoveToTargetLocation(Stimulus.StimulusLocation);
		}
	}
}

void AEnemyAIControllerBase::TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (bIsChasingTarget)
	{
		if (UpdateInfo.Stimulus.WasSuccessfullySensed())
		{
			MoveToTargetLocation(UpdateInfo.Stimulus.StimulusLocation);
		}
		else
		{
			bIsChasingTarget = false;
		}
	}
}

void AEnemyAIControllerBase::MoveToTargetLocation(const FVector& TargetLocation)
{
	// Only move and chase if there is a valid player character
	if (TargetLocation != FVector::ZeroVector)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetLocation);
	}
}
