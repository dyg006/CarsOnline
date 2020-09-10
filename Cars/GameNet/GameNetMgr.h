
#pragma once

#include "Net/Manager.h"
#include "Math/Vector.h"

class UCarsGameInstance;
class ACar;
class AProjectile;

class CGameNetMgr : public Net::CManager::IObserver
{
public:
  CGameNetMgr();
  CGameNetMgr(UCarsGameInstance* _pOwner);
  virtual ~CGameNetMgr();

  ACar* GetOwnCar() const;
  unsigned int GetCarID(ACar* _pCar) const;
  unsigned int GetProjectileID(AProjectile* _pProjectile) const;

  virtual void dataPacketReceived(Net::CPaquete* packet);
  virtual void connexionPacketReceived(Net::CPaquete* packet);
  virtual void disconnexionPacketReceived(Net::CPaquete* packet);

private:
  void CreateCar(int _iClient, FVector _vPos);
  void CreateProjectile(int _iClient);
private:
  UCarsGameInstance* m_pCarsGameInstance = nullptr;
  Net::CManager* m_pManager = nullptr;
  unsigned int m_uMapLoadedNotifications = 0u;
  std::map<unsigned int, ACar*> m_vPlayers;
  std::map<ACar*, unsigned int> m_vPlayerIDs;

  std::map<unsigned int, AProjectile*> m_vProjectiles;
  std::map<AProjectile*, unsigned int> m_vProjectilesIDs;

  int numberOfProjectiles = 0;
};
