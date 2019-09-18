#ifndef __UIELIPSEBUTTON_H__
#define __UIELIPSEBUTTON_H__

#pragma once

#include <gdiplus.h>
#pragma comment( lib, "GdiPlus.lib" )

namespace DuiLib
{
    class UILIB_API CEllipseButtonUI :  public CButtonUI
    {
    public:
        CEllipseButtonUI();
        virtual ~CEllipseButtonUI();

        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        void SetBkImage(LPCTSTR pStrImage);

        virtual void PaintBkImage(HDC hDC);

        virtual bool PointInRegion(const POINT& pt);

        void DoInit();

        DWORD GetPenColor(HDC hDC);

        virtual void PaintBorder(HDC hDC);
        virtual void SetPos(RECT rc);
        virtual void SetHotBorder(bool bHotBorder);
    private:
        HRGN                    m_hrgn;          //¿Ø¼þÇøÓò
        bool                    m_bHotBorder;
    };

}

#endif // __UIELIPSEBUTTON_H__

