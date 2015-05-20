//
//  AnimaGBuffer.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaGBuffer__
#define __Anima__AnimaGBuffer__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaAllocator.h"
#include "AnimaTexture.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

/*!
 *	\brief		Classe AnimaGBuffer
 *	\details	Questa classe viene utilizzata delle classi AnimaRenderer, e derivate, per poter effettuare il rendering su texture multiple.
				Un'istanza di AnimaGBuffer infatti contiene più texture su che possono essere destinazione, oppure sorgente, di uno shader.
				Tutte le texture hanno la stessa dimensione e possono essere usate o tutte come input, o tutte come output. Inoltre se le
				texture	di un GBuffer, o il GBuffer in se, sono impostatecome input, non possono essere utilizzate allo stesso tempo anche
				come destinazione di output.
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaGBuffer
{
public:
	/*!
	 *	\brief		Costruttore base di AnimaGBuffer
	 *	\details	Costruttore base di AnimaGBuffer. Accetta in ingresso un allocator usato poi dalle texture per poter 
					salvare dati in memoria
	 *	\param[in]	allocator Allocator usato dalle texture
	 *	\author		Zille Marco
	 */
	AnimaGBuffer(AnimaAllocator* allocator);
	
	/*!
	 *	\brief		Costruttore di AnimaGBuffer
	 *	\details	Costruttore di AnimaGBuffer. Accetta in ingresso un allocator usato poi dalle texture per poter
					salvare dati in memoria e le dimensioni delle texture.
	 *	\param[in]	allocator	Allocator usato dalle texture
	 *	\param[in]	width		Larghezza che avranno le texture
	 *	\param[in]	height		Altezza che avranno le texture
	 *	\author		Zille Marco
	 */
	AnimaGBuffer(AnimaAllocator* allocator, AUint width, AUint height);
	
	/*!
	 *	\brief		Costruttore di copia di AnimaGBuffer
	 *	\details	Costruttore di copia di AnimaGBuffer
	 *	\param[in]	src	Altra istanza di AnimaGBuffer da cui verranno copiate tutte le impostazione e dati. Le texture vengono copiate ma non duplicate.
	 *	\author		Zille Marco
	 */
	AnimaGBuffer(const AnimaGBuffer& src);
	
	/*!
	 *	\brief		Costruttore di copia di AnimaGBuffer
	 *	\details	Costruttore di copia di AnimaGBuffer
	 *	\param[in]	src	Altra istanza di AnimaGBuffer da cui verranno copiate tutte le impostazione e dati. Le texture vengono copiate ma non duplicate.
	 *	\author		Zille Marco
	 */
	AnimaGBuffer(AnimaGBuffer&& src);
	
	/*!
	 *	\brief		Distruttore della classe
	 *	\details	Distruttore della classe. Si occupa anche di deallocate tutta la memoria utilizzata dalle texture
	 *	\author		Zille Marco
	 */
	~AnimaGBuffer();
	
	/*!
	 *	\brief		Operatore di copia di AnimaGBuffer
	 *	\details	Operatore di copia di AnimaGBuffer
	 *	\param[in]	src	Altra istanza di AnimaGBuffer da cui verranno copiate tutte le impostazione e dati. Le texture eventualmente contenute dall'istanza vengono distrutta 
						per poi copiare le texture dell'instanza sorgente
	 *	\author		Zille Marco
	 */
	AnimaGBuffer& operator=(const AnimaGBuffer& src);
	
	/*!
	 *	\brief		Operatore di copia di AnimaGBuffer
	 *	\details	Operatore di copia di AnimaGBuffer
	 *	\param[in]	src	Altra istanza di AnimaGBuffer da cui verranno copiate tutte le impostazione e dati. Le texture eventualmente contenute dall'istanza vengono distrutta
					per poi copiare le texture dell'instanza sorgente
	 *	\author		Zille Marco
	 */
	AnimaGBuffer& operator=(AnimaGBuffer&& src);

public:
	/*!
	 *	\brief		Aggiunge una texture al GBuffer
	 *	\details	Aggiunge una texture al GBuffer. Se esiste già una texture con il nome passato tra quelle già presenti nel GBuffer la funzione non va a buon fine e torna false
	 *	\param[in]	name			Nome della texture che si vuole aggiungere
	 *	\param[in]	target			Target della texture (ad esempio GL_TEXTURE_2D, GL_TEXTURE_3D, ecc...)
	 *	\param[in]	attachment		Parametro a cui si deve legare la texture per l'output (ad esempio GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, ecc...)
	 +	\param[in]	internalFormat	Formato interno della texture (ad esempio GL_RGB, GL_RGBA, GL_BGR, ecc...)
	 *	\param[in]	format			Formato della texture (ad esempio GL_RGB, GL_RGBA, ecc...)
	 *	\param[in]	dataType		Tipo di dati memorizzati nella texture (ad esempio GL_FLOAT, GL_BYTE, ecc...)
	 *	\param[in]	filter			Filtro da applicare alla texture (ad esempio GL_LINEAR, ecc...)
	 *	\param[in]	clamp			Metodo di clamping della texture (ad esempio GL_REPEAT, GL_BORDER, ecc...)
	 *	\return		True se il nome della texture non è già presente all'interno della lista delle texture del GBuffer, false altrimenti.
	 *	\author		Zille Marco
	 */
	bool AddTexture(const AnimaString& name, AUint target, AUint attachment, AUint internalFormat, AUint format, AUint dataType, AUint filter, AUint clamp);
	
	/*!
	 *	\brief		Aggiunge una texture al GBuffer
	 *	\details	Aggiunge una texture al GBuffer. Se esiste già una texture con il nome passato tra quelle già presenti nel GBuffer la funzione non va a buon fine e torna false
	 *	\param[in]	name			Nome della texture che si vuole aggiungere
	 *	\param[in]	target			Target della texture (ad esempio GL_TEXTURE_2D, GL_TEXTURE_3D, ecc...)
	 *	\param[in]	attachment		Parametro a cui si deve legare la texture per l'output (ad esempio GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, ecc...)
	 +	\param[in]	internalFormat	Formato interno della texture (ad esempio GL_RGB, GL_RGBA, GL_BGR, ecc...)
	 *	\param[in]	format			Formato della texture (ad esempio GL_RGB, GL_RGBA, ecc...)
	 *	\param[in]	dataType		Tipo di dati memorizzati nella texture (ad esempio GL_FLOAT, GL_BYTE, ecc...)
	 *	\param[in]	filter			Filtro da applicare alla texture (ad esempio GL_LINEAR, ecc...)
	 *	\param[in]	clamp			Metodo di clamping della texture (ad esempio GL_REPEAT, GL_BORDER, ecc...)
	 *	\return		True se il nome della texture non è già presente all'interno della lista delle texture del GBuffer, false altrimenti.
	 *	\author		Zille Marco
	 */
	bool AddTexture(const char* name, AUint target, AUint attachment, AUint internalFormat, AUint format, AUint dataType, AUint filter, AUint clamp);
	
	/*!
	 *	\brief		Ridimensione le texture del GBuffer
	 *	\details	Ridimensione le texture del GBuffer. Le texture non vengono subito ridimensionate, ma vengono modificati i valori _width e _heigth ed impostato il flag
					_needsResize a true, in modo che alla prima chiamata di Create() le texture vengano ridimensionate
	 *	\param[in]	width	Nuova larghezza delle texture
	 *	\param[in]	height	Nuova larghezza delle texture
	 *	\sa			_needsResize, _width, _height, Create()
	 *	\author		Zille Marco
	 */
	void Resize(AUint width, AUint height);
	
	/*!
	 *	\brief		Crea le texture del GBuffer
	 *	\details	Crea le texture del GBuffer. Se le texture sono già state create non fa nulla, tranne se è stato impostato il flag _needsResize che al quel punto la funzione
					procede con il ridimensionamento delle texture
	 *	\return		Torna true se il GBuffer riesce a creare correttamente tutte le texture, false altrimenti
	 *	\sa			_needsResize, _width, _height, Resize()
	 *	\author		Zille Marco
	 */
	bool Create();
	
	/*!
	 *	\brief		Distrugge le texture del GBuffer
	 *	\details	Distrugge le texture del GBuffer
	 *	\author		Zille Marco
	 */
	void Destroy();
	
	/*!
	 *	\brief		Imposta le texture del GBuffer come destinazione del rendering
	 *	\details	Imposta le texture del GBuffer come destinazione del rendering. Deve essere stato chiamato il metodo Create() in precedenza e deve essere andato a buon fine
	 *	\sa			Create()
	 *	\author		Zille Marco
	 */
	void BindAsRenderTarget() const;
	
	/*!
	 *	\brief		Torna una delle texture del GBuffer
	 *	\details	Torna una delle texture del GBuffer
	 *	\param[in]	name	Nome della texture che si vuole recuperare
	 *	\return		Puntatore alla texture contenuta dal GBuffer che ha il nome specificato. NULL se la texture col nome specificato non è presente nel GBuffer
	 *	\author		Zille Marco
	 */
	AnimaTexture* GetTexture(const AnimaString& name);
	
	/*!
	 *	\brief		Torna una delle texture del GBuffer
	 *	\details	Torna una delle texture del GBuffer
	 *	\param[in]	name	Nome della texture che si vuole recuperare
	 *	\return		Puntatore alla texture contenuta dal GBuffer che ha il nome specificato. NULL se la texture col nome specificato non è presente nel GBuffer
	 *	\author		Zille Marco
	 */
	AnimaTexture* GetTexture(const char* name);

protected:
	/*!
	 *	\brief		Elimina completamente le texture del GBuffer deallocandone anche la memoria
	 *	\details	Elimina completamente le texture del GBuffer deallocandone anche la memoria
	 *	\author		Zille Marco
	 */
	void ClearTextures();

protected:
	/*!
	 *	\brief		Classe che identifica i dati di una singola texture del GBuffer
	 *	\details	Classe che identifica i dati di una singola texture del GBuffer. Contiene la texture, il suo nome, l'indice che deve avere nei binding con gli shader e
					il buffer in output a cui la texture è legata
	 *	\author		Zille Marco
	 */
	class AnimaGBufferData
	{
	public:
		/*!
		 *	\brief		Costruttore della classe
		 *	\details	Costruttore della classe che accetta in input in nome della texture, la texture, l'indice che deve avere nei binding con gli shader e
						il buffer in output a cui la texture è legata
		 *	\param[in]	name		Nome della texture
		 *	\param[in]	texture		Texture da legare al GBuffer
		 *	\param[in]	index		Indice di binding con gli shader
		 *	\param[in]	attachment	Buffer in output dello shader a cui la texture è legata
		 *	\author		Zille Marco
		 */
		AnimaGBufferData(const AnimaString& name, AnimaTexture* texture, AUint index, AUint attachment) {
			_name = name;
			_index = index;
			_texture = texture;
			_attachment = attachment;
		}
		
		/*!
		 *	\brief		Costruttore di copia della classe
		 *	\details	Costruttore di copia della classe
		 *	\param[in]	src	Altra istanza della classe AnimaGBufferData da cui vengono copiati i dati
		 *	\author		Zille Marco
		 */
		AnimaGBufferData(const AnimaGBufferData& src) {
			_name = src._name;
			_index = src._index;
			_texture = src._texture;
			_attachment = src._attachment;
		}
		
		/*!
		 *	\brief		Costruttore di copia della classe
		 *	\details	Costruttore di copia della classe
		 *	\param[in]	src	Altra istanza della classe AnimaGBufferData da cui vengono copiati i dati
		 *	\author		Zille Marco
		 */
		AnimaGBufferData(const AnimaGBufferData&& src) {
			_name = src._name;
			_index = src._index;
			_texture = src._texture;
			_attachment = src._attachment;
		}

	public:
		AnimaString		_name;			/*!< Nome della texture */
		AUint			_index;			/*!< Indice di binding della textuer con uno shader */
		AUint			_attachment;	/*!< Buffer in output dello shader a cui la texture sarà legata */
		AnimaTexture*	_texture;		/*!< Texture del GBuffer */
	};

	/*!
	*	\brief		Definizione del dataset utilizzato per mappare le texture del GBuffer
	*	\details	Definizione del dataset utilizzato per mappare le texture del GBuffer. Viene utilizzata un'indicizzazione secondo due
					indici: il nome della texture per una ricerca veloce e l'indice di binding per collegarle in modo ordinato con gli shader
	*/
	typedef boost::multi_index_container<
		AnimaGBufferData*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<BOOST_MULTI_INDEX_MEMBER(AnimaGBufferData, AUint, _index)>,
		boost::multi_index::hashed_unique<BOOST_MULTI_INDEX_MEMBER(AnimaGBufferData, AnimaString, _name), AnimaString::Hasher> >
	> AnimaGBufferDataSet;

	/*!
	 *	\brief		Definizione del wrapper di una texture del GBuffer in base all'indice di binding
	 *	\details	Definizione del wrapper di una texture del GBuffer in base all'indice di binding
	 */
	typedef AnimaGBufferDataSet::nth_index<0>::type AnimaGBufferDataSetByIndex;
	
	/*!
	 *	\brief		Definizione del wrapper di una texture del GBuffer in base al nome della texture
	 *	\details	Definizione del wrapper di una texture del GBuffer in base al nome della texture
	 */
	typedef AnimaGBufferDataSet::nth_index<1>::type AnimaGBufferDataSetByName;

protected:
	AnimaAllocator* _allocator;		/*!<	Allocator usato per le texture del GBuffer */
	AUint	_width;					/*!<	Larghezza delle texture del GBuffer */
	AUint	_height;				/*!<	Altezza delle texture del GBuffer */
	AUint	_frameBuffer;			/*!<	Frame buffer di OpenGL a cui il GBuffer è legato */
	AUint	_renderBuffer;			/*!<	Render buffer di OpenGL a cui il GBuffer è legato */
	bool	_created;				/*!<	Flag che indica se il GBuffer è già stato creato */
	bool	_needsResize;			/*!<	Flag che indica che è stato chiamato il metodo Resize() e le texture devono ancora essere											ridimensionate */
	
#pragma warning (disable: 4251)
	AnimaGBufferDataSet	_texturesSet;	/*!< Set delle texture contenute dal GBuffer */
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaGBuffer__) */
