//
//  AnimaTimer.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaTimer__
#define __Anima__AnimaTimer__

#include <iostream>
#include <chrono>
#include "AnimaEngineCore.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

/*!
 *	\brief		Classe timer utilizzata da AnimaEngine
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaTimer
{
public:
	/*!
	 *	\brief		Costruttore della classe AnimaTimer
	 *	\details	Costruttore della classe AnimaTimer. Chiama in automatico il metodo Reset() per azzerare il timer e 
					cominciare a contare il tempo
	 *	\sa			Reset()
	 *	\author		Zille Marco
	 */
	AnimaTimer();
	
public:
	/*!
	 *	\brief		Azzera il timer
	 *	\details	Azzera il timer. Va ad impostare il valore di _beg. Chiamata anche dal costruttore della classe
	 *	\sa			AnimaTimer(), _beg
	 *	\author		Zille Marco
	 */
	void Reset();
	
	/*!
	 *	\brief		Torna il tempo che è trascoro dall'ultima chiamata al metodo Reset()
	 *	\details	Torna il tempo che è trascoro dall'ultima chiamata al metodo Reset() in millisecondi
	 *	\sa			Reset()
	 *	\author		Zille Marco
	 */
	double Elapsed() const;
	
	/*!
	 *	\brief		Stampa a console il tempo che è trascorso dall'ultima chiamata al metodo Reset()
	 *	\details	Stampa a console il tempo che è trascorso dall'ultima chiamata al metodo Reset() in millisecondi
	 *	\sa			Elapsed(), Reset()
	 *	\author		Zille Marco
	 */
	void PrintElapsed();
	
private:
	// Sopprimo il warning 4251 di VS che dice che per poter utilizzare questo tipo al di fuori della libreria deve avere un'interfaccia dichiarata
#pragma warning (disable: 4251) 
	
	/*!
	 *	\brief		Definizione del tipo di orologio utilizzato dalla classe. In questo caso è un orologio ad alta definizione
	 *	\details	Definizione del tipo di orologio utilizzato dalla classe. In questo caso è un orologio ad alta definizione
	 */
	typedef std::chrono::high_resolution_clock _clock;
	
	/*!
	 *	\brief		Definizione della precisione di un secondo di orologio
	 *	\details	Definizione della precisione di un secondo di orologio. In questo caso si tratta di 1000 millisecondi
	 */
	typedef std::chrono::duration<double, std::ratio<1> > _second;
	
	/*!
	 *	\brief		Attributo in cui viene salvato l'istante di tempo in cui viene chiamato il metodo Reset()
	 *	\details	Attributo in cui viene salvato l'istante di tempo in cui viene chiamato il metodo Reset()
	 */
	std::chrono::time_point<_clock> _beg;
	
	// Riabilito il warning 4251
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTimer__) */
