#include "GaAsteroidComponent.h"
#include "GaAsteroidFieldComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsCollisionComponent.h"
#include "System/Scene/Physics/ScnPhysicsEvents.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaAsteroidComponent );

void GaAsteroidComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "MassSizeRatio_", &GaAsteroidComponent::MassSizeRatio_, bcRFF_IMPORTER ),
		new ReField( "Size_", &GaAsteroidComponent::Size_, bcRFF_IMPORTER ),
	};
	
	ReRegisterClass< GaAsteroidComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaAsteroidComponent::GaAsteroidComponent():
	MassSizeRatio_( 1.0f ),
	Size_( 1.0f ),
	OldSize_( 0.0f )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaAsteroidComponent::~GaAsteroidComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaAsteroidComponent::update( BcF32 Tick )
{
	if( Size_ > 0.0f )
	{
		if( Size_ != OldSize_ )
		{
			auto Collision = getComponentByType< ScnPhysicsCollisionComponent >();
			auto RigidBody = getComponentByType< ScnPhysicsRigidBodyComponent >();
			BcAssert( Collision );
			BcAssert( RigidBody );
			Collision->setLocalScaling( MaVec3d( Size_, Size_, Size_ ) );
			RigidBody->setMass( Size_ * MassSizeRatio_ );

			OldSize_ = Size_;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaAsteroidComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Collision.
	Parent->subscribe( (EvtID)ScnPhysicsEvents::COLLISION, this,
		[ this, Parent ]( EvtID, const EvtBaseEvent& BaseEvent )
		{
			const auto& Event = BaseEvent.get< ScnPhysicsEventCollision >();
			PSY_LOG( "Asteroid has been hit." );
			return evtRET_PASS;
		} );

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// recycle
void GaAsteroidComponent::recycle()
{
	auto Field = getParentEntity()->getParentEntity()->getComponentByType< GaAsteroidFieldComponent >();
	Field->recycle( this );
}
