#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaSceneComponent
class GaSceneComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaSceneComponent, ScnComponent );

	GaSceneComponent();
	virtual ~GaSceneComponent();

	virtual void update( BcF32 Tick );
};
