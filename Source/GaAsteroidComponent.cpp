#include "GaAsteroidComponent.h"
#include "GaAsteroidFieldComponent.h"
#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
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
	ParticlesAdd_( nullptr ),
	ParticlesSub_( nullptr ),
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

			ScnParticle* Particle = nullptr;
			auto TotalVel = Event.BodyA_->getLinearVelocity().magnitude() + Event.BodyA_->getLinearVelocity().magnitude();

			for( BcU32 Idx = 0; Idx < 10; ++Idx )
			{
				if( ParticlesAdd_->allocParticle( Particle ) )
				{
					Particle->Position_ = Event.ContactPoints_[ 0 ].PointA_ * 2.0f;
					Particle->Velocity_ = MaVec3d(
						BcRandom::Global.randReal(),
						BcRandom::Global.randReal(),
						BcRandom::Global.randReal() ) * TotalVel * 5.0f;
					Particle->Acceleration_ = MaVec3d( 0.0f, 0.0f, 0.0f );
					Particle->Scale_ = MaVec2d( 0.0f, 0.1f );
					Particle->MinScale_ = MaVec2d( 0.2f, 0.2f );
					Particle->MaxScale_ = MaVec2d( 0.5f, 0.5f );
					Particle->Rotation_ = BcRandom::Global.randReal();
					Particle->RotationMultiplier_ = BcRandom::Global.randReal();
					Particle->Colour_ = RsColour( 1.0f, 0.9f, 0.8f, 0.0f );
					Particle->MinColour_ = RsColour( 1.0f, 0.9f, 0.8f, 0.0f );
					Particle->MaxColour_ = RsColour( 0.0f, 0.0f, 0.0f, 1.0f );
					Particle->TextureIndex_ = 0;
					Particle->CurrentTime_ = 0.0f;
					Particle->MaxTime_ = 0.5f;
					Particle->Alive_ = BcTrue;
				}
			}
			
			return evtRET_PASS;
		} );

	Unit_ = getComponentByType< GaUnitComponent >();
	Collision_ = getComponentByType< ScnPhysicsCollisionComponent >();
	RigidBody_ = getComponentByType< ScnPhysicsRigidBodyComponent >();
	Model_ = getComponentByType< ScnModelComponent >();

	ParticlesAdd_ = getComponentAnyParentByType< ScnParticleSystemComponent >( 0 );
	BcAssert( ParticlesAdd_ );
	ParticlesSub_ = getComponentAnyParentByType< ScnParticleSystemComponent >( 1 );
	BcAssert( ParticlesSub_ );

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// recycle
void GaAsteroidComponent::recycle()
{
	auto Field = getParentEntity()->getParentEntity()->getComponentByType< GaAsteroidFieldComponent >();
	Field->recycle( this );
}
