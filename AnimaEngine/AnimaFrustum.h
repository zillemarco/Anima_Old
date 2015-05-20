//
//  AnimaFrustum.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFrustum__
#define __Anima__AnimaFrustum__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

/*!
 *	\brief		Classe che identifica una 'piramide' e le equazioni dei suoi 6 piani
 *	\details	Classe che identifica una 'piramide' e le equazioni dei suoi 6 piani
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaFrustum
{
protected:
	
	/*! Enumerazione degli identificatori dei piani della piramide */
	enum AF_PLANES {
		TOP = 0,	/*!< Piano sopra	*/
		BOTTOM,		/*!< Piano sotto	*/
		LEFT,		/*!< Piano sinistra */
		RIGHT,		/*!< Piano destra	*/
		NEARP,		/*!< Piano vicino	*/
		FARP		/*!< Piano lontano	*/
	};
	
	/*! Enumerazione degli identificatori degli angoli della piramide */
	enum AF_CORNERS {
		TOPLEFT = 0,	/*!< Angolo in alto a sinistra	*/
		TOPRIGHT,		/*!< Angolo in alto a destra	*/
		BOTTOMLEFT,		/*!< Angolo in basso a sinistra */
		BOTTOMRIGHT		/*!< Angolo in basso a destra	*/
	};

public:
	/*!
	 *	\brief		Costruttore base della classe
	 *	\details	Costruttore base della classe
	 *	\author		Zille Marco
	 */
	AnimaFrustum();
	
	/*!
	 *	\brief		Costruttore di copia della classe
	 *	\details	Costruttore di copia della classe. Copia le 6 equazioni dei piani, gli 8 vertici della piramide ed il suo bounding box
	 *	\param[in]	src Altra istanza della classe AnimaFrustum da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaFrustum(const AnimaFrustum& src);
	
	/*!
	 *	\brief		Costruttore di copia della classe
	 *	\details	Costruttore di copia della classe. Copia le 6 equazioni dei piani, gli 8 vertici della piramide ed il suo bounding box
	 *	\param[in]	src Altra istanza della classe AnimaFrustum da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaFrustum(AnimaFrustum&& src);
	
	/*!
	 *	\brief		Distruttore della classe
	 *	\details	Distruttore della classe
	 *	\author		Zille Marco
	 */
	virtual ~AnimaFrustum();
	
	/*!
	 *	\brief		Operatore di copia della classe
	 *	\details	Operatore di copia della classe. Copia le 6 equazioni dei piani, gli 8 vertici della piramide ed il suo bounding box
	 *	\param[in]	src Altra istanza della classe AnimaFrustum da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaFrustum& operator=(const AnimaFrustum& src);
	
	/*!
	 *	\brief		Operatore di copia della classe
	 *	\details	Operatore di copia della classe. Copia le 6 equazioni dei piani, gli 8 vertici della piramide ed il suo bounding box
	 *	\param[in]	src Altra istanza della classe AnimaFrustum da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaFrustum& operator=(AnimaFrustum&& src);
	
	/*!
	 *	\brief		Torna uno degli 8 vertici della piramide
	 *	\details	Dato l'indice del vertice che si vuole ricavare, torna il valore di tale vertice
	 *	\param[in]	index Indice del vertice che si vuole ricavare. Compreso tra 0 e 8 (0 <= index < 8)
	 *	\return		Vertice richiesto
	 *	\author		Zille Marco
	 */
	AnimaVertex3f GetFrustumVertex(AInt index) const;
	
	/*!
	 *	\brief		Torna una copia degli 8 vertici della piramide
	 *	\details	Torna una copia degli 8 vertici della piramide copiandoli su di un array di 8 elementi dato in input
	 *	\param[out]	vertices Array di 8 elementi in cui verranno copiati gli 8 vertici della piramide
	 *	\author		Zille Marco
	 */
	void GetFrustumVertices(AnimaVertex3f vertices[8]) const;
	
	/*!
	 *	\brief		Torna il vertice minimo del bounding box della piramide
	 *	\details	Torna il vertice minimo del bounding box della piramide
	 *	\return		Vertice minimo del bounding box della piramide
	 *	\author		Zille Marco
	 */
	AnimaVertex3f GetBoundingBoxMin() const;
	
	/*!
	 *	\brief		Torna il vertice massimo del bounding box della piramide
	 *	\details	Torna il vertice massimo del bounding box della piramide
	 *	\return		Vertice massimo del bounding box della piramide
	 *	\author		Zille Marco
	 */
	AnimaVertex3f GetBoundingBoxMax() const;
	
	/*!
	 *	\brief		Torna il vertice centrale del bounding box della piramide
	 *	\details	Torna il vertice centrale del bounding box della piramide
	 *	\return		Vertice centrale del bounding box della piramide
	 *	\author		Zille Marco
	 */
	AnimaVertex3f GetBoundingBoxCenter() const;

public:
	/*!
	 *	\brief		Data una matrice di proiezione costruisce le equazioni dei 6 piani corrispondenti alla piramide creata da tale matrice
	 *	\details	Data una matrice di proiezione costruisce le equazioni dei 6 piani corrispondenti alla piramide creata da tale matrice
	 *	\param[in]	projectionViewMatrix Matrice dalla quale ricavare le equazioni dei 6 piani della piramide
	 *	\author		Zille Marco
	 */
	void ComputeFrustum(const AnimaMatrix& projectionViewMatrix);

public:
	/*!
	 *	\brief		Indica se una sfera è contenuta (o intersecata) dalla piramide
	 *	\details	Indica se una sfera è contenuta (o intersecata) dalla piramide andando a controllare se la distanza tra il
					centro della sfera e uno dei 6 piani della piramide è miore del raggio della sfera
	 *	\param[in]	center	Posizione del centro della sfera
	 *	\param[in]	radius	Raggio della sfera
	 *	\return		True se la sfera è contenuta (o intersecata) dalla piramide
	 *	\author		Zille Marco
	 */
	bool SphereInFrustum(const AnimaVertex3f& center, AFloat radius) const;

protected:
	AFloat			_frustum[6][4];			/*!< Array con le 6 equazioni dei piani della piramide */
	AnimaVertex3f	_frustumVertices[8];	/*!< Array degli 8 vertici della piramide */

	AnimaVertex3f	_boundingBoxMin;		/*!< Vertice minimo del bounding box della piramide */
	AnimaVertex3f	_boundingBoxMax;		/*!< Vertice massimo del bounding box della piramide */
	AnimaVertex3f	_boundingBoxCenter;		/*!< Vertice centrale del bounding box della piramide */
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFrustum__) */
