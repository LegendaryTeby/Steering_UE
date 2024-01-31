#include "Target.h"

ATarget::ATarget()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

void ATarget::BeginPlay()
{
	Super::BeginPlay();

	Center = GetActorLocation();
	DrawDebugBox(GetWorld(), Center, FVector(RandomRange.X, RandomRange.Y, 0), DebugColor, true);
	
	FTimerHandle _handle;
	GetWorldTimerManager().SetTimer(_handle, this, &ATarget::ChooseRandomLocation, fRandomRate, true);
	ChooseRandomLocation();
}

void ATarget::ChooseRandomLocation()
{
	const FVector& _location = GetRandomLocation();
	
	SetActorLocation(_location);
	DrawDebugSphere(GetWorld(), _location, 5, 1, DebugColor, false, fRandomRate, 0, 5);	
}

FVector ATarget::GetRandomLocation() const
{
	const float x = RandomRange.X;
	const float y = RandomRange.Y;
	const FVector& _random = FVector(FMath::RandRange(-x, x), FMath::RandRange(-y, y), 0);
	return Center + _random;
}
