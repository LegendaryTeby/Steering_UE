#include "Path.h"

void UPath::InitPath(const TArray<FVector>& _points, const EPathFollowMode _mode)
{
	Points = _points;
	FollowMode = _mode;

	iFollowDirection = 1;
	iCurrentPoint = 0;
}

FVector UPath::GetPathPoint(const FVector& _location)
{
	CheckPathProgression(_location);
	
	if (!IsPathValid()) return _location;
	
	return Points[iCurrentPoint];		
}

void UPath::CheckPathProgression(const FVector& _location)
{
	if (!IsPathValid()) return;
	
	const FVector& _currentPoint = Points[iCurrentPoint];
	const float _distance = FVector::Dist(_location, _currentPoint);

	if (_distance < fAcceptance)
	{
		iCurrentPoint += iFollowDirection;

		if (iCurrentPoint >= Points.Num() || iCurrentPoint < 0)
		{
			if (FollowMode == EPathFollowMode::Normal)
			{
				Points.Empty();
				OnPathCompleted.Broadcast();	
			}
			if (FollowMode == EPathFollowMode::Loop)
			{
				iCurrentPoint = 0;
			}
			if (FollowMode == EPathFollowMode::Patrol)
			{
				iFollowDirection *= -1;
				iCurrentPoint += iFollowDirection;
			}
		}
	}
}
