//---------------------------------------------------------------------------
// servidorENet.h
//---------------------------------------------------------------------------

/**
 * @file servidorENet.h
 *
 * Implementa un servidor de red con ENet
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Junio, 2006
 */

#ifndef __SERVIDORENET_H
#define __SERVIDORENET_H


#include "servidor.h"
#include "enet/enet.h"


namespace Net {

/**
 * Servidor de red implementado mediante la librer�a eNet
 */
class CServidorENet: public CServidor {

public:

	/**
	 * Constructor
	 */ 
	CServidorENet();

	/**
	 * Destructor
	 */
	virtual ~CServidorENet();

	/**
	 * Inicializa el servidor escuchando en un puerto determinado.
	 * @param port es el puerto donde se escucha
	 * @param clients es el n�mero m�ximo de clientes. El m�ximo ser� establecido por la implementaci�n.
	 * @param maxinbw Ancho de banda de entrada m�ximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida m�ximo en bytes/segundo (0 = ilimitado)
	 */
	bool init(int port, int clients, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);
	
	/**
	 * Refresca el servidor obteniendo todos los paquetes recibidos despu�s de la �ltima llamada a este m�todo
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los paquetes recibidos
	 * @param paquetesRecibidos Vector de CPaquete* donde se almacenan los paquetes recibidos
	 */
	void service(std::vector<CPaquete*>& paquetesRecibidos);

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este m�todo desconectar todas las conexiones en este punto.
	 */ 
	void deInit();
	
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
	void sendData(CConexion* conexion, void* data, size_t longData, int channel, bool reliable);

	/**
	 * Env�a el mismo dato a todos los clientes conectados
	 * @param data son los datos a enviar
	 * @param longData tama�o de los datos a enviar
	 * @param channel canal l�gico por el que se enviar�n los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
	 */
	void sendAll(void* data, size_t longData, int channel, bool reliable);

	/**
	 * Desconecta la conexi�n indicada.
	 */
	void disconnect(CConexion * conexion);

	/**
	 * Indica si el cliente tiene alguna conexi�n activa.
	 */
	bool isConnected();

	/**
	 * Indica si el cliente est� inicializado
	 */
	bool isInitialized();

private:

	enum EstadoServidorENet{
		NO_INIT,
		INIT_NOT_CONNECTED,
		INIT_AND_CONNECTED
	};

	void disconnectAll();

	void disconnectReceived(CConexion* conexion);
	
	EstadoServidorENet estado;

	std::vector<CConexion*> listaConexiones;

	ENetHost* server;
};

} // namespace Net

#endif // __SERVIDOR_H
