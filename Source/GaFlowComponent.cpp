#include "GaFlowComponent.h"

#include "System/Content/CsCore.h"
#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"

#include "System/Os/OsCore.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaFlowComponent );

void GaFlowComponent::StaticRegisterClass()
{
	ReRegisterClass< GaFlowComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaFlowComponent::GaFlowComponent():
	CurrentEntity_( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaFlowComponent::~GaFlowComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaFlowComponent::update( BcF32 Tick )
{
#if 0
	ScnDebugRenderComponent::pImpl()->drawGrid( 
		MaVec3d( 0.0f, 0.0f, 0.0f ),
		MaVec3d( 500.0f, 0.0f, 500.0f ),
		1.0f,
		10.0f,
		0 );
#endif
	Super::update( Tick );
}


//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaFlowComponent::onAttach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->subscribe( osEVT_INPUT_KEYDOWN, this, 
		[ this ]( EvtID ID, const EvtBaseEvent& BaseEvent )
		{	
			if( CurrentEntity_ != nullptr )
			{
				//ScnCore::pImpl()->removeEntity( CurrentEntity_ );
				CurrentEntity_ = nullptr;
			}

			//CurrentEntity_ = ScnCore::pImpl()->spawnEntity( ScnEntitySpawnParams( "SceneEntity", "default", "SceneEntity", MaMat4d(), getParentEntity() ) );
			
			return evtRET_PASS;
		} );

	CurrentEntity_ = ScnCore::pImpl()->spawnEntity( ScnEntitySpawnParams( "SceneEntity", "default", "IntroEntity", MaMat4d(), getParentEntity() ) );

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaFlowComponent::onDetach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->unsubscribeAll( this );

	Super::onDetach( Parent );
}
