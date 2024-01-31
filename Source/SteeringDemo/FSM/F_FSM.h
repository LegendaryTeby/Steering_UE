#pragma once

#include "CoreMinimal.h"
#include "S_State.h"
#include "F_FSM.generated.h"

UCLASS()
class STEERINGDEMO_API UF_FSM : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UFSMComponent* FSMComponent = nullptr;
	UPROPERTY(VisibleAnywhere)
	US_State* CurrentState = nullptr;

	bool bStarted = false;	
public:
	FORCEINLINE bool IsValidFSM() const { return FSMComponent != nullptr; }
	FORCEINLINE bool IsValidState() const { return CurrentState != nullptr; }
	
public:
	void SetupFSM(UFSMComponent* _fsmComponent);

	//Start FSM with the given State 
	void RunFSM(TSubclassOf<US_State> _initialState);
	
	void StartFSM();
	void UpdateFSM(const float _deltaTime);
	void StopFSM();

protected:
	UFUNCTION() void ToNextState(TSubclassOf<US_State> _newState); 
};
