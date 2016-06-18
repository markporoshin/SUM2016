#ifndef __UNITS_H_
#define __UNITS_H_
#include "ANIM.H"


mp2UNIT * MP2_UnitCreateBall( FLT x, FLT y, FLT z );
mp2UNIT * MP2_UnitCreateCube( FLT x, FLT y, FLT z );
mp2UNIT * MP2_UnitCreateSTATICMODEL( FLT x, FLT y, FLT z );
mp2UNIT * MP2_UnitCreateSee( VOID );

VOID LoadSphere( VOID );

#endif /* __UNITS_H_ */