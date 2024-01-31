#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class STEERINGDEMO_API ATarget : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector2D RandomRange =FVector2D(500, 500); 
	UPROPERTY(EditAnywhere, Category = "Settings")
	float fRandomRate = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Debug")
	FColor DebugColor = FColor::Silver;

	FVector Center = FVector(0);
	
public:	
	ATarget();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION() void ChooseRandomLocation();

	FVector GetRandomLocation() const;
};
