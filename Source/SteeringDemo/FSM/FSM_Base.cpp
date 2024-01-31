#include "FSM_Base.h"

void UFSM_Base::ReceiveEnter_Implementation(UFSMComponent* _fsmComponent)
{
	Enter(_fsmComponent);
}
void UFSM_Base::ReceiveUpdate_Implementation(UFSMComponent* _fsmComponent)
{
	Update(_fsmComponent);
}
void UFSM_Base::ReceiveExit_Implementation(UFSMComponent* _fsmComponent)
{
	Exit(_fsmComponent);	
}

void UFSM_Base::Enter(UFSMComponent* _fsmComponent)
{
	
}
void UFSM_Base::Update(UFSMComponent* _fsmComponent)
{
	
}
void UFSM_Base::Exit(UFSMComponent* _fsmComponent)
{
	
}