#include "stdafx.h"
#include "UIWin.h"


namespace DuiLib
{

    CWndUI::CWndUI() : m_hWnd(NULL)
        , m_bWndInternVisible(true)
    {
        m_rcWnd.left = 0;
        m_rcWnd.top = 0;
        m_rcWnd.right = 0;
        m_rcWnd.bottom = 0;
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
            if (!bVisible)
            {
                ::ShowWindow(m_hWnd, bVisible);
            }
            else if (m_bVisible && m_bWndInternVisible)
            {
                ::ShowWindow(m_hWnd, bVisible);
            }
        }
    }

    void CWndUI::SetPos(RECT rc)
    {
        __super::SetPos(rc);
        if (NULL != m_hWnd)
        {
            RECT tmp = rc;
            if (0 != m_nBorderSize)
            {
                tmp.left += m_nBorderSize;
                tmp.top += m_nBorderSize;
                tmp.right -= m_nBorderSize;
                tmp.bottom -= m_nBorderSize;
            }

            m_rcWnd = tmp;
            ::SetWindowPos(m_hWnd, NULL, m_rcWnd.left, m_rcWnd.top, m_rcWnd.right - m_rcWnd.left, m_rcWnd.bottom - m_rcWnd.top, SWP_NOZORDER | SWP_NOACTIVATE);
            RECT rcClient = { 0 };
            ::GetClientRect(m_hWnd, &rcClient);
            ::InvalidateRect(m_hWnd, &rcClient, TRUE);
            ::UpdateWindow(m_hWnd);
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