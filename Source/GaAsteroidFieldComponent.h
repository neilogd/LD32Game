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


private:
	std::vector< ScnEntity* > AsteroidTemplates_;
	BcU32 NoofAsteroids_;

};
