#pragma once
#include "Psybrus.h"
#include "System/Os/OsEvents.h"

//////////////////////////////////////////////////////////////////////////
// GaGameIcon
enum class GaGameIcon : BcU32
{
	LMB = 0,
	RMB,
	SELECT,
	DRILL
};

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

	class GaUnitComponent* getUnitAt( MaVec2d& MousePosition );
	const struct GaUnitAction* getAction( BcU32 MouseButton, GaUnitComponent* TargetUnit );

	BcBool isUnitSelectable( class GaUnitComponent* Unit );

	

private:
	class ScnEntity* MothershipEntity_;
	class ScnMaterial* UIMaterial_;
	class ScnMaterialComponent* UIMaterialComponent_;

	class ScnViewComponent* View_;
	class ScnPhysicsWorldComponent* World_;
	class ScnCanvasComponent* Canvas_;

	class GaMothershipComponent* PlayerShip_;
	class GaMothershipComponent* EnemyShip_;

	MaVec2d MousePosition_;

	class GaUnitComponent* SelectedUnit_;

	std::vector< std::pair< EvtID, OsEventInputMouse > > MouseEvents_;

};

