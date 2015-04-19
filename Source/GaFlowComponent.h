#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaFlowComponent
class GaFlowComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaFlowComponent, ScnComponent );

	GaFlowComponent();
	virtual ~GaFlowComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

private:
	ScnEntity* CurrentEntity_;
};
 