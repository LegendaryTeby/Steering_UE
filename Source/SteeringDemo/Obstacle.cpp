#include "Obstacle.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = false;
}

FVector AObstacle::GetAvoidanceDirection(const FVector& _location) const
{
	return (_location - GetActorLocation()).GetSafeNormal2D();
}
