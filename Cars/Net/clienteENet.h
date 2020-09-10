//---------------------------------------------------------------------------
// clienteENet.h
//---------------------------------------------------------------------------

/**
 * @file clienteEnet.h
 *
 * Implementa un cliente de red mediante la librer�a eNet
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Junio, 2006
 */

#ifndef __CLIENTEENET_H
#define __CLIENTEENET_H


#include "enet/enet.h"
#include "cliente.h"

namespace Net {

/**
 * Cliente de red implementado mediante la librer�a eNet
 */
class CClienteENet: public CCliente {

public:

	/**
	 * Constructor
	 */ 
	CClienteENet();

	/**
	 * Destructor
	 */
	virtual ~CClienteENet();

	/**
	 * Inicializa el cliente de red.
	 * @param maxConnections M�ximo de conexiones permitidas
	 * @param maxinbw Ancho de banda de entrada m�ximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida m�ximo en bytes/segundo (0 = ilimitado)
	 * @param true si todo ok
	 */
	bool init(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

	/**
	 * Devuelve la conexi�n (peer) creada para la direcci�n y puerto indicados
	 * @param address Direcci�n IP o nombre de la m�quina remota a conectar
	 * @param puerto Puerto remoto al que conectar. (El puerto local utilizado no se especifica en este m�tdo ya que normalmente es asignado por el S.O.)
     * @param channels N�mero de canales l�gicos a crear sobre la conexi�n
	 * @param timeout Tiempo m�ximo de espera para conectar con la m�quina remota.
	 * @return La conexi�n creada. Devuelve NULL si la conexi�n no ha podido ser establecida o si no se llam� a init() anteriormente.
	 */
	CConexion* connect(char* address, int port, int channels, unsigned int timeout = 5000);

	/**
	 * Devuelve una lista de las conexiones creadas 
	 */
	std::vector<CConexion*>::iterator listarConnexiones();

	/**
	 * Env�a datos a trav�s de una conexi�n.
	 * @param conexion por la que enviar los datos
	 * @param data son los datos a enviar
	 * @param longData tama�o de los datos a enviar
	 * @param channel canal l�gico por el que se enviar�n los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
	 */
	void sendData(CConexion* conexion, void* data, int longData, int channel, bool reliable);

  /**
   * Env�a el mismo dato a todos los clientes conectados
   * @param data son los datos a enviar
   * @param longData tama�o de los datos a enviar
   * @param channel canal l�gico por el que se enviar�n los datos
   * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
   */
  void sendAll(void* data, size_t longData, int channel, bool reliable);

	/**
	 * Refresca el cliente obteniendo todos los paquetes recibidos despu�s de la �ltima llamada a este m�todo
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los paquetes recibidos
	 * @param paquetesRecibidos Vector de CPaquete* donde se almacenan los paquetes recibidos
	 */
	virtual void service(std::vector<CPaquete*>& paquetesRecibidos);

	/**
	 * Obtiene el siguiente paquete recibido
	 * IMPORTANTE: Es responsabilidad del invocador de readPacket() borrar el paquete recibido
	 * @return �ltimo paquete recibido
	 */
	virtual CPaquete* readPacket();

	/**
	 * Desconecta la conexi�n indicada.
	 */
	void disconnect(CConexion * conexion);

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Desconecta autom�ticamente todas las conexiones activas.
	 */ 
	void deInit();

	/**
	 * Indica si el cliente tiene alguna conexi�n activa.
	 */
	bool isConnected();

	/**
	 * Indica si el cliente est� inicializado
	 */
	virtual bool isInitialized();

private:
	
	enum EstadoClienteENet{
		NO_INIT,
		INIT_NOT_CONNECTED,
		INIT_AND_CONNECTED
	};

	void disconnectAll();

	void disconnectReceived(CConexion* conexion);

	EstadoClienteENet estado;

	std::vector<CConexion*> listaConexiones;
	ENetHost* client;

};

} // namespace Net

#endif // __CLIENTE_H
