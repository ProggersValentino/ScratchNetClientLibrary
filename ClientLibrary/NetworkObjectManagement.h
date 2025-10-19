#pragma once
#include <Snapshot.h>
#include <unordered_map>
#include <queue>



class NetworkObjectManagement
{
public:
	NetworkObjectManagement();

	std::unordered_map<int, Snapshot> networkedObjects;
	std::queue<Snapshot> requestedObjectsToSpawn;

	void AddObjectToSpawnRequest(Snapshot objectToSpawn);

	void ProcessObjectSpawnRequest();

	Snapshot* GetTopRequest();

	bool TryUpdatingNetworkedObject(Snapshot changesToUpdate);

	//return false if there is already a object in being tracked
	bool TryInsertNewNetworkObject(Snapshot snapshotToInsert);
};