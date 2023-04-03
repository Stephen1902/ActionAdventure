// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DeathBox.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API ADeathBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	ADeathBox();
	
	UFUNCTION()
	void OnBeginOverlap(AActor* MyActor, AActor* OtherActor);

	virtual void BeginPlay() override;
private:
	FTimerHandle ReloadLevelHandle;

	void ReloadLevel();
};
