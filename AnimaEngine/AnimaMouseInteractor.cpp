//
//  AnimaMouseInteractor.cpp
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaMouseInteractor.h"
#include "AnimaKeyboardInteractor.h"
#include "AnimaLogger.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

boost::unordered_map<long, AnimaMouseInteractor*> AnimaMouseInteractor::_installedInteractors;

AnimaMouseInteractor::AnimaMouseInteractor()
{
	_windowId = 0;
	_engine = nullptr;
	_installed = false;
	_mouseMoved = false;
	_windowsProcHook = NULL;
	_messageProcHook = NULL;

	_modifiers = 0;
	_mousePosition = AnimaVertex2f(0, 0);
	_delta = AnimaVertex2f(0, 0);
	_leftButtonDown = false;
	_rightButtonDown = false;
	
	_supportedEvents = {
		//		{"onMouseMoved",		""},
			{ "onLeftMouseDragged", "" },
			{ "onLeftMouseDown", "" },
			{ "onLeftMouseUp", "" },
			{ "onLeftMouseClick", "" },
			{ "onRightMouseDragged", "" },
			{ "onRightMouseDown", "" },
			{ "onRightMouseUp", "" },
			{ "onRightMouseClick", "" },
			{"onUpdateScene", ""}
	};
}

AnimaMouseInteractor::AnimaMouseInteractor(const AnimaMouseInteractor& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	_mouseMoved = src._mouseMoved;
	_mousePosition = src._mousePosition;
	_windowsProcHook = src._windowsProcHook;
	_messageProcHook = src._messageProcHook;

	_modifiers = src._modifiers;
	_mousePosition = src._mousePosition;
	_delta = src._delta;
	_leftButtonDown = src._leftButtonDown;
	_rightButtonDown = src._rightButtonDown;
}

AnimaMouseInteractor::AnimaMouseInteractor(AnimaMouseInteractor&& src)
{
	_windowId = src._windowId;
	_engine = src._engine;
	_installed = src._installed;
	_mouseMoved = src._mouseMoved;
	_mousePosition = src._mousePosition;
	_windowsProcHook = src._windowsProcHook;
	_messageProcHook = src._messageProcHook;

	_modifiers = src._modifiers;
	_mousePosition = src._mousePosition;
	_delta = src._delta;
	_leftButtonDown = src._leftButtonDown;
	_rightButtonDown = src._rightButtonDown;
}

AnimaMouseInteractor::~AnimaMouseInteractor()
{
	Remove();
}

AnimaMouseInteractor& AnimaMouseInteractor::operator=(const AnimaMouseInteractor& src)
{
	if (this != &src)
	{
		_windowId = src._windowId;
		_engine = src._engine;
		_installed = src._installed;
		_mouseMoved = src._mouseMoved;
		_mousePosition = src._mousePosition;
		_windowsProcHook = src._windowsProcHook;
		_messageProcHook = src._messageProcHook;

		_modifiers = src._modifiers;
		_mousePosition = src._mousePosition;
		_delta = src._delta;
		_leftButtonDown = src._leftButtonDown;
		_rightButtonDown = src._rightButtonDown;
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
		_mousePosition = src._mousePosition;
		_windowsProcHook = src._windowsProcHook;
		_messageProcHook = src._messageProcHook;

		_modifiers = src._modifiers;
		_mousePosition = src._mousePosition;
		_delta = src._delta;
		_leftButtonDown = src._leftButtonDown;
		_rightButtonDown = src._rightButtonDown;
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

	UnhookWindowsHook(WH_CALLWNDPROC, WindowsProcCallback);
	UnhookWindowsHook(WH_GETMESSAGE, MessageProcCallback);

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

		POINT point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		
		if (PtInRect(&rc, point))
		{
			AnimaVertex2f pt(xPos, yPos);
			AnimaVertex2f delta = interactor->_mousePosition - pt;
			AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

			// Siccome il messaggio di mouse move viene lanciato anche al primo click con i tasti del mouse,
			// controllo se la posizione non è diversa da quella dell'ultima posizione nota e il flag _mouseMoved è falso
			// che in questo caso significa che ho appena fatto click con un pulsante e non devo mandare il messaggio di mouse move
			// o dragged e soprattuo non impostare il flag _mouseMoved a true, altrimenti non verranno mai inviati dei messaggi di click
			if (pt.x == interactor->_mousePosition.x && pt.y == interactor->_mousePosition.y && interactor->_mouseMoved == false)
				return;

			interactor->_mousePosition = pt;
			interactor->_mouseMoved = true;
			interactor->_delta += delta;
			interactor->_modifiers = TranslateFlags();
			interactor->_leftButtonDown = false;
			interactor->_rightButtonDown = false;

			if ((wParam & MK_LBUTTON) == MK_LBUTTON)
			{
				interactor->_leftButtonDown = true;

				AnimaEventArgs* args = new AnimaMouseDraggedEventArgs(interactor, pt, size, TranslateFlags(), delta);
				interactor->LaunchEvent("onLeftMouseDragged", args);

				delete args;
				args = nullptr;
			}
			else if ((wParam & MK_RBUTTON) == MK_RBUTTON)
			{
				interactor->_rightButtonDown = true;

				AnimaEventArgs* args = new AnimaMouseDraggedEventArgs(interactor, pt, size, TranslateFlags(), delta);
				interactor->LaunchEvent("onRightMouseDragged", args);

				delete args;
				args = nullptr;
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		POINT point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		if (PtInRect(&rc, point))
		{
			AnimaVertex2f pt(xPos, yPos);
			AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

			interactor->_mousePosition = pt;
			interactor->_mouseMoved = false;
			interactor->_delta = AnimaVertex2f(0, 0);
			interactor->_modifiers = TranslateFlags();
			interactor->_leftButtonDown = true;
			interactor->_rightButtonDown = false;

			AnimaEventArgs* args = new AnimaMouseEventArgs(interactor, pt, size, TranslateFlags());
			interactor->LaunchEvent("onLeftMouseDown", args);

			delete args;
			args = nullptr;
		}
		break;
	}
	case WM_RBUTTONDOWN:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		POINT point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		if (PtInRect(&rc, point))
		{
			AnimaVertex2f pt(xPos, yPos);
			AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

			interactor->_mousePosition = pt;
			interactor->_mouseMoved = false;
			interactor->_delta = AnimaVertex2f(0, 0);
			interactor->_modifiers = TranslateFlags();
			interactor->_leftButtonDown = false;
			interactor->_rightButtonDown = true;

			AnimaEventArgs* args = new AnimaMouseEventArgs(interactor, pt, size, TranslateFlags());
			interactor->LaunchEvent("onRightMouseDown", args);

			delete args;
			args = nullptr;
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		POINT point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		if (PtInRect(&rc, point))
		{
			AnimaVertex2f pt(xPos, yPos);
			AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

			interactor->_mousePosition = pt;
			interactor->_delta = AnimaVertex2f(0, 0);
			interactor->_modifiers = TranslateFlags();
			interactor->_leftButtonDown = false;
			interactor->_rightButtonDown = false;

			AnimaEventArgs* argsUp = new AnimaMouseEventArgs(interactor, pt, size, TranslateFlags());
			interactor->LaunchEvent("onLeftMouseUp", argsUp);

			delete argsUp;
			argsUp = nullptr;

			if (interactor->_mouseMoved == false)
			{
				AnimaEventArgs* argsClick = new AnimaMouseEventArgs(interactor, pt, size, TranslateFlags());
				interactor->LaunchEvent("onLeftMouseClick", argsClick);

				delete argsClick;
				argsClick = nullptr;
			}
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		AFloat xPos = (AFloat)GET_X_LPARAM(lParam);
		AFloat yPos = (AFloat)(rc.bottom - GET_Y_LPARAM(lParam));

		POINT point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		if (PtInRect(&rc, point))
		{
			AnimaVertex2f pt(xPos, yPos);
			AnimaVertex2f size((AFloat)rc.right - (AFloat)rc.left, (AFloat)rc.bottom - (AFloat)rc.top);

			interactor->_mousePosition = pt;
			interactor->_delta = AnimaVertex2f(0, 0);
			interactor->_modifiers = TranslateFlags();
			interactor->_leftButtonDown = false;
			interactor->_rightButtonDown = false;

			AnimaEventArgs* argsUp = new AnimaMouseEventArgs(interactor, pt, size, TranslateFlags());
			interactor->LaunchEvent("onRightMouseUp", argsUp);

			delete argsUp;
			argsUp = nullptr;

			if (interactor->_mouseMoved == false)
			{
				AnimaEventArgs* argsClick = new AnimaMouseEventArgs(interactor, pt, size, TranslateFlags());
				interactor->LaunchEvent("onRightMouseClick", argsClick);

				delete argsClick;
				argsClick = nullptr;
			}
		}
		break;
	}
	}
}

void AnimaMouseInteractor::UpdateScene(AnimaScene* scene, AFloat elapsedTime)
{
	AnimaEventArgs* args = new AnimaUpdateSceneEventArgs(this, scene, elapsedTime);

	LaunchEvent("onUpdateScene", args);

	delete args;
	args = nullptr;
}

END_ANIMA_ENGINE_NAMESPACE