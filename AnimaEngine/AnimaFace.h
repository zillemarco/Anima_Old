//
//  AnimaFace.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFace__
#define __Anima__AnimaFace__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

/*!
 *	\brief		Classe che identifica una singola faccia di una mesh 3D
 *	\details	Classe che identifica una singola faccia di una mesh 3D. Viene caratterizzata da 3 indici 
				(gli indici dei 3 vertici appartenente alla faccia) ed un vettore normale (alla faccia)
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaFace
{
public:
	/*!
	 *	\brief		Costruttore della classe AnimaFace
	 *	\details	Costruttore di default. Imposta tutti gli indici della faccia a 0 e la normale a puntare verso l'alto (0, 1, 0)
	 *	\author		Zille Marco
	 */
	AnimaFace();
	
	/*!
	 *	\brief		Costruttore della classe AnimaFace
	 *	\details	Costruttore di che accetta in input i 3 indici dei vertici che faranno parte della faccia sotto forma di array di 3 elementi. 
					Imposta la normale a puntare verso l'alto (0, 1, 0)
	 *	\param[in]	indexes Array di 3 elementi contenente i 3 indici dei vertici della faccia
	 *	\author		Zille Marco
	 */
	AnimaFace(AUint indexes[3]);
	
	/*!
	 *	\brief		Costruttore della classe AnimaFace
	 *	\details	Costruttore di che accetta in input i 3 indici dei vertici che faranno parte della faccia. Imposta la normale a puntare verso l'alto (0, 1, 0)
	 *	\param[in]	a Indice primo vertice della faccia
	 *	\param[in]	b Indice secondo vertice della faccia
	 *	\param[in]	c Indice terzo vertice della faccia
	 *	\author		Zille Marco
	 */
	AnimaFace(AUint a, AUint b, AUint c);
	
	/*!
	 *	\brief		Costruttore di copia della classe AnimaFace
	 *	\details	Costruttore di copia della classe AnimaFace
	 *	\param[in]	src Altra istanza della classe AnimaFace da cui copiare i dati. Copia sia gli indici dei vertici, sia la normale
	 *	\author		Zille Marco
	 */
	AnimaFace(const AnimaFace& src);
	
	/*!
	 *	\brief		Costruttore di copia della classe AnimaFace
	 *	\details	Costruttore di copia della classe AnimaFace
	 *	\param[in]	src Altra istanza della classe AnimaFace da cui copiare i dati. Copia sia gli indici dei vertici, sia la normale
	 *	\author		Zille Marco
	 */
	AnimaFace(AnimaFace&& src);
	
	/*!
	 *	\brief		Distruttore della classe AnimaFace
	 *	\details	Distruttore della classe AnimaFace. Impsta gli indici della faccia a 0 e la normale a puntare verso l'alto (0, 1, 0)
	 *	\author		Zille Marco
	 */
	~AnimaFace();
	
	/*!
	 *	\brief		Operatore di copia della classe AnimaFace
	 *	\details	Operatore di copia della classe AnimaFace
	 *	\param[in]	src Altra istanza della classe AnimaFace da cui copiare i dati. Copia sia gli indici dei vertici, sia la normale
	 *	\author		Zille Marco
	 */
	AnimaFace& operator=(const AnimaFace& src);
	
	/*!
	 *	\brief		Operatore di copia della classe AnimaFace
	 *	\details	Operatore di copia della classe AnimaFace
	 *	\param[in]	src Altra istanza della classe AnimaFace da cui copiare i dati. Copia sia gli indici dei vertici, sia la normale
	 *	\author		Zille Marco
	 */
	AnimaFace& operator=(AnimaFace&& src);
	
public:
	
	/*!
	 *	\brief		Imposta gli indici dei vertici della faccia
	 *	\details	Imposta gli indici dei vertici della faccia. Accetta in input i 3 indici dei vertici che faranno parte della faccia sotto forma di array di 3 elementi.
	 *	\param[in]	indexes Array di 3 elementi contenente i 3 indici dei vertici della faccia
	 *	\author		Zille Marco
	 */
	void SetIndexes(AUint indexes[3]);
	
	/*!
	 *	\brief		Imposta gli indici dei vertici della faccia
	 *	\details	Imposta gli indici dei vertici della faccia. Accetta in input i 3 indici dei vertici che faranno parte della faccia
	 *	\param[in]	a Indice primo vertice della faccia
	 *	\param[in]	b Indice secondo vertice della faccia
	 *	\param[in]	c Indice terzo vertice della faccia
	 *	\author		Zille Marco
	 */
	void SetIndexes(AUint a, AUint b, AUint c);
	
	/*!
	 *	\brief		Torna l'indice del vertice richiesto
	 *	\details	Dato in input in indice compreso tra 0 e 3 (0 <= index < 3), torna l'indice del vertice salvato a quell'indice
	 *	\param[in]	index Indice del vertice di cui si vuole sapere l'indice
	 *	\return		Indice del vertice richiesto
	 *	\author		Zille Marco
	 */
	AUint GetIndex(AInt index) const;
	
	/*!
	 *	\brief		Torna un array di 3 elementi contenente gli indici dei vertici della faccia in questione
	 *	\details	Torna un array di 3 elementi contenente gli indici dei vertici della faccia in questione
	 *	\return		Array di 3 elementi contenente gli indici dei vertici
	 *	\author		Zille Marco
	 */
	AUint* GetIndexes();
	
	/*!
	 *	\brief		Torna un array costante (non modificabile) di 3 elementi contenente gli indici dei vertici della faccia in questione
	 *	\details	Torna un array costante (non modificabile) di 3 elementi contenente gli indici dei vertici della faccia in questione
	 *	\return		Array costante (non modificabile) di 3 elementi contenente gli indici dei vertici
	 *	\author		Zille Marco
	 */
	const AUint* GetConstIndexes();
	
	/*!
	 *	\brief			Salva gli indici dei vertici della faccia in questione in un altro array
	 *	\details		Salva gli indici dei vertici della faccia in questione in un altro array
	 *	\param[out]		outIndexes	Array su cui verranno salvati gli indici dei vertici della faccia. Non deve essere NULL e deve avere almeno 3 locazioni disponibili
	 *	\param[in/out]	inOutSize	In ingresso corrisponde alla dimensione dell'array outIndexes passato, in uscita indica il numero di indici dei vertici salvato
	 *	\return			Torna true se la dimensione di outIndexes è almeno di 3 elementi e riesce a salvare gli indici dei vertici
	 *	\author			Zille Marco
	 */
	bool GetIndexes(AUint* outIndexes, AInt& inOutSize);
	
	/*!
	 *	\brief			Fa una copia degli indici dei vertici della faccia in questione in un altro array
	 *	\details		Fa una copia degli indici dei vertici della faccia in questione in un altro array
	 *	\param[out]		outIndexes	Array su cui verranno copiati gli indici dei vertici della faccia. Non deve essere NULL e deve avere almeno 3 locazioni disponibili
	 *	\param[in/out]	inOutSize	In ingresso corrisponde alla dimensione dell'array outIndexes passato, in uscita indica il numero di indici dei vertici copiato
	 *	\return			Torna true se la dimensione di outIndexes è almeno di 3 elementi e riesce a fare una copia degli indici dei vertici
	 *	\author			Zille Marco
	 */
	bool GetConstIndexes(AUint* outIndexes, AInt& inOutSize);
	
	/*!
	 *	\brief		Torna il numero di indici dei vertici della faccia
	 *	\details	Torna il numero di indici dei vertici della faccia
	 *	\return		Numero di indici dei vertici della faccia
	 *	\author		Zille Marco
	 */
	AInt GetIndexesCount();
	
	/*!
	 *	\brief		Indica se nella faccia contiene anche l'indice del vertice passato
	 *	\details	Indica se nella faccia contiene anche l'indice del vertice passato
	 *	\param[in]	index Indice di un vertice
	 *	\return		Torna true se index è uno dei 3 indici dei vertici della faccia, false altrimenti
	 *	\author		Zille Marco
	 */
	bool HasIndex(AUint index) const;
	
	/*!
	 *	\brief		Imposta il vettore normale della faccia
	 *	\details	Imposta il vettore normale della faccia
	 *	\param[in]	n	Vettore normale della faccia. Non viene effettuata una normalizzazione di tale vettore all'interno del metodo
	 *	\author		Zille Marco
	 */
	void SetNormal(const AnimaVertex3f& n);
	
	/*!
	 *	\brief		Imposta il vettore normale della faccia
	 *	\details	Imposta il vettore normale della faccia date le 3 componenti di tale vettore. Non viene effettuata una normalizzazione di tale vettore all'interno del metodo
	 *	\param[in]	x	Componente x del vettore normale della faccia
	 *	\param[in]	y	Componente y del vettore normale della faccia
	 *	\param[in]	z	Componente z del vettore normale della faccia
	 *	\author		Zille Marco
	 */
	void SetNormal(const AFloat& x, const AFloat& y, const AFloat& z);
	
	/*!
	 *	\brief		Torna il vettore normale della faccia
	 *	\details	Torna il vettore normale della faccia
	 *	\return		Vettore normale della faccia
	 *	\author		Zille Marco
	 */
	AnimaVertex3f GetNormal() const;
				
protected:
	AUint _indexes[3];		/*!< Array di 3 elementi contenente gli indici dei vertici della faccia */
	AnimaVertex3f _normal;	/*!< Vettore normale della faccia */
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFace__) */
