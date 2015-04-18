#include "GaAsteroidFieldComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
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
		new ReField( "AsteroidTemplates_", &GaAsteroidFieldComponent::AsteroidTemplates_, bcRFF_IMPORTER ),
		new ReField( "NoofAsteroids_", &GaAsteroidFieldComponent::NoofAsteroids_, bcRFF_IMPORTER ),
		new ReField( "MinSize_", &GaAsteroidFieldComponent::MinSize_, bcRFF_IMPORTER ),
		new ReField( "MaxSize_", &GaAsteroidFieldComponent::MaxSize_, bcRFF_IMPORTER ),
		new ReField( "MinVelocity_", &GaAsteroidFieldComponent::MinVelocity_, bcRFF_IMPORTER ),
		new ReField( "MaxVelocity_", &GaAsteroidFieldComponent::MaxVelocity_, bcRFF_IMPORTER ),
		new ReField( "Width_", &GaAsteroidFieldComponent::Width_, bcRFF_IMPORTER ),
		new ReField( "Height_", &GaAsteroidFieldComponent::Height_, bcRFF_IMPORTER ),
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
	Height_( 32.0f ),
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
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -Height_ ),
		MaVec3d(  Width_, 0.0f, -Height_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f,  Height_ ),
		MaVec3d(  Width_, 0.0f,  Height_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -Height_ ),
		MaVec3d( -Width_, 0.0f,  Height_ ),
		RsColour::GREEN,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d(  Width_, 0.0f, -Height_ ),
		MaVec3d(  Width_, 0.0f,  Height_ ),
		RsColour::GREEN,
		0 );

	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f, -( Height_ + Margin_ ) ),
		MaVec3d(  Width_, 0.0f, -( Height_ + Margin_ ) ),
		RsColour::RED,
		0 );
	ScnDebugRenderComponent::pImpl()->drawLine( 
		MaVec3d( -Width_, 0.0f,  ( Height_ + Margin_ ) ),
		MaVec3d(  Width_, 0.0f,  ( Height_ + Margin_ ) ),
		RsColour::RED,
		0 );

	// Check if we need to move any to other top/bottom side,
	// or constrain them to the width.
	for( auto Asteroid : Asteroids_ )
	{
		auto RigidBody = Asteroid->getComponentByType< ScnPhysicsRigidBodyComponent >();
		auto Position = RigidBody->getPosition();

		if( Position.z() < -( Height_ + Margin_ ) )
		{
			MaVec3d NewPosition = MaVec3d(
				BcRandom::Global.randRealRange( -Width_, Width_ ),
				0.0f,
				Height_ - 0.05f );
			RigidBody->translate( -Position + NewPosition );

			RigidBody->setLinearVelocity( RigidBody->getLinearVelocity() * MaVec3d( 0.0f, 0.0f, 1.0f ) );
		}
		else if( Position.z() > ( Height_ + Margin_ ) )
		{
			MaVec3d NewPosition = MaVec3d(
				BcRandom::Global.randRealRange( -Width_, Width_ ),
				0.0f,
				-Height_ + 0.05f );
			RigidBody->translate( -Position + NewPosition );

			RigidBody->setLinearVelocity( RigidBody->getLinearVelocity() * MaVec3d( 0.0f, 0.0f, 1.0f ) );
		}
		else
		{
#if 0
			const BcF32 ForceMultiplier = 10.0f;
			if( Position.x() < -Width_ )
			{
				auto ForceAmount = -( Position.x() + Width_ ) * ForceMultiplier;
				RigidBody->applyCentralForce( MaVec3d( ForceAmount, 0.0f, 0.0f ) );
			}
			else if( Position.x() > Width_ )
			{
				auto ForceAmount = -( Position.x() - Width_ ) * ForceMultiplier;
				RigidBody->applyCentralForce( MaVec3d( ForceAmount, 0.0f, 0.0f ) );
			}

			if( Position.y() < -Depth_ )
			{
				auto ForceAmount = -( Position.y() + Depth_ ) * ForceMultiplier;
				RigidBody->applyCentralForce( MaVec3d( 0.0f, ForceAmount, 0.0f ) );
			}
			else if( Position.y() > Depth_ )
			{
				auto ForceAmount = -( Position.y() - Depth_ ) * ForceMultiplier;
				RigidBody->applyCentralForce( MaVec3d( 0.0f, ForceAmount, 0.0f ) );
			}
#endif
		}
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
				BcRandom::Global.randRealRange( -Height_, Height_ )
				) );

		// Spawn new asteroid.
		auto Entity = ScnCore::pImpl()->spawnEntity( 
			ScnEntitySpawnParams( 
				Template->getName().getUnique(),
				Template, 
				Transform,
				getParentEntity() )	);
		BcAssert( Entity != nullptr );
		Asteroids_.push_back( Entity );

		// Register for an event to setup at the end of the update tick.
		ScnCore::pImpl()->subscribe( sysEVT_SYSTEM_POST_UPDATE, this,
			[ this, Entity ]( EvtID, const EvtBaseEvent& )->eEvtReturn
			{
				auto Velocity = BcRandom::Global.randRealRange( MinVelocity_, MaxVelocity_ );
				auto RigidBody = Entity->getComponentByType< ScnPhysicsRigidBodyComponent >();
				RigidBody->setLinearVelocity( MaVec3d( 0.0f, 0.0f, Velocity ) );

				const BcF32 AngularRange = 1.0f;
				RigidBody->setAngularVelocity( MaVec3d( 
					BcRandom::Global.randRealRange( -AngularRange, AngularRange ),
					BcRandom::Global.randRealRange( -AngularRange, AngularRange ),
					BcRandom::Global.randRealRange( -AngularRange, AngularRange ) ) );

				auto Size = BcRandom::Global.randRealRange( MinSize_, MaxSize_ );
				auto CollisionComponent = Entity->getComponentByType< ScnPhysicsSphereCollisionComponent >();
				CollisionComponent->setLocalScaling( MaVec3d( Size, Size, Size ) );
				return evtRET_REMOVE;
			} );
	}

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
void GaAsteroidFieldComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}

