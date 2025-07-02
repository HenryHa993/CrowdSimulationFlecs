// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitConfigSet.h"


// Add default functionality here for any IUnitConfigSet functions that are not pure virtual.
void IUnitConfigSet::SetUnit(UUnitConfig* Config)
{
	UnitConfig = Config;
}
