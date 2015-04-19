#include "GaGameComponent.h"
#include "GaUnitComponent.h"

#include "System/Content/CsPackage.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnViewComponent.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"
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
		new ReField( "MothershipEntity_", &GaGameComponent::MothershipEntity_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "UIMaterial_", &GaGameComponent::UIMaterial_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "UIMaterialComponent_", &GaGameComponent::UIMaterialComponent_, bcRFF_TRANSIENT ),
		new ReField( "View_", &GaGameComponent::View_, bcRFF_TRANSIENT ),
		new ReField( "World_", &GaGameComponent::World_, bcRFF_TRANSIENT ),
		new ReField( "SelectedUnit_", &GaGameComponent::SelectedUnit_, bcRFF_TRANSIENT ),
		new ReField( "MouseEvents_", &GaGameComponent::MouseEvents_, bcRFF_TRANSIENT ),
	};

	ReRegisterClass< GaGameComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( -10 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaGameComponent::GaGameComponent():
	MothershipEntity_( nullptr ),
	UIMaterial_( nullptr ),
	UIMaterialComponent_( nullptr ),
	View_( nullptr ),
	World_( nullptr ),
	Canvas_( nullptr ),
	MousePosition_( 0.0f, 0.0f ),
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
	// Get projection for UI.
	OsClient* Client = OsCore::pImpl()->getClient( 0 );
	BcF32 HalfWidth = static_cast< BcF32 >( Client->getWidth() / 2 );
	BcF32 HalfHeight = static_cast< BcF32 >( Client->getHeight() / 2 );

	MaMat4d UIProjection;
	UIProjection.orthoProjection( -HalfWidth, HalfWidth, HalfHeight, -HalfHeight, -1.0f, 1.0f );

	Canvas_->clear();
	Canvas_->pushMatrix( UIProjection );
	Canvas_->setMaterialComponent( UIMaterialComponent_ );

	BcAssert( View_ != nullptr );
	BcAssert( World_ != nullptr );
	BcF32 IconSize = 64.0f;

	// Check hover.
	{

		GaUnitComponent* HoverUnit = getUnitAt( MousePosition_ );
		if( HoverUnit != nullptr )
		{
			auto Position = View_->getScreenPosition( HoverUnit->getParentEntity()->getWorldPosition() );

			BcU32 NoofActions = 0;
			std::array< const struct GaUnitAction*, 3 > Actions;				
			for( BcU32 MouseButton = 0; MouseButton < Actions.size(); ++MouseButton )
			{
				auto Action = getAction( MouseButton, HoverUnit );
				if( Action )
				{
					Actions[ NoofActions++ ] = Action;
				}
			}

			MaVec2d Offsets[3] =
			{
				MaVec2d( -IconSize, 0.0f ),
				MaVec2d( IconSize, 0.0f ),
				MaVec2d( 0.0f, 0.0f )
			};

			for( BcU32 Idx = 0; Idx < NoofActions; ++Idx )
			{
				auto Action = Actions[ Idx ];
				auto Offset = Offsets[ Action->MouseButton_ ];
				Canvas_->drawSpriteCentered( Position + Offset - MaVec2d( 0.0f, IconSize ), MaVec2d( IconSize, IconSize ), Action->MouseButton_, RsColour::GREEN, 100 );
				Canvas_->drawSpriteCentered( Position + Offset, MaVec2d( IconSize, IconSize ), Action->Icon_, RsColour::GREEN, 100 );
			}

			// Hover selection.
			if( NoofActions == 0 || SelectedUnit_ == nullptr )
			{
				// Selectable? Draw that thing.
				if( isUnitSelectable( HoverUnit ) )
				{
					Canvas_->drawSpriteCentered( Position, MaVec2d( IconSize, IconSize ), (BcU32)GaGameIcon::SELECT, RsColour::GREEN, 100 );
				}
			}
		}
	}

	if( SelectedUnit_ != nullptr )
	{
		auto Position = View_->getScreenPosition( SelectedUnit_->getParentEntity()->getWorldPosition() );
		Canvas_->drawSpriteCentered( Position, MaVec2d( IconSize, IconSize ), (BcU32)GaGameIcon::SELECT, RsColour::GREEN, 90 );
	}
	
	for( const auto& EventPair : MouseEvents_ )
	{
		auto ID = EventPair.first;
		const auto& Event = EventPair.second;

		MaVec2d Position( Event.MouseX_, Event.MouseY_ );
		GaUnitComponent* ClickedUnit = getUnitAt( Position );
		// If we have clicked on a unit, we may need to determine the action also.
		if( ClickedUnit != nullptr)
		{
			BcBool PerformedAction = BcFalse;
			if( SelectedUnit_ != nullptr )
			{
				auto Action = getAction( Event.ButtonCode_, ClickedUnit );

				if( Action != nullptr )
				{
					PSY_LOG( "Sending action." );
					GaUnitActionEvent Event;
					Event.SourceUnit_ = SelectedUnit_;
					Event.TargetUnit_ = ClickedUnit;
					SelectedUnit_->getParentEntity()->publish( Action->ActionID_, Event );
					PerformedAction = BcTrue;
				}
			}

			// No action? Reselect.
			if( isUnitSelectable( ClickedUnit ) )
			{
				if( PerformedAction == BcFalse )
				{
					if( SelectedUnit_ ) SelectedUnit_->removeNotifier( this );
					SelectedUnit_ = ClickedUnit;
					if( SelectedUnit_ ) SelectedUnit_->addNotifier( this );
				}
			}
		}
		else
		{
			if( SelectedUnit_ ) SelectedUnit_->removeNotifier( this );
			SelectedUnit_ = nullptr;
		}

	}
	MouseEvents_.clear();
}

//////////////////////////////////////////////////////////////////////////
// update
void GaGameComponent::onAttach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEMOVE, this, 
		[ this ]( EvtID ID, const EvtBaseEvent& BaseEvent )
		{	
			auto Event = BaseEvent.get< OsEventInputMouse >();
			MousePosition_ = MaVec2d( Event.MouseX_, Event.MouseY_ );
			return evtRET_PASS;
		} );

	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, this, 
		[ this ]( EvtID ID, const EvtBaseEvent& BaseEvent )
		{	
			auto Event = BaseEvent.get< OsEventInputMouse >();
			MouseEvents_.push_back( std::make_pair( ID, Event ) );
			return evtRET_PASS;
		} );

	ScnShaderPermutationFlags ShaderPermutation = 
		ScnShaderPermutationFlags::MESH_STATIC_2D |
		ScnShaderPermutationFlags::RENDER_FORWARD |
		ScnShaderPermutationFlags::LIGHTING_NONE;
	BcAssert( UIMaterial_ );
	UIMaterialComponent_ = Parent->attach< ScnMaterialComponent >( BcName::INVALID, UIMaterial_, ShaderPermutation );

	View_ = getComponentAnyParentByType< ScnViewComponent >();
	World_ = getComponentAnyParentByType< ScnPhysicsWorldComponent >();
	Canvas_ = getComponentAnyParentByType< ScnCanvasComponent >();

	// Spawn motherships.
	auto SceneEntity = getParentEntity()->getParentEntity();

	MaMat4d MothershipTransform0;
	MaMat4d MothershipTransform1;
	MothershipTransform0.translation( MaVec3d(  32.0f, -1.0f, 0.0f ) );
	MothershipTransform1.translation( MaVec3d( -32.0f, -1.0f, 0.0f ) );

	BcAssert( MothershipEntity_ );
	{
		auto MotherShipEntity0 = ScnCore::pImpl()->spawnEntity(
			ScnEntitySpawnParams(
				"MothershipEntity_0",
				MothershipEntity_,
				MothershipTransform0,
				SceneEntity ) );
		auto MotherUnit = MotherShipEntity0->getComponentByType< GaUnitComponent >();
		MotherUnit->setTeam( 1 );
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

//////////////////////////////////////////////////////////////////////////
// getUnitAt
class GaUnitComponent* GaGameComponent::getUnitAt( MaVec2d& MousePosition )
{
	MaVec3d Near, Far;
	View_->getWorldPosition( MousePosition, Near, Far );

	GaUnitComponent* ClickedUnit = nullptr;
	ScnPhysicsLineCastResult Result;
	if( World_->sphereCast( Near, Far, 1.0f, &Result ) )
	{
		auto Entity = Result.Entity_;
		auto Unit = Entity->getComponentByType< GaUnitComponent >();
		if( Unit != nullptr )
		{
			ClickedUnit = Unit;
		}
	}
	return ClickedUnit;
}

//////////////////////////////////////////////////////////////////////////
// isUnitSelectable
BcBool GaGameComponent::isUnitSelectable( class GaUnitComponent* Unit )
{
	if( Unit->getTeam() == 1 )
	{
		return BcTrue;	
	}
	return BcFalse;	
}

//////////////////////////////////////////////////////////////////////////
// getAction
const struct GaUnitAction* GaGameComponent::getAction( BcU32 MouseButton, GaUnitComponent* TargetUnit )
{
	const struct GaUnitAction* RetAction = nullptr;
	if( SelectedUnit_ != nullptr )
	{
		// Check for unit actions.
		EvtID ActionID = BcErrorCode;
		for( const auto& Action : SelectedUnit_->getActions() )
		{
			if( Action.MouseButton_ == MouseButton )
			{
				for( auto& Component : TargetUnit->getParentEntity()->getComponents() )
				{
					for( const auto& TargetClass : Action.TargetClasses_ )
					{
						if( Component->getClass()->getName() == TargetClass )
						{
							RetAction = &Action;
							break;
						}
					}
					if( RetAction != nullptr ) break;
				}
			}
			if( RetAction != nullptr ) break;
		}
	}
	return RetAction;
}
