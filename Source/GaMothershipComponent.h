#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaMothershipComponent
class GaMothershipComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaMothershipComponent, ScnComponent );

	GaMothershipComponent();
	virtual ~GaMothershipComponent();

	virtual void update( BcF32 Tick );
};
