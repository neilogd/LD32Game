#include "GaGameComponent.h"
#include "GaUnitComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnViewComponent.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"
#include "System/Scene/Physics/ScnPhysicsCollisionComponent.h"

#include "System/Os/OsCore.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaGameComponent );

void GaGameComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "View_", &GaGameComponent::View_, bcRFF_TRANSIENT ),
		new ReField( "World_", &GaGameComponent::View_, bcRFF_TRANSIENT ),
		new ReField( "SelectedUnit_", &GaGameComponent::SelectedUnit_, bcRFF_TRANSIENT ),
		new ReField( "MouseEvents_", &GaGameComponent::MouseEvents_, bcRFF_TRANSIENT ),
	};

	ReRegisterClass< GaGameComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaGameComponent::GaGameComponent():
	View_( nullptr ),
	World_( nullptr ),
	SelectedUnit_(),
	MouseEvents_()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaGameComponent::~GaGameComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaGameComponent::update( BcF32 Tick )
{
	for( const auto& EventPair : MouseEvents_ )
	{
		auto ID = EventPair.first;
		const auto& Event = EventPair.second;

		BcAssert( View_ != nullptr );
		BcAssert( World_ != nullptr );

		MaVec2d Position( Event.MouseX_, Event.MouseY_ );
		MaVec3d Near, Far;
		View_->getWorldPosition( Position, Near, Far );

		ScnPhysicsLineCastResult Result;
		if( World_->lineCast( Near, Far, &Result ) )
		{
			auto Entity = Result.Entity_;
			auto Unit = Entity->getComponentByType< GaUnitComponent >();
			if( Unit != nullptr )
			{
				SelectedUnit_ = Unit;
			}
		}
	}
	MouseEvents_.clear();

	if( SelectedUnit_ != nullptr )
	{
		ScnDebugRenderComponent::pImpl()->drawEllipsoid( SelectedUnit_->getParentEntity()->getWorldPosition(), MaVec3d( 3.0f, 3.0f, 3.0f ), RsColour::RED, 0 );
	}
}

//////////////////////////////////////////////////////////////////////////
// update
void GaGameComponent::onAttach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, this, 
		[ this ]( EvtID ID, const EvtBaseEvent& BaseEvent )
		{	
			auto Event = BaseEvent.get< OsEventInputMouse >();
			MouseEvents_.push_back( std::make_pair( ID, Event ) );
			return evtRET_PASS;
		} );

	View_ = Parent->getComponentAnyParentByType< ScnViewComponent >();
	World_ = Parent->getComponentAnyParentByType< ScnPhysicsWorldComponent >();

	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// update
void GaGameComponent::onDetach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->unsubscribeAll( this );
	Super::onDetach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onObjectDeleted
void GaGameComponent::onObjectDeleted( class ReObject* Object )
{
	if( SelectedUnit_ == Object )
	{
		SelectedUnit_ = nullptr;
	}
}
