#pragma once

#include "CoreMinimal.h"
#include "PathActor.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UENUM()
enum EAgentState
{
	None,
	Wander,
	Seek,
	Flee,
	Pursuit,
	Evade,
	PathFollowing,
	LeaderFollowing
};

UCLASS()
class STEERINGDEMO_API AAgent : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	TEnumAsByte<EAgentState> AgentState = EAgentState::Wander;
	
	EAgentState PreviousAgentState = EAgentState::None; 
	
	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	AAgent* TargetAgent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	AActor* Target = nullptr;
	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	APathActor* TargetPath = nullptr;
	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	float fMass = 10;
	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	float fMaxSpeed = 250;
	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	float fMaxSteeringForce = 10;
	
	UPROPERTY(EditAnywhere, Category = "Agent|Seek")
	float fSlowRange = 75;
	
	UPROPERTY(EditAnywhere, Category = "Agent|Wander")
	float fWanderDistance = 100;
	UPROPERTY(EditAnywhere, Category = "Agent|Wander")
	float fWanderRadius = 50;
	UPROPERTY(EditAnywhere, Category = "Agent|Wander")
	float fWanderAngleRandom = 5;
	
	UPROPERTY(EditAnywhere, Category = "Agent|Pursuit")
	float fPrediction = 1;
	
	UPROPERTY(EditAnywhere, Category = "Agent|Leader")
	float fLeaderFollowDistance = 50;
	UPROPERTY(EditAnywhere, Category = "Agent|Leader")
	float fSeparationRadius = 75;
	
	UPROPERTY(EditAnywhere, Category = "Agent|Queue")
	float fQueueAhead = 50;
	UPROPERTY(EditAnywhere, Category = "Agent|Queue")
	float fMaxQueueRadius = 25;
	UPROPERTY(EditAnywhere, Category = "Agent|Queue", meta = (ClampMin = "0.1", ClampMax = "1"))
	float fBrakeForce = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Agent|Queue", meta = (ClampMin = "0.1", ClampMax = "1"))
	float fHardStopForce = 0.3f;
	
	UPROPERTY(EditAnywhere, Category = "Agent|Settings")
	TArray<TEnumAsByte<EObjectTypeQuery>> AvoidanceObjectTypes { };
	UPROPERTY(EditAnywhere, Category = "Agent|Avoidance")
	float fForwardAvoidance = 150;
	UPROPERTY(EditAnywhere, Category = "Agent|Avoidance")
	float fMaxAvoidanceForce = 500;
	
	UPROPERTY(VisibleAnywhere, Category = "Agent|Values")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, Category = "Agent|Values")
	FVector Steering = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Agent|Values")
	float fWanderAngle = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Agent|Values")
	TArray<AAgent*> Followers = { };
	
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	bool bDebug = true;
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	FColor AgentDebugColor = FColor::Blue;	
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	FColor MaxVelocityDebugColor = FColor::Blue;
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	FColor VelocityDebugColor = FColor::Yellow;
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	FColor SlowRangeDebugColor = FColor::Cyan;
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	FColor WanderDebugColor = FColor::Cyan;
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	FColor AvoidanceDebugColor = FColor::Red;
	UPROPERTY(EditAnywhere, Category = "Agent|Debug")
	FColor PathDebugColor = FColor::Green;

	UPROPERTY()
	UPath* Path = nullptr; 
	
public:
	FORCEINLINE bool HaveTarget() const { return Target || TargetAgent; }
	
	FORCEINLINE const FVector& GetAgentVelocity() const { return Velocity; }
	FORCEINLINE const TEnumAsByte<EAgentState>& GetAgentState() const { return AgentState; }

	FORCEINLINE bool IsLeader() const { return Followers.Num() > 0; }
	FORCEINLINE const float& GetAgentSeparationRadius() const { return fSeparationRadius; }
	FORCEINLINE const TArray<AAgent*>& GetAgentFollowers() const { return Followers; }

	FORCEINLINE bool IsPathValid() const { return Path != nullptr; }
	
public:	
	AAgent();

	FORCEINLINE void AddFollower(AAgent* _agent)	{
		if (!_agent || Followers.Contains(_agent)) return;
		Followers.Add(_agent);
	}
	FORCEINLINE void RemoveFollower(AAgent* _agent)	{
		if (!_agent || !Followers.Contains(_agent)) return;
		Followers.Remove(_agent);
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FVector GetSteeringFromAgentState();

	FVector GetSteeringFromDesiredVelocity(const FVector& _desiredVelocity) const;
	
#pragma region Seek/Flee
	FVector Seek(const FVector& _seekLocation) const;
	FVector Flee(const FVector& _fleeLocation) const;
#pragma endregion

#pragma region Pursuit/Evade
	FVector Evade(const AAgent* _agent) const;
	FVector Pursuit(const AAgent* _agent) const;

	FVector GetPredictedLocation(const AAgent* _agent) const;
#pragma endregion
	
#pragma region Wander
	FVector Wander();
	FVector GetWanderDisplacement(const float _angle) const;
#pragma endregion

#pragma region Avoidance
	FVector CheckForObstacle();

	FVector Brake(const AAgent* _agent);
#pragma endregion

#pragma region Path Following
	void InitPathFollowing();

	FVector FollowPath() const;
#pragma endregion
	
#pragma region Leader Following
	void InitLeaderFollowing();
	
	FVector FollowLeader() const;
	FVector Separation() const;
#pragma endregion

#pragma region Debug
	void DrawAgentDebug() const;
#pragma endregion
};
