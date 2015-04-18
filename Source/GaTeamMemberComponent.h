#pragma once
#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaTeamMemberComponent
class GaTeamMemberComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTeamMemberComponent, ScnComponent );

	GaTeamMemberComponent();
	virtual ~GaTeamMemberComponent();

	BcU32 getTeam() const { return Team_; }

private:
	BcU32 Team_;

};
