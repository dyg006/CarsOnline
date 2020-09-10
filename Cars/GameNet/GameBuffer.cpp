// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBuffer.h"
#include "Net/Manager.h"
#include "Math/Transform.h"
#include "RotationMatrix.h"

CGameBuffer::CGameBuffer(size_t initsize, size_t delta)
  : Net::CBuffer(initsize, delta)
{
}

CGameBuffer::~CGameBuffer()
{
}

void CGameBuffer::write(const FVector& _data)
{
  write(_data.X);
  write(_data.Y);
  write(_data.Z);
}

void CGameBuffer::read(FVector& data_)
{
  read(data_.X);
  read(data_.Y);
  read(data_.Z);
}

void CGameBuffer::write(const FVector2D& _data)
{
  write(_data.X);
  write(_data.Y);
}

void CGameBuffer::read(FVector2D& data_)
{
  read(data_.X);
  read(data_.Y);
}

void CGameBuffer::write(const FTransform& _data) {
    write(_data.GetRotation().GetAxisX());
    write(_data.GetTranslation());
    write(_data.GetScale3D());
}

void CGameBuffer::read(FTransform& _data) {
    FVector vDir;
    read(vDir);
    FMatrix tMatrix = FRotationMatrix::MakeFromX(vDir);
    _data.SetFromMatrix(tMatrix);
    FVector vPos;
    read(vPos);
    _data.SetTranslation(vPos);
    FVector vScale;
    read(vScale);
    _data.SetScale3D(vScale);
}



void CGameBuffer::write(Net::NetMessageType _data)
{
  write(&_data, sizeof(_data));
}

void CGameBuffer::read(Net::NetMessageType& _data)
{ 
  read(&_data, sizeof(_data));
}