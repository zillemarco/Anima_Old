#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaShader.h"
#include "AnimaShaderProgram.h"
#include "AnimaShaderInclude.h"
#include "AnimaMappedArray.h"
#include "AnimaArray.h"
#include <boost/property_tree/ptree.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaMaterial;
class AnimaMesh;
class AnimaMeshInstance;

class ANIMA_ENGINE_EXPORT AnimaShadersManager
{
	friend AnimaShaderProgram;

public:
	/*!
	 *	\brief		Costruttore della classe AnimaShadersManager
	 *	\details	Costruttore della classe AnimaShadersManager
	 *	\param[in]	engine	Puntatore ad un istanza di AnimaEngine
	 *	\author		Zille Marco
	 */
	AnimaShadersManager(AnimaEngine* engine);

	/*!
	 *	\brief		Distruttore della classe AnimaShadersManager
	 *	\details	Distruttore della classe AnimaShadersManager. Si occupa anche di rilasciare tutti gli shaders in memoria
	 *	\author		Zille Marco
	 */
	~AnimaShadersManager();

	/*!
	 *	\brief		Crea uno shader
	 *	\details	Crea uno shader senza alcun dato. Se uno shader con il nome passato è già esistente torna null
	 *	\param[in]	name	Nome dello shader che si vuole creare
	 *	\return		Puntatore al nuovo shader appena creato. Null se uno shader con il nome passato era già esistente
	 *	\author		Zille Marco
	 */
	AnimaShader* CreateShader(const AnimaString& name);

	/*!
	 *	\brief		Crea un programma
	 *	\details	Crea un programma senza alcun dato. Se un programma con il nome passato è già esistente torna null
	 *	\param[in]	name	Nome del programma che si vuole creare
	 *	\return		Puntatore al nuovo programma appena creato. Null se un programma con il nome passato era già esistente
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* CreateProgram(const AnimaString& name);

	/*!
	 *	\brief		Crea un file di inclusione
	 *	\details	Crea un file di inclusione senza alcun dato. Se un file di inclusione con il nome passato è già esistente torna null
	 *	\param[in]	name	Nome del file di inclusione che si vuole creare
	 *	\return		Puntatore al nuovo file di inclusione appena creato. Null se un file di inclusione con il nome passato era già esistente
	 *	\author		Zille Marco
	 */
	AnimaShaderInclude* CreateShaderInclude(const AnimaString& name);

	/*!
	 *	\brief		Crea un programma da una mesh ed il suo materiale
	 *	\details	Crea un programma da una mesh ed il suo materiale. Recupera tutti gli shader associati alla mesh ed al materiale e li mette assieme
	 				creando un programma (nuovo se non già esistente) che utilizza quegli shader.
	 *	\param[in]	mesh Mesh da cui andare a prendere gli shader
	 *	\param[in]	material Materiale da cui andare a prendere gli shader
	 *	\return		Puntatore al nuovo programma appena creato
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* CreateProgram(AnimaMesh* mesh, const AnimaMaterial* material);

	/*!
	 *	\brief		Crea un programma dall'istanza di una mesh ed il suo materiale
	 *	\details	Crea un programma dall'istanza di una mesh ed il suo materiale. Recupera tutti gli shader associati all'istanza ed al materiale e li mette assieme
	 				creando un programma (nuovo se non già esistente) che utilizza quegli shader.
	 *	\param[in]	meshInstance Istanza da cui andare a prendere gli shader
	 *	\param[in]	material Materiale da cui andare a prendere gli shader
	 *	\return		Puntatore al nuovo programma appena creato
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* CreateProgram(AnimaMeshInstance* meshInstance, const AnimaMaterial* material);

	/*!
	 *	\brief		Crea un nuovo shader con il nome e del tipo passati ed imposta il suo codice
	 *	\details	Crea un nuovo shader con il nome e del tipo passati ed imposta il suo codice. Se uno shader con il nome passato è già esistente torna null
	 *	\param[in]	name Nome dello shader che si vuole creare
	 *	\param[in]	text Codice dello shader
	 *	\param[in]	type Tipo dello shader
	 *	\return		Puntatore al nuovo shader appena creato. Null se uno shader con il nome passato era già esistente
	 *	\author		Zille Marco
	 */
	AnimaShader* LoadShader(const AnimaString& name, const AnimaString& text, AnimaShaderType type);

	/*!
	 *	\brief		Crea un nuovo shader con il nome e del tipo passati ed imposta il suo codice leggendolo dal file passato
	 *	\details	Crea un nuovo shader con il nome e del tipo passati ed imposta il suo codice leggendolo dal file passato. Se uno shader con il nome passato è già esistente torna null
	 *	\param[in]	name Nome dello shader che si vuole creare
	 *	\param[in]	filePath Path del file da dove viene recupero il codice dello shader
	 *	\param[in]	type Tipo dello shader
	 *	\return		Puntatore al nuovo shader appena creato. Null se uno shader con il nome passato era già esistente
	 *	\author		Zille Marco
	 */
	AnimaShader* LoadShaderFromFile(const AnimaString& name, const AnimaString& filePath, AnimaShaderType type);

	/*!
	 *	\brief		Crea un nuovo shader dato il percorso di un file contenente la definizione di uno shader
	 *	\details	Crea un nuovo shader dato il percorso di un file contenente la definizione di uno shader. Se la creazione non va a buon fine torna null
	 *	\param[in]	filePath Path del file contenente la definizione dello shader da caricare
	 *	\return		Puntatore al nuovo shader appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShader* LoadShaderFromFile(const AnimaString& filePath);

	/*!
	 *	\brief		Crea un nuovo shader dato un xml contenente la definizione di uno shader
	 *	\details	Crea un nuovo shader dato un xml contenente la definizione di uno shader. Se la creazione non va a buon fine torna null
	 *	\param[in]	shaderXmlDefinition Xml contenente la definizione dello shader
	 *	\return		Puntatore al nuovo shader appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShader* LoadShaderFromXml(const AnimaString& shaderXmlDefinition);

	/*!
	 *	\brief		Crea un nuovo shader interpretando i dati dato un albero XML in input
	 *	\details	Crea un nuovo shader interpretando i dati dato un albero XML in input. Se la creazione non va a buon fine torna null
	 *	\param[in]	xmlTree	Struttura dati contenente i dati XML dell'albero
	 *	\return		Puntatore al nuovo shader appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShader* LoadShaderFromXml(const boost::property_tree::ptree& xmlTree);

	/*!
	 *	\brief		Carica tutti i file di definizione di shader contenuto nel percorso passato
	 *	\details	Carica tutti i file di definizione di shader contenuto nel percorso passato chiamando LoadShaderFromFile per ogni file. Se il caricamento di un file non
	 				va a buon fine torna null
	 *	\param[in]	partsPath Path della cartella contenente i file da caricare
	 *	\return		True se il caricamento va a buon fine, false altrimenti
	 *	\author		Zille Marco
	 */
	bool LoadShadersParts(const AnimaString& partsPath);

	/*!
	 *	\brief		Crea un nuovo programma dato il percorso di un file contenente la definizione di un programma
	 *	\details	Crea un nuovo programma dato il percorso di un file contenente la definizione di un programma. Se la creazione non va a buon fine torna null
	 *	\param[in]	filePath Path del file contenente la definizione del programma da caricare
	 *	\return		Puntatore al nuovo programma appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* LoadShaderProgramFromFile(const AnimaString& filePath);

	/*!
	 *	\brief		Crea un nuovo programma dato un xml contenente la definizione di un programma
	 *	\details	Crea un nuovo programma dato un xml contenente la definizione di un programma. Se la creazione non va a buon fine torna null
	 *	\param[in]	shaderProgramXmlDefinition Xml contenente la definizione del programma da caricare
	 *	\return		Puntatore al nuovo programma appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* LoadShaderProgramFromXml(const AnimaString& shaderProgramXmlDefinition);

	/*!
	 *	\brief		Crea un nuovo programma interpretando i dati dato un albero XML in input
	 *	\details	Crea un nuovo programma interpretando i dati dato un albero XML in input. Se la creazione non va a buon fine torna null
	 *	\param[in]	xmlTree	Struttura dati contenente i dati XML dell'albero
	 *	\return		Puntatore al nuovo programma appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* LoadShaderProgramFromXml(const boost::property_tree::ptree& xmlTree);
	
	/*!
	 *	\brief		Crea un nuovo shader con il nome e del tipo passati ed imposta il suo codice leggendolo dal file passato
	 *	\details	Crea un nuovo shader con il nome e del tipo passati ed imposta il suo codice leggendolo dal file passato. Se uno shader con il nome passato è già esistente torna null
	 *	\param[in]	name Nome dello shader che si vuole creare
	 *	\param[in]	filePath Path del file da dove viene recupero il codice dello shader
	 *	\param[in]	type Tipo dello shader
	 *	\return		Puntatore al nuovo shader appena creato. Null se uno shader con il nome passato era già esistente
	 *	\author		Zille Marco
	 */
	AnimaShaderInclude* LoadShaderIncludeFromFile(const AnimaString& name, const AnimaString& filePath);

	/*!
	 *	\brief		Crea un nuovo shader dato il percorso di un file contenente la definizione di uno shader
	 *	\details	Crea un nuovo shader dato il percorso di un file contenente la definizione di uno shader. Se la creazione non va a buon fine torna null
	 *	\param[in]	filePath Path del file contenente la definizione dello shader da caricare
	 *	\return		Puntatore al nuovo shader appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShaderInclude* LoadShaderIncludeFromFile(const AnimaString& filePath);

	/*!
	 *	\brief		Carica un file di definizione di inclusione di shader dato un xml contenente la definizione di uno shader
	 *	\details	Carica un file di definizione di inclusione di shader dato un xml contenente la definizione di uno shader. Se la creazione non va a buon fine torna null
	 *	\param[in]	shaderXmlDefinition Xml contenente la definizione dello shader
	 *	\return		Puntatore al nuovo shader appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShaderInclude* LoadShaderIncludeFromXml(const AnimaString& includeXmlDefinition);

	/*!
	 *	\brief		Carica un file di definizione di inclusione di shader dato un albero XML in input
	 *	\details	Carica un file di definizione di inclusione di shader dato un albero XML in input. Se la creazione non va a buon fine torna null
	 *	\param[in]	xmlTree	Struttura dati contenente i dati XML dell'albero
	 *	\return		Puntatore al nuovo shader appena creato. Null se la creazione non va a buon fine
	 *	\author		Zille Marco
	 */
	AnimaShaderInclude* LoadShaderIncludeFromXml(const boost::property_tree::ptree& xmlTree);

	/*!
	 *	\brief		Carica tutti i file di definizione di inclusione di shader contenuti nel percorso passato
	 *	\details	Carica tutti i file di definizione di inclusione di shader contenuti nel percorso passato chiamando LoadShaderIncludeFromFile per ogni file. Se il caricamento di un file non
	 				va a buon fine torna null
	 *	\param[in]	includesPath Path della cartella contenente i file da caricare
	 *	\return		True se il caricamento va a buon fine, false altrimenti
	 *	\author		Zille Marco
	 */
	bool LoadShadersIncludes(const AnimaString& includesPath);

	/*!
	 *	\brief		Torna uno shader dato il suo indice all'interno dell'array contenente tutti gli shader
	 *	\details	Torna uno shader dato il suo indice all'interno dell'array contenente tutti gli shader
	 *	\param[in]	index Indice dello shader che di vuole recuperare
	 *	\return		Puntatore allo shader presente all'indice passato all'interno dell'array contenente tutti gli shader
	 *	\author		Zille Marco
	 */
	AnimaShader* GetShader(AInt index);

	/*!
	 *	\brief		Torna uno shader dato il suo nome
	 *	\details	Torna uno shader dato il suo nome
	 *	\param[in]	name Nome dello shader che di vuole recuperare
	 *	\return		Puntatore allo shader corrispondente al nome passato
	 *	\author		Zille Marco
	 */
	AnimaShader* GetShaderFromName(const AnimaString& name);

	/*!
	 *	\brief		Torna un programma dato il suo indice all'interno dell'array contenente tutti i programmi
	 *	\details	Torna un programma dato il suo indice all'interno dell'array contenente tutti i programmi
	 *	\param[in]	index Indice del programm che di vuole recuperare
	 *	\return		Puntatore al programma presente all'indice passato all'interno dell'array contenente tutti i programmi
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* GetProgram(AInt index);

	/*!
	 *	\brief		Torna un programma dato il suo nome
	 *	\details	Torna un programma dato il suo nome
	 *	\param[in]	name Nome del programma che di vuole recuperare
	 *	\return		Puntatore al programma corrispondente al nome passato
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* GetProgramFromName(const AnimaString& name);

	/*!
	 *	\brief		Imposta il programma attualmente attivo per il rendering
	 *	\details	Imposta il programma attualmente attivo per il rendering
	 *	\param[in]	program Puntatore al programma attivo per il rendering
	 *	\author		Zille Marco
	 */
	void SetActiveProgram(AnimaShaderProgram* program);

	/*!
	 *	\brief		Imposta il programma attualmente attivo per il rendering dato il nome
	 *	\details	Imposta il programma attualmente attivo per il rendering dato il nome
	 *	\param[in]	name Nome del programma attivo per il rendering
	 *	\author		Zille Marco
	 */
	void SetActiveProgramFromName(const AnimaString& name);

	/*!
	 *	\brief		Torna il punatote al programma attualmente attivo per il rendering
	 *	\details	Torna il punatote al programma attualmente attivo per il rendering
	 *	\return		Puntatore al programma attualmente attivo per il rendering. Null se nessun programma è attualmente attivo
	 *	\author		Zille Marco
	 */
	AnimaShaderProgram* GetActiveProgram();

	/*!
	 *	\brief		Cerca nel testo dello shader passato dei riferimenti a include e li sostituisce
	 *	\details	Cerca nel testo dello shader passato dei riferimenti a include e li sostituisce
	 *	\param[in]	shader Shader da controllare
	 *	\author		Zille Marco
	 */
	void AttachIncludes(AnimaShader* shader);

private:
	void ClearShaders();
	void ClearPrograms();
	void ClearIncludes();

	void NotifyProgramActivation(AnimaShaderProgram* program);
	void NotifyProgramDeactivation(AnimaShaderProgram* program);

private:
	AnimaEngine* _engine;								/*!< Puntatore all'istanza di un'istanza di AnimaEngine di cui questa classe fa parte */
	AnimaMappedArray<AnimaShader*> _shaders;			/*!< Mappa contenente tutti gli shader */
	AnimaMappedArray<AnimaShaderProgram*> _programs;	/*!< Mappa contenente tutti i programmi */
	AnimaMappedArray<AnimaShaderInclude*> _includes;	/*!< Mappa contenente tutti gli include possibili dei programmi */

	AnimaShaderProgram* _activeProgram;					/*!< Puntatore al programma attivo per il rendering */
};

END_ANIMA_ENGINE_NAMESPACE