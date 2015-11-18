//
//  AnimaTransformation.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaTransformation__
#define __Anima__AnimaTransformation__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaMatrix.h"

#include <boost/property_tree/ptree.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

using ptree = boost::property_tree::ptree;

class AnimaSceneObject;

/*!
 *	\brief		Classe che gestisce le trasformazioni di un oggetto
 *	\details	Classe che gestisce le trasformazioni di un oggetto. Se viene specificato l'oggetto padre della trasformazione, questa classe andrà a valorizzare le seguenti
				prorpietà, mappate,  di tale oggetto (usate poi per aggiornare il valore dei valori in ingresso dagli shader che hanno lo stesso nome di queste proprietà):
				- AModelMatrix: matrice di trasformazione totale dell'ggetto (comprende anche le trasformazioni degli 'antenati' dell'oggetto'
				- ANormalMatrix: matrice di trasfromazione delle normali totale dell'oggetto (comprende anche le trasformazioni degli 'antenati' dell'oggetto)
				- RModelMatrix: matrice di trasformazione relativa dell'oggetto (NON comprende le trasformazioni degli 'antenati' dell'oggetto)
				- RNormalMatrix: matrice di trasfromazione delle normali relativa dell'oggetto (NON comprende le trasformazioni degli 'antenati' dell'oggetto)
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaTransformation
{
	friend class AnimaSceneObject;

public:
	/*!
	 *	\brief		Costruttore della classe
	 *	\details	Costruttore della classe. Imposta tutte le trasformazioni come traformazioni nulle (matrici identità)
	 *	\author		Zille Marco
	 */
	AnimaTransformation();

	/*!
	 *	\brief		Costruttore della classe
	 *	\details	Costruttore della classe. Imposta tutte le trasformaioni in base ai parametri passati
	 *	\param[in]	t	Valore della traslazione della trasformazione
	 *	\param[in]	r	Valore della rotazione della trasformazione. Ongi componente del vettore corrisponde alla rotazione attorno all'asse corrispondente al componente, 
						espresso in radianti
	 *	\param[in]	s	Valore della scala della trasformazione
	 *	\author		Zille Marco
	 */
	AnimaTransformation(const AnimaVertex3f& t, const AnimaVertex3f& r, const AnimaVertex3f& s);
	
	/*!
	 *	\brief		Costruttore della classe
	 *	\details	Costruttore della classe. Imposta tutte le trasformaioni in base ai parametri passati
	 *	\param[in]	tx	Valore X della traslazione della trasformazione
	 *	\param[in]	ty	Valore Y della traslazione della trasformazione
	 *	\param[in]	tz	Valore Z della traslazione della trasformazione
	 *	\param[in]	rx	Valore della rotazione secondo l'asse X della trasformazione. Espresso in radianti
	 *	\param[in]	ry	Valore della rotazione secondo l'asse Y della trasformazione. Espresso in radianti
	 *	\param[in]	rz	Valore della rotazione secondo l'asse Z della trasformazione. Espresso in radianti
	 *	\param[in]	sx	Valore X della scala della trasformazione
	 *	\param[in]	sy	Valore Y della scala della trasformazione
	 *	\param[in]	sz	Valore Z della scala della trasformazione
	 *	\author		Zille Marco
	 */
	AnimaTransformation(AFloat tx, AFloat ty, AFloat tz, AFloat rx, AFloat ry, AFloat rz, AFloat sx, AFloat sy, AFloat sz);
	
	/*!
	 *	\brief		Costruttore di copia della classe
	 *	\details	Costruttore di copia della classe
	 *	\param[in]	src	Altra istanza della classe AnimaTransformation da cui vengono copiati tutti i dati: matrici di trasformazione, oggetto padre, ecc...
	 *	\author		Zille Marco
	 */
	AnimaTransformation(const AnimaTransformation& src);
	
	/*!
	 *	\brief		Costruttore di copia della classe
	 *	\details	Costruttore di copia della classe
	 *	\param[in]	src	Altra istanza della classe AnimaTransformation da cui vengono copiati tutti i dati: matrici di trasformazione, oggetto padre, ecc...
	 *	\author		Zille Marco
	 */
	AnimaTransformation(AnimaTransformation&& src);
	
	/*!
	 *	\brief		Distruttore della classe
	 *	\details	Distruttore della classe
	 *	\author		Zille Marco
	 */
	~AnimaTransformation();
	
	/*!
	 *	\brief		Operatore di copia della classe
	 *	\details	Operatore di copia della classe
	 *	\param[in]	src	Altra istanza della classe AnimaTransformation da cui vengono copiati tutti i dati: matrici di trasformazione, oggetto padre, ecc...
	 *	\author		Zille Marco
	 */
	AnimaTransformation& operator=(const AnimaTransformation& src);
	
	/*!
	 *	\brief		Operatore di copia della classe
	 *	\details	Operatore di copia della classe
	 *	\param[in]	src	Altra istanza della classe AnimaTransformation da cui vengono copiati tutti i dati: matrici di trasformazione, oggetto padre, ecc...
	 *	\author		Zille Marco
	 */
	AnimaTransformation& operator=(AnimaTransformation&& src);
	
public:
	/*!
	 *	\brief		Applica una traslazione t alla trasformazione
	 *	\details	Applica una traslazione t alla trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	t	Valore della traslazione da applicare alla trasformazione
	 *	\author		Zille Marco
	 */
	void Translate(const AnimaVertex3f& t);
	
	/*!
	 *	\brief		Applica una traslazione alla trasformazione
	 *	\details	Applica una traslazione alla trasformazione secondo i parametri passati e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	tx	Valore X della traslazione da applicare alla trasformazione
	 *	\param[in]	ty	Valore Y della traslazione da applicare alla trasformazione
	 *	\param[in]	tz	Valore Z della traslazione da applicare alla trasformazione
	 *	\author		Zille Marco
	 */
	void Translate(AFloat tx, AFloat ty, AFloat tz);
	
	/*!
	 *	\brief		Applica una traslazione secondo l'asse X alla trasformazione
	 *	\details	Applica una traslazione secondo l'asse X alla trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	tx	Valore della traslazione da applicare alla trasformazione
	 *	\author		Zille Marco
	 */
	void TranslateX(AFloat tx);
	
	/*!
	 *	\brief		Applica una traslazione secondo l'asse Y alla trasformazione
	 *	\details	Applica una traslazione secondo l'asse Y alla trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	ty	Valore della traslazione da applicare alla trasformazione
	 *	\author		Zille Marco
	 */
	void TranslateY(AFloat ty);
	
	/*!
	 *	\brief		Applica una traslazione secondo l'asse Z alla trasformazione
	 *	\details	Applica una traslazione secondo l'asse Z alla trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	tz	Valore della traslazione da applicare alla trasformazione
	 *	\author		Zille Marco
	 */
	void TranslateZ(AFloat tz);
	
	/*!
	 *	\brief		Imposta il valore della traslazione della trasformazione
	 *	\details	Imposta il valore della traslazione della trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	t	Nuovo valore della traslazione da impostare per la trasformazione
	 *	\author		Zille Marco
	 */
	void SetTranslation(const AnimaVertex3f& t);
	
	/*!
	 *	\brief		Imposta il valore della traslazione della trasformazione
	 *	\details	Imposta il valore della traslazione della trasformazione secondo i parametri passati e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	tx	Nuovo valore X della traslazione da impostare per la trasformazione
	 *	\param[in]	ty	Nuovo valore Y della traslazione da impostare per la trasformazione
	 *	\param[in]	tz	Nuovo valore Z della traslazione da impostare per la trasformazione
	 *	\author		Zille Marco
	 */
	void SetTranslation(AFloat tx, AFloat ty, AFloat tz);
	
	/*!
	 *	\brief		Imposta il valore X della traslazione della trasformazione
	 *	\details	Imposta il valore X della traslazione della trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	tx	Nuovo valore X della traslazione da impostare per la trasformazione
	 *	\author		Zille Marco
	 */
	void SetTranslationX(AFloat tx);
	
	/*!
	 *	\brief		Imposta il valore Y della traslazione della trasformazione
	 *	\details	Imposta il valore Y della traslazione della trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	ty	Nuovo valore Y della traslazione da impostare per la trasformazione
	 *	\author		Zille Marco
	 */
	void SetTranslationY(AFloat ty);
	
	/*!
	 *	\brief		Imposta il valore Z della traslazione della trasformazione
	 *	\details	Imposta il valore Z della traslazione della trasformazione e chiama il metodo UpdateMatrix() per aggiornare le matrici di trasformazione
	 *	\param[in]	tz	Nuovo valore Z della traslazione da impostare per la trasformazione
	 *	\author		Zille Marco
	 */
	void SetTranslationZ(AFloat tz);
	
	/*!
	 *	\brief		Torna il valore della traslazione della trasformazione
	 *	\details	Torna il valore della traslazione della trasformazione
	 *	\return		Valore della traslazione della trasformazione
	 *	\author		Zille Marco
	 */
	AnimaVertex3f GetTranslation();
	
	/*!
	 *	\brief		Torna il valore X della traslazione della trasformazione
	 *	\details	Torna il valore X della traslazione della trasformazione
	 *	\return		Valore X della traslazione della trasformazione
	 *	\author		Zille Marco
	 */
	AFloat GetTranslationX();
	
	/*!
	 *	\brief		Torna il valore Y della traslazione della trasformazione
	 *	\details	Torna il valore Y della traslazione della trasformazione
	 *	\return		Valore Y della traslazione della trasformazione
	 *	\author		Zille Marco
	 */
	AFloat GetTranslationY();
	
	/*!
	 *	\brief		Torna il valore Z della traslazione della trasformazione
	 *	\details	Torna il valore Z della traslazione della trasformazione
	 *	\return		Valore Z della traslazione della trasformazione
	 *	\author		Zille Marco
	 */
	AFloat GetTranslationZ();
	
	AnimaVertex3f GetCompleteTranslation() const;

	void Rotate(const AnimaVertex3f& r);
	void Rotate(AFloat rx, AFloat ry, AFloat rz);
	void RotateX(AFloat rx);
	void RotateY(AFloat ry);
	void RotateZ(AFloat rz);
	void SetRotation(const AnimaVertex3f& r);
	void SetRotation(AFloat rx, AFloat ry, AFloat rz);
	void SetRotationX(AFloat rx);
	void SetRotationY(AFloat ry);
	void SetRotationZ(AFloat rz);
	void RotateDeg(const AnimaVertex3f& r);
	void RotateDeg(AFloat rx, AFloat ry, AFloat rz);
	void RotateXDeg(AFloat rx);
	void RotateYDeg(AFloat ry);
	void RotateZDeg(AFloat rz);
	void SetRotationDeg(const AnimaVertex3f& r);
	void SetRotationDeg(AFloat rx, AFloat ry, AFloat rz);
	void SetRotationXDeg(AFloat rx);
	void SetRotationYDeg(AFloat ry);
	void SetRotationZDeg(AFloat rz);
	AnimaVertex3f GetRotation();
	AFloat GetRotationX();
	AFloat GetRotationY();
	AFloat GetRotationZ();
	
	AnimaVertex3f GetCompleteRotation() const;

	void Scale(const AnimaVertex3f& s);
	void Scale(AFloat sx, AFloat sy, AFloat sz);
	void ScaleX(AFloat sx);
	void ScaleY(AFloat sy);
	void ScaleZ(AFloat sz);
	void SetScale(const AnimaVertex3f& s);
	void SetScale(AFloat sx, AFloat sy, AFloat sz);
	void SetScaleX(AFloat sx);
	void SetScaleY(AFloat sy);
	void SetScaleZ(AFloat sz);
	AnimaVertex3f GetScale();
	AFloat GetScaleX();
	AFloat GetScaleY();
	AFloat GetScaleZ();
	AnimaVertex3f GetCompleteScale() const;

	void UpdateMatrix();
	void SetTransformationMatrix(const AnimaMatrix& m);
	void SetTransformationMatrix(AFloat m[16]);
	void SetNormalMatrix(const AnimaMatrix& m);
	void SetNormalMatrix(AFloat m[16]);
	void SetAnimationGlobalInverseMatrix(const AnimaMatrix& m);
	void SetAnimationGlobalInverseMatrix(AFloat m[16]);

	AnimaMatrix	GetTransformationMatrix();
	AnimaMatrix* GetPTransformationMatrix();
	AnimaMatrix	GetNormalMatrix();
	AnimaMatrix* GetPNormalMatrix();
	AnimaMatrix	GetAnimationGlobalInverseMatrix();
	AnimaMatrix* GetPAnimationGlobalInverseMatrix();
	
	AnimaSceneObject* GetParentObject() const;
	
public:
	virtual ptree GetObjectTree() const;
	virtual bool ReadObject(const ptree& objectTree);

protected:
	void SetParentObject(AnimaSceneObject* parentObject);
	
protected:
	AnimaVertex3f _translation;
	AnimaVertex3f _rotation;
	AnimaVertex3f _scale;

	AnimaSceneObject* _parentObject;

	AnimaMatrix _transformationMatrix;
	AnimaMatrix _normalMatrix;
	AnimaMatrix _animationGlobalIverseMatrix;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTransformation__) */
