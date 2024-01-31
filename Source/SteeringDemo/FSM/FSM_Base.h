#pragma once

#include "CoreMinimal.h"
#include "FSM_Base.generated.h"

class UFSMComponent;

UCLASS()
class STEERINGDEMO_API UFSM_Base : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent) void ReceiveEnter(UFSMComponent* _fsmComponent);
	UFUNCTION(BlueprintNativeEvent) void ReceiveUpdate(UFSMComponent* _fsmComponent);
	UFUNCTION(BlueprintNativeEvent) void ReceiveExit(UFSMComponent* _fsmComponent);
private:
	void ReceiveEnter_Implementation(UFSMComponent* _fsmComponent);
	void ReceiveUpdate_Implementation(UFSMComponent* _fsmComponent);
	void ReceiveExit_Implementation(UFSMComponent* _fsmComponent);

protected:	
	virtual void Enter(UFSMComponent* _fsmComponent);
	virtual void Update(UFSMComponent* _fsmComponent);
	virtual void Exit(UFSMComponent* _fsmComponent);
};
