#include "StdAfx.h"
#include "UIEllipseButton.h"


namespace DuiLib
{
    CEllipseButtonUI::CEllipseButtonUI()
    {
        CControlUI::m_bSpecialCtrl = true;
        m_bHotBorder = false;
        m_hrgn = NULL;
    }

    CEllipseButtonUI::~CEllipseButtonUI()
    {
        if (NULL != m_hrgn)
        {
            DeleteObject(m_hrgn);
            m_hrgn = NULL;
        }
    }

    LPCTSTR CEllipseButtonUI::GetClass() const
    {
        return _T("EllipseButtonUI");
    }

    LPVOID CEllipseButtonUI::GetInterface(LPCTSTR pstrName)
    {
        if (_tcscmp(pstrName, DUI_CTR_ELLIPSEBUTTON) == 0) return static_cast<CEllipseButtonUI*>(this);
        return CButtonUI::GetInterface(pstrName);
    }

    void CEllipseButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if (_tcscmp(pstrName, _T("bkimage")) == 0)
        {
            SetBkImage(pstrValue);
        }            
        else if (_tcscmp(pstrName, _T("hotborder")) == 0)
        {
            SetHotBorder(_tcscmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            return __super::SetAttribute(pstrName, pstrValue);
        }
            
    }

    void CEllipseButtonUI::SetBkImage(LPCTSTR pStrImage)
    {
        if (m_sBkImage == pStrImage) return;

        m_sBkImage = pStrImage;
        Invalidate();        
    }

    DWORD CEllipseButtonUI::GetPenColor(HDC hDC)
    {
        if (0 == m_nBorderSize || 0 == m_dwBorderColor || (m_bHotBorder && ((m_uButtonState & UISTATE_HOT) == 0)))
        {
            //像素值颜色取点（ pt.x + 1, pt.y + 1）的值
            RECT rc = GetPos();
            COLORREF color = GetPixel(hDC, rc.left + 1, rc.top + 1);

            BYTE r = GetRValue(color);
            BYTE g = GetGValue(color);
            BYTE b = GetBValue(color);

            return Color(255, r, g, b).GetValue();
        }      

        return m_dwBorderColor;
    }

    void CEllipseButtonUI::DoInit()
    {
    }

    void CEllipseButtonUI::PaintBkImage(HDC hDC)
    {
        const TImageInfo* data = m_pManager->GetGdiplusImageEx((LPCTSTR)m_sBkImage);
        if (NULL == data)
        {
            m_pManager->AddGdiplusImage((LPCTSTR)m_sBkImage, NULL, false);
            data = m_pManager->GetGdiplusImage((LPCTSTR)m_sBkImage);
        }        

        if (NULL == data || data->pGdiplusImage->GetLastStatus() != Gdiplus::Ok)
        {
            return;
        }

        //坐标
        POINT pt = { m_rcItem.left, m_rcItem.top };
        SIZE sz = { m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top };
        if (0 != m_nBorderSize && 1 != m_nBorderSize)
        {
            pt.x += m_nBorderSize;
            pt.y += m_nBorderSize;
            sz.cx -= m_nBorderSize*2;
            sz.cy -= m_nBorderSize*2;
        }
         
        Gdiplus::Graphics graphics(hDC);
        if (graphics.GetLastStatus() != Gdiplus::Ok)
            return;

        //消除锯齿
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

        Gdiplus::GraphicsPath graphicspath;
        if (graphicspath.GetLastStatus() != Gdiplus::Ok)
            return;

        graphicspath.AddEllipse(pt.x, pt.y, sz.cx, sz.cy);

        //设置裁剪圆
        graphics.SetClip(&graphicspath, Gdiplus::CombineModeReplace);

        //绘制图像
        graphics.DrawImage(data->pGdiplusImage, pt.x, pt.y, sz.cx, sz.cy);
    }

    bool CEllipseButtonUI::PointInRegion(const POINT& pt)
    {
        return (TRUE == ::PtInRegion(m_hrgn, pt.x, pt.y)) ? true : false;
    }

    void CEllipseButtonUI::PaintBorder(HDC hDC)
    {
        Gdiplus::Pen myPen(GetPenColor(hDC));//默认宽度为1
        if (myPen.GetLastStatus() != Gdiplus::Ok)
        {
            return;
        }

        if (0 != m_nBorderSize && 1 != m_nBorderSize)
        {
            myPen.SetWidth(m_nBorderSize);
        }

        Gdiplus::Graphics graphics(hDC);
        if (graphics.GetLastStatus() != Gdiplus::Ok)
        {
            return;
        }

        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        int x = m_rcItem.left;
        int y = m_rcItem.top;
        int width = m_rcItem.right - m_rcItem.left;
        int height = m_rcItem.bottom - m_rcItem.top;
        if (0 != m_nBorderSize && 1 != m_nBorderSize)
        {
            int offset = m_nBorderSize % 2;
            x += m_nBorderSize / 2 + offset;
            y += m_nBorderSize / 2 + offset;
            width -= m_nBorderSize * 2 - offset;
            height -= m_nBorderSize * 2 - offset;
        }

        graphics.DrawEllipse(&myPen,x, y, width, height);
    }

    void CEllipseButtonUI::SetPos(RECT rc)
    {
        __super::SetPos(rc);
        if (NULL != m_hrgn)
        {
            DeleteObject(m_hrgn);
            m_hrgn = NULL;
        }

        m_hrgn = ::CreateEllipticRgn(m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
    }

    void CEllipseButtonUI::SetHotBorder(bool bHotBorder)
    {
        m_bHotBorder = bHotBorder;
    }

}

