#include "stdafx.h"
#include "UIWin.h"


namespace DuiLib
{

    CWndUI::CWndUI() : m_hWnd(NULL)
    {
    }

    CWndUI::~CWndUI()
    {
    }

    LPCTSTR CWndUI::GetClass() const
    {
        return _T("WndUI");
    }

    LPVOID CWndUI::GetInterface(LPCTSTR pstrName)
    {
        if (_tcscmp(pstrName, _T("Wnd")) == 0) return static_cast<CWndUI*>(this);
        return CControlUI::GetInterface(pstrName);
    }

    void CWndUI::SetInternVisible(bool bVisible /*= true*/)
    {
        __super::SetInternVisible(bVisible);
        if (NULL != m_hWnd)
        {
            ::ShowWindow(m_hWnd, bVisible);
        }
    }

    void CWndUI::SetPos(RECT rc)
    {
        __super::SetPos(rc);
        if (NULL != m_hWnd)
        {
            ::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }

    BOOL CWndUI::Attach(HWND hWnd)
    {
        if (NULL == hWnd || !::IsWindow(hWnd))
        {
            return FALSE;
        }

        m_hWnd = hWnd;
        return TRUE;
    }

    HWND CWndUI::Detach()
    {
        if (NULL == m_hWnd)
        {
            return NULL;
        }

        HWND hWnd = m_hWnd;
        m_hWnd = NULL;
        return hWnd;
    }

    HWND CWndUI::GetHwnd()
    {
        return m_hWnd;
    }

    SIZE CWndUI::EstimateSize(SIZE szAvailable)
    {
        return CControlUI::EstimateSize(szAvailable);
    }

    void CWndUI::PaintText(HDC hDC)
    {
        CLabelUI::PaintText(hDC);
    }
}