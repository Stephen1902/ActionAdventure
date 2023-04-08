// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveBase.generated.h"

UCLASS()
class ACTIONADVENTURE_API AInteractiveBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<class USphereComponent> SphereComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<UStaticMeshComponent> HandleMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<UStaticMeshComponent> FlameMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<UParticleSystemComponent> ParticleSystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<class UWidgetComponent> WidgetComp;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	bool bIsInteractive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	bool bStartParticlesActivated = false;

	void SetCanInteractToFalse() { bIsInteractive = false; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
