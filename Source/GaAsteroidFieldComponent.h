#pragma once
#include "Psybrus.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"

//////////////////////////////////////////////////////////////////////////
// GaAsteroidFieldComponent
class GaAsteroidFieldComponent:
	public ScnComponent,
	public ScnIPhysicsWorldUpdate
{
public:
	REFLECTION_DECLARE_DERIVED( GaAsteroidFieldComponent, ScnComponent );

	GaAsteroidFieldComponent();
	virtual ~GaAsteroidFieldComponent();

	void update( BcF32 Tick ) override;
	void onPhysicsUpdate( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;


private:
	std::vector< ScnEntity* > AsteroidTemplates_;
	BcU32 NoofAsteroids_;
	BcF32 MinVelocity_;
	BcF32 MaxVelocity_;
	BcF32 MinSize_;
	BcF32 MaxSize_;
	BcF32 Width_;
	BcF32 Height_;
	BcF32 Depth_;

	std::vector< ScnEntity* > Asteroids_;
};

