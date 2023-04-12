// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIControllerBase();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;
		
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* FoundActor, FAIStimulus Stimulus);

	UFUNCTION()
	void TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);
	
	// Function to move the enemy to the player character position
	void MoveToTargetLocation(const FVector& TargetLocation);

	UPROPERTY()
	TObjectPtr<AActor> TargetActor = nullptr;

	bool bIsChasingTarget = false;
};
