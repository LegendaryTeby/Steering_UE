#pragma once

#include "CoreMinimal.h"
#include "S_State.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

class UF_FSM;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class STEERINGDEMO_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "FSM Component|Settings")
	TSubclassOf<US_State> InitialState = TSubclassOf<US_State>(US_State::StaticClass());
	
	UPROPERTY(VisibleAnywhere, Category = "FSM Component|Values")
	UF_FSM* FSM = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "FSM Component|Values")
	AActor* ControlledActor = nullptr;

public:
	UFUNCTION(BlueprintCallable) FORCEINLINE AActor* GetControlledActor() const { return ControlledActor; }
	
public:	
	UFSMComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitFSMComponent();

	UFUNCTION(BlueprintCallable) void TestFunc() { UE_LOG(LogTemp, Warning, TEXT("AAAAAAAAAAAAAAAAAAAAAAAAAA")); }
};
