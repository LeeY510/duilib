#ifndef SDL_PLAY_WND_HPP
#define SDL_PLAY_WND_HPP

#include "WinImplBase.h"

namespace DuiLib
{

	class CSdlPlayWnd : public WindowImplBase

	{
		virtual CControlUI* CreateControl(LPCTSTR pstrClassName)
		{
			if (_tcsicmp(pstrClassName, _T("WndPlayPanel")) == 0)

			{
				if (!GetVideoInfo("a.h265")) MessageBox(NULL, L"获取长宽失败", NULL, NULL);//用结构体储存宽高

				pSdlCtrl
				//pSdlCtrl = new CWindowWnd;


				if (SDL_Init(SDL_INIT_VIDEO) == -1) MessageBox(NULL, L"初始化失败", NULL, NULL);

				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

				hWnd = CreateWindow(_T("#32770"), _T("WndMediaDisplay"), WS_VISIBLE | WS_CHILD, 0, 31, _VideoInfo.w, _VideoInfo.h, m_PaintManager.GetPaintWindow(), (HMENU)0, NULL, NULL);
				pSdlCtrl->CreateDuiWindow()
				pUI->Attach(hWnd);

				return pUI;

			}

			return NULL;

		}

		virtual void Notify(TNotifyUI& msg)

		{

			if (msg.sType == _T("click"))

			{

				if (msg.pSender->GetName() == _T("BtnClose"))

				{

					//退出按钮

					return;

				}

				else if (msg.pSender->GetName() == _T("BtnMin"))

				{

					//最小化按钮

					return;

				}

				else if (msg.pSender->GetName() == _T("BtnMax"))

				{

					//最大化按钮

					return;

				}

				else if (msg.pSender->GetName() == _T("BtnRestore"))

				{

					//还原按钮

					return;

				}

			}

		}

		virtual void InitWindow()

		{

			CenterWindow();

		}

	public:

		CControlUI *pSdlCtrl;

	};
}

#endif // SDL_PLAY_WND_HPP
