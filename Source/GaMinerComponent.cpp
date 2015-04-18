#include "GaMinerComponent.h"
#include "GaAsteroidComponent.h"
#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaMinerComponent );

void GaMinerComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "MaxVelocity_", &GaMinerComponent::MaxVelocity_, bcRFF_IMPORTER ),
		new ReField( "MaxForce_", &GaMinerComponent::MaxForce_, bcRFF_IMPORTER ),

		new ReField( "State_", &GaMinerComponent::State_, bcRFF_TRANSIENT ),
		new ReField( "CirclingTimer_", &GaMinerComponent::CirclingTimer_, bcRFF_TRANSIENT ),
		new ReField( "TargetPosition_", &GaMinerComponent::TargetPosition_, bcRFF_TRANSIENT ),
		new ReField( "Target_", &GaMinerComponent::Target_, bcRFF_TRANSIENT ),
	};
	
	ReRegisterClass< GaMinerComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaMinerComponent::GaMinerComponent():
	State_( State::IDLE ),
	MaxVelocity_( 0.0f ),
	MaxForce_( 0.0f ),
	CirclingTimer_( 0.0f ),
	TargetPosition_( 0.0f, 0.0f, 0.0f ),
	Target_( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaMinerComponent::~GaMinerComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaMinerComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onPhysicsUpdate
void GaMinerComponent::onPhysicsUpdate( BcF32 Tick )
{
	switch( State_ )
	{
	case State::IDLE:
		{

		}
		break;

	case State::MINING:
		{
			TargetPosition_ = Target_->getParentEntity()->getLocalPosition() + MaVec3d( 0.0f, 8.0f, 0.0f );
		}
		break;

	case State::ACCIDENTING:
		{

		}
		break;
	}

	// Move to target position.
	if( Target_ != nullptr )
	{
		auto RigidBody = Target_->getComponentByType< ScnPhysicsRigidBodyComponent >();
		auto Position = RigidBody->getPosition();
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaMinerComponent::onAttach( ScnEntityWeakRef Parent )
{
	auto World = Parent->getComponentAnyParentByType< ScnPhysicsWorldComponent >();
	BcAssert( World );
	World->registerWorldUpdateHandler( this );

	// Mine.
	Parent->subscribe( 0, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();

			PSY_LOG( "GaMinerComponent: Mine event" );

			return evtRET_PASS;
		} );

	// Accident.
	Parent->subscribe( 1, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();

			PSY_LOG( "GaMinerComponent: Accident event" );

			return evtRET_PASS;
		} );

	// Return.
	Parent->subscribe( 1, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();

			PSY_LOG( "GaMinerComponent: Return event" );

			return evtRET_PASS;
		} );


	TargetPosition_ = Parent->getLocalPosition();

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
void GaMinerComponent::onDetach( ScnEntityWeakRef Parent )
{
	auto World = Parent->getComponentAnyParentByType< ScnPhysicsWorldComponent >();
	if( World )
	{
		World->deregisterWorldUpdateHandler( this );
	}
	Super::onDetach( Parent );
}
