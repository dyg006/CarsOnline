//---------------------------------------------------------------------------
// factoriaredenet.h
//---------------------------------------------------------------------------

/**
 * @file factoriaredenet.h
 *
 * Especifica una factoria de objetos de red implementados con ENet
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Octumbre, 2006
 */

#ifndef __FACTORIAREDENET_H
#define __FACTORIAREDENET_H

#include "servidorEnet.h"
#include "clienteEnet.h"
#include "factoriared.h"

namespace Net {

/**
 * Factor�a para obtener el servidor y cliente de red implementados con la librer�a ENet
 */
class CFactoriaRedEnet: public CFactoriaRed {

public:

	CFactoriaRedEnet() {};

	CServidor* buildServidor();


	CCliente*  buildCliente();

};



CServidor* CFactoriaRedEnet::buildServidor()
{
	return new CServidorENet();
}

CCliente* CFactoriaRedEnet::buildCliente()
{
	return new CClienteENet();
}


} //namespace Net

#endif