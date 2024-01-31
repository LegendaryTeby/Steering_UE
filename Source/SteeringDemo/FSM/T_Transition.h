#pragma once

#include "CoreMinimal.h"
#include "FSM_Base.h"
#include "S_State.h"
#include "T_Transition.generated.h"

class UC_Condition;

UCLASS(Blueprintable, BlueprintType)
class STEERINGDEMO_API UT_Transition : public UFSM_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Transition Settings")
	TSubclassOf<US_State> NextState = TSubclassOf<US_State>(US_State::StaticClass());
	UPROPERTY(EditAnywhere, Category = "Transition Settings")
	TArray<TSubclassOf<UC_Condition>> TransitionConditions = { };	

	UPROPERTY(VisibleAnywhere, Category = "Transition Values")
	TArray<UC_Condition*> Conditions = { };

	UPROPERTY()
	UFSMComponent* FSMComponent = nullptr;
	
public:
    FORCEINLINE const TSubclassOf<US_State>& GetNextState() const { return NextState; }
	    
public:
	virtual void Enter(UFSMComponent* _fsmComponent) override;
	virtual void Exit(UFSMComponent* _fsmComponent) override;
	
	bool CheckTransitionCondition() const;
	
private:
	void CreateConditions(UFSMComponent* _fsmComponent);
	void ClearConditions(UFSMComponent* _fsmComponent);
};
