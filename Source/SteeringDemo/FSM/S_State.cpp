#include "S_State.h"

#include "T_Transition.h"

void US_State::Enter(UFSMComponent* _fsmComponent)
{
	CreateTransitions(_fsmComponent);

	//...
}
void US_State::Update(UFSMComponent* _fsmComponent)
{
	UpdateTransitions();

	//...
}
void US_State::Exit(UFSMComponent* _fsmComponent)
{
	ClearTransitions(_fsmComponent);

	//...
}

void US_State::CreateTransitions(UFSMComponent* _fsmComponent)
{
	const int _max = StateTransitions.Num();
	for (int i = 0; i < _max; ++i)
	{
		const TSubclassOf<UT_Transition>& _type = StateTransitions[i];
		if (!_type.Get()) continue;
		
		if (UT_Transition* _transition = NewObject<UT_Transition>(this, StateTransitions[i]))
		{
			_transition->ReceiveEnter(_fsmComponent);
			Transition.Add(_transition);
		}
	}
}
void US_State::UpdateTransitions()
{
	const int _max = Transition.Num();
	for (int i = 0; i < _max; ++i)
	{
		if (const UT_Transition* _transition = Transition[i])
		{
			if (_transition->CheckTransitionCondition())
			{
				OnNextState.Broadcast(_transition->GetNextState());
				return;
			}
		}
	}
}
void US_State::ClearTransitions(UFSMComponent* _fsmComponent)
{
	const int _max = Transition.Num();
	for (int i = 0; i < _max; ++i)
		if (UT_Transition* _transition = Transition[i])
			_transition->Exit(_fsmComponent);
	Transition.Empty();
}
