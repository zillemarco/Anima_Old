//
//  AnimaMath.h
//  Anima
//
//  Created by Marco Zille on 17/12/14.
//
//

#ifndef __Anima__AnimaMath__
#define __Anima__AnimaMath__

#include "AnimaEngineCore.h"
#include "AnimaEngine.h"
#include "AnimaTypes.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"
#include "AnimaQuaternion.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

/*!
 *	\brief		Classe di supporto per operazioni matematiche
 *	\details	Classe di supporto per operazioni matematiche
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaMath
{
public:
	
	/*!
	 *	\brief		Moltiplicazione di un quaternione q1 per un quaternione q2
	 *	\details	Moltiplicazione di un quaternione q1 per un quaternione q2
	 *	\param[in]	q1	Primo quaternione utilizzato per la moltiplicazione
	 *	\param[in]	q2	Secondo quaternione utilizzato per la moltiplicazione
	 *	\return		Torna il quaternione che è il risultato della moltiplicazione tra q1 e q2
	 *	\sa			AnimaQuaternion
	 *	\author		Zille Marco
	 */
	static AnimaQuaternion QuaternionMulQuaternion(const AnimaQuaternion& q1, const AnimaQuaternion& q2);
	
	/*!
	 *	\brief		Moltiplicazione di un quaternione q per un vettore v
	 *	\details	Moltiplicazione di un quaternione q per un vettore v
	 *	\param[in]	q	Quaternione utilizzato per la moltiplicazione
	 *	\param[in]	v	Vettore utilizzato per la moltiplicazione
	 *	\return		Torna il quaternione che è il risultato della moltiplicazione tra q e v
	 *	\sa			AnimaQuaternion, AnimaVertex3f
	 *	\author		Zille Marco
	 */
	static AnimaQuaternion QuaternionMulVector(const AnimaQuaternion& q, const AnimaVertex3f& v);
	
	/*!
	 *	\brief		Applica al vettore v una rotazione angle (in radianti) secondo l'asse axsis
	 *	\details	Applica al vettore v una rotazione angle (in radianti) secondo l'asse axsis. Non modifica il vettore v
	 *	\param[in]	v		Vettore da ruotare
	 *	\param[in]	angle	Angolo di rotazione del vettore in radianti
	 *	\param[in]	axis	Asse secondo il quale ruotare il vettore v
	 *	\return		Torna il vettore v ruotato di angle secondo axis
	 *	\sa			AnimaQuaternion, AnimaVertex3f
	 *	\author		Zille Marco
	 */
	static AnimaVertex3f RotateVector(const AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis);
	
	/*!
	 *	\brief		Applica al vettore v una rotazione angle (in radianti) secondo l'asse axsis
	 *	\details	Applica al vettore v una rotazione angle (in radianti) secondo l'asse axsis. Il vettore v viene modificato
	 *	\param[in]	v		Vettore da ruotare
	 *	\param[in]	angle	Angolo di rotazione del vettore in radianti
	 *	\param[in]	axis	Asse secondo il quale ruotare il vettore v
	 *	\sa			AnimaQuaternion, AnimaVertex3f
	 *	\author		Zille Marco
	 */
	static void RotateVector(AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis);
	
	/*!
	 *	\brief		Controlla se un punto point è contenuto dalla sfera centrata in sphereCenter di raggio sphereRadius
	 *	\details	Controlla se un punto point è contenuto dalla sfera centrata in sphereCenter di raggio sphereRadius. Effettua semplicemente il controllo controllando se
					la distanza tra il punto e il centro della sfera è minore del raggio della sfera
	 *	\param[in]	point			Punto da controllare
	 *	\param[in]	sohereCenter	Posizione del centro della sfera
	 *	\param[in]	sphereRadius	Raggio della sfera
	 *	\return		True se il punto è contenuto dalla sfera, false altrimenti
	 *	\author		Zille Marco
	 */
	static bool PointInsideSphere(const AnimaVertex3f& point, const AnimaVertex3f& sphereCenter, const AFloat& sphereRadius);
	
	/*!
	 *	\brief		Controlla se un punto point è contenuto in un cono definito dai parametri in input
	 *	\details	Controlla se un punto point è contenuto in un cono definito dai parametri in input
	 *	\param[in]	point			Punto da controllare
	 *	\param[in]	coneTip			Posizione della punta del cono
	 *	\param[in]	coneDirection	Direzione verso la cui il cono è rivolto
	 *	\param[in]	coneHeight		Altezza del cono
	 *	\param[in]	coneAngle		Angolo di apertura del cono aspresso in radianti
	 *	\return		True se il punto è contenuto dal cono, false altrimenti
	 *	\author		Zille Marco
	 */
	static bool PointInsideCone(const AnimaVertex3f& point, const AnimaVertex3f& coneTip, const AnimaVertex3f& coneDirection, float coneHeight, float coneAngle);
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMath__) */
