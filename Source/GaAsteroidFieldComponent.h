#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaAsteroidFieldComponent
class GaAsteroidFieldComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaAsteroidFieldComponent, ScnComponent );

	GaAsteroidFieldComponent();
	virtual ~GaAsteroidFieldComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	void recycle( class GaAsteroidComponent* Asteroid, BcBool SetPosition = BcTrue );

private:
	std::vector< ScnEntity* > AsteroidTemplates_;

	BcU32 NoofAsteroids_;
	BcF32 MinVelocity_;
	BcF32 MaxVelocity_;
	BcF32 MinSize_;
	BcF32 MaxSize_;
	BcF32 Width_;
	BcF32 MaxExtents_;
	BcF32 Depth_;
	BcF32 Margin_;

	std::vector< GaAsteroidComponent* > Asteroids_;

	class ScnParticleSystemComponent* ParticlesAdd_;
	class ScnParticleSystemComponent* ParticlesSub_;
};

