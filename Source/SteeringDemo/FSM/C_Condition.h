#pragma once

#include "CoreMinimal.h"
#include "FSM_Base.h"
#include "C_Condition.generated.h"

UCLASS(Blueprintable, BlueprintType)
class STEERINGDEMO_API UC_Condition : public UFSM_Base
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent) bool ReceiveCheckCondition(UFSMComponent* _fsmComponent);
private:
	bool ReceiveCheckCondition_Implementation(UFSMComponent* _fsmComponent);

protected:
	virtual bool CheckCondition(UFSMComponent* _fsmComponent);
};
