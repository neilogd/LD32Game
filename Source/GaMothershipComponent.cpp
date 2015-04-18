#include "GaMothershipComponent.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"

#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaMothershipComponent );

void GaMothershipComponent::StaticRegisterClass()
{
	ReRegisterClass< GaMothershipComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaMothershipComponent::GaMothershipComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaMothershipComponent::~GaMothershipComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaMothershipComponent::update( BcF32 Tick )
{
	auto RigidBody = getComponentByType< ScnPhysicsRigidBodyComponent >();

	// Stabilise ship position.
	auto RigidBodyPosition = RigidBody->getPosition();
	auto Displacement = TargetPosition_ - RigidBodyPosition;
	RigidBody->applyCentralForce( Displacement * RigidBody->getMass() );

	// Stabilise ship rotation.
	auto RigidBodyRotation = RigidBody->getRotation();
	RigidBodyRotation.inverse();
	MaQuat RotationDisplacement = TargetRotation_ * RigidBodyRotation;
	auto EularRotationDisplacement = RotationDisplacement.asEuler();
	RigidBody->applyTorque( EularRotationDisplacement * RigidBody->getMass() * 10.0f );

	// Dampen.
	RigidBody->setLinearVelocity( RigidBody->getLinearVelocity() * 0.99f );
	RigidBody->setAngularVelocity( RigidBody->getAngularVelocity() * 0.99f );

	// Random impulse just to make it look a bit more funky.
	if( BcRandom::Global.randRealRange( 0.0f, 100.0f ) < 1.0f )
	{
		RigidBody->applyImpulse( 
			MaVec3d( 
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ) ) * RigidBody->getMass() * 0.1f,
			MaVec3d( 
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ),
				BcRandom::Global.randRealRange( -1.0f, 1.0f ) ) * 0.5f );
	}
}

//////////////////////////////////////////////////////////////////////////
// update
void GaMothershipComponent::onAttach( ScnEntityWeakRef Parent )
{
	auto WorldMatrix = Parent->getWorldMatrix();
	TargetPosition_ = WorldMatrix.translation();
	TargetRotation_.fromMatrix4d( WorldMatrix );
	Super::onAttach( Parent );
}
