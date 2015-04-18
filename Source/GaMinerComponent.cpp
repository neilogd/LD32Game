#include "GaMinerComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaMinerComponent );

void GaMinerComponent::StaticRegisterClass()
{
	/*
	ReField* Fields[] = 
	{
		new ReField( "Size_", &GaMinerComponent::Size_ ),
	};
	*/
	
	ReRegisterClass< GaMinerComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaMinerComponent::GaMinerComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaMinerComponent::~GaMinerComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaMinerComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onPhysicsUpdate
void GaMinerComponent::onPhysicsUpdate( BcF32 Tick )
{
}

//////////////////////////////////////////////////////////////////////////
// onAttach
void GaMinerComponent::onAttach( ScnEntityWeakRef Parent )
{
	auto World = Parent->getComponentAnyParentByType< ScnPhysicsWorldComponent >();
	BcAssert( World );
	World->registerWorldUpdateHandler( this );
	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
void GaMinerComponent::onDetach( ScnEntityWeakRef Parent )
{
	auto World = Parent->getComponentAnyParentByType< ScnPhysicsWorldComponent >();
	if( World )
	{
		World->deregisterWorldUpdateHandler( this );
	}
	Super::onAttach( Parent );
}
