#include "GaIntroComponent.h"

#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Rendering/ScnFont.h"
#include "System/Scene/Rendering/ScnViewComponent.h"
#include "System/Os/OsCore.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaIntroComponent );

void GaIntroComponent::StaticRegisterClass()
{
	ReRegisterClass< GaIntroComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaIntroComponent::GaIntroComponent():
	Canvas_( nullptr ),
	Font_( nullptr ),
	Timer_( 0.0f )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaIntroComponent::~GaIntroComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaIntroComponent::update( BcF32 Tick )
{
	OsClient* Client = OsCore::pImpl()->getClient( 0 );
	BcF32 HalfWidth = static_cast< BcF32 >( Client->getWidth() / 2 );
	BcF32 HalfHeight = static_cast< BcF32 >( Client->getHeight() / 2 );

	{
		std::string Title = "Mined the 'roids!\nA game by NeiloGD for Ludum Dare 32!";
		std::string Message = 
			
			"The aim of the game is to eliminate your competitor, Blue Corp., from the 'roid mining business."
			"Unfortunately, we can't simply attack them. It must look like an accident, or the law will be on to us.\n\n"
			"Click on the Red Inc. mothership, and it will show you its commands:\n"
			" - LMB to build a miner (Costs $50).\n"
			" - RMB to repair any damage (Costs $50).\n\n"
			"Click on a miner, then point at a 'roid to see its commands:\n"
			" - LMB to mine a 'roid.\n"
			" - RMB to 'accidentally' crash.\n\n"
			"Click on a miner, then point at a mothership to see its commands:\n"
			" - RMB to take mined asteroid bits back.\n\n";
		
		ScnFontDrawParams DrawParams = ScnFontDrawParams()
			.setSize( 50.0f )
			.setMargin( 32.0f )
			.setWrappingEnabled( BcTrue )
			.setTextColour( RsColour::WHITE )
			.setAlignment( ScnFontAlignment::HCENTRE | ScnFontAlignment::TOP );

		auto Size = Font_->drawText( Canvas_, DrawParams, MaVec2d( -HalfWidth, -HalfHeight ), MaVec2d( HalfWidth * 2.0f, 0.0f ), Title );

		DrawParams.setSize( 25.0f );
		DrawParams.setAlignment( ScnFontAlignment::LEFT | ScnFontAlignment::TOP );
		Font_->drawText( Canvas_, DrawParams, MaVec2d( -HalfWidth, -HalfHeight + Size.y() + DrawParams.getMargin() ), MaVec2d( HalfWidth * 2.0f, 0.0f ), Message );

		if( Timer_ > 5.0f )
		{
			DrawParams.setSize( 50.0f );
			DrawParams.setAlignment( ScnFontAlignment::HCENTRE | ScnFontAlignment::BOTTOM );
			DrawParams.setTextColour( RsColour::GREEN );
			Font_->drawText( Canvas_, DrawParams, MaVec2d( -HalfWidth, -HalfHeight ), MaVec2d( HalfWidth * 2.0f, HalfHeight * 2.0f ), "Press any key to start!" );
		}
	}

	Timer_ += Tick;

	Super::update( Tick );
}


//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaIntroComponent::onAttach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->subscribe( osEVT_INPUT_KEYDOWN, this, 
		[ this ]( EvtID ID, const EvtBaseEvent& BaseEvent )
		{	
			if( Timer_ > 5.0f )
			{
				ScnCore::pImpl()->spawnEntity( ScnEntitySpawnParams( "SceneEntity", "default", "SceneEntity", MaMat4d(), getParentEntity()->getParentEntity() ) );
				ScnCore::pImpl()->removeEntity( getParentEntity() );
				return evtRET_REMOVE;
			}
			
			return evtRET_PASS;
		} );

	Canvas_ = getComponentAnyParentByType< ScnCanvasComponent >();
	BcAssert( Canvas_ );
	Font_ = getComponentAnyParentByType< ScnFontComponent >();
	BcAssert( Font_ );

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaIntroComponent::onDetach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->unsubscribeAll( this );
	Super::onDetach( Parent );
}
