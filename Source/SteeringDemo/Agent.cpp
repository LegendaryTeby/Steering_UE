#include "Agent.h"

#include "Obstacle.h"
#include "Utils.h"

#include "Kismet/KismetSystemLibrary.h"

AAgent::AAgent()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

void AAgent::BeginPlay()
{
	Super::BeginPlay();

	//Velocity = GetActorForwardVector().GetSafeNormal2D2D2D2D2D() * fMaxSpeed;
}

void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AgentState == EAgentState::PathFollowing && PreviousAgentState != EAgentState::PathFollowing)
		InitPathFollowing();
	if (AgentState == EAgentState::LeaderFollowing && PreviousAgentState != EAgentState::LeaderFollowing)
		InitLeaderFollowing();
	
	Steering = FVector::ZeroVector;
	Steering += GetSteeringFromAgentState();
	Steering += CheckForObstacle();

	Steering = ClampVector(Steering, fMaxSteeringForce);
	Steering = Steering / fMass;
	
	Velocity = ClampVector(Velocity + Steering, fMaxSpeed);
	
	const FVector& _location = GetActorLocation();
	const FVector& _newLocation = _location + Velocity * DeltaTime;	
	SetActorLocation(_newLocation);

	PreviousAgentState = AgentState;
	
	DrawAgentDebug();
}

FVector AAgent::GetSteeringFromAgentState()
{
	if (AgentState == EAgentState::None)
	{
		return Velocity = FVector::ZeroVector;
	}
	if (AgentState == EAgentState::Wander)
	{
		return Wander();
	}
	if (AgentState == EAgentState::Seek)
	{
		const FVector& _targetLocation = Target ? Target->GetActorLocation() : GetActorLocation();
		return GetSteeringFromDesiredVelocity(Seek(_targetLocation));		
	}
	if (AgentState == EAgentState::Flee)
	{
		const FVector& _targetLocation = Target ? Target->GetActorLocation() : GetActorLocation();
		return GetSteeringFromDesiredVelocity(Flee(_targetLocation));		
	}
	if (AgentState == EAgentState::Pursuit)
	{
		return GetSteeringFromDesiredVelocity(Pursuit(TargetAgent));
	}
	if (AgentState == EAgentState::Evade)
	{
		return GetSteeringFromDesiredVelocity(Evade(TargetAgent));		
	}
	if (AgentState == EAgentState::PathFollowing)
	{
		return GetSteeringFromDesiredVelocity(Seek(FollowPath()));
	}
	if (AgentState == EAgentState::LeaderFollowing)
	{
		return GetSteeringFromDesiredVelocity(Seek(FollowLeader()));
	}

	return FVector::ZeroVector;
}

FVector AAgent::GetSteeringFromDesiredVelocity(const FVector& _desiredVelocity) const
{
	return _desiredVelocity - Velocity;
}

#pragma region Seek/Flee Behavior
FVector AAgent::Seek(const FVector& _seekLocation) const
{
	const float _distance = FVector::Distance(GetActorLocation(), _seekLocation);
	FVector _velocity = (_seekLocation - GetActorLocation()).GetSafeNormal2D() * fMaxSpeed;
	
	if (_distance < fSlowRange)
		_velocity *= _distance / fSlowRange;
	
	return _velocity;	
}
FVector AAgent::Flee(const FVector& _fleeLocation) const
{
	return (GetActorLocation() - _fleeLocation).GetSafeNormal2D() * fMaxSpeed;
}
#pragma endregion

#pragma region Pursuit/Evade Behavior
FVector AAgent::Evade(const AAgent* _agent) const
{
	return Flee(GetPredictedLocation(_agent));
}
FVector AAgent::Pursuit(const AAgent* _agent) const
{
	return Seek(GetPredictedLocation(_agent));
}

FVector AAgent::GetPredictedLocation(const AAgent* _agent) const
{
	if (!_agent) return GetActorLocation();
	const FVector& _loc = _agent->GetActorLocation(); 
	const FVector& _vel = _agent->GetAgentVelocity();

	return _loc + _vel * fPrediction;
}
#pragma endregion

#pragma region Wander
FVector AAgent::Wander()
{
	fWanderAngle += FMath::RandRange(-fWanderAngleRandom, fWanderAngleRandom);
	const FVector& _wanderCircle = Velocity.GetSafeNormal2D() * fWanderDistance;
	const FVector& _displacement = GetWanderDisplacement(FMath::DegreesToRadians(fWanderAngle));
	const FVector& _wander = _wanderCircle + _displacement;

	return _wander;
}

FVector AAgent::GetWanderDisplacement(const float _angle) const
{
	const float x = FMath::Cos(_angle) * fWanderRadius;
	const float y = FMath::Sin(_angle) * fWanderRadius;
	return FVector(x, y, 0);
}
#pragma endregion

#pragma region Avoidance
FVector AAgent::CheckForObstacle() 
{
	FVector _avoidance = FVector::ZeroVector;
	const FVector& _location = GetActorLocation();
	const FVector& _direction = Velocity.GetSafeNormal2D() * fForwardAvoidance;
	const FVector& _endLocation = _location + _direction;

	FHitResult _result;
	const TArray<AActor*>& _ignore = { this /*, TargetAgent*/ };
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _location, _endLocation, AvoidanceObjectTypes, false, _ignore, EDrawDebugTrace::None, _result, true))
	{
		const AActor* _hitActor = _result.GetActor();
		if (const AObstacle* _obstacle = Cast<AObstacle>(_hitActor))
		{
			_avoidance = _obstacle->GetAvoidanceDirection(_endLocation);
		}
		
		if (const AAgent* _agent = Cast<AAgent>(_hitActor))
		{
			if (!IsLeader())
				return Brake(_agent);
		}
	}

	return _avoidance * fMaxAvoidanceForce;
}

FVector AAgent::Brake(const AAgent* _agent)
{
	FVector _brake = FVector::ZeroVector;
	const FVector& _location = GetActorLocation();
	const FVector& _queueHead = _location + Velocity.GetSafeNormal2D() * fQueueAhead;
	const FVector& _agentLocation = _agent->GetActorLocation();
	
	if (FVector::Distance(_queueHead, _agentLocation) < fMaxQueueRadius)	//	If Agent Head is too close from given Agent 
		_brake = -Steering * fBrakeForce + -Velocity;						//	Brake 

	if (FVector::Distance(_location, _agentLocation) < fMaxQueueRadius)		//	If Agent if too close from Given Agent
		Velocity *= fHardStopForce;											//	Hard Stop
	
	return _brake;
}

void AAgent::InitPathFollowing()
{
	Path = nullptr;
	if (!TargetPath) return;
	Path = TargetPath->GetPath();
}
#pragma endregion

#pragma region Path Following
FVector AAgent::FollowPath() const
{
	if (!IsPathValid()) return FVector::ZeroVector;

	return Path->GetPathPoint(GetActorLocation());
}

void AAgent::InitLeaderFollowing()
{
	if (!TargetAgent) return;
	TargetAgent->AddFollower(this);
}
#pragma endregion

#pragma region Leader Following
FVector AAgent::FollowLeader() const
{
	if (!TargetAgent) return Seek(GetActorLocation());

	const float _leaderRadius = TargetAgent->GetAgentSeparationRadius();
	const FVector& _leaderLocation = TargetAgent->GetActorLocation();
	const FVector& _leaderVelocity = TargetAgent->GetAgentVelocity().GetSafeNormal2D();
	const FVector& _behind = _leaderLocation + -_leaderVelocity * fLeaderFollowDistance;
	const FVector& _ahead = _leaderLocation + _leaderVelocity * fLeaderFollowDistance;
	
	FVector _force = _behind;
	
	if (FVector::Distance(GetActorLocation(), _ahead) < _leaderRadius || FVector::Distance(GetActorLocation(), _leaderLocation) < _leaderRadius)
		_force += Evade(TargetAgent);
	
	_force += Separation();
	
	return _force;
}

FVector AAgent::Separation() const
{
	if (!TargetAgent) return FVector::ZeroVector;
	const FVector& _location = GetActorLocation();

	const float _leaderRadius = TargetAgent->GetAgentSeparationRadius(); 
	const TArray<AAgent*> _neighbors = TargetAgent->GetAgentFollowers();
	FVector _separation = FVector::ZeroVector;
	
	const int _max = _neighbors.Num();
	for (int i = 0; i < _max; ++i)
	{
		if (const AAgent* _agent = _neighbors[i])
		{
			const FVector& _neighborLocation = _agent->GetActorLocation();
			if (_agent == this || FVector::Distance(_location, _neighborLocation) > _leaderRadius) continue;
			_separation += _neighborLocation - _location;
		}
	}

	if (_max > 0)
	{
		_separation /= _max;
		_separation *= -1;	
	}
	
	return _separation.GetSafeNormal2D() * (_leaderRadius * _max);
}
#pragma endregion

void AAgent::DrawAgentDebug() const
{
	if (!bDebug) return;
	constexpr float _thickness = 1;
	
	const UWorld* W = GetWorld();
	const FVector& _location = GetActorLocation();
	
	DrawDebugSphere(W, _location, 10, 8, AgentDebugColor, false, -1, 0, 2);
	
	DrawDebugDirectionalArrow(W, _location, _location + Velocity, FVector::Distance(_location, _location + Velocity), VelocityDebugColor, false, -1, 0, _thickness);
	DrawDebugDirectionalArrow(W, _location + FVector(0, 0, -25), _location + Velocity.GetSafeNormal2D() * fMaxSpeed + FVector(0, 0, -25), fMaxSpeed, MaxVelocityDebugColor, false, -1, 0, _thickness);
	
	DrawDebugDirectionalArrow(W, _location, _location + Velocity.GetSafeNormal2D() * fForwardAvoidance, fForwardAvoidance, AvoidanceDebugColor, false, -1, 0, _thickness);

	if (AgentState == EAgentState::Wander)
	{
		const FVector& _wanderCircle = _location + Velocity.GetSafeNormal2D() * fWanderDistance;
		const FVector& _displacement = GetWanderDisplacement(FMath::DegreesToRadians(fWanderAngle));
		const FVector& _wanderLocation = _wanderCircle + _displacement;
		DrawDebugCircle(W, _wanderCircle, fWanderRadius, 25, WanderDebugColor, false, -1, 0, _thickness, FVector(0, 1, 0), FVector(1, 0, 0));
		DrawDebugLine(W, _location, _wanderLocation, WanderDebugColor, false, -1, 0, _thickness);
	}
	if (AgentState == EAgentState::Seek || AgentState == EAgentState::Flee)
	{
		const FVector& _targetLocation = Target ? Target->GetActorLocation() : GetActorLocation();
		const FVector& _desiredVel = AgentState == EAgentState::Seek ? Seek(_targetLocation) : Flee(_targetLocation);
		DrawDebugLine(W, _location, _location + _desiredVel, AgentDebugColor, false, -1, 0, _thickness);
	}
	if (AgentState == EAgentState::Pursuit || AgentState == EAgentState::Evade)
	{
		const FVector& _predictedLocation = GetPredictedLocation(TargetAgent);
		const FVector& _desiredVel = AgentState == EAgentState::Pursuit ? Seek(_predictedLocation) : Flee(_predictedLocation);

		if (AgentState == EAgentState::Pursuit)
		{
			DrawDebugSphere(W, _predictedLocation, 5, 8, AgentDebugColor, false, -1, 0, _thickness / 2.0f);
			DrawDebugLine(W, _location, _predictedLocation, AgentDebugColor, false, -1, 0, _thickness / 2.0f);
		}
		DrawDebugDirectionalArrow(W, _location, _location + _desiredVel, FVector::Dist(_location, _location + _desiredVel), AgentDebugColor, false, -1, 0, _thickness / 2.0f);
	}
	if (AgentState == EAgentState::PathFollowing && IsPathValid())
	{
		const FVector& _pointLocation = Path->GetPathPoint(_location);

		DrawDebugSphere(W, _pointLocation, 5, 8, PathDebugColor, false, -1, 0, _thickness / 2.0f);
		DrawDebugLine(W, _location, _pointLocation, PathDebugColor, false, -1, 0, _thickness / 2.0f);
	}
	if (AgentState == EAgentState::LeaderFollowing)
	{
		const FVector& _leaderLocation = TargetAgent ? TargetAgent->GetActorLocation() + (-TargetAgent->GetAgentVelocity()).GetSafeNormal2D() * fLeaderFollowDistance: _location;

		DrawDebugLine(W, _location, _leaderLocation, AgentDebugColor, false, -1, 0, _thickness);		
	}
	

	if (AgentState == EAgentState::Seek || AgentState == EAgentState::Pursuit || AgentState == EAgentState::PathFollowing)
	{
		//	Get Target Location (Seek: Target | Pursuit: TargetAgent | Path: Point)
		const FVector& _targetLocation = AgentState == EAgentState::Seek ? Target ? Target->GetActorLocation() : _location : AgentState == EAgentState::Pursuit ? TargetAgent ? TargetAgent->GetActorLocation() : _location : IsPathValid() ? Path->GetCurrentPoint() : _location;    
		const float _distance = FVector::Dist(_location, _targetLocation);
		if (_distance < fSlowRange)
			DrawDebugSphere(W, _location, fSlowRange, 16, SlowRangeDebugColor, false, -1, 0, _thickness);
	}
}