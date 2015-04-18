#pragma once
#include "Psybrus.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"

//////////////////////////////////////////////////////////////////////////
// GaMinerComponent
class GaMinerComponent:
	public ScnComponent,
	public ScnIPhysicsWorldUpdate
{
public:
	REFLECTION_DECLARE_DERIVED( GaMinerComponent, ScnComponent );

	GaMinerComponent();
	virtual ~GaMinerComponent();

	void update( BcF32 Tick ) override;
	void onPhysicsUpdate( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;


private:



};
