#include "F_FSM.h"

void UF_FSM::SetupFSM(UFSMComponent* _fsmComponent)
{
	FSMComponent = _fsmComponent;	
}

void UF_FSM::RunFSM(TSubclassOf<US_State> _initialState)
{
	if (!IsValidFSM() || !_initialState.Get()) return;
	
	StopFSM();
	ToNextState(_initialState);
	StartFSM();
}

void UF_FSM::StartFSM()
{
	if (!IsValidFSM()) return;
	
	bStarted = true;
}
void UF_FSM::UpdateFSM(const float _deltaTime)
{
	if (!bStarted) return;
	
	if (IsValidFSM() && IsValidState())
	{
		CurrentState->ReceiveUpdate(FSMComponent);
	}
}
void UF_FSM::StopFSM()
{
	bStarted = false;
}

void UF_FSM::ToNextState(TSubclassOf<US_State> _newState)
{
	if (!IsValidFSM() || !_newState.Get()) return;
	
	if (IsValidState())
	{
		CurrentState->OnNextState.RemoveDynamic(this, &UF_FSM::ToNextState);
		CurrentState->ReceiveExit(FSMComponent);
		CurrentState = nullptr;
	}

	CurrentState = NewObject<US_State>(this, _newState);
	CurrentState->OnNextState.AddDynamic(this, &UF_FSM::ToNextState);
	CurrentState->ReceiveEnter(FSMComponent);
}

