#pragma once
#include "Psybrus.h"
#include "Math/MaQuat.h"

//////////////////////////////////////////////////////////////////////////
// GaMothershipComponent
class GaMothershipComponent:
	public ScnComponent,
	public ReIObjectNotify
{
public:
	REFLECTION_DECLARE_DERIVED( GaMothershipComponent, ScnComponent );

	GaMothershipComponent();
	virtual ~GaMothershipComponent();

	void update( BcF32 Tick ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;

	void onObjectDeleted( class ReObject* Object ) override;

	void addResources( BcF32 Resources );
	BcBool subResources( BcF32 Resources );
	BcF32 getResources() const { return TotalResources_; }

	class GaAsteroidComponent* findNearestAsteroid( const MaVec3d& Position );
	class GaAsteroidComponent* findAsteroid( std::function< BcF32( class GaAsteroidComponent* ) > HeuristicFunc );

private:
	class ScnEntity* MinerEntity_;
	BcF32 TotalResources_;
	BcF32 TotalHull_;

	MaVec3d TargetPosition_;	
	MaQuat TargetRotation_;

	class ScnCanvasComponent* Canvas_;
	class ScnFontComponent* Font_;
	class ScnViewComponent* View_;

	GaMothershipComponent* Enemy_;
	std::vector< class GaAsteroidComponent* > Asteroids_;
	std::vector< class GaMinerComponent* > Miners_;

	// AI stuff.
	BcF32 AttackWeight_;
	BcF32 MineWeight_;
	BcF32 BuildWeight_;
	BcF32 RepairWeight_;
	BcF32 ReturnWeight_;

public:
	class GaUnitComponent* Unit_;
	class ScnPhysicsRigidBodyComponent* RigidBody_;
};
