#include "pch.h"
#include "NetworkObjectManagement.h"

NetworkObjectManagement::NetworkObjectManagement()
{
}

void NetworkObjectManagement::AddObjectToSpawnRequest(Snapshot objectToSpawn)
{
    requestedObjectsToSpawn.push(objectToSpawn);
}

void NetworkObjectManagement::ProcessObjectSpawnRequest()
{
    requestedObjectsToSpawn.pop();
}

Snapshot* NetworkObjectManagement::GetTopRequest()
{
    Snapshot* snap = &requestedObjectsToSpawn.front();
    return snap;
}



bool NetworkObjectManagement::TryUpdatingNetworkedObject(Snapshot changesToUpdate)
{
    if (!networkedObjects.contains(changesToUpdate.objectId))
    {
        return false; //we already have 
    }

    networkedObjects[changesToUpdate.objectId] = changesToUpdate;
    return true;
}

bool NetworkObjectManagement::TryInsertNewNetworkObject(Snapshot snapshotToInsert)
{
    if (networkedObjects.contains(snapshotToInsert.objectId))
    {
        return false; //we already have 
    }

    networkedObjects[snapshotToInsert.objectId] = snapshotToInsert;
    AddObjectToSpawnRequest(snapshotToInsert); //store the request for the code on unity to extract and spawn it
    return true;
}
