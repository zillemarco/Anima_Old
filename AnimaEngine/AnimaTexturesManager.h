//
//  AnimaTexturesManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaTexturesManager__
#define __Anima__AnimaTexturesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaTexture.h"
#include "AnimaMappedArray.h"
#include "AnimaScene.h"

#include <boost/property_tree/ptree.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

struct AnimaTGAHeader {
	GLubyte Header[12];
};

struct AnimaTGA {
	GLubyte		header[6];
	GLuint		bytesPerPixel;
	GLuint		imageSize;
	GLuint		temp;
	GLuint		type;
	GLuint		Height;
	GLuint		Width;
	GLuint		Bpp;
};

class ANIMA_ENGINE_EXPORT AnimaTexturesManager
{
public:
	AnimaTexturesManager(AnimaScene* scene);
	~AnimaTexturesManager();

	/*!
	 *	\brief		Crea una nuova texture con il nome passato
	 *	\details	Crea una nuova texture con il nome passato
	 *	\param[in]	textureName	Nome della texture da creare
	 *	\return		Nel caso la creazione abbia successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 */
	AnimaTexture* CreateTexture(const AnimaString& textureName);

	/*!
	 *	\brief		Crea una texture e ci associa automaticamente il buffer passato in input
	 *	\details	Crea una texture e ci associa automaticamente il buffer passato in input.
					Si dovranno comunque impostare dati riguardo al tipo di formato, formato interno, clamp, ecc...
					Perfettamente equivalente a LoadTextureFromData()
	 *	\param[in]	textureName	Nome della texture da creare
	 *	\param[in]	width		Larghezza della texture
	 *	\param[in]	height		Altezza della texture
	 *	\param[in]	data		Puntatore al buffer con i dati della texture
	 *	\param[in]	dataSize	Lunghezza del buffer con i dati
	 *	\return		Nel caso la creazione della texture abbia avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 *	\sa			LoadTextureFromData()
	 */
	AnimaTexture* CreateTexture(const AnimaString& textureName, AUint width, AUint height, AUchar* data, ASizeT dataSize);

	/*!
	 *	\brief		Carica i dati contentuti in un file immagine riconoscendo il tipo di file e crea automaticamente la texture associata
	 *	\details	Carica i dati contentuti in un file immagine riconoscendo il tipo di file e crea automaticamente la texture associata.
					Si dovranno comunque impostare dati riguardo al tipo di formato, formato interno, clamp, ecc...
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[in]	textureName	Nome della texture da creare
	 *	\return		Nel caso la lettura e la creazione abbiano avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 *	\sa			LoadTextureFromBMPFile(), LoadTextureFromTGAFile(), LoadTextureFromDDSFile(), LoadTextureFromData()
	 */
	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath, const AnimaString& textureName);
	
	/*!
	 *	\brief		Crea una texture leggengo i dati da un file XML contenente le informazioni
	 *	\details	Crea una texture leggengo i dati da un file XML contenente le informazioni
	 *	\param[in]	filePath	Path del file XML con i dati da leggere
	 *	\return		Nel caso la lettura e la creazione abbiano avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 */
	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath);
	
	/*!
	 *	\brief		Crea una texture leggengo i dati da una stringa XML che contiene le informazioni
	 *	\details	Crea una texture leggengo i dati da una stringa XML che contiene le informazioni
	 *	\param[in]	textureXmlDefinition	Stringa XML che contiene le informazioni
	 *	\return		Nel caso la lettura e la creazione abbiano avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 */
	AnimaTexture* LoadTextureFromXml(const AnimaString& textureXmlDefinition);
	
	/*!
	 *	\brief		Crea una texture interpretando i dati dato un albero XML in input
	 *	\details	Crea una texture interpretando i dati dato un albero XML in input
	 *	\param[in]	textureXmlDefinition	Stringa XML che contiene le informazioni
	 *	\return		Nel caso la lettura e la creazione abbiano avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 */
	AnimaTexture* LoadTextureFromXml(const boost::property_tree::ptree& xmlTree);
	
	/*!
	 *	\brief		Crea delle texture leggendo i file XML contenuti nella cartella passata in input
	 *	\details	Crea delle texture leggendo i file XML contenuti nella cartella passata in input
	 *	\param[in]	texturesPath	Path della cartella da cui caricare i file XML contenenti del informazioni delle texture
	 *	\return		True nel caso la lettura e la creazione di tutti i file contenuti della cartella sia andata a buon fine, false altrimenti
	 *	\author		Zille Marco
	 */
	bool LoadTextures(const AnimaString& texturesPath);

	/*!
	 *	\brief		Carica i dati contentuti in un file BMP e crea automaticamente la texture associata
	 *	\details	Carica i dati contentuti in un file BMP e crea automaticamente la texture associata.
					Si dovranno comunque impostare dati riguardo al tipo di formato, formato interno, clamp, ecc...
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[in]	textureName	Nome della texture da creare
	 *	\return		Nel caso la lettura e la creazione abbiano avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 *	\sa			LoadTextureFromFile(), LoadTextureFromTGAFile(), LoadTextureFromDDSFile(), LoadTextureFromData()
	 */
	AnimaTexture* LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName);

	/*!
	 *	\brief		Carica i dati contentuti in un file TGA e crea automaticamente la texture associata
	 *	\details	Carica i dati contentuti in un file TGA e crea automaticamente la texture associata.
					Si dovranno comunque impostare dati riguardo al tipo di formato, formato interno, clamp, ecc...
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[in]	textureName	Nome della texture da creare
	 *	\return		Nel caso la lettura e la creazione abbiano avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 *	\sa			LoadTextureFromFile(), LoadTextureFromBMPFile(), LoadTextureFromDDSFile(), LoadTextureFromData()
	 */
	AnimaTexture* LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName);

	/*!
	 *	\brief		Carica i dati contentuti in un file DDS e crea automaticamente la texture associata
	 *	\details	Carica i dati contentuti in un file DDS e crea automaticamente la texture associata.
					Si dovranno comunque impostare dati riguardo al tipo di formato, formato interno, clamp, ecc...
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[in]	textureName	Nome della texture da creare
	 *	\return		Nel caso la lettura e la creazione abbiano avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 *	\sa			LoadTextureFromFile(), LoadTextureFromBMPFile(), LoadTextureFromTGAFile(), LoadTextureFromData()
	 */
	AnimaTexture* LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName);

	/*!
	 *	\brief		Crea una texture e ci associa automaticamente il buffer passato in input
	 *	\details	Crea una texture e ci associa automaticamente il buffer passato in input.
					Si dovranno comunque impostare dati riguardo al tipo di formato, formato interno, clamp, ecc...
	 *	\param[in]	textureName	Nome della texture da creare
	 *	\param[in]	data		Puntatore al buffer con i dati della texture
	 *	\param[in]	dataSize	Lunghezza del buffer con i dati
	 *	\param[in]	width		Larghezza della texture
	 *	\param[in]	height		Altezza della texture
	 *	\return		Nel caso la creazione della texture abbia avuto successo torna il puntatore alla nuova texture appena creata, NULL altrimenti
	 *	\author		Zille Marco
	 *	\sa			LoadTextureFromFile(), LoadTextureFromBMPFile(), LoadTextureFromTGAFile(), LoadTextureFromDDSFile()
	 */
	AnimaTexture* LoadTextureFromData(const AnimaString& textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height);

	/*!
	 *	\brief		Carica i dati contentuti in un file immagine utile per una texture riconoscendo il tipo di file
	 *	\details	Carica i dati contentuti in un file immagine utile per una texture riconoscendo il tipo di file. Nel caso di una texture
					in formato DDS torna anche il formato, altrimenti il formato che viene tornato è 0
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[out]	data		Parametro che verrà valorizzato con i dati del file. All'inizio il puntatore puntato da data deve essere NULL
								e viene allocatto all'interno della procedura. Quando i dati non servono più si deve procedere con il deallocarli
								manualmente tramite la funzione free()
	 *	\param[out]	dataSize	Conterrà la lunghezza del buffer letto
	 *	\param[out]	width		Conterrà da larghezza dell'immagine
	 *	\param[out]	height		Conterrà l'altezza dell'immagine
	 *	\param[out]	format		Nel caso il file sia una texture DDS questo parametro conterrà il formato della texture, altrimenti 0
	 *	\return		True nel caso la lettura abbia avuto successo, false altrimenti
	 *	\author		Zille Marco
	 *	\sa			GetTextureDataFromBMPFile(), GetTextureDataFromTGAFile(), GetTextureDataFromDDSFile()
	 */
	bool GetTextureDataFromFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height, AnimaTextureFormat& format);

	/*!
	 *	\brief		Carica i dati contentuti in un file BMP
	 *	\details	Carica i dati contentuti in un file BMP
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[out]	data		Parametro che verrà valorizzato con i dati del file. All'inizio il puntatore puntato da data deve essere NULL
								e viene allocatto all'interno della procedura. Quando i dati non servono più si deve procedere con il deallocarli
								manualmente tramite la funzione free()
	 *	\param[out]	dataSize	Conterrà la lunghezza del buffer letto
	 *	\param[out]	width		Conterrà da larghezza dell'immagine
	 *	\param[out]	height		Conterrà l'altezza dell'immagine
	 *	\return		True nel caso la lettura abbia avuto successo, false altrimenti
	 *	\author		Zille Marco
	 *	\sa			GetTextureDataFromFile(), GetTextureDataFromTGAFile(), GetTextureDataFromDDSFile()
	 */
	bool GetTextureDataFromBMPFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height);

	/*!
	 *	\brief		Carica i dati contentuti in un file TGA
	 *	\details	Carica i dati contentuti in un file TGA
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[out]	data		Parametro che verrà valorizzato con i dati del file. All'inizio il puntatore puntato da data deve essere NULL
								e viene allocatto all'interno della procedura. Quando i dati non servono più si deve procedere con il deallocarli
								manualmente tramite la funzione free()
	 *	\param[out]	dataSize	Conterrà la lunghezza del buffer letto
	 *	\param[out]	width		Conterrà da larghezza dell'immagine
	 *	\param[out]	height		Conterrà l'altezza dell'immagine
	 *	\return		True nel caso la lettura abbia avuto successo, false altrimenti
	 *	\author		Zille Marco
	 *	\sa			GetTextureDataFromFile(), GetTextureDataFromBMPFile(), GetTextureDataFromDDSFile()
	 */
	bool GetTextureDataFromTGAFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height);

	/*!
	 *	\brief		Carica i dati contentuti in un file DDS
	 *	\details	Carica i dati contentuti in un file DDS
	 *	\param[in]	filePath	Path del file da leggere
	 *	\param[out]	data		Parametro che verrà valorizzato con i dati del file. All'inizio il puntatore puntato da data deve essere NULL
								e viene allocatto all'interno della procedura. Quando i dati non servono più si deve procedere con il deallocarli
								manualmente tramite la funzione free()
	 *	\param[out]	dataSize	Conterrà la lunghezza del buffer letto
	 *	\param[out]	width		Conterrà da larghezza dell'immagine
	 *	\param[out]	height		Conterrà l'altezza dell'immagine
	 *	\param[out]	format		Conterrà il formato della texture, altrimenti 0
	 *	\return		True nel caso la lettura abbia avuto successo, false altrimenti
	 *	\author		Zille Marco
	 *	\sa			GetTextureDataFromFile(), GetTextureDataFromBMPFile(), GetTextureDataFromTGAFile()
	 */
	bool GetTextureDataFromDDSFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height, AnimaTextureFormat& format);

	/*!
	 *	\brief		Torna il numero di texture contenute dal manager
	 *	\details	Torna il numero di texture contenute dal manager
	 *	\return		Il numero di texture contenute dal manager
	 *	\author		Zille Marco
	 */
	AInt GetTexturesCount() const;

	/*!
	 *	\brief		Torna il puntatore ad una texture contenuta nel manager dato il suo indice
	 *	\details	Torna il puntatore ad una texture contenuta nel manager dato il suo indice
	 *	\return		Puntatore alla texture contenuta dal manager all'indice passato. NULL se l'indice non è valido
	 *	\author		Zille Marco
	 */
	AnimaTexture* GetTexture(AUint index);

	/*!
	 *	\brief		Torna il puntatore ad una texture contenuta nel manager dato il suo nome
	 *	\details	Torna il puntatore ad una texture contenuta nel manager dato il suo nome
	 *	\return		Puntatore alla texture contenuta dal manager con il nome passato. NULL se la texture con quel nome non esiste
	 *	\author		Zille Marco
	 */
	AnimaTexture* GetTexture(const AnimaString& textureName);
	
private:
	void ClearTextures();

	bool GetUncompressedTGAData(FILE* file, AUchar** data, AUint& dataSize, AUint& width, AUint& height);
	bool GetCompressedTGAData(FILE* file, AUchar** data, AUint& dataSize, AUint& width, AUint& height);
	
private:
	AnimaScene* _scene;

	AnimaMappedArray<AnimaTexture*> _textures;
};

END_ANIMA_ENGINE_NAMESPACE

#endif  /* defined(__Anima__AnimaTexturesManager__) */
