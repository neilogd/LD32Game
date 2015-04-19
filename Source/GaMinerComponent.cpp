#include "GaMinerComponent.h"
#include "GaAsteroidComponent.h"
#include "GaUnitComponent.h"
#include "GaMothershipComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsEvents.h"

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
		new ReField( "MiningRate_", &GaMinerComponent::MiningRate_, bcRFF_IMPORTER ),
		new ReField( "MaxCapacity_", &GaMinerComponent::MaxCapacity_, bcRFF_IMPORTER ),
		new ReField( "MaxExtents_", &GaMinerComponent::MaxExtents_, bcRFF_IMPORTER ),		
		new ReField( "MiningSizeThreshold_", &GaMinerComponent::MiningSizeThreshold_, bcRFF_IMPORTER ),

		new ReField( "State_", &GaMinerComponent::State_, bcRFF_TRANSIENT ),
		new ReField( "CirclingTimer_", &GaMinerComponent::CirclingTimer_, bcRFF_TRANSIENT ),
		new ReField( "TargetPosition_", &GaMinerComponent::TargetPosition_, bcRFF_TRANSIENT ),
		new ReField( "Target_", &GaMinerComponent::Target_, bcRFF_TRANSIENT ),
		new ReField( "AmountMined_", &GaMinerComponent::AmountMined_, bcRFF_TRANSIENT ),
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
	MiningDistance_( 4.0f ),
	MaxExtents_( 32.0f ),
	MiningSizeThreshold_( 0.1f ),
	MiningRate_( 0.1f ),
	MaxCapacity_( 2.0f ),
	CirclingTimer_( 0.0f ),
	TargetPosition_( 0.0f, 0.0f, 0.0f ),
	Target_( nullptr ),
	AmountMined_( 0.0f )
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
	auto RigidBody = getComponentByType< ScnPhysicsRigidBodyComponent >();
	auto Position = RigidBody->getPosition();

	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -100.0f, 0.0f, -MaxExtents_ ),
		MaVec3d(  100.0f, 0.0f, -MaxExtents_ ),
		RsColour::YELLOW,
		0 );

	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -100.0f, 0.0f, MaxExtents_ ),
		MaVec3d(  100.0f, 0.0f, MaxExtents_ ),
		RsColour::YELLOW,
		0 );

	ScnPhysicsRigidBodyComponent* TargetRigidBody = nullptr;
	if( Target_ != nullptr )
	{
		TargetRigidBody = Target_->getComponentByType< ScnPhysicsRigidBodyComponent >();
		auto TargetRigidBodyPosition = TargetRigidBody->getPosition();

		RsColour Colour = GaUnitComponent::RADAR_COLOUR;
		if( State_ == State::ACCIDENTING )
		{
			Colour = GaUnitComponent::RADAR_COLOUR_ATTACK;
		}

		ScnDebugRenderComponent::pImpl()->drawLine( 
			MaVec3d( TargetRigidBodyPosition.x(), GaUnitComponent::RADAR_GROUND_Y, TargetRigidBodyPosition.z() ),
			MaVec3d( Position.x(), GaUnitComponent::RADAR_GROUND_Y, Position.z() ),
			Colour,
			0 );
	}

	switch( State_ )
	{
	case State::IDLE:
		{

		}
		break;

	case State::MINING:
		{
			BcAssert( TargetRigidBody );
			TargetPosition_ = TargetRigidBody->getPosition() + MaVec3d( BcCos( CirclingTimer_ ), 1.0f, BcSin( CirclingTimer_ ) ).normal() * MiningDistance_;
		}
		break;

	case State::ACCIDENTING:
		{
			BcAssert( TargetRigidBody );
			TargetPosition_ = TargetRigidBody->getPosition();
		}
		break;

	case State::RETURNING:
		{
			BcAssert( TargetRigidBody );
			TargetPosition_ = TargetRigidBody->getPosition() + MaVec3d( 0.0f, MiningDistance_, 0.0f );
		}
		break;
	}

	// Movement handling.
	BcF32 Damping = 0.0f;
	auto Displacement = TargetPosition_ - Position;
	auto DisplacementMag = Displacement.magnitude();
	if( DisplacementMag > 2.0f )
	{
		auto ForceAmount = Displacement.normal() * MaxForce_ * RigidBody->getMass();
		RigidBody->applyCentralForce( ForceAmount );

		// TODO: Draw thruster in opposite direction.
	}

	if( Target_ != nullptr )
	{
		if( State_ == State::MINING )
		{
			if( DisplacementMag < ( MiningDistance_ * 1.1f ) )
			{
				// DRAW LAZ0R.
				ScnDebugRenderComponent::pImpl()->drawLine( 
					Position,
					Target_->getParentEntity()->getWorldPosition(),
					RsColour::ORANGE,
					0 );

				auto Asteroid = Target_->getComponentByType< GaAsteroidComponent >();
				BcAssert( Asteroid );

				if( AmountMined_ < MaxCapacity_ )
				{
					auto Size = Asteroid->getSize();
					auto AmountMined = Tick * MiningRate_;
					Size -= AmountMined;
					AmountMined_ += AmountMined;
					Asteroid->setSize( Size );

					// Return when asteroid is deaded, or TODO we are full.
					if( Size < MiningSizeThreshold_ )
					{
						Target_ = nullptr;
						State_ = State::IDLE;

						// TODO: Notify done.
					}
				}
				else
				{
					Target_ = nullptr;
					State_ = State::IDLE;

					// TODO: notify Returning.
				}
			}
		}
		else if( State_ == State::RETURNING )
		{
			if( DisplacementMag < ( MiningDistance_ * 1.1f ) )
			{
				auto Mothership = Target_->getComponentByType< GaMothershipComponent >();
				BcAssert( Mothership );

				if( AmountMined_ > 0.0f )
				{
					auto AmountMined = std::min( AmountMined_, Tick * MiningRate_ );
					Mothership->addResources( AmountMined * 100.0f );
					AmountMined_ -= AmountMined;
				}
				else
				{
					// TODO: notify.
					Target_ = nullptr;
					State_ = State::IDLE;
				}
			}
		}

		// Out of bounds.
		BcF32 ZMax = MaxExtents_;
		if( TargetPosition_.z() < -ZMax || TargetPosition_.z() > ZMax )
		{
			// TODO: notify.
			Target_ = nullptr;
			State_ = State::IDLE;

			if( Position.z() < -ZMax )
			{
				TargetPosition_ = MaVec3d( Position.x(), Position.y(), Position.z() + 8.0f );
			}
			else
			{
				TargetPosition_ = MaVec3d( Position.x(), Position.y(), Position.z() - 8.0f );
			}
		}

		BcF32 XMax = MaxExtents_ * 1.5f;
		if( TargetPosition_.x() < -XMax || TargetPosition_.x() > XMax )
		{
			// TODO: notify.
			Target_ = nullptr;
			State_ = State::IDLE;

			if( Position.x() < -XMax )
			{
				TargetPosition_ = MaVec3d( Position.x() + 8.0f, Position.y(), Position.z() );
			}
			else
			{
				TargetPosition_ = MaVec3d( Position.x() - 8.0f, Position.y(), Position.z() );
			}
		}

		if( RigidBody->getLinearVelocity().magnitude() > MaxVelocity_ )
		{
			Damping += 0.01f;
		}
		Damping += ( BcClamp( Displacement.magnitude(), 0.0f, 4.0f ) * 0.25f );
	}
	else
	{
		Damping += 0.25f;
	}

	Damping = 1.0f - BcClamp( Damping, 0.0f, 1.0f ) * 0.05f;
	RigidBody->setLinearVelocity( RigidBody->getLinearVelocity() * Damping );

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

	// Collision.
	Parent->subscribe( (EvtID)ScnPhysicsEvents::COLLISION, this,
		[ this, Parent ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< ScnPhysicsEventCollision >();
			if( State_ == State::ACCIDENTING )
			{
				auto OtherEntity = Event.BodyB_->getParentEntity();
				auto AsteroidComponent = OtherEntity->getComponentByType< GaAsteroidComponent >();
				if( AsteroidComponent )
				{
					auto Unit = getComponentByType< GaUnitComponent >();
					for( auto Component : Parent->getParentEntity()->getComponents() )
					{
						if( Component->getComponentByType< GaMothershipComponent >() )
						{
							auto ShipUnit = Component->getComponentByType< GaUnitComponent >();
							if( ShipUnit->getTeam() != Unit->getTeam() )
							{
								// Apply a force to the asteroid to hit the shit.
								auto AsteroidRigidBody = AsteroidComponent->getParentEntity()->getComponentByType< ScnPhysicsRigidBodyComponent >();
								AsteroidRigidBody->setLinearVelocity( MaVec3d( 0.0f, 0.0f, 0.0f ) );
								auto AsteroidPosition = AsteroidRigidBody->getPosition();
								auto ShipPosition = ShipUnit->getParentEntity()->getLocalPosition();
								auto Displacement = ShipPosition - AsteroidPosition;

								// Apply impulse based on our *own* mass + amount mined..
								AsteroidRigidBody->applyCentralImpulse( Displacement.normal() * ( Event.BodyA_->getMass() + AmountMined_ * 10.0f ) );

								// Remove ourself.
								ScnCore::pImpl()->removeEntity( getParentEntity() );
								// Unregister from further events (invalid from this point)
								return evtRET_REMOVE;
							}
						}
					}
				} 
			}

			// Destroy self.

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

//////////////////////////////////////////////////////////////////////////
// onDetach
void GaMinerComponent::onObjectDeleted( class ReObject* Object )
{
	if( Object == Target_ )
	{
		// TODO: notify.
		Target_ = nullptr;
		State_ = State::IDLE;
	}
}
