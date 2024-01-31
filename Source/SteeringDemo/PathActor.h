#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Path.h"

#include "PathActor.generated.h"

UCLASS()
class STEERINGDEMO_API APathActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Path")
	TArray<FVector> PathPoints = { FVector(0) };
	UPROPERTY(EditAnywhere, Category = "Path")
	TEnumAsByte<EPathFollowMode> PathFollowMode = EPathFollowMode::Normal;  
	
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug = true;
	UPROPERTY(EditAnywhere, Category = "Debug")
	FColor DebugColor = FColor::Purple;
	
public:	
	FORCEINLINE virtual bool ShouldTickIfViewportsOnly() const override { return bDebug; }
	
public:	
	APathActor();

	UPath* GetPath();
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void InitPathPoints();
	
	void DrawDebugPath();


public:
	
	UFUNCTION(BlueprintNativeEvent) void Test();
	void Test_Implementation();
	
};
