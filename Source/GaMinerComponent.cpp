#include "GaMinerComponent.h"
#include "GaAsteroidComponent.h"
#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"

#include "Base/BcMath.h"
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
		new ReField( "MiningDistance_", &GaMinerComponent::MiningDistance_, bcRFF_IMPORTER ),

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
	MiningDistance_( 4.0 ),
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
	switch( State_ )
	{
	case State::IDLE:
		{

		}
		break;

	case State::MINING:
		{
			auto TargetRigidBody = Target_->getComponentByType< ScnPhysicsRigidBodyComponent >();
			TargetPosition_ = TargetRigidBody->getPosition() + MaVec3d( BcCos( CirclingTimer_ ), 1.0f, BcSin( CirclingTimer_ ) ).normal() * MiningDistance_;
		}
		break;

	case State::ACCIDENTING:
		{

		}
		break;

	case State::RETURNING:
		{
			auto TargetRigidBody = Target_->getComponentByType< ScnPhysicsRigidBodyComponent >();
			TargetPosition_ = TargetRigidBody->getPosition() + MaVec3d( 0.0f, 3.0f, 0.0f );
		}
		break;
	}

	// Move to target position.
	if( Target_ != nullptr )
	{
		auto RigidBody = getComponentByType< ScnPhysicsRigidBodyComponent >();
		auto Position = RigidBody->getPosition();

		auto Displacement = TargetPosition_ - Position;

		if( Displacement.magnitude() > 2.0f )
		{
			auto ForceAmount = Displacement.normal() * MaxForce_;
			RigidBody->applyCentralForce( ForceAmount );

			ScnDebugRenderComponent::pImpl()->drawLine( 
				Position,
				Position + ForceAmount * 0.025f,
				RsColour::RED,
				0 );
		}

		BcF32 Damping = 0.0f;
		if( RigidBody->getLinearVelocity().magnitude() > MaxVelocity_ )
		{
			Damping += 0.01f;
		}

		Damping += ( BcClamp( Displacement.magnitude(), 0.0f, 4.0f ) * 0.25f );
		Damping = 1.0f - BcClamp( Damping, 0.0f, 1.0f ) * 0.05f;
		RigidBody->setLinearVelocity( RigidBody->getLinearVelocity() * Damping );
	}

	CirclingTimer_ += Tick;
	if( CirclingTimer_ > BcPIMUL2 )
	{
		CirclingTimer_ -= BcPIMUL2;
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaMinerComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Mine.
	Parent->subscribe( 0, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();

			PSY_LOG( "GaMinerComponent: Mine event" );
			State_ = State::MINING;

			Target_ = Event.TargetUnit_;

			return evtRET_PASS;
		} );

	// Accident.
	Parent->subscribe( 1, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();

			PSY_LOG( "GaMinerComponent: Accident event" );
			State_ = State::ACCIDENTING;

			Target_ = Event.TargetUnit_;

			return evtRET_PASS;
		} );

	// Return.
	Parent->subscribe( 2, this,
		[ this ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< GaUnitActionEvent >();

			PSY_LOG( "GaMinerComponent: Return event" );
			State_ = State::RETURNING;

			Target_ = Event.TargetUnit_;

			return evtRET_PASS;
		} );


	TargetPosition_ = Parent->getLocalPosition();

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
void GaMinerComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}
