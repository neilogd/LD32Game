#include "GaAsteroidFieldComponent.h"
#include "GaAsteroidComponent.h"
#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsSphereCollisionComponent.h"
#include "System/Scene/ScnCore.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaAsteroidFieldComponent );

void GaAsteroidFieldComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "AsteroidTemplates_", &GaAsteroidFieldComponent::AsteroidTemplates_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "NoofAsteroids_", &GaAsteroidFieldComponent::NoofAsteroids_, bcRFF_IMPORTER ),
		new ReField( "MinSize_", &GaAsteroidFieldComponent::MinSize_, bcRFF_IMPORTER ),
		new ReField( "MaxSize_", &GaAsteroidFieldComponent::MaxSize_, bcRFF_IMPORTER ),
		new ReField( "MinVelocity_", &GaAsteroidFieldComponent::MinVelocity_, bcRFF_IMPORTER ),
		new ReField( "MaxVelocity_", &GaAsteroidFieldComponent::MaxVelocity_, bcRFF_IMPORTER ),
		new ReField( "Width_", &GaAsteroidFieldComponent::Width_, bcRFF_IMPORTER ),
		new ReField( "MaxExtents_", &GaAsteroidFieldComponent::MaxExtents_, bcRFF_IMPORTER ),
		new ReField( "Depth_", &GaAsteroidFieldComponent::Depth_, bcRFF_IMPORTER ),
		new ReField( "Margin_", &GaAsteroidFieldComponent::Margin_, bcRFF_IMPORTER ),

		new ReField( "Asteroids_", &GaAsteroidFieldComponent::Asteroids_, bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaAsteroidFieldComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaAsteroidFieldComponent::GaAsteroidFieldComponent():
	NoofAsteroids_( 0 ),
	MinSize_( 1.0f ),
	MaxSize_( 1.0f ),
	MinVelocity_( 1.0f ),
	MaxVelocity_( 1.0f ),
	Width_( 32.0f ),
	MaxExtents_( 64.0f ),
	Depth_( 1.0f ),
	Margin_( 1.0f )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaAsteroidFieldComponent::~GaAsteroidFieldComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaAsteroidFieldComponent::update( BcF32 Tick )
{
#if 0
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -MaxExtents_ ),
		MaVec3d(  Width_, 0.0f, -MaxExtents_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f,  MaxExtents_ ),
		MaVec3d(  Width_, 0.0f,  MaxExtents_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -MaxExtents_ ),
		MaVec3d( -Width_, 0.0f,  MaxExtents_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d(  Width_, 0.0f, -MaxExtents_ ),
		MaVec3d(  Width_, 0.0f,  MaxExtents_ ),
		RsColour::GREEN,
		0 );

	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -( MaxExtents_ + Margin_ ) ),
		MaVec3d(  Width_, 0.0f, -( MaxExtents_ + Margin_ ) ),
		RsColour::RED,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f,  ( MaxExtents_ + Margin_ ) ),
		MaVec3d(  Width_, 0.0f,  ( MaxExtents_ + Margin_ ) ),
		RsColour::RED,
		0 );
#endif 
	// Check if we need to move any to other top/bottom side,
	// or constrain them to the width.
	for( auto Asteroid : Asteroids_ )
	{
		auto RigidBody = Asteroid->RigidBody_;
		auto Position = RigidBody->getPosition();

		if( Position.z() < -( MaxExtents_ + Margin_ ) ||
			Position.z() > ( MaxExtents_ + Margin_ ) ||
			Position.x() < -( MaxExtents_ + Margin_ ) ||
			Position.x() > ( MaxExtents_ + Margin_ ) )
		{
			recycle( Asteroid );
		}
		else if( Asteroid->getSize() < 0.1f )
		{
			recycle( Asteroid );
		}

		if( Position.y() < -Depth_ )
		{
			RigidBody->applyCentralForce( MaVec3d( 0.0f, RigidBody->getMass(), 0.0f ) );
		}
		else if( Position.y() > Depth_ )
		{
			RigidBody->applyCentralForce( MaVec3d( 0.0f, -RigidBody->getMass(), 0.0f ) );
		}


		// Try to keep em moving along.
		RigidBody->applyCentralForce( MaVec3d( 0.0f, 0.0f, 0.01f ) );

	}

	// Starfield particles.
	ScnParticle* Particle = nullptr;
	if( ParticlesAdd_->allocParticle( Particle ) )
	{
		MaVec3d NewPosition = MaVec3d(
			BcRandom::Global.randRealRange( -Width_ * 6.0f, Width_ * 6.0f ),
			-6.0f,
			-MaxExtents_ + 0.05f );

		BcF32 TravelDistance = MaxExtents_ * 2.0f;

		Particle->Position_ = NewPosition;
		Particle->Velocity_ = MaVec3d( 0.0f, 0.0f, BcRandom::Global.randRealRange( 5.0f, 20.0f ) );
		Particle->Acceleration_ = MaVec3d( 0.0f, 0.0f, 0.0f );
		Particle->Scale_ = MaVec2d( 0.1f, 0.1f );
		Particle->MinScale_ = MaVec2d( 2.2f, 2.2f );
		Particle->MaxScale_ = MaVec2d( 2.2f, 2.2f );
		Particle->Rotation_ = 0.0f;
		Particle->RotationMultiplier_ = 0.0f;
		Particle->Colour_ = RsColour( 0.0f, 0.0f, 0.0f, 1.0f );
		Particle->MinColour_ = RsColour( 0.0f, 0.0f, 0.0f, 4.0f );
		Particle->MaxColour_ = RsColour( 4.0f, 4.0f, 4.0f, 0.0f );
		Particle->TextureIndex_ = 7;
		Particle->CurrentTime_ = 0.0f;
		Particle->MaxTime_ = TravelDistance / Particle->Velocity_.z();
		Particle->Alive_ = BcTrue;
	}

	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaAsteroidFieldComponent::onAttach( ScnEntityWeakRef Parent )
{
	// Spawn some.
	BcAssert( AsteroidTemplates_.size() > 0 );

	for( BcU32 Idx = 0; Idx < NoofAsteroids_; ++Idx )
	{
		size_t TemplateIdx = static_cast< size_t >( BcRandom::Global.rand() ) % AsteroidTemplates_.size();
		auto Template = AsteroidTemplates_[ TemplateIdx ];

		// Random position.
		MaMat4d Transform;

		Transform.translation(
			MaVec3d( 
				BcRandom::Global.randRealRange( -Width_, Width_ ),
				0.0f,
				BcRandom::Global.randRealRange( -MaxExtents_, MaxExtents_ )
				) );

		// Spawn new asteroid.
		auto Entity = ScnCore::pImpl()->spawnEntity( 
			ScnEntitySpawnParams( 
				Template->getName().getUnique(),
				Template, 
				Transform,
				getParentEntity() )	);
		BcAssert( Entity != nullptr );

		auto AsteroidComponent = Entity->getComponentByType< GaAsteroidComponent >();
		Asteroids_.push_back( AsteroidComponent );

		recycle( AsteroidComponent, BcFalse );
	}

	ParticlesAdd_ = getComponentAnyParentByType< ScnParticleSystemComponent >( 0 );
	BcAssert( ParticlesAdd_ );
	ParticlesSub_ = getComponentAnyParentByType< ScnParticleSystemComponent >( 1 );
	BcAssert( ParticlesSub_ );

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
void GaAsteroidFieldComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// recycle
void GaAsteroidFieldComponent::recycle( GaAsteroidComponent* Asteroid, BcBool SetPosition )
{
	// Register for an event to setup at the end of the update tick.
	ScnCore::pImpl()->subscribe( sysEVT_SYSTEM_POST_UPDATE, this,
		[ this, Asteroid, SetPosition ]( EvtID, const EvtBaseEvent& )->eEvtReturn
		{
			if( Asteroid->getParentEntity() != nullptr )
			{
				auto Unit = Asteroid->getComponentByType< GaUnitComponent >();
				Unit->setupShadow();
				auto RigidBody = Asteroid->getComponentByType< ScnPhysicsRigidBodyComponent >();
				auto Position = RigidBody->getPosition();
				auto Size = BcRandom::Global.randRealRange( MinSize_, MaxSize_ );
				Asteroid->setSize( Size );

				if( SetPosition )
				{
					MaVec3d NewPosition = MaVec3d(
						BcRandom::Global.randRealRange( -Width_, Width_ ),
						0.0f,
						-MaxExtents_ + 0.05f );
					RigidBody->translate( -Position + NewPosition );
					Asteroid->getParentEntity()->setLocalPosition( RigidBody->getPosition() );
				}

				const BcF32 AngularRange = 1.0f;
				RigidBody->setAngularVelocity( MaVec3d( 
					BcRandom::Global.randRealRange( -AngularRange, AngularRange ),
					BcRandom::Global.randRealRange( -AngularRange, AngularRange ),
					BcRandom::Global.randRealRange( -AngularRange, AngularRange ) ) );

				auto Velocity = BcRandom::Global.randRealRange( MinVelocity_, MaxVelocity_ );
				RigidBody->setLinearVelocity( MaVec3d( 0.0f, 0.0f, Velocity ) );
			}
			return evtRET_REMOVE;
		} );

}
