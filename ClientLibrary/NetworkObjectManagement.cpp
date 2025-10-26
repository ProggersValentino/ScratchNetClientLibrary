#include "pch.h"
#include "NetworkObjectManagement.h"

NetworkObjectManagement::NetworkObjectManagement()
{
}

void NetworkObjectManagement::AddObjectToSpawnRequest(ScratchNetObject* objectToSpawn)
{
    requestedObjectsToSpawn.push(objectToSpawn);
}

void NetworkObjectManagement::ProcessObjectSpawnRequest()
{
    requestedObjectsToSpawn.pop();
}

ScratchNetObject* NetworkObjectManagement::GetTopRequest()
{
    if (requestedObjectsToSpawn.empty())
    {
        return nullptr; //cant extract 
    }

    ScratchNetObject* snap = requestedObjectsToSpawn.front();
    return snap;
}



bool NetworkObjectManagement::TryUpdatingNetworkedObject(uint16_t sequence, Snapshot changesToUpdate)
{
    if (!networkedObjects.contains(changesToUpdate.objectId))
    {
        return false; //we already have 
    }

  networkedObjects[changesToUpdate.objectId]->ssRecordKeeper->InsertNewRecord(sequence, changesToUpdate);
    return true;
}

bool NetworkObjectManagement::TryInsertNewNetworkObject(uint16_t sequence, Snapshot snapshotToInsert)
{
    if (networkedObjects.contains(snapshotToInsert.objectId))
    {
        return false; //we already have 
    }

    //init new network object
    networkedObjects[snapshotToInsert.objectId] = new ScratchNetObject(snapshotToInsert.objectId);
    networkedObjects[snapshotToInsert.objectId]->ssRecordKeeper->InsertNewRecord(sequence, snapshotToInsert);
    
    AddObjectToSpawnRequest(networkedObjects[snapshotToInsert.objectId]); //store the request for the code on unity to extract and spawn it
    return true;
}

NOM_API ScratchNetObject* RetrieveSpawnRequest(NetworkObjectManagement* nom)
{
    ScratchNetObject* request = nom->GetTopRequest();
 
    if (request == nullptr)
    {
        printf("Failed to retrieve request");
        return nullptr;
    }

    return request;
}

void CompleteSpawnRequest(NetworkObjectManagement* nom)
{
    nom->ProcessObjectSpawnRequest();
}

ScratchNetObject* RetrieveScratchNetObject(NetworkObjectManagement* nom, int objectID)
{
    if (!nom->networkedObjects.contains(objectID)) //if we cant find a NOS then we return with error msg
    {
        std::cout << "Failed to find Networked Object \n" << std::endl;
        return nullptr;
    }

    return nom->networkedObjects[objectID];
}

NOM_API Snapshot* GetBaselineNetworkedObjSnapshot(ScratchNetObject* snObject)
{
    if (snObject == nullptr)
    {
        std::cout << "Inputed Scratch Net Object was null ptr \n" << std::endl;
        return nullptr;
    }

    Snapshot* nosSnap = snObject->ssRecordKeeper->baselineRecord.recordedSnapshot;

    if (nosSnap == nullptr)
    {
        std::cout << "Failed to retrieve baseline snapshot\n" << std::endl;
    }

    return nosSnap;
}

NOM_API float GetPosXFromNOS(Snapshot* snapshot)
{
    return snapshot->posX;
}

float GetPosYFromNOS(Snapshot* snapshot)
{

    return snapshot->posY;
}

float GetPosZFromNOS(Snapshot* snapshot)
{
    return snapshot->posZ;
}
