#include "FSMComponent.h"

#include "F_FSM.h"

UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	InitFSMComponent();
}
void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FSM)
	{
		FSM->UpdateFSM(DeltaTime);
	}
}

void UFSMComponent::InitFSMComponent()
{
	ControlledActor = GetOwner();
	FSM = NewObject<UF_FSM>(this);
	FSM->SetupFSM(this);
	
	FSM->RunFSM(InitialState);
}

