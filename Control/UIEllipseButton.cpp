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
        DWORD dwBorderColor = GetBorderColor();
        if (0 == m_nBorderSize || 0 == dwBorderColor || (m_bHotBorder && ((m_uButtonState & UISTATE_HOT) == 0)))
        {
            //像素值颜色取点（ pt.x + 1, pt.y + 1）的值
            RECT rc = GetPos();
            COLORREF color = GetPixel(hDC, rc.left + 1, rc.top + 1);//左上角
            if (CLR_INVALID == color)
            {
                color = GetPixel(hDC, rc.right - 1, rc.bottom - 1);//右下角
                if (CLR_INVALID == color)
                {
                    color = GetPixel(hDC, rc.left + 1, rc.bottom - 1);//左下角
                    if (CLR_INVALID == color)
                    {
                        color = GetPixel(hDC, rc.right - 1, rc.top + 1);//右上角
                        if (CLR_INVALID == color)
                        {
                            return dwBorderColor;
                        }
                    }
                }
            }
            
            BYTE r = GetRValue(color);
            BYTE g = GetGValue(color);
            BYTE b = GetBValue(color);

            return Color(255, r, g, b).GetValue();
        }      

        return dwBorderColor;
    }

    void CEllipseButtonUI::DoInit()
    {
    }

    void CEllipseButtonUI::PaintBkColor(HDC hDC)
    {
        Gdiplus::RectF rcDraw((Gdiplus::REAL)m_rcItem.left, (Gdiplus::REAL)m_rcItem.top, (Gdiplus::REAL)m_rcItem.right - m_rcItem.left, (Gdiplus::REAL)m_rcItem.bottom - m_rcItem.top);
        Gdiplus::SolidBrush texture(Gdiplus::Color(GetAdjustColor(m_dwBackColor)));
        Gdiplus::Brush* brush = &texture;
        Gdiplus::Graphics graphics(hDC);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.FillEllipse(brush, rcDraw);
    }
    
    bool CEllipseButtonUI::DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify /*= NULL*/)
    {
        const TImageInfo* data = m_pManager->GetGdiplusImageEx(pStrImage);
        if (NULL == data)
        {
            m_pManager->AddGdiplusImage(pStrImage, NULL, false);
            data = m_pManager->GetGdiplusImage(pStrImage);
        }

        if (NULL == data || data->pGdiplusImage->GetLastStatus() != Gdiplus::Ok)
        {
            return false;
        }

        //坐标
        int iWidth = m_rcItem.right - m_rcItem.left;
        int iHeight = m_rcItem.bottom - m_rcItem.top;

        if (0 != m_nBorderSize && 1 != m_nBorderSize)
        {
            iWidth -= m_nBorderSize * 2;
            iHeight -= m_nBorderSize * 2;
        }

        Gdiplus::Graphics graphics(hDC);
        if (graphics.GetLastStatus() != Gdiplus::Ok)
            return false;

        //消除锯齿
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

        Gdiplus::GraphicsPath graphicspath;
        if (graphicspath.GetLastStatus() != Gdiplus::Ok)
            return false;

        Gdiplus::REAL rBorderSize = m_nBorderSize;
        Gdiplus::REAL rLeft = m_rcItem.left;
        Gdiplus::REAL rTop = m_rcItem.top;
        Gdiplus::REAL rWidth = iWidth;
        Gdiplus::REAL rHeight = iHeight;
        if (0 != m_nBorderSize && 1 != m_nBorderSize)
        {
            rLeft += rBorderSize / 2 - 1;
            rTop += rBorderSize / 2 - 1;
            rWidth += rBorderSize / 2 + 1;
            rHeight += rBorderSize / 2 + 1;
        }
        else
        {
            --rWidth;
            --rHeight;
        }
        graphicspath.AddEllipse(rLeft, rTop, rWidth, rHeight);

        //设置裁剪圆
        graphics.SetClip(&graphicspath, Gdiplus::CombineModeReplace);

        //绘制图像
        graphics.DrawImage(data->pGdiplusImage, rLeft, rTop, rWidth, rHeight);
        return true;
    }

    void CEllipseButtonUI::PaintStatusImage(HDC hDC)
    {
        if (IsFocused()) m_uButtonState |= UISTATE_FOCUSED;
        else m_uButtonState &= ~UISTATE_FOCUSED;
        if (!IsEnabled()) m_uButtonState |= UISTATE_DISABLED;
        else m_uButtonState &= ~UISTATE_DISABLED;

        if ((m_uButtonState & UISTATE_DISABLED) != 0) {
            if (!m_sDisabledImage.IsEmpty())
            {
                if (!DrawImage(hDC, (LPCTSTR)m_sDisabledImage)) m_sDisabledImage.Empty();
                else goto Label_ForeImage;
            }
        }
        else if ((m_uButtonState & UISTATE_PUSHED) != 0) {
            if (!m_sPushedImage.IsEmpty()) {
                if (!DrawImage(hDC, (LPCTSTR)m_sPushedImage)){
                    m_sPushedImage.Empty();
                }
                if (!m_sPushedForeImage.IsEmpty())
                {
                    if (!DrawImage(hDC, (LPCTSTR)m_sPushedForeImage))
                        m_sPushedForeImage.Empty();
                    return;
                }
                else goto Label_ForeImage;
            }
        }
        else if ((m_uButtonState & UISTATE_HOT) != 0) {
            if (!m_sHotImage.IsEmpty()) {
                if (!DrawImage(hDC, (LPCTSTR)m_sHotImage)){
                    m_sHotImage.Empty();
                }
                if (!m_sHotForeImage.IsEmpty()) {
                    if (!DrawImage(hDC, (LPCTSTR)m_sHotForeImage))
                        m_sHotForeImage.Empty();
                    return;
                }
                else goto Label_ForeImage;
            }
            else if (m_dwHotBkColor != 0) {
                Gdiplus::RectF rcDraw((Gdiplus::REAL)m_rcItem.left, (Gdiplus::REAL)m_rcItem.top, (Gdiplus::REAL)m_rcItem.right - m_rcItem.left, (Gdiplus::REAL)m_rcItem.bottom - m_rcItem.top);
                Gdiplus::SolidBrush texture(Gdiplus::Color(GetAdjustColor(m_dwHotBkColor)));
                Gdiplus::Brush* brush = &texture;
                Gdiplus::Graphics graphics(hDC);
                graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
                graphics.FillEllipse(brush, rcDraw);
                return;
            }
        }
        else if ((m_uButtonState & UISTATE_FOCUSED) != 0) {
            if (!m_sFocusedImage.IsEmpty()) {
                if (!DrawImage(hDC, (LPCTSTR)m_sFocusedImage)) m_sFocusedImage.Empty();
                else goto Label_ForeImage;
            }
        }

        if (!m_sNormalImage.IsEmpty()) {
            if (!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) m_sNormalImage.Empty();
            else goto Label_ForeImage;
        }

        if (!m_sForeImage.IsEmpty())
            goto Label_ForeImage;

        return;

    Label_ForeImage:
        if (!m_sForeImage.IsEmpty()) {
            if (!DrawImage(hDC, (LPCTSTR)m_sForeImage)) m_sForeImage.Empty();
        }
    }
    

    bool CEllipseButtonUI::PointInRegion(const POINT& pt)
    {
        return (TRUE == ::PtInRegion(m_hrgn, pt.x, pt.y)) ? true : false;
    }

    void CEllipseButtonUI::PaintBorder(HDC hDC)
    {
        DWORD dwBorderColor = GetBorderColor();
        if (m_sBkImage.IsEmpty() && (!m_nBorderSize || !dwBorderColor))
        {
            return;
        }

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
            width -= m_nBorderSize;
            height -= m_nBorderSize;
        }
        else
        {
            --width;
            --height;
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

