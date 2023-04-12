// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "DeathBox.h"

#include "ActionAdventureCharacter.h"
#include "Kismet/GameplayStatics.h"

ADeathBox::ADeathBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ADeathBox::OnBeginOverlap);
}

void ADeathBox::OnBeginOverlap(AActor* MyActor, AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, *OtherActor->GetName());
	if (AActionAdventureCharacter* PlayerCharacter = Cast<AActionAdventureCharacter>(OtherActor))
	{
		// Slow the game down by half for a visual effect
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
		// Call the function on the player which launches the death animation
		PlayerCharacter->SetPlayerDeath();
		// Call a timer, after 2 seconds to reload the level / reset the player
		GetWorld()->GetTimerManager().SetTimer(ReloadLevelHandle, this, &ADeathBox::ReloadLevel, 2.0f, false);
	}
}

void ADeathBox::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, TEXT("I'm here."));
	
}

void ADeathBox::ReloadLevel()
{
	const FName LevelName = FName(*GetWorld()->GetName());
	UGameplayStatics::OpenLevel(this, LevelName, false);
}
