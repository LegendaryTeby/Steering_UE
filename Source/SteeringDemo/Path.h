#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Path.generated.h"

UENUM()
enum EPathFollowMode
{
	Normal,	//	Begin -> End then stop
	Loop,	//	Begin -> End then go back to Begin -> End...
	Patrol	//	Begin -> End then go reverse		
};

UCLASS()
class STEERINGDEMO_API UPath : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPathCompleted);
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Points = { };

	UPROPERTY(VisibleAnywhere)
	int iFollowDirection = 1;
	
	UPROPERTY(VisibleAnywhere)
	int iCurrentPoint = 0;
	UPROPERTY(VisibleAnywhere)
	float fAcceptance = 10.0f;
	EPathFollowMode FollowMode = EPathFollowMode::Normal;

public:
	FOnPathCompleted OnPathCompleted;
	
public:
	FORCEINLINE FVector GetCurrentPoint() const	{
		if (iCurrentPoint >= 0 && iCurrentPoint < Points.Num())
			return Points[iCurrentPoint];
		return FVector::ZeroVector;
	}
	FORCEINLINE bool IsPathValid() const { return Points.Num() > 0; }
	
	FORCEINLINE void SetFollowMode(const EPathFollowMode _mode) { FollowMode = _mode; }
	FORCEINLINE void SetAcceptance(const float _value) { fAcceptance = _value; }

	void InitPath(const TArray<FVector>& _points, const EPathFollowMode _mode = EPathFollowMode::Normal);
	
	FVector GetPathPoint(const FVector& _location);

	void CheckPathProgression(const FVector& _location);	
};
