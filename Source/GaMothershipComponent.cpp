#include "GaMothershipComponent.h"
#include "GaAsteroidComponent.h"
#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsEvents.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaMothershipComponent );

void GaMothershipComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "MinerEntity_", &GaMothershipComponent::MinerEntity_, bcRFF_IMPORTER )
	};

	ReRegisterClass< GaMothershipComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaMothershipComponent::GaMothershipComponent():
	MinerEntity_( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaMothershipComponent::~GaMothershipComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaMothershipComponent::update( BcF32 Tick )
{
	auto RigidBody = getComponentByType< ScnPhysicsRigidBodyComponent >();

	// Stabilise ship position.
	auto RigidBodyPosition = RigidBody->getPosition();
	auto Displacement = TargetPosition_ - RigidBodyPosition;
	RigidBody->applyCentralForce( Displacement * RigidBody->getMass() );

	// Stabilise ship rotation.
	auto RigidBodyRotation = RigidBody->getRotation();
	RigidBodyRotation.inverse();
	MaQuat RotationDisplacement = TargetRotation_ * RigidBodyRotation;
	auto EularRotationDisplacement = RotationDisplacement.asEuler();
	RigidBody->applyTorque( EularRotationDisplacement * RigidBody->getMass() * 10.0f );

	// Dampen.
	RigidBody->setLinearVelocity( RigidBody->getLinearVelocity() * 0.99f );
	RigidBody->setAngularVelocity( RigidBody->getAngularVelocity() * 0.99f );

	// Random impulse just to make it look a bit more funky.
	if( BcRandom::Global.randRealRange( 0.0f, 100.0f ) < 1.0f )
	{
		RigidBody->applyImpulse( 
			MaVec3d( 
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ) ) * RigidBody->getMass() * 0.1f,
			MaVec3d( 
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ) ) * 0.5f );
	}
}

//////////////////////////////////////////////////////////////////////////
// update
void GaMothershipComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Build
	Parent->subscribe( 0, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();
			if( Event.SourceUnit_ != Event.TargetUnit_ )
			{
				return evtRET_PASS;
			}

			PSY_LOG( "GaMothershipComponent: Build event." );

			MaMat4d MinerTransform0 = getParentEntity()->getWorldMatrix();
			MinerTransform0.translation( MinerTransform0.translation() + MaVec3d( 0.0f, 5.0f, 0.0f ) );

			BcAssert( MinerEntity_ );
			auto MinerEntity0 = ScnCore::pImpl()->spawnEntity(
			ScnEntitySpawnParams(
				"MinerEntity_0",
				MinerEntity_,
				MinerTransform0,
				getParentEntity()->getParentEntity() ) );

			// Set same team.
			auto MinerUnit = MinerEntity0->getComponentByType< GaUnitComponent >();
			MinerUnit->setTeam( getComponentByType< GaUnitComponent >()->getTeam() );

			return evtRET_PASS;
		} );

	// Repair
	Parent->subscribe( 1, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();
			if( Event.SourceUnit_ != Event.TargetUnit_ )
			{
				return evtRET_PASS;
			}

			PSY_LOG( "GaMothershipComponent: Repair event." );

			return evtRET_PASS;
		} );

	// Collision.
	Parent->subscribe( (EvtID)ScnPhysicsEvents::COLLISION, this,
		[ this, Parent ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< ScnPhysicsEventCollision >();

			PSY_LOG( "GaMothershipComponent: Collision. Take damage." );

			auto OtherEntity = Event.BodyB_->getParentEntity();
			auto AsteroidComponent = OtherEntity->getComponentByType< GaAsteroidComponent >();
			if( AsteroidComponent )
			{
				AsteroidComponent->recycle();
			}
			return evtRET_PASS;
		} );

	// Cache transforms.
	auto WorldMatrix = Parent->getWorldMatrix();
	TargetPosition_ = WorldMatrix.translation();
	TargetRotation_.fromMatrix4d( WorldMatrix );
	Super::onAttach( Parent );
}
