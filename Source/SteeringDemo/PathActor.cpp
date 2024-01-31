#include "PathActor.h"

APathActor::APathActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

UPath* APathActor::GetPath()
{
	UPath* _path = NewObject<UPath>();
	if (_path)
		_path->InitPath(PathPoints, PathFollowMode);
	
	return _path;
}

void APathActor::BeginPlay()
{
	Test();
	
	Super::BeginPlay();

	InitPathPoints();
}

void APathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DrawDebugPath();
}

void APathActor::InitPathPoints()
{
	const FVector& _location = GetActorLocation();
	TArray<FVector> _points = PathPoints;

	PathPoints.Empty();
	for (const FVector& _point : _points)
		PathPoints.Add(_location + _point);
}

void APathActor::DrawDebugPath()
{
	if (!bDebug) return;
	
	const UWorld* W = GetWorld();
	const FVector& _location = W->HasBegunPlay() ? FVector::ZeroVector : GetActorLocation();
	TArray<FVector> _points = PathPoints;

	const int _max = _points.Num();
	for (int i = 0; i < _max; ++i)
	{
		const FVector& _point = _points[i];
		DrawDebugSphere(W, _location + _point, 5, 8, DebugColor, false, -1, 0, 1);

		if (i + 1 < _max)
		{
			const FVector& _nextPoint = _points[i + 1];
			DrawDebugSphere(W, _location + _nextPoint, 5, 8, DebugColor, false, -1, 0, 1);
			DrawDebugLine(W, _location + _point, _location + _nextPoint, DebugColor, false, -1, 0, 1);
		}
	}
}

void APathActor::Test_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default Impletementation..."));
}

