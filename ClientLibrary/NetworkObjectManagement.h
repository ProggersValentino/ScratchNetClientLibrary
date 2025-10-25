#pragma once
#include "ScratchNetObject.h"

#include <Snapshot.h>
#include <unordered_map>
#include <queue>

#define NOM_EXPORTS

#ifdef NOM_EXPORTS
#define NOM_API __declspec(dllexport)
#else
#define NOM_API  __declspec(dllimport)
#endif

/// <summary>
/// the purpose of this class is to manage any networked objects that isnt hte client directly and keep they're states up to date
/// </summary>
class NetworkObjectManagement
{
public:
	NetworkObjectManagement();

	std::unordered_map<int, ScratchNetObject*> networkedObjects;
	std::queue<ScratchNetObject*> requestedObjectsToSpawn;

	void AddObjectToSpawnRequest(ScratchNetObject* objectToSpawn);

	void ProcessObjectSpawnRequest();

	ScratchNetObject* GetTopRequest();

	bool TryUpdatingNetworkedObject(uint16_t sequence, Snapshot changesToUpdate);

	//return false if there is already a object in being tracked
	bool TryInsertNewNetworkObject(uint16_t sequence, Snapshot snapshotToInsert);
};

extern "C"
{
	//Get the top network object from the spawn request 
	NOM_API ScratchNetObject* RetrieveSpawnRequest(NetworkObjectManagement* nom);

	NOM_API void CompleteSpawnRequest(NetworkObjectManagement* nom);

	NOM_API ScratchNetObject* RetrieveScratchNetObject(NetworkObjectManagement* nom, int objectID);
	
	//Extract the snapshot from the given the ScratchNetObject 
	NOM_API Snapshot* GetBaselineNetworkedObjSnapshot(ScratchNetObject* snObject);

	//Get positional data from the snapshot of the ScratchNetObject (NOS)
	NOM_API float GetPosXFromNOS(Snapshot* snapshot);
	NOM_API float GetPosYFromNOS(Snapshot* snapshot);
	NOM_API float GetPosZFromNOS(Snapshot* snapshot);
}