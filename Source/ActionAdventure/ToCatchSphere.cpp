// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "ToCatchSphere.h"

#include "SphereToMove.h"
#include "Components/BoxComponent.h"

// Sets default values
AToCatchSphere::AToCatchSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Comp"));
	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(CollisionComp);

	MovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloatObject(TEXT("/Script/Engine.CurveFloat'/Game/Blueprints/ToCatchSphere/BP_MovementCurve.BP_MovementCurve'"));
	FCurve = CurveFloatObject.Object;

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
}

// Called when the game starts or when spawned
void AToCatchSphere::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AToCatchSphere::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AToCatchSphere::OnEndOverlap);

	MeshStartLocation = MeshComp->GetRelativeLocation();
	MeshEndLocation = MeshComp->GetRelativeLocation();
	MeshEndLocation.Z =- 1500.f;

	SetMovementCurveInfo();
}

// Called every frame
void AToCatchSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AToCatchSphere::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASphereToMove* SphereOverlapping = Cast<ASphereToMove>(OtherActor))
	{
		SpheresOverlapping ++;

		if (SpheresOverlapping >= NumberNeededToTrigger)
		{
			if (FCurve)
			{
				MovementTimeline->PlayFromStart();
			}
		}
	}
}

void AToCatchSphere::OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASphereToMove* SphereOverlapping = Cast<ASphereToMove>(OtherActor))
	{
		SpheresOverlapping--;
		SpheresOverlapping = FMath::Clamp(SpheresOverlapping, 0, 999);
	}
}

void AToCatchSphere::TimelineFloatReturn(float Value)
{
	MeshComp->SetRelativeLocation(FMath::Lerp(MeshStartLocation, MeshEndLocation, Value));
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Red, TEXT("Moving"));
}

void AToCatchSphere::SetMovementCurveInfo()
{
	if (FCurve)
	{
		// Update function
		MovementTimeline->AddInterpFloat(FCurve, InterpFunction, FName("Alpha"));

		// Set the timeline settings
		MovementTimeline->SetLooping(false);
		MovementTimeline->SetIgnoreTimeDilation(true);

		//float MinCurveTimeRange;
		//FCurve->GetTimeRange(MinCurveTimeRange, MaxCurveTimeRange);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Float Curve was not found.  Check the location in code"));
	}
}

