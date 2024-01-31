#include "T_Transition.h"

#include "C_Condition.h"

void UT_Transition::Enter(UFSMComponent* _fsmComponent)
{
	FSMComponent = _fsmComponent;
	CreateConditions(_fsmComponent);
}
bool UT_Transition::CheckTransitionCondition() const
{
	if (Conditions.IsEmpty()) return true;

	const int _max = Conditions.Num();
	for (int i = 0; i < _max; ++i)
	{
		if (UC_Condition* _condition = Conditions[i])
			if (_condition->ReceiveCheckCondition(FSMComponent))
				return true;			
	}
	
	return false;
}
void UT_Transition::Exit(UFSMComponent* _fsmComponent)
{
	FSMComponent = nullptr;
	ClearConditions(_fsmComponent);
}

void UT_Transition::CreateConditions(UFSMComponent* _fsmComponent)
{
	const int _max = TransitionConditions.Num();
	for (int i = 0; i < _max; ++i)
	{
		const TSubclassOf<UC_Condition>& _type = TransitionConditions[i];
		if (!_type.Get()) continue;
		
		if (UC_Condition* _condition = NewObject<UC_Condition>(this, TransitionConditions[i]))
		{
			_condition->ReceiveEnter(_fsmComponent);	
			Conditions.Add(_condition);		
		}
	}	
}
void UT_Transition::ClearConditions(UFSMComponent* _fsmComponent)
{
	const int _max = Conditions.Num();
	for (int i = 0; i < _max; ++i)
	{
		if (UC_Condition* _condition = Conditions[i])
			_condition->ReceiveExit(_fsmComponent);			
	}
	Conditions.Empty();
}
