#include "GaAsteroidComponent.h"
#include "GaAsteroidFieldComponent.h"
#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnModel.h"
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
	OldSize_( 0.0f ),
	Unit_( nullptr ),
	Collision_( nullptr ),
	RigidBody_( nullptr ),
	Model_( nullptr )
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
			Collision_->setLocalScaling( MaVec3d( Size_, Size_, Size_ ) );
			RigidBody_->setMass( Size_ * MassSizeRatio_ );
			if( Model_ )
			{
				Model_->setBaseTransform( MaVec3d( 0.0f, 0.0f, 0.0f ), MaVec3d( Size_, Size_, Size_ ), MaVec3d( 0.0f, 0.0f, 0.0f ) );
			}

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
			
			return evtRET_PASS;
		} );

	Unit_ = getComponentByType< GaUnitComponent >();
	Collision_ = getComponentByType< ScnPhysicsCollisionComponent >();
	RigidBody_ = getComponentByType< ScnPhysicsRigidBodyComponent >();
	Model_ = getComponentByType< ScnModelComponent >();
	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// recycle
void GaAsteroidComponent::recycle()
{
	auto Field = getParentEntity()->getParentEntity()->getComponentByType< GaAsteroidFieldComponent >();
	Field->recycle( this );
}
