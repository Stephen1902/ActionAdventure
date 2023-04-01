// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "ToCatchSphere.generated.h"

UCLASS()
class ACTIONADVENTURE_API AToCatchSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToCatchSphere();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	double NumberNeededToTrigger = 3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// The number of spheres overlapping this object
	int32 SpheresOverlapping = 0;

	// Starting relative location of the mesh
	FVector MeshStartLocation;

	// Ending relative location of the mesh
	FVector MeshEndLocation;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> MovementTimeline;

	UFUNCTION()
	virtual void TimelineFloatReturn(float Value);

	/** Declare a delegate to call with TimeLineFloatReturn */
	FOnTimelineFloat InterpFunction{};

	UPROPERTY()
	TObjectPtr<UTimelineComponent> TimelineComponent;

	/** Curve to use when turning the vehicle round a corner */
	UPROPERTY()
	TObjectPtr<UCurveFloat> FCurve;

	void SetMovementCurveInfo();

};
