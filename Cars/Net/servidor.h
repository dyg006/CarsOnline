//---------------------------------------------------------------------------
// servidor.h
//---------------------------------------------------------------------------

/**
 * @file servidor.h
 *
 * Especifica un servidor de red.
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Junio, 2006
 */

#ifndef __SERVIDOR_H
#define __SERVIDOR_H


#include "paquete.h"
#include "conexion.h"
#include <vector>

namespace Net {
/**
 * Especifica un servidor de red.
 * Esta interfaz podr� ser implementada de distintas maneras o con distintas librer�as. 
 * Con ella se pretende representar el comportamiento de un servidor de red gen�rico 
 * e independiente de la implementaci�n.
 */
class CServidor {

public:

	/**
	 * Destructor virtual.
	 */
	virtual ~CServidor(){}

	/**
	 * Inicializa el servidor escuchando en un puerto determinado.
	 * @param port es el puerto donde se escucha
	 * @param clients es el n�mero m�ximo de clientes. El m�ximo ser� establecido por la implementaci�n.
	 * @param maxinbw Ancho de banda de entrada m�ximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida m�ximo en bytes/segundo (0 = ilimitado)
	 */
	virtual bool init(int port, int clients, unsigned int maxinbw = 0, unsigned int maxoutbw = 0)=0;
	
	/**
	 * Refresca el servidor obteniendo todos los paquetes recibidos despu�s de la �ltima llamada a este m�todo
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los paquetes recibidos
	 * @param paquetesRecibidos Vector de CPaquete* donde se almacenan los paquetes recibidos
	 */
	virtual void service(std::vector<CPaquete*>& paquetesRecibidos)=0;

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este m�todo desconectar todas las conexiones en este punto.
	 */ 
	virtual void deInit()=0;

	/**
	 * Devuelve una lista de las conexiones creadas 
	 */
	virtual std::vector<CConexion*>::iterator listarConnexiones()=0;

	/**
	 * Env�a datos a trav�s de una conexi�n.
	 * @param conexion por la que enviar los datos
	 * @param data son los datos a enviar
	 * @param longData tama�o de los datos a enviar
	 * @param channel canal l�gico por el que se enviar�n los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
	 */
	virtual void sendData(CConexion* conexion, void* data, size_t longData, int channel, bool reliable)=0;

	/**
	 * Env�a el mismo dato a todos los clientes conectados
	 * @param data son los datos a enviar
	 * @param longData tama�o de los datos a enviar
	 * @param channel canal l�gico por el que se enviar�n los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
	 */
	virtual void sendAll(void* data, size_t longData, int channel, bool reliable)=0;

	/**
	 * Desconecta la conexi�n indicada.
	 */
	virtual void disconnect(CConexion * conexion)=0;

	/**
	 * Indica si el cliente tiene alguna conexi�n activa.
	 */
	virtual bool isConnected()=0;

	/**
	 * Indica si el cliente est� inicializado
	 */
	virtual bool isInitialized()=0;
};

} // namespace Net

#endif // __SERVIDOR_H
