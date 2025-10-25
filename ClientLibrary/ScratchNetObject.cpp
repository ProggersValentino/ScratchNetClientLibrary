#include "pch.h"
#include "ScratchNetObject.h"

ScratchNetObject::ScratchNetObject(int inputedObjID)
{
	objectID = inputedObjID;
	ssRecordKeeper = InitRecordKeeper();
}

int GetSNObjectID(ScratchNetObject* sno)
{
	return sno->objectID;
}

SnapshotRecordKeeper* GetSNORecordKeeper(ScratchNetObject* sno)
{
	return sno->ssRecordKeeper;
}
