#include "C_Condition.h"

bool UC_Condition::CheckCondition(UFSMComponent* _fsmComponent)
{
	return false;
}

bool UC_Condition::ReceiveCheckCondition_Implementation(UFSMComponent* _fsmComponent)
{
	return CheckCondition(_fsmComponent);
}

