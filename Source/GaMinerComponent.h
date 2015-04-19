#pragma once
#include "Psybrus.h"
#include "Math/MaQuat.h"

//////////////////////////////////////////////////////////////////////////
// GaMinerComponent
class GaMinerComponent:
	public ScnComponent,
	public ReIObjectNotify
{
public:
	REFLECTION_DECLARE_DERIVED( GaMinerComponent, ScnComponent );

	GaMinerComponent();
	virtual ~GaMinerComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	void onObjectDeleted( class ReObject* Object ) override;

	BcBool isIdle() const { return State_ == State::IDLE; }
	BcBool isMining() const { return State_ == State::MINING; }
	BcBool isAttacking() const { return State_ == State::ACCIDENTING; }
	BcBool isReturning() const { return State_ == State::RETURNING; }

	MaVec3d getPosition() const { return getParentEntity()->getWorldPosition(); }

	BcBool isFull( BcF32 Fraction ) const { return ( AmountMined_ ) >= ( MaxCapacity_ * Fraction ); }

private:
	BcF32 MaxVelocity_;
	BcF32 MaxForce_;
	BcF32 MiningDistance_;
	BcF32 MiningRate_;
	BcF32 MaxCapacity_;
	BcF32 MaxExtents_;
	BcF32 MiningSizeThreshold_;

	BcBool ReturningLastFrame_;
	BcBool MiningLastFrame_;
	BcBool ThrustingLastFrame_;

	MaQuat TargetRotation_;

	enum class State
	{
		IDLE,
		MINING,
		ACCIDENTING,
		RETURNING,
	};

	State State_;

	BcF32 CirclingTimer_;
	MaVec3d TargetPosition_;

	void setTarget( class GaUnitComponent* Target );

	class GaUnitComponent* Target_;
	class GaAsteroidComponent* TargetAsteroid_;
	class GaMothershipComponent* TargetMothership_;
	class ScnPhysicsRigidBodyComponent* TargetRigidBody_;

	BcF32 AmountMined_;

	class ScnParticleSystemComponent* ParticlesAdd_;
	class ScnParticleSystemComponent* ParticlesSub_;
	struct ScnParticle* ShadowParticle_;

	BcF32 PulseTimer_;

	class ScnCanvasComponent* Canvas_;
	class ScnViewComponent* View_;

public:
	class GaUnitComponent* Unit_;
	class ScnPhysicsRigidBodyComponent* RigidBody_;
};
