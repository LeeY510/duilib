#ifndef __UIWIN_H__
#define __UIWIN_H__

#pragma once

namespace DuiLib
{
    class UILIB_API CWndUI : public CLabelUI
	{
	public:

        CWndUI();
        ~CWndUI();

        virtual LPCTSTR GetClass() const;

        virtual LPVOID GetInterface(LPCTSTR pstrName);

        virtual void SetInternVisible(bool bVisible = true);

        virtual void SetPos(RECT rc);

        BOOL Attach(HWND hWnd);

        HWND Detach();

        HWND GetHwnd();

        SIZE EstimateSize(SIZE szAvailable);

        void PaintText(HDC hDC);

        void SetWndInternVisible(bool bVisible)
        {
            m_bWndInternVisible = bVisible;
        }
	protected:
		HWND m_hWnd;
        RECT m_rcWnd;
        bool m_bWndInternVisible;
	};
}

#endif // __UIWIN_H__