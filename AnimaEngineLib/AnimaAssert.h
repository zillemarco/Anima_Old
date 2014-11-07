//
//  AnimaAssert.h
//  Anima
//
//  Created by Marco Zille on 30/10/14.
//
//

#ifndef Anima_AnimaAssert_h
#define Anima_AnimaAssert_h

#include "AnimaEngineLib.h"

#include <QApplication>
#include <QDialog>

#if defined ANIMA_ENGINE_EXPORT_ENABLED
#	if _MSC_VER
		void __declspec(dllexport) reportAssertionFailure(const char* expr, const char* fileName, int line);
#	else
		extern "C" { void reportAssertionFailure(const char* expr, const char* fileName, int line); }
#	endif
#else
#	if _MSC_VER
		void __declspec(dllimport) reportAssertionFailure(const char* expr, const char* fileName, int line);
#	else
		void reportAssertionFailure(const char* expr, const char* fileName, int line);
#	endif
#endif

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

BEGIN_DERIVED_CLASS_EXPORT(AnimaAssertWindow, public QDialog)
{
	Q_OBJECT

public:
	AnimaAssertWindow(const char* expr, const char* fileName, int line, QWidget* parent = NULL);

private:
	QLabel*			_label;
	QPushButton*	_closeButton;
	QPushButton*	_reportButton;
};

#ifndef _MSC_VER
#include <signal.h>
#endif

#if defined _MSC_VER
#define debug_break()   //__debugbreak()
#else
#define debug_break()   //raise(SIGTRAP)
#endif

#define ANIMA_ASSERT(expr)                  \
        if(expr) { }                        \
        else                                \
        {                                   \
            reportAssertionFailure(#expr,   \
                __FILE__, __LINE__);        \
            debug_break();                  \
        }                                   \

#endif
