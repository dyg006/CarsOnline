
#pragma once

#include "Net/buffer.h"
#include "Math/Vector.h"

namespace Net
{
  enum NetMessageType;
}

class CGameBuffer : public Net::CBuffer
{
public:
  CGameBuffer(size_t initsize = 500, size_t delta = 100);
  virtual ~CGameBuffer();

  using Net::CBuffer::write;
  using Net::CBuffer::read;

  void write(const FVector& _data);
  void read(FVector& data_);

  void write(const FVector2D& _data);
  void read(FVector2D& data_);

  void write(const FTransform& _data);
  void read(FTransform& _data);

  void write(Net::NetMessageType _data);
  void read(Net::NetMessageType& _data);
};
