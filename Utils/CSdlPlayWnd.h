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
				if (!GetVideoInfo("a.h265")) MessageBox(NULL, L"��ȡ����ʧ��", NULL, NULL);//�ýṹ�崢����

				pSdlCtrl
				//pSdlCtrl = new CWindowWnd;


				if (SDL_Init(SDL_INIT_VIDEO) == -1) MessageBox(NULL, L"��ʼ��ʧ��", NULL, NULL);

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

					//�˳���ť

					return;

				}

				else if (msg.pSender->GetName() == _T("BtnMin"))

				{

					//��С����ť

					return;

				}

				else if (msg.pSender->GetName() == _T("BtnMax"))

				{

					//��󻯰�ť

					return;

				}

				else if (msg.pSender->GetName() == _T("BtnRestore"))

				{

					//��ԭ��ť

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
