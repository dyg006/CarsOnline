#include "ServerCars.h"
#include "Net/buffer.h"
#include <iostream>

CServerCars::CServerCars()
{
  Net::CManager::Init();
  m_pManager = Net::CManager::getSingletonPtr();
  m_pManager->addObserver(this);
  m_pManager->activateAsServer(65785);
}

//--------------------------------

CServerCars::~CServerCars()
{
  Net::CManager::Release();
}

//--------------------------------

void CServerCars::Step()
{
  m_pManager->tick();
}

//--------------------------------

void CServerCars::dataPacketReceived(Net::CPaquete* packet)
{

}

//--------------------------------

void CServerCars::connexionPacketReceived(Net::CPaquete* packet)
{
  // Creamos un buffer con los datos para leer de manera más cómoda
  Net::CBuffer data;
  const char* sHellow = "Connected";
  data.write(sHellow, sizeof(sHellow));
  m_pManager->send(data.getbuffer(), data.getSize(), true);
  std::cout << "Client connected! ";
}

//--------------------------------

void CServerCars::disconnexionPacketReceived(Net::CPaquete* packet)
{

}
