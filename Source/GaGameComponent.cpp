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
		new ReField( "MothershipEntity_", &GaGameComponent::MothershipEntity_, bcRFF_IMPORTER ),
		new ReField( "MinerEntity_", &GaGameComponent::MinerEntity_, bcRFF_IMPORTER ),

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
	MothershipEntity_( nullptr ),
	MinerEntity_( nullptr ),
	View_( nullptr ),
	World_( nullptr ),
	SelectedUnit_( nullptr ),
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

		GaUnitComponent* ClickedUnit = nullptr;

		ScnPhysicsLineCastResult Result;
		if( World_->lineCast( Near, Far, &Result ) )
		{
			auto Entity = Result.Entity_;
			auto Unit = Entity->getComponentByType< GaUnitComponent >();
			if( Unit != nullptr )
			{
				ClickedUnit = Unit;
			}
		}
		else
		{
			SelectedUnit_ = nullptr;
		}

		// If we have clicked on a unit, we may need to determine the action also.
		if( ClickedUnit != nullptr )
		{
			BcBool PerformedAction = BcFalse;
			if( SelectedUnit_ != nullptr )
			{
				// Check for unit actions.
				EvtID ActionID = BcErrorCode;
				for( const auto& Action : SelectedUnit_->getActions() )
				{
					for( auto& Component : ClickedUnit->getParentEntity()->getComponents() )
					{
						for( const auto& TargetClass : Action.TargetClasses_ )
						{
							if( Component->getClass()->getName() == TargetClass )
							{
								ActionID = Action.ActionID_;
								break;
							}
						}
						if( ActionID != BcErrorCode ) break;
					}
					if( ActionID != BcErrorCode ) break;
				}

				if( ActionID != BcErrorCode )
				{
					PSY_LOG( "Sending action." );
					GaUnitActionEvent Event;
					Event.SourceUnit_ = SelectedUnit_;
					Event.TargetUnit_ = ClickedUnit;
					SelectedUnit_->getParentEntity()->publish( ActionID, Event );
					PerformedAction = BcTrue;
				}
			}
			else
			{
#if 0
				// Check for non-unit actions.
				// Check for unit actions.
				EvtID ActionID = BcErrorCode;
				for( const auto& Action : SelectedUnit_->getActions() )
				{
					for( auto& Component : ClickedUnit->getParentEntity()->getComponents() )
					{
						if( Action.TargetClasses_.size() == 0 )
						{
							ActionID = Action.ActionID_;
						}
					}
					if( ActionID != BcErrorCode ) break;
				}

				if( ActionID != BcErrorCode )
				{
					GaUnitActionEvent Event;
					Event.SourceUnit_ = SelectedUnit_;
					Event.TargetUnit_ = nullptr;
					SelectedUnit_->getParentEntity()->publish( ActionID, Event );
					PerformedAction = BcTrue;
				}
#endif
			}

			// No action? Reselect.
			if( PerformedAction == BcFalse )
			{
				SelectedUnit_ = ClickedUnit;
			}
		}
		else
		{
			SelectedUnit_ = nullptr;
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

	// Spawn motherships.
	auto SceneEntity = getParentEntity()->getParentEntity();

	MaMat4d MothershipTransform0;
	MaMat4d MothershipTransform1;
	MaMat4d MinerTransform0;
	MaMat4d MinerTransform1;
	MothershipTransform0.translation( MaVec3d( -32.0f, 0.0f, 0.0f ) );
	MothershipTransform1.translation( MaVec3d(  32.0f, 0.0f, 0.0f ) );
	MinerTransform0.translation( MaVec3d( -32.0f, 8.0f, 0.0f ) );
	MinerTransform1.translation( MaVec3d(  32.0f, 8.0f, 0.0f ) );

	BcAssert( MothershipEntity_ );
	BcAssert( MinerEntity_ );
	{
		auto MotherShipEntity0 = ScnCore::pImpl()->spawnEntity(
			ScnEntitySpawnParams(
				"MothershipEntity_0",
				MothershipEntity_,
				MothershipTransform0,
				SceneEntity ) );
		auto MotherUnit = MotherShipEntity0->getComponentByType< GaUnitComponent >();
		MotherUnit->setTeam( 1 );

		auto MinerEntity0 = ScnCore::pImpl()->spawnEntity(
			ScnEntitySpawnParams(
				"MinerEntity_0",
				MinerEntity_,
				MinerTransform0,
				SceneEntity ) );
		auto MinerUnit = MinerEntity0->getComponentByType< GaUnitComponent >();
		MinerUnit->setTeam( 1 );
	}

	{
		auto MotherShipEntity1 = ScnCore::pImpl()->spawnEntity(
			ScnEntitySpawnParams(
				"MothershipEntity_1",
				MothershipEntity_,
				MothershipTransform1,
				SceneEntity ) );
		auto Unit = MotherShipEntity1->getComponentByType< GaUnitComponent >();
		Unit->setTeam( 2 );

		auto MinerEntity1 = ScnCore::pImpl()->spawnEntity(
			ScnEntitySpawnParams(
				"MinerEntity_1",
				MinerEntity_,
				MinerTransform1,
				SceneEntity ) );
		auto MinerUnit = MinerEntity1->getComponentByType< GaUnitComponent >();
		MinerUnit->setTeam( 2 );
	}


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
