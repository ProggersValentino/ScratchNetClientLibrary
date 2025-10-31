#pragma once
#include "Socket.h"
#include "Snapshot.h"
#include "ScratchAck.h"
#include "SnaphotRecordKeeper.h"
#include "NetworkObjectManagement.h"
#include "Address.h"

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <random>
#include <chrono>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>


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

    void UpdateNetworkedObject(uint16_t sequence, Snapshot changesToBeMade);

    int generateObjectID();

    //queuing snapshots ready for sending to the server
    void QueueSnapshot(int objectID, float posX, float posY, float posZ);
    Snapshot ExtractTopSnapshotToProcess();
    void ProcessTopSnapshot(); //pops the top most snapshot when request is fulfilled

    bool CanSendPacket();

    void UpdateSendRateCalc();

public:
    //std::unordered_map<int, Snapshot> networkedObjects;

    int objectID = 0;

    NetworkObjectManagement* nom;
    SnapshotRecordKeeper* ssRecordKeeper;
    ScratchAck* packetAckMaintence;
    Snapshot clientSnap;
    Socket clientSock;

    Address* sendAddress;

    int packetSendRate = 0;
    float packetMilliConverted = 1.f / packetSendRate;
    float accum = 0.f;

    std::thread clientThread;

    std::queue<Snapshot> snapshotsToProcess; //when the unity object needs to sends its changes it will queue up a snapshot for the client to process
   
    //we need these mutexes to lock up variables to ensure they cannot be accessed by other threads ensuring no data corruption and we dont get silent errors
    std::mutex snapshotQueueMutex;
    std::mutex packetMaintenceMutex;
    std::mutex snapRecordKeeperMutex;


    static BOOL ConsoleHandler(DWORD signal) {
        switch (signal) {
        case CTRL_C_EVENT:
            /*case CTRL_CLOSE_EVENT:*/
            std::cout << "\nGracefully shutting down...\n";
            shutDownRequested = true; // signal threads to stop
            return TRUE;     // tell the OS you handled it
        default:
            return FALSE;
        }
    }

};

extern "C"
{
    SNC_API ScratchNetClient* InitializeClient();

    SNC_API NetworkObjectManagement* ExtractNOM(ScratchNetClient* client);

    SNC_API void BeginClientProcess(ScratchNetClient* client); //begin the client process of sending and recieving packets

    SNC_API void CleanupClient(ScratchNetClient* client);

    SNC_API int GetObjectID(ScratchNetClient* client);

    SNC_API void QueuePositionToClient(ScratchNetClient* client, int objectID, float posX, float posY, float posZ);

    //when the unity object needs to extract the data from the baseline snapshot
    SNC_API float RetrieveBaselinePacketPosX(ScratchNetClient* client);
    SNC_API float RetrieveBaselinePacketPosY(ScratchNetClient* client);
    SNC_API float RetrieveBaselinePacketPosZ(ScratchNetClient* client);

    SNC_API bool CanPacketBeQueuedToSend(ScratchNetClient* client);

    //extracts a snapshot from the given index from the client's RecordKeeper
    SNC_API Snapshot* ExtractSnapshotFromIndex(ScratchNetClient* clientObject, int index);

    SNC_API SnapshotRecord* ExtractSnapshotRecordFromIndex(ScratchNetClient* clientObject, int index);
    
    //because hte record keeper is constantly changing we'll need to grab the new index the record is at
    SNC_API int FindRecordIndex(ScratchNetClient* clientObject, SnapshotRecord* recordToLookFor);

    SNC_API float RetrievePacketPosX(Snapshot* chosenSnapshot);
    SNC_API float RetrievePacketPosY(Snapshot* chosenSnapshot);
    SNC_API float RetrievePacketPosZ(Snapshot* chosenSnapshot);

    //to set how many packets we want to send to the server per second
    SNC_API void SetPacketSendRate(ScratchNetClient* clientObject, int sendRate);
}
