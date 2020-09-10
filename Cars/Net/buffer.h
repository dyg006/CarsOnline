//---------------------------------------------------------------------------
// buffer.h
//---------------------------------------------------------------------------

/**
 * @file buffer.h
 *
 * Implementa un buffer dinámico que los objetos utilizan para serializarse o deserializarse
 *
 *
 * @author Juan A. Recio-García
 * @date Octubre, 2006
 */

#ifndef __BUFFER_H
#define __BUFFER_H


namespace Net {

typedef unsigned char byte;

/**
 * Implementa un buffer dinámico que los objetos utilizan para serializarse o deserializarse
 */
class CBuffer{
public:

	/**
	 * Crea un buffer dinámico.
	 * @param initsize Tamaño inicial del buffer
	 * @param delta Indica cuanto crecerá el buffer cada vez que se necesite ampliarlo
     */
	CBuffer(size_t initsize=500, size_t delta=100);

	/**
	 * Destructora
	 */
	~CBuffer();

	/**
	 * Devuelve el buffer como una secuencia de bytes
	 */
	byte* getbuffer();

	/**
	 * Devuelve el tamaño del buffer. 
	 * Se utilizará normalmente para saber cuantos bytes devuelve getbuffer()
	 */
	size_t getSize();

	/**
	 * Reinicia el buffer.
	 * No modifica su tamaño actual
	 */
	void reset();

	/**
	 * Escribe datos en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
  void write(const void* data, size_t datalength);
  void write(int data) { write(&data, sizeof(data)); }
  void write(size_t data) { write(&data, sizeof(data)); }
  void write(unsigned int data) { write(&data, sizeof(data)); }
  void write(float data) { write(&data, sizeof(data)); }
  void write(bool data) { write(&data, sizeof(data)); }
	void write(const char* data);

	/**
	 * Lee datos del buffer.
	 * Al hacer esto el buffer se "vacia"
	 * \param data es un puntero indicando a donde se deben copiar los datos desde el buffer
	 * \param datalength es el número de datos (bytes) a leer
	 */
  void read(void* data, size_t datalength);
  void read(int& data) { read(&data, sizeof(data)); }
  void read(size_t& data) { read(&data, sizeof(data)); }
  void read(unsigned int& data) { read(&data, sizeof(data)); }
  void read(float& data) { read(&data, sizeof(data)); }
  void read(bool& data) { read(&data, sizeof(data)); }
	void read(char* data);


protected:
	void realloc();

	byte* _begin;
	byte* _current;
	size_t _maxsize;
	size_t _size;
	size_t _delta;

};

} // namespace Net

#endif // _CBUFFER_H