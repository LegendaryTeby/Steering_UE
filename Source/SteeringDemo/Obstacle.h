#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class STEERINGDEMO_API AObstacle : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug")
	bool bDebug = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug")
	FColor DebugColor = FColor::Magenta;
	
public:	
	AObstacle();
	
	FVector GetAvoidanceDirection(const FVector& _location) const;
};
