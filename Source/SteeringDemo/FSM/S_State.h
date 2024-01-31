#pragma once

#include "CoreMinimal.h"
#include "FSM_Base.h"
#include "S_State.generated.h"

class UT_Transition;

UCLASS(Blueprintable, BlueprintType)
class STEERINGDEMO_API US_State : public UFSM_Base
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextState, TSubclassOf<US_State>, _state);
	FOnNextState OnNextState;

protected:
	UPROPERTY(EditAnywhere, Category = "State Settings")
	TArray<TSubclassOf<UT_Transition>> StateTransitions = { };

	UPROPERTY(VisibleAnywhere, Category = "State Values")
	TArray<UT_Transition*> Transition = { };

public:
	virtual void Enter(UFSMComponent* _fsmComponent) override;
	virtual void Update(UFSMComponent* _fsmComponent) override;
	virtual void Exit(UFSMComponent* _fsmComponent) override;

protected:
	void CreateTransitions(UFSMComponent* _fsmComponent);
	void UpdateTransitions();
	void ClearTransitions(UFSMComponent* _fsmComponent);
};
