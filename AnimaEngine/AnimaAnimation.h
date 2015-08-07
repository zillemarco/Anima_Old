//
//  AnimaAnimation.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaAnimation__
#define __Anima__AnimaAnimation__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaQuaternion.h"
#include "AnimaTypes.h"
#include "AnimaMappedArray.h"
#include "AnimaMappedValues.h"
#include "AnimaMesh.h"
#include "AnimaNamedObject.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaAnimationPositionKey;
class AnimaAnimationRotationKey;
class AnimaAnimationScalingKey;
class AnimaAnimationNode;
class AnimaModel;

/*!
 *	\brief		Classe usata per memorizzare le informazioni su animazioni con scheletro e per poi applicarle ai modelli
 *	\details	Classe usata per memorizzare le informazioni su animazioni con scheletro e per poi applicarle ai modelli
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaAnimation : public AnimaNamedObject
{
public:
	/*!
	 *	\brief		Costruttore della classe
	 *	\details	Costruttore della classe
	 *	\param[in]	name		Nome dell'animazione
	 *	\param[in]	allocator	Manager della memoria dell'animazione
	 *	\author		Zille Marco
	 */
	AnimaAnimation(const AnimaString& name, AnimaAllocator* allocator);

	/*!
	 *	\brief		Costruttore di copia della classe
	 *	\details	Costruttore di copia della classe
	 *	\param[in]	src	Altra istanza di AnimaAnimation da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaAnimation(const AnimaAnimation& src);

	/*!
	 *	\brief		Costruttore di copia della classe
	 *	\details	Costruttore di copia della classe
	 *	\param[in]	src	Altra istanza di AnimaAnimation da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaAnimation(AnimaAnimation&& src);

	/*!
	*	\brief		Distruttore della classe
	*	\details	Distruttore della classe
	*	\author		Zille Marco
	*/
	~AnimaAnimation();

	/*!
	 *	\brief		Operatore di copia della classe
	 *	\details	Operatore di copia della classe
	 *	\param[in]	src	Altra istanza di AnimaAnimation da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaAnimation& operator=(const AnimaAnimation& src);

	/*!
	 *	\brief		Operatore di copia della classe
	 *	\details	Operatore di copia della classe
	 *	\param[in]	src	Altra istanza di AnimaAnimation da cui copiare i dati
	 *	\author		Zille Marco
	 */
	AnimaAnimation& operator=(AnimaAnimation&& src);
	
public:
	/*!
	 *	\brief		Funzione usata per calcolare le matrici di trasformazione per applicare l'animazione dal modello passato in input
	 *	\details	Funzione usata per calcolare le matrici di trasformazione per applicare l'animazione dal modello passato in input
	 *	\param[in]	model	Puntatore ad un'istanza della classe AnimaModel contenente i dati del modello a cui si vuole applicare l'animazione
	 *	\param[in]	time	Indica il momento dell'animazione da applicare al modello
	 *	\author		Zille Marco
	 */
	void UpdateAnimation(AnimaModel* model, AFloat time);

protected:
	/*!
	 *	\brief			Vera e propria funzione che si occupa di calcolare le matrici di trasformazione per applicare l'animazione ad un modello
	 *	\details		Vera e propria funzione che si occupa di calcolare le matrici di trasformazione per applicare l'animazione ad un modello
	 *	\param[in]		model				Puntatore ad un'istanza della classe AnimaModel contenente i dati del modello a cui si vuole applicare l'animazione
	 *	\param[in]		animationTime		Indica il momento dell'animazione da applicare al modello
	 *	\param[in]		parentMatrix		Matrice che indica la trasformazione applicata al modello padre di quello attuale (model)
	 *	\param[in]		globalInverseMatrix	Matrice utilizzata per trasformare il modello attuale (model) nel sistema di riferimento del modello principale
	 *	\param[in,out]	mesheBonesInfo		Array che alla fine del processo di calcolo dell'animazione conterrà tutte le matrici da applicare alle varie mesh del modello
	 *	\author			Zille Marco
	 */
	void UpdateModelNodesAnimation(AnimaModel* model, AFloat animationTime, const AnimaMatrix& parentMatrix, const AnimaMatrix& globalInverseMatrix, AnimaMappedArray<AnimaMeshBoneInfo*>* mesheBonesInfo);

public:
	/*!
	 *	\brief		Torna il numero di nodi da cui è composta l'animazione
	 *	\details	Torna il numero di nodi da cui è composta l'animazione
	 *	\return		Numero di nodi da cui è composta l'animazione
	 *	\author		Zille Marco
	 */
	AInt GetAnimationNodesCount() const;

	/*!
	 *	\brief		Crea un nuovo nodo per l'animazione dato il suo nome
	 *	\details	Crea un nuovo nodo per l'animazione dato il suo nome
	 *	\param[in]	name	Nome del nodo da creare
	 *	\return		Puntatore al nuovo nodo appena creato. NULL se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaAnimationNode* CreateAnimationNode(const AnimaString& name);

	/*!
	 *	\brief		Torna un nodo dell'animazione dato il suo indice
	 *	\details	Torna un nodo dell'animazione dato il suo indice
	 *	\param[in]	index	Indice del nodo da cercare
	 *	\return		Puntatore al nodo richiesto. NULL se il valore di index non è valido
	 *	\author		Zille Marco
	 */
	AnimaAnimationNode* GetAnimationNode(AInt index);

	/*!
	 *	\brief		Torna un puntatore all'array che contiene tutti i nodi dell'animazione
	 *	\details	Torna un puntatore all'array che contiene tutti i nodi dell'animazione
	 *	\return		Puntatore all'array di nodi dell'animazione
	 *	\author		Zille Marco
	 */
	AnimaMappedArray<AnimaAnimationNode*>* GetAnimationNodes();

	/*!
	 *	\brief		Elimina tutti i nodi dell'animazione
	 *	\details	Elimina tutti i nodi dell'animazione
	 *	\author		Zille Marco
	 */
	void ClearAnimationNodes();

	/*!
	 *	\brief		Imposta il tempo di durata totale dell'animazione
	 *	\details	Imposta il tempo di durata totale dell'animazione
	 *	\param[in]	duration	Tempo di durata totale dell'animazione
	 *	\author		Zille Marco
	 */
	void SetDuration(AFloat duration);

	/*!
	 *	\brief		Torna il tempo di durata totale dell'animazione
	 *	\details	Torna il tempo di durata totale dell'animazione
	 *	\return		Tempo di durata totale dell'animazione
	 *	\author		Zille Marco
	 */
	AFloat GetDuration() const;

	/*!
	 *	\brief		Imposta il numero di tick al secondo dell'animazione
	 *	\details	Imposta il numero di tick al secondo dell'animazione
	 *	\param[in]	ticksPerSecond	Numero di tick al secondo dell'animazione
	 *	\author		Zille Marco
	 */
	void SetTicksPerSecond(AFloat ticksPerSecond);

	/*!
	 *	\brief		Torna il numero di tick al secondo dell'animazione
	 *	\details	Torna il numero di tick al secondo dell'animazione
	 *	\return		Numero di tick al secondo dell'animazione
	 *	\author		Zille Marco
	 */
	AFloat GetTicksPerSecond() const;

protected:
	AFloat _duration;		/*!< Attributo dove viene memorizzata la durata totale dell'animazione */
	AFloat _ticksPerSecond;	/*!< Attributo dove viene memorizzato il numero di tick al secondo dell'animazione */

	AnimaMappedArray<AnimaAnimationNode*> _animationNodes;	/*!< Attributo dove vengono memorizzati i nodi dell'animazione */
};

/*!
 *	\brief		Classe usata per memorizzare le informazioni di un nodo di un'animazione
 *	\details	Classe usata per memorizzare le informazioni di un nodo di un'animazione
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaAnimationNode : public AnimaNamedObject
{
public:
	/*!
	 *	\brief		Costruttore della classe
	 *	\details	Costruttore della classe
	 *	\param[in]	name		Nome del nodo
	 *	\param[in]	allocator	Manager della memoria del nodo
	 *	\author		Zille Marco
	 */
	AnimaAnimationNode(const AnimaString& name, AnimaAllocator* allocator);

	/*!
	 *	\brief		Distruttore della classe
	 *	\details	Distruttore della classe
	 *	\author		Zille Marco
	 */
	~AnimaAnimationNode();

public:
	/*!
	 *	\brief		Calcola il valore della scala da applicare ad un determinato istante di tempo
	 *	\details	Calcola il valore della scala da applicare ad un determinato istante di tempo. 
					Se non viene trovata una corrispondenza esatta con una chiave di scala, vengono interpolati i valori delle due chiavi di scala che si
					avvicinano di più a quello cercato.
	 *	\param[in]	animationTime	Istante di tempo utilizzato per calcolare il valore della scala
	 *	\return		Vettore contenente le tre componenti della scala
	 *	\author		Zille Marco
	 */
	AnimaVertex3f ComputeInterpolatedScaling(AFloat animationTime) const;

	/*!
	 *	\brief		Calcola il valore della rotazione da applicare ad un determinato istante di tempo
	 *	\details	Calcola il valore della rotazione da applicare ad un determinato istante di tempo.
					Se non viene trovata una corrispondenza esatta con una chiave di rotazione, vengono interpolati i valori delle due chiavi di rotazione che si
					avvicinano di più a quello cercato.
	 *	\param[in]	animationTime	Istante di tempo utilizzato per calcolare il valore della rotazione
	 *	\return		Quaternione contenente le informazioni sulla rotazione
	 *	\author		Zille Marco
	 */
	AnimaQuaternion ComputeInterpolatedRotation(AFloat animationTime) const;

	/*!
	 *	\brief		Calcola il valore della traslazione da applicare ad un determinato istante di tempo
	 *	\details	Calcola il valore della traslazione da applicare ad un determinato istante di tempo.
					Se non viene trovata una corrispondenza esatta con una chiave di traslazione, vengono interpolati i valori delle due chiavi di traslazione che si
					avvicinano di più a quello cercato.
	 *	\param[in]	animationTime	Istante di tempo utilizzato per calcolare il valore della traslazione
	 *	\return		Vettore contenente le tre componenti della traslazione
	 *	\author		Zille Marco
	 */
	AnimaVertex3f ComputeInterpolatedPosition(AFloat animationTime) const;

	/*!
	 *	\brief		Dato un istante di tempo, torna l'indice della chiave di scala che corrisponde al meglio a quell'istante
	 *	\details	Dato un istante di tempo, torna l'indice della chiave di scala che corrisponde al meglio a quell'istante
	 *	\param[in]	animationTime	Istante di tempo utilizzato per calcolare l'indice della chiave
	 *	\return		Indice della chiave che corrisponde al meglio a quell'istante
	 *	\author		Zille Marco
	 */
	AInt FindScaling(AFloat animationTime) const;
	
	/*!
	 *	\brief		Dato un istante di tempo, torna l'indice della chiave di rotazione che corrisponde al meglio a quell'istante
	 *	\details	Dato un istante di tempo, torna l'indice della chiave di rotazione che corrisponde al meglio a quell'istante
	 *	\param[in]	animationTime	Istante di tempo utilizzato per calcolare l'indice della chiave
	 *	\return		Indice della chiave che corrisponde al meglio a quell'istante
	 *	\author		Zille Marco
	 */
	AInt FindRotation(AFloat animationTime) const;

	/*!
	 *	\brief		Dato un istante di tempo, torna l'indice della chiave di tralsazione che corrisponde al meglio a quell'istante
	 *	\details	Dato un istante di tempo, torna l'indice della chiave di tralsazione che corrisponde al meglio a quell'istante
	 *	\param[in]	animationTime	Istante di tempo utilizzato per calcolare l'indice della chiave
	 *	\return		Indice della chiave che corrisponde al meglio a quell'istante
	 *	\author		Zille Marco
	 */
	AInt FindPosition(AFloat animationTime) const;

public:
	AInt GetPositionKeysCount() const;
	void AddPositionKey(AnimaAnimationPositionKey key);
	AnimaAnimationPositionKey GetPositionKey(AInt index) const;
	AnimaArray<AnimaAnimationPositionKey>* GetPositionKeys();
	void ClearPositionKeys();

	AInt GetRotationKeysCount() const;
	void AddRotationKey(AnimaAnimationRotationKey key);
	AnimaAnimationRotationKey GetRotationKey(AInt index) const;
	AnimaArray<AnimaAnimationRotationKey>* GetRotationKeys();
	void ClearRotationKeys();

	AInt GetScalingKeysCount() const;
	void AddScalingKey(AnimaAnimationScalingKey key);
	AnimaAnimationScalingKey GetScalingKey(AInt index) const;
	AnimaArray<AnimaAnimationScalingKey>* GetScalingKeys();
	void ClearScalingKeys();

protected:
	AnimaArray<AnimaAnimationPositionKey> _positionKeys;	/*!< Array dove vengono memorizzate tutte le chiavi di traslazione del nodo */
	AnimaArray<AnimaAnimationRotationKey> _rotationKeys;	/*!< Array dove vengono memorizzate tutte le chiavi di rotazione del nodo */
	AnimaArray<AnimaAnimationScalingKey> _scalingKeys;		/*!< Array dove vengono memorizzate tutte le chiavi di scala del nodo */
};

class ANIMA_ENGINE_EXPORT AnimaAnimationPositionKey
{
public:
	AnimaAnimationPositionKey();
	AnimaAnimationPositionKey(AFloat time, AnimaVertex3f value);
	~AnimaAnimationPositionKey();

	void SetTime(AFloat time);
	AFloat GetTime() const;

	void SetValue(AnimaVertex3f value);
	AnimaVertex3f GetValue() const;

protected:
	AFloat _time;
	AnimaVertex3f _value;
};

class ANIMA_ENGINE_EXPORT AnimaAnimationRotationKey
{
public:
	AnimaAnimationRotationKey();
	AnimaAnimationRotationKey(AFloat time, AnimaQuaternion value);
	~AnimaAnimationRotationKey();

	void SetTime(AFloat time);
	AFloat GetTime() const;

	void SetValue(AnimaQuaternion value);
	AnimaQuaternion GetValue() const;

protected:
	AFloat _time;
	AnimaQuaternion _value;
};

class ANIMA_ENGINE_EXPORT AnimaAnimationScalingKey
{
public:
	AnimaAnimationScalingKey();
	AnimaAnimationScalingKey(AFloat time, AnimaVertex3f value);
	~AnimaAnimationScalingKey();

	void SetTime(AFloat time);
	AFloat GetTime() const;

	void SetValue(AnimaVertex3f value);
	AnimaVertex3f GetValue() const;

protected:
	AFloat _time;
	AnimaVertex3f _value;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaAnimation__) */
