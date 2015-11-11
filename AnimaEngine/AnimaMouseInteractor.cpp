//
//  AnimaMouseInteractor.cpp
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaMouseInteractor.h"
#include "AnimaLogger.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#if _MSC_VER >= 1300    // for VC 7.0
	// from ATL 7.0 sources
	#ifndef _delayimp_h
		extern "C" IMAGE_DOS_HEADER __ImageBase;
	#endif
#endif

HMODULE GetCurrentModule()
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

	// Here's a trick that will get you the handle of the module
	// you're running in without any a-priori knowledge:
	// http://www.dotnet247.com/247reference/msgs/13/65259.aspx

	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery(&dummy, &mbi, sizeof(mbi));

	return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else    // VC 7.0

	// from ATL 7.0 sources
	return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}


boost::unordered_map<long, AnimaMouseInteractor*> AnimaMouseInteractor::_installedInteractors;

AnimaString FetchLastErrorMessage(DWORD dwError, AnimaString& strMessage)
{
	LPVOID lpMsgBuf = NULL;

	// Search for the message description in the std windows
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	if (lpMsgBuf != NULL)
	{
		strMessage = (LPCTSTR)lpMsgBuf;
		LocalFree(lpMsgBuf);
	}
	else
	{
		if (dwError)
		{
			//strMessage.Format("Communications Error Number (%d)", dwError);
		}

	}

	return strMessage;
}

AnimaMouseInteractor::AnimaMouseInteractor()
{
	_windowId = 0;
	_engine = nullptr;
	_installed = false;
	_mouseMoved = false;
	_windowsProcHook = NULL;
	_messageProcHook = NULL;
	
	_supportedEvents = {
		//		{"onMouseMoved",		""},
			{ "onLeftMouseDragged", "" },
			{ "onLeftMouseDown", "" },
			{ "onLeftMouseUp", "" },
			{ "onLeftMouseClick", "" },
			{ "onRightMouseDragged", "" },
			{ "onRightMouseDown", "" },
			{ "onRightMouseUp", "" },
			{ "onRightMouseClick", "" }
	};
}

AnimaMouseInteractor::AnimaMouseInteractor(const AnimaMouseInteractor& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	_mouseMoved = src._mouseMoved;
	_lastMousePosition = src._lastMousePosition;
	_windowsProcHook = src._windowsProcHook;
	_messageProcHook = src._messageProcHook;
}

AnimaMouseInteractor::AnimaMouseInteractor(AnimaMouseInteractor&& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	_mouseMoved = src._mouseMoved;
	_lastMousePosition = src._lastMousePosition;
	_windowsProcHook = src._windowsProcHook;
	_messageProcHook = src._messageProcHook;
}

AnimaMouseInteractor::~AnimaMouseInteractor()
{

}

AnimaMouseInteractor& AnimaMouseInteractor::operator=(const AnimaMouseInteractor& src)
{
	if (this != &src)
	{
		_windowId = src._windowId;
		_engine = src._engine;
		_installed = src._installed;
		_mouseMoved = src._mouseMoved;
		_lastMousePosition = src._lastMousePosition;
		_windowsProcHook = src._windowsProcHook;
		_messageProcHook = src._messageProcHook;
	}

	return *this;
}

AnimaMouseInteractor& AnimaMouseInteractor::operator=(AnimaMouseInteractor&& src)
{
	if (this != &src)
	{
		_windowId = src._windowId;
		_engine = src._engine;
		_installed = src._installed;
		_mouseMoved = src._mouseMoved;
		_lastMousePosition = src._lastMousePosition;
		_windowsProcHook = src._windowsProcHook;
		_messageProcHook = src._messageProcHook;
	}

	return *this;
}

bool AnimaMouseInteractor::Install(long windowId, AnimaEngine* engine)
{
	AnimaLogger::LogMessage("Installing mouse interactor...");

	if (_installed)
	{
		AnimaLogger::LogMessage("Mouse interactor already installed");
		return true;
	}

	_windowId = windowId;
	_engine = engine;

	HMODULE module = GetCurrentModule();
	_windowsProcHook = SetWindowsHookEx(WH_CALLWNDPROC, WindowsProcCallback, module, NULL);
	if (_windowsProcHook == NULL)
	{
		AnimaLogger::LogMessage("Unable to install mouse interactor");
		return false;
	}
	_messageProcHook = SetWindowsHookEx(WH_GETMESSAGE, MessageProcCallback, module, NULL);
	if (_messageProcHook == NULL)
	{
		AnimaLogger::LogMessage("Unable to install mouse interactor");
		return false;
	}

	// Aggingo questo interactor alla lista di interactor installati
	_installedInteractors[windowId] = this;

	_installed = true;

	AnimaLogger::LogMessage("Mouse interactor installed succesfully");

	return true;
}

bool AnimaMouseInteractor::Remove()
{
	if (_installed == false)
		return false;

	// Rimuovo questo interactor dalla lista di interactor installati
	auto interactor = _installedInteractors.find(_windowId);
	if (interactor != _installedInteractors.end())
		_installedInteractors.erase(interactor);

	_windowId = 0;
	_installed = false;
	return true;
}

bool AnimaMouseInteractor::SetEventHandler(const AnimaString& eventName, std::function<void(AnimaEventArgs* eventArgs)> handler)
{
	if (EventIsSupported(eventName))
	{
		_eventsMap[eventName] = handler;
		return true;
	}
	return false;
}

bool AnimaMouseInteractor::LaunchEvent(const AnimaString& eventName, AnimaEventArgs* eventArgs) const
{
	auto func = _eventsMap.find(eventName);
	if (func != _eventsMap.end() && func->second != nullptr)
	{
		func->second(eventArgs);
		return true;
	}

	return false;
}

AnimaArray<AnimaEventInfo> AnimaMouseInteractor::GetSupportedEvents() const
{
	return _supportedEvents;
}

bool AnimaMouseInteractor::EventIsSupported(const AnimaString& eventName) const
{
	for (auto item : _supportedEvents)
	{
		if (item._eventName == eventName)
			return true;
	}

	return false;
}

LRESULT CALLBACK AnimaMouseInteractor::WindowsProcCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* pWndProcStruct = reinterpret_cast<CWPSTRUCT*>(lParam);
	if (pWndProcStruct == nullptr)
		return 0;

	auto pair = _installedInteractors.find((long)pWndProcStruct->hwnd);
	if (pair == _installedInteractors.end())
		return 0;

	AnimaMouseInteractor* pMouseInteractor = pair->second;
	if (pMouseInteractor == nullptr)
		return 0;
	
	if (nCode == HC_ACTION)
	{
		HWND wndProcWnd = pWndProcStruct->hwnd;
		AUint wndProcMessage = pWndProcStruct->message;
		WPARAM wndProcWparam = pWndProcStruct->wParam;
		WPARAM wndProcLparam = pWndProcStruct->lParam;

		HandleMessage(wndProcWnd, wndProcMessage, wndProcWparam, wndProcLparam, pMouseInteractor);
	}

	return CallNextHookEx(pMouseInteractor->_windowsProcHook, nCode, wParam, lParam);
}

LRESULT CALLBACK AnimaMouseInteractor::MessageProcCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSG* pMessageProcStruct = reinterpret_cast<MSG*>(lParam);
	if (pMessageProcStruct == nullptr)
		return 0;

	auto pair = _installedInteractors.find((long)pMessageProcStruct->hwnd);
	if (pair == _installedInteractors.end())
		return 0;

	AnimaMouseInteractor* pMouseInteractor = pair->second;
	if (pMouseInteractor == nullptr)
		return 0;

	if (nCode == HC_ACTION)
	{
		HWND wndProcWnd = pMessageProcStruct->hwnd;
		AUint wndProcMessage = pMessageProcStruct->message;
		WPARAM wndProcWparam = pMessageProcStruct->wParam;
		WPARAM wndProcLparam = pMessageProcStruct->lParam;

		HandleMessage(wndProcWnd, wndProcMessage, wndProcWparam, wndProcLparam, pMouseInteractor);
	}

	return CallNextHookEx(pMouseInteractor->_windowsProcHook, nCode, wParam, lParam);
}

void AnimaMouseInteractor::HandleMessage(HWND hWnd, AUint message, WPARAM wParam, LPARAM lParam, AnimaMouseInteractor* interactor)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		AnimaVertex2f pt(xPos, yPos);
		AnimaVertex2f delta = interactor->_lastMousePosition - pt;
		AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

		// Siccome il messaggio di mouse move viene lanciato anche al primo click con i tasti del mouse,
		// controllo se la posizione non è diversa da quella dell'ultima posizione nota e il flag _mouseMoved è falso
		// che in questo caso significa che ho appena fatto click con un pulsante e non devo mandare il messaggio di mouse move
		// o dragged e soprattuo non impostare il flag _mouseMoved a true, altrimenti non verranno mai inviati dei messaggi di click
		if (pt.x == interactor->_lastMousePosition.x && pt.y == interactor->_lastMousePosition.y && interactor->_mouseMoved == false)
			return;

		interactor->_lastMousePosition = pt;
		interactor->_mouseMoved = true;

		if ((wParam & MK_LBUTTON) == MK_LBUTTON)
		{
			AnimaEventArgs* args = new AnimaMouseDraggedEventArgs(interactor, pt, size, 0, delta);
			interactor->LaunchEvent("onLeftMouseDragged", args);

			delete args;
			args = nullptr;
		}
		else if ((wParam & MK_RBUTTON) == MK_RBUTTON)
		{
			AnimaEventArgs* args = new AnimaMouseDraggedEventArgs(interactor, pt, size, 0, delta);
			interactor->LaunchEvent("onRightMouseDragged", args);

			delete args;
			args = nullptr;
		}

		break;
	}
	case WM_LBUTTONDOWN:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));
		
		AnimaVertex2f pt(xPos, yPos);
		AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

		interactor->_lastMousePosition = pt;
		interactor->_mouseMoved = false;

		AnimaEventArgs* args = new AnimaMouseEventArgs(interactor, pt, size, 0);
		interactor->LaunchEvent("onLeftMouseDown", args);

		delete args;
		args = nullptr;

		break;
	}
	case WM_RBUTTONDOWN:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		AnimaVertex2f pt(xPos, yPos);
		AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

		interactor->_lastMousePosition = pt;
		interactor->_mouseMoved = false;

		AnimaEventArgs* args = new AnimaMouseEventArgs(interactor, pt, size, 0);
		interactor->LaunchEvent("onRightMouseDown", args);

		delete args;
		args = nullptr;

		break;
	}
	case WM_LBUTTONUP:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		AnimaVertex2f pt(xPos, yPos);
		AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

		interactor->_lastMousePosition = pt;

		AnimaEventArgs* argsUp = new AnimaMouseEventArgs(interactor, pt, size, 0);
		interactor->LaunchEvent("onLeftMouseUp", argsUp);

		delete argsUp;
		argsUp = nullptr;

		if (interactor->_mouseMoved == false)
		{
			AnimaEventArgs* argsClick = new AnimaMouseEventArgs(interactor, pt, size, 0);
			interactor->LaunchEvent("onLeftMouseClick", argsClick);

			delete argsClick;
			argsClick = nullptr;
		}

		break;
	}
	case WM_RBUTTONUP:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		AnimaVertex2f pt(xPos, yPos);
		AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

		interactor->_lastMousePosition = pt;

		AnimaEventArgs* argsUp = new AnimaMouseEventArgs(interactor, pt, size, 0);
		interactor->LaunchEvent("onRightMouseUp", argsUp);

		delete argsUp;
		argsUp = nullptr;

		if (interactor->_mouseMoved == false)
		{
			AnimaEventArgs* argsClick = new AnimaMouseEventArgs(interactor, pt, size, 0);
			interactor->LaunchEvent("onRightMouseClick", argsClick);

			delete argsClick;
			argsClick = nullptr;
		}

		break;
	}
	}
}

END_ANIMA_ENGINE_NAMESPACE