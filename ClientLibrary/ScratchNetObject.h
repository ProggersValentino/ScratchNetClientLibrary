#pragma once
#include <SnaphotRecordKeeper.h>

#ifdef SNO_EXPORTS
#define SNO_API __declspec(dllexport)
#else
#define SNO_API  __declspec(dllimport)
#endif

class ScratchNetObject
{
public:

	ScratchNetObject(int objectID);

	int objectID = 0;

	SnapshotRecordKeeper* ssRecordKeeper;
};

extern "C"
{
	SNO_API int GetSNObjectID(ScratchNetObject* sno);
	SNO_API SnapshotRecordKeeper* GetSNORecordKeeper(ScratchNetObject* sno);
}