#pragma once
#include "Socket.h"
#include "Snapshot.h"
#include "ScratchAck.h"
#include "SnaphotRecordKeeper.h"
#include "NetworkObjectManagement.h"
#include "Address.h"

#include <iostream>
#include <random>
#include <chrono>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <queue>


#define SNC_EXPORTS

#ifdef SNC_EXPORTS
#define SNC_API __declspec(dllexport)
#else
#define SNC_API  __declspec(dllimport)
#endif

extern std::atomic<bool> shutDownRequested = false;

class ScratchNetClient
{
public:
    ScratchNetClient();

    void InitSockets();
    void ClientProcess();
    void ClientListen(void* recieveBuf);

    void UpdateNetworkedObject(Snapshot changesToBeMade);

    int generateObjectID();

    //queuing snapshots ready for sending to the server
    void QueueSnapshot(int objectID, float posX, float posY, float posZ);
    Snapshot ExtractTopSnapshotToProcess();
    void ProcessTopSnapshot(); //pops the top most snapshot when request is fulfilled

public:
    //std::unordered_map<int, Snapshot> networkedObjects;

    int objectID = 0;

    NetworkObjectManagement nom;
    SnapshotRecordKeeper* ssRecordKeeper;
    ScratchAck* packetAckMaintence;
    Snapshot clientSnap;
    Socket clientSock;

    Address* sendAddress;

    std::thread clientThread;

    std::queue<Snapshot> snapshotsToProcess; //when the unity object needs to sends its changes it will queue up a snapshot for the client to process

};

extern "C"
{
    SNC_API ScratchNetClient* InitializeClient();

    SNC_API void BeginClientProcess(ScratchNetClient* client); //begin the client process of sending and recieving packets

    SNC_API void CleanupClient(ScratchNetClient* client);

    SNC_API int GetObjectID(ScratchNetClient* client);

    SNC_API void QueuePositionToClient(ScratchNetClient* client, int objectID, float posX, float posY, float posZ);

    //when the unity object needs to extract the data from the baseline snapshot
    SNC_API float RetrieveBaselinePacketPosX(ScratchNetClient* client);
    SNC_API float RetrieveBaselinePacketPosY(ScratchNetClient* client);
    SNC_API float RetrieveBaselinePacketPosZ(ScratchNetClient* client);


}
