#include "GaSceneComponent.h"

#include "System/Content/CsCore.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnFont.h"
#include "System/Scene/Rendering/ScnViewComponent.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"

#include "System/Os/OsCore.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaSceneComponent );

void GaSceneComponent::StaticRegisterClass()
{
	ReRegisterClass< GaSceneComponent, Super >()
		.addAttribute( new ScnComponentAttribute( -30 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaSceneComponent::GaSceneComponent():
	Canvas_( nullptr ),
	Font_( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaSceneComponent::~GaSceneComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaSceneComponent::update( BcF32 Tick )
{
	// Get projection for UI.
	OsClient* Client = OsCore::pImpl()->getClient( 0 );
	BcF32 HalfWidth = static_cast< BcF32 >( Client->getWidth() / 2 );
	BcF32 HalfHeight = static_cast< BcF32 >( Client->getHeight() / 2 );

	MaMat4d UIProjection;
	UIProjection.orthoProjection( -HalfWidth, HalfWidth, HalfHeight, -HalfHeight, -1.0f, 1.0f );

	Canvas_->clear();
	Canvas_->pushMatrix( UIProjection );

	Super::update( Tick );
}


//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaSceneComponent::onAttach( ScnEntityWeakRef Parent )
{
	auto Emitter = getComponentAnyParentByType< ScnSoundEmitterComponent >();
	BcAssert( Emitter );
	ScnSoundRef Sound;
	if( CsCore::pImpl()->requestResource( "sounds", "music", Sound ) )
	{
		Emitter->play( Sound );
	}	

	Canvas_ = getComponentAnyParentByType< ScnCanvasComponent >();
	BcAssert( Canvas_ );
	Font_ = getComponentAnyParentByType< ScnFontComponent >();
	BcAssert( Font_ );
	Super::onAttach( Parent );
}
