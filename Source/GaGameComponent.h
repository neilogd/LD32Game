#pragma once
#include "Psybrus.h"
#include "System/Os/OsEvents.h"

//////////////////////////////////////////////////////////////////////////
// GaGameComponent
class GaGameComponent:
	public ScnComponent,
	public ReIObjectNotify
{
public:
	REFLECTION_DECLARE_DERIVED( GaGameComponent, ScnComponent );

	GaGameComponent();
	virtual ~GaGameComponent();

	virtual void update( BcF32 Tick );

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	void onObjectDeleted( class ReObject* Object ) override;

private:
	class ScnEntity* MothershipEntity_;
	class ScnMaterial* UIMaterial_;
	class ScnMaterialComponent* UIMaterialComponent_;

	class ScnViewComponent* View_;
	class ScnPhysicsWorldComponent* World_;
	class ScnCanvasComponent* Canvas_;

	class GaUnitComponent* SelectedUnit_;

	std::vector< std::pair< EvtID, OsEventInputMouse > > MouseEvents_;

};

