// Fill out your copyright notice in the Description page of Project Settings.

#include "GameNetMgr.h"
#include "GameNet/GameBuffer.h"
#include "Net/Paquete.h"
#include "CarsGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Car.h"
#include "Game/Projectile.h"
#include "GameNet/NetCarComponent.h"
#include "GameNet/NetProjectileComponent.h"
#include "Engine/World.h"
#include "Engine/LevelStreaming.h"

CGameNetMgr::CGameNetMgr()
{
  Net::CManager::Init();
  m_pManager = Net::CManager::getSingletonPtr();
  m_pManager->addObserver(this);
}

CGameNetMgr::CGameNetMgr(UCarsGameInstance* _pOwner)
  : m_pCarsGameInstance(_pOwner)
{
  Net::CManager::Init();
  m_pManager = Net::CManager::getSingletonPtr();
  m_pManager->addObserver(this);
}

CGameNetMgr::~CGameNetMgr()
{
  m_pManager->removeObserver(this);
  Net::CManager::Release();
}

void CGameNetMgr::dataPacketReceived(Net::CPaquete* packet)
{
    CGameBuffer data;
    data.write(packet->getData(), packet->getDataLength());
    data.reset();
    Net::NetMessageType xType;
    data.read(xType);
    switch (xType)
    {
        case Net::LOAD_MAP:
        {
            char sLevel[64];
            data.read(sLevel);
            UGameplayStatics::OpenLevel(m_pCarsGameInstance->GetWorld(), sLevel);
            CGameBuffer data2;
            Net::NetMessageType iID2 = Net::NetMessageType::MAP_LOADED;
            data2.write(iID2);
            m_pManager->send(data2.getbuffer(), data2.getSize(), true);
        } break;
        case Net::MAP_LOADED:
        {
            ++m_uMapLoadedNotifications;
            if (m_uMapLoadedNotifications >= m_pManager->getConnections().size())
            {
                for (auto client : m_pManager->getConnections()) {
                    CGameBuffer dataCar;
                    Net::NetMessageType iIDLoad = Net::NetMessageType::LOAD_PLAYER;
                    dataCar.write(iIDLoad);
                    dataCar.write(client.first);
                    FVector vPos(220.f, -310.f + client.first * 40.f, 0.f);
                    dataCar.write(vPos);
                    m_pManager->send(dataCar.getbuffer(), dataCar.getSize(), true);
                    CreateCar(client.first, vPos);
                }
            }
      } break;
        case Net::LOAD_PLAYER:
        {
            unsigned int uClient;
            data.read(uClient);
            FVector vPos;
            data.read(vPos);
            CreateCar(uClient, vPos);
            if (uClient == m_pManager->getID())
            {
                APlayerController* pPC = GEngine->GetFirstLocalPlayerController(m_pCarsGameInstance->GetWorld());
                if (pPC)
                {
                pPC->Possess(m_vPlayers[uClient]);
                }
            }
        } break;
        case Net::ENTITY_MSG:
        {
            unsigned int uClient;
            data.read(uClient);
            ACar* pCar = m_vPlayers.at(uClient);
            pCar->GetNetComponent()->DeserializeData(data);
        }
        break;
        //crear proyectil
        case Net::CREATE_ENTITY:
        {
            unsigned int uClient;
            data.read(uClient);
            if (m_pManager->getID() == Net::ID::SERVER)
            {
                CreateProjectile(uClient);

                //Send a message to all clients to create the projectile on them (it has no effects on clients)
                for (auto client : m_pManager->getConnections()) {
                    CGameBuffer dataProjectile;
                    Net::NetMessageType iIDLoad = Net::NetMessageType::LOAD_PROJECTILE;
                    dataProjectile.write(iIDLoad);
                    dataProjectile.write(uClient);
                    dataProjectile.write(client.first);
                    m_pManager->send(dataProjectile.getbuffer(), dataProjectile.getSize(), true);
                }
            }
        }
        break;

        case Net::LOAD_PROJECTILE:
        {
            unsigned int clientProjectile, uClient;
            data.read(clientProjectile);
            data.read(uClient);
            if (m_pManager->getID() == uClient) {
                
                //Create the projectile on client    
                CreateProjectile(clientProjectile);
            }
        }
        break;

        case Net::DESTROY_PROJECTILE:
        {
            if (m_pManager->getID() != Net::ID::SERVER) {
                unsigned int projectileID, carID;
                 data.read(projectileID);
                AProjectile* projectile = m_vProjectiles.at(projectileID);
                //clean the maps with the corresponding projectile
              /*  m_vProjectiles.erase(projectileID);
                m_vProjectilesIDs.erase(projectile);
                --numberOfProjectiles;*/
                data.read(carID);
                ACar* carHit = m_vPlayers.at(carID);
                projectile->GetNetComponent()->ReplicateProjectCollision(carHit);
            }
        }
        break;


    default: break;
    }
}

void CGameNetMgr::connexionPacketReceived(Net::CPaquete* packet)
{
  if (m_pManager->getID() == Net::ID::SERVER)
  {
    if (GEngine)
    {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Client Connected!");
    }
  }
}

void CGameNetMgr::disconnexionPacketReceived(Net::CPaquete* packet)
{

}

void CGameNetMgr::CreateCar(int _iClient, FVector _vPos)
{
  FActorSpawnParameters SpawnInfo;
  SpawnInfo.Name = FName("Car", _iClient);
  SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  ACar* pCar = m_pCarsGameInstance->GetWorld()->SpawnActor<ACar>(_vPos, FRotator::ZeroRotator, SpawnInfo);
  if (pCar)
  {
    m_vPlayers[_iClient] = pCar;
    m_vPlayerIDs[pCar] = _iClient;
  }
}

void CGameNetMgr::CreateProjectile(int _iClient)
{
    ACar* car = m_vPlayers.at(_iClient);
    if (car != nullptr && m_pCarsGameInstance != nullptr) {
        /*if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Projectile created");
        }*/
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AProjectile* projectile = m_pCarsGameInstance->GetWorld()->SpawnActor<AProjectile>(car->GetActorLocation(), car->GetActorRotation(), SpawnInfo);

        check(projectile);

        projectile->ownCar = car;
        if (projectile != nullptr)
        {
            
            m_vProjectiles[numberOfProjectiles] = projectile;            
            m_vProjectilesIDs[projectile] = numberOfProjectiles;
            ++numberOfProjectiles;
        }
    }
    
  
}

ACar* CGameNetMgr::GetOwnCar() const
{
  if (m_pManager->getID() != Net::ID::SERVER)
  {
    return m_vPlayers.at(m_pManager->getID());
  }
  return nullptr;
}

unsigned int CGameNetMgr::GetCarID(ACar* _pCar) const
{
    if (_pCar != nullptr) {
        return m_vPlayerIDs.at(_pCar);
    }
    else {
        return 0;
    }
}


unsigned int CGameNetMgr::GetProjectileID(AProjectile* _pProjectile) const
{
    if (_pProjectile != nullptr) {
        return m_vProjectilesIDs.at(_pProjectile);
    }
    else {
        return 0;
    }
}