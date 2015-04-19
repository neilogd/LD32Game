#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaAsteroidComponent
class GaAsteroidComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaAsteroidComponent, ScnComponent );

	GaAsteroidComponent();
	virtual ~GaAsteroidComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;

	void recycle();

	void setSize( BcF32 Size ) { Size_ = Size; }
	BcF32 getSize() const { return Size_; }

	MaVec3d getPosition() const { return getParentEntity()->getWorldPosition(); }

private:
	BcF32 MassSizeRatio_;
	BcF32 Size_;
	BcF32 OldSize_;

public:
	class GaUnitComponent* Unit_;
	class ScnPhysicsCollisionComponent* Collision_;
	class ScnPhysicsRigidBodyComponent* RigidBody_;
	class ScnModelComponent* Model_;
};
