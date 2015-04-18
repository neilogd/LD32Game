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


private:
	BcF32 MinSize_;
	BcF32 MaxSize_;
	BcF32 MinVelocity_;
	BcF32 MaxVelocity_;

	BcF32 Size_;
	BcF32 WrapDistance_;
};
