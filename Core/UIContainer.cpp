#include "StdAfx.h"

namespace DuiLib
{

    /////////////////////////////////////////////////////////////////////////////////////
    //
    //

    CContainerUI::CContainerUI()
        : m_iChildPadding(0),
        m_bAutoDestroy(true),
        m_bDelayedDestroy(true),
        m_bMouseChildEnabled(true),
        m_pVerticalScrollBar(NULL),
        m_pHorizontalScrollBar(NULL),
        m_bScrollProcess(false),
        m_bDragable(false),
        m_uButtonState(0),
        m_iSepSize(4),
        m_bImmMode(false),
        m_iDragDir(eDragAll),
        m_pDragCb(NULL),
        m_pDragCbParam(NULL)
    {
        m_ptLastMouse.x = m_ptLastMouse.y = 0;
        ::ZeroMemory(&m_rcInset, sizeof(m_rcInset));
        ::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
    }

    CContainerUI::~CContainerUI()
    {
        m_bDelayedDestroy = false;
        RemoveAll();
        if (m_pVerticalScrollBar) delete m_pVerticalScrollBar;
        if (m_pHorizontalScrollBar) delete m_pHorizontalScrollBar;
    }

    LPCTSTR CContainerUI::GetClass() const
    {
        return _T("ContainerUI");
    }

    LPVOID CContainerUI::GetInterface(LPCTSTR pstrName)
    {
        if (_tcscmp(pstrName, _T("IContainer")) == 0) return static_cast<IContainerUI*>(this);
        else if (_tcscmp(pstrName, DUI_CTR_CONTAINER) == 0) return static_cast<CContainerUI*>(this);
        return CControlUI::GetInterface(pstrName);
    }

    CControlUI* CContainerUI::GetItemAt(int iIndex) const
    {
        if (iIndex < 0 || iIndex >= m_items.GetSize()) return NULL;
        return static_cast<CControlUI*>(m_items[iIndex]);
    }

    int CContainerUI::GetItemIndex(CControlUI* pControl) const
    {
        for (int it = 0; it < m_items.GetSize(); it++) {
            if (static_cast<CControlUI*>(m_items[it]) == pControl) {
                return it;
            }
        }

        return -1;
    }

    bool CContainerUI::SetItemIndex(CControlUI* pControl, int iIndex)
    {
        for (int it = 0; it < m_items.GetSize(); it++) {
            if (static_cast<CControlUI*>(m_items[it]) == pControl) {
                NeedUpdate();
                m_items.Remove(it);
                return m_items.InsertAt(iIndex, pControl);
            }
        }

        return false;
    }

    int CContainerUI::GetCount() const
    {
        return m_items.GetSize();
    }

    bool CContainerUI::Add(CControlUI* pControl)
    {
        return Add(pControl, true);
    }

    bool CContainerUI::Add(CControlUI* pControl, bool bRefresh)
    {
        if (pControl == NULL) return false;

        if (m_pManager != NULL) m_pManager->InitControls(pControl, this);
        if (IsVisible()) NeedUpdate(bRefresh);
        else pControl->SetInternVisible(false);
        return m_items.Add(pControl);
    }

    bool CContainerUI::AddAt(CControlUI* pControl, int iIndex)
    {
        if (pControl == NULL) return false;

        if (m_pManager != NULL) m_pManager->InitControls(pControl, this);
        if (IsVisible()) NeedUpdate();
        else pControl->SetInternVisible(false);
        return m_items.InsertAt(iIndex, pControl);
    }

    bool CContainerUI::Remove(CControlUI* pControl)
    {
        if (pControl == NULL) return false;

        for (int it = 0; it < m_items.GetSize(); it++) {
            if (static_cast<CControlUI*>(m_items[it]) == pControl) {
                NeedUpdate();
                if (m_bAutoDestroy) {
                    if (m_bDelayedDestroy && m_pManager) m_pManager->AddDelayedCleanup(pControl);
                    else delete pControl;
                }
                return m_items.Remove(it);
            }
        }
        return false;
    }

    bool CContainerUI::RemoveAt(int iIndex)
    {
        CControlUI* pControl = GetItemAt(iIndex);
        if (pControl != NULL) {
            return CContainerUI::Remove(pControl);
        }

        return false;
    }

    void CContainerUI::RemoveAll()
    {
        for (int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++) {
            if (m_bDelayedDestroy && m_pManager) m_pManager->AddDelayedCleanup(static_cast<CControlUI*>(m_items[it]));
            else delete static_cast<CControlUI*>(m_items[it]);
        }
        m_items.Empty();
        NeedUpdate();
    }

    bool CContainerUI::IsAutoDestroy() const
    {
        return m_bAutoDestroy;
    }

    void CContainerUI::SetAutoDestroy(bool bAuto)
    {
        m_bAutoDestroy = bAuto;
    }

    bool CContainerUI::IsDelayedDestroy() const
    {
        return m_bDelayedDestroy;
    }

    void CContainerUI::SetDelayedDestroy(bool bDelayed)
    {
        m_bDelayedDestroy = bDelayed;
    }

    RECT CContainerUI::GetInset() const
    {
        return m_rcInset;
    }

    void CContainerUI::SetInset(RECT rcInset)
    {
        m_rcInset = rcInset;
        NeedUpdate();
    }

    int CContainerUI::GetChildPadding() const
    {
        return m_iChildPadding;
    }

    void CContainerUI::SetChildPadding(int iPadding)
    {
        m_iChildPadding = iPadding;
        NeedUpdate();
    }

    bool CContainerUI::IsMouseChildEnabled() const
    {
        return m_bMouseChildEnabled;
    }

    void CContainerUI::SetMouseChildEnabled(bool bEnable)
    {
        m_bMouseChildEnabled = bEnable;
    }

    void CContainerUI::SetVisible(bool bVisible)
    {
        if (m_bVisible == bVisible) return;
        CControlUI::SetVisible(bVisible);
        for (int it = 0; it < m_items.GetSize(); it++) {
            static_cast<CControlUI*>(m_items[it])->SetInternVisible(IsVisible());
        }
    }

    // 逻辑上，对于Container控件不公开此方法
    // 调用此方法的结果是，内部子控件隐藏，控件本身依然显示，背景等效果存在
    void CContainerUI::SetInternVisible(bool bVisible)
    {
        CControlUI::SetInternVisible(bVisible);
        if (m_items.IsEmpty()) return;
        for (int it = 0; it < m_items.GetSize(); it++) {
            // 控制子控件显示状态
            // InternVisible状态应由子控件自己控制
            static_cast<CControlUI*>(m_items[it])->SetInternVisible(IsVisible());
        }
    }

    void CContainerUI::SetMouseEnabled(bool bEnabled)
    {
        if (m_pVerticalScrollBar != NULL) m_pVerticalScrollBar->SetMouseEnabled(bEnabled);
        if (m_pHorizontalScrollBar != NULL) m_pHorizontalScrollBar->SetMouseEnabled(bEnabled);
        CControlUI::SetMouseEnabled(bEnabled);
    }

    void CContainerUI::DoEvent(TEventUI& event)
    {
        if (HandleDrag(event))
        {
            return;
        }

        if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
            if (m_pParent != NULL) m_pParent->DoEvent(event);
            else CControlUI::DoEvent(event);
            return;
        }

        if (event.Type == UIEVENT_SETFOCUS)
        {
            m_bFocused = true;
            return;
        }
        if (event.Type == UIEVENT_KILLFOCUS)
        {
            m_bFocused = false;
            return;
        }
        if (m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() && m_pVerticalScrollBar->IsEnabled())
        {
            if (event.Type == UIEVENT_KEYDOWN)
            {
                switch (event.chKey) {
                case VK_DOWN:
                    LineDown();
                    return;
                case VK_UP:
                    LineUp();
                    return;
                case VK_NEXT:
                    PageDown();
                    return;
                case VK_PRIOR:
                    PageUp();
                    return;
                case VK_HOME:
                    HomeUp();
                    return;
                case VK_END:
                    EndDown();
                    return;
                }
            }
            else if (event.Type == UIEVENT_SCROLLWHEEL)
            {
                switch (LOWORD(event.wParam)) {
                case SB_LINEUP:
                    LineUp();
                    return;
                case SB_LINEDOWN:
                    LineDown();
                    return;
                }
            }
        }
        else if (m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() && m_pHorizontalScrollBar->IsEnabled()) {
            if (event.Type == UIEVENT_KEYDOWN)
            {
                switch (event.chKey) {
                case VK_DOWN:
                    LineRight();
                    return;
                case VK_UP:
                    LineLeft();
                    return;
                case VK_NEXT:
                    PageRight();
                    return;
                case VK_PRIOR:
                    PageLeft();
                    return;
                case VK_HOME:
                    HomeLeft();
                    return;
                case VK_END:
                    EndRight();
                    return;
                }
            }
            else if (event.Type == UIEVENT_SCROLLWHEEL)
            {
                switch (LOWORD(event.wParam)) {
                case SB_LINEUP:
                    LineLeft();
                    return;
                case SB_LINEDOWN:
                    LineRight();
                    return;
                }
            }
        }
        CControlUI::DoEvent(event);
    }

    SIZE CContainerUI::GetScrollPos() const
    {
        SIZE sz = { 0, 0 };
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) sz.cy = m_pVerticalScrollBar->GetScrollPos();
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) sz.cx = m_pHorizontalScrollBar->GetScrollPos();
        return sz;
    }

    SIZE CContainerUI::GetScrollRange() const
    {
        SIZE sz = { 0, 0 };
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) sz.cy = m_pVerticalScrollBar->GetScrollRange();
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) sz.cx = m_pHorizontalScrollBar->GetScrollRange();
        return sz;
    }

    void CContainerUI::SetScrollPos(SIZE szPos)
    {
        int cx = 0;
        int cy = 0;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
            int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
            m_pVerticalScrollBar->SetScrollPos(szPos.cy);
            cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
        }

        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
            int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
            m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
            cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
        }

        if (cx == 0 && cy == 0) return;

        RECT rcPos;
        for (int it2 = 0; it2 < m_items.GetSize(); it2++) {
            CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
            if (!pControl->IsVisible()) continue;
            if (pControl->IsFloat()) continue;

            rcPos = pControl->GetPos();
            rcPos.left -= cx;
            rcPos.right -= cx;
            rcPos.top -= cy;
            rcPos.bottom -= cy;
            pControl->SetPos(rcPos);
        }

        Invalidate();
    }

    void CContainerUI::LineUp()
    {
        int cyLine = 8;
        if (m_pManager) cyLine = m_pManager->GetDefaultFontInfo()->tm.tmHeight + 8;

        SIZE sz = GetScrollPos();
        sz.cy -= cyLine;
        SetScrollPos(sz);
    }

    void CContainerUI::LineDown()
    {
        int cyLine = 8;
        if (m_pManager) cyLine = m_pManager->GetDefaultFontInfo()->tm.tmHeight + 8;

        SIZE sz = GetScrollPos();
        sz.cy += cyLine;
        SetScrollPos(sz);
    }

    void CContainerUI::PageUp()
    {
        SIZE sz = GetScrollPos();
        int iOffset = m_rcItem.bottom - m_rcItem.top - m_rcInset.top - m_rcInset.bottom;
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
        sz.cy -= iOffset;
        SetScrollPos(sz);
    }

    void CContainerUI::PageDown()
    {
        SIZE sz = GetScrollPos();
        int iOffset = m_rcItem.bottom - m_rcItem.top - m_rcInset.top - m_rcInset.bottom;
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
        sz.cy += iOffset;
        SIZE range = GetScrollRange();
        if (sz.cy >= range.cy)
        {
            return;
        }
        SetScrollPos(sz);
    }

    void CContainerUI::HomeUp()
    {
        SIZE sz = GetScrollPos();
        sz.cy = 0;
        SetScrollPos(sz);
    }

    void CContainerUI::EndDown()
    {
        SIZE sz = GetScrollPos();
        sz.cy = GetScrollRange().cy;
        SetScrollPos(sz);
    }

    void CContainerUI::LineLeft()
    {
        SIZE sz = GetScrollPos();
        sz.cx -= 8;
        SetScrollPos(sz);
    }

    void CContainerUI::LineRight()
    {
        SIZE sz = GetScrollPos();
        sz.cx += 8;
        SetScrollPos(sz);
    }

    void CContainerUI::PageLeft()
    {
        SIZE sz = GetScrollPos();
        int iOffset = m_rcItem.right - m_rcItem.left - m_rcInset.left - m_rcInset.right;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) iOffset -= m_pVerticalScrollBar->GetFixedWidth();
        sz.cx -= iOffset;
        SetScrollPos(sz);
    }

    void CContainerUI::PageRight()
    {
        SIZE sz = GetScrollPos();
        int iOffset = m_rcItem.right - m_rcItem.left - m_rcInset.left - m_rcInset.right;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) iOffset -= m_pVerticalScrollBar->GetFixedWidth();
        sz.cx += iOffset;
        SetScrollPos(sz);
    }

    void CContainerUI::HomeLeft()
    {
        SIZE sz = GetScrollPos();
        sz.cx = 0;
        SetScrollPos(sz);
    }

    void CContainerUI::EndRight()
    {
        SIZE sz = GetScrollPos();
        sz.cx = GetScrollRange().cx;
        SetScrollPos(sz);
    }

    void CContainerUI::EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal)
    {
        if (bEnableVertical && !m_pVerticalScrollBar) {
            m_pVerticalScrollBar = new CScrollBarUI;
            m_pVerticalScrollBar->SetOwner(this);
            m_pVerticalScrollBar->SetManager(m_pManager, NULL, false);
            if (m_pManager) {
                LPCTSTR pDefaultAttributes = m_pManager->GetDefaultAttributeList(_T("VScrollBar"));
                if (pDefaultAttributes) {
                    m_pVerticalScrollBar->ApplyAttributeList(pDefaultAttributes);
                }
            }
        }
        else if (!bEnableVertical && m_pVerticalScrollBar) {
            delete m_pVerticalScrollBar;
            m_pVerticalScrollBar = NULL;
        }

        if (bEnableHorizontal && !m_pHorizontalScrollBar) {
            m_pHorizontalScrollBar = new CScrollBarUI;
            m_pHorizontalScrollBar->SetHorizontal(true);
            m_pHorizontalScrollBar->SetOwner(this);
            m_pHorizontalScrollBar->SetManager(m_pManager, NULL, false);
            if (m_pManager) {
                LPCTSTR pDefaultAttributes = m_pManager->GetDefaultAttributeList(_T("HScrollBar"));
                if (pDefaultAttributes) {
                    m_pHorizontalScrollBar->ApplyAttributeList(pDefaultAttributes);
                }
            }
        }
        else if (!bEnableHorizontal && m_pHorizontalScrollBar) {
            delete m_pHorizontalScrollBar;
            m_pHorizontalScrollBar = NULL;
        }

        NeedUpdate();
    }

    CScrollBarUI* CContainerUI::GetVerticalScrollBar() const
    {
        return m_pVerticalScrollBar;
    }

    CScrollBarUI* CContainerUI::GetHorizontalScrollBar() const
    {
        return m_pHorizontalScrollBar;
    }

    int CContainerUI::FindSelectable(int iIndex, bool bForward /*= true*/) const
    {
        // NOTE: This is actually a helper-function for the list/combo/ect controls
        //       that allow them to find the next enabled/available selectable item
        if (GetCount() == 0) return -1;
        iIndex = CLAMP(iIndex, 0, GetCount() - 1);
        if (bForward) {
            for (int i = iIndex; i < GetCount(); i++) {
                if (GetItemAt(i)->GetInterface(_T("ListItem")) != NULL
                    && GetItemAt(i)->IsVisible()
                    && GetItemAt(i)->IsEnabled()) return i;
            }
            return -1;
        }
        else {
            for (int i = iIndex; i >= 0; --i) {
                if (GetItemAt(i)->GetInterface(_T("ListItem")) != NULL
                    && GetItemAt(i)->IsVisible()
                    && GetItemAt(i)->IsEnabled()) return i;
            }
            return FindSelectable(0, true);
        }
    }

    void CContainerUI::SetPos(RECT rc)
    {
        CControlUI::SetPos(rc);

        if (m_items.IsEmpty()) return;
        rc = m_rcItem;
        
        // Adjust for inset
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };

        int iPosY = rc.top;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
            iPosY -= m_pVerticalScrollBar->GetScrollPos();
        }
        int iPosX = rc.left;
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
            iPosX -= m_pHorizontalScrollBar->GetScrollPos();
        }

        int cyNeeded = 0;
        int cxNeeded = 0;

        for (int it = 0; it < m_items.GetSize(); ++it) {
            CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
            if (!pControl->IsVisible()) continue;
            if (pControl->IsFloat()) {
                SetFloatPos(it);
                continue;
            }

            SIZE sz = pControl->EstimateSize(szAvailable);

            SIZE szXY = pControl->GetFixedXY();
            if ((sz.cx + szXY.cx) > cxNeeded) {
                cxNeeded = sz.cx + szXY.cx;
            }

            if ((sz.cy + szXY.cy) > cyNeeded) {
                cyNeeded = sz.cy + szXY.cy;
            }

            RECT rcCtrl = { szXY.cx, szXY.cy, 0, 0};

            rcCtrl.left += iPosX;
            rcCtrl.top += iPosY;
            
            if (sz.cx == 0) {
                sz.cx = szAvailable.cx;
            }
            if (sz.cy == 0) {
                sz.cy = szAvailable.cy;
            }
            rcCtrl.right = rcCtrl.left + sz.cx;
            rcCtrl.bottom = rcCtrl.top + sz.cy;

            pControl->SetPos(rcCtrl);
        }

        cxNeeded += m_rcInset.left + m_rcInset.right;
        cyNeeded += m_rcInset.top + m_rcInset.bottom;
        cxNeeded = MAX(cxNeeded, 0);
        cyNeeded = MAX(cyNeeded, 0);

        ProcessScrollBar(rc, cxNeeded, cyNeeded);
    }

    void CContainerUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if (_tcscmp(pstrName, _T("inset")) == 0) {
            RECT rcInset = { 0 };
            LPTSTR pstr = NULL;
            rcInset.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
            rcInset.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
            rcInset.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
            rcInset.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
            SetInset(rcInset);
        }
        else if (_tcscmp(pstrName, _T("mousechild")) == 0) SetMouseChildEnabled(_tcscmp(pstrValue, _T("true")) == 0);
        else if (_tcscmp(pstrName, _T("vscrollbar")) == 0) {
            EnableScrollBar(_tcscmp(pstrValue, _T("true")) == 0, GetHorizontalScrollBar() != NULL);
        }
        else if (_tcscmp(pstrName, _T("vscrollbarstyle")) == 0) {
            EnableScrollBar(true, GetHorizontalScrollBar() != NULL);
            if (GetVerticalScrollBar()) GetVerticalScrollBar()->ApplyAttributeList(pstrValue);
        }
        else if (_tcscmp(pstrName, _T("hscrollbar")) == 0) {
            EnableScrollBar(GetVerticalScrollBar() != NULL, _tcscmp(pstrValue, _T("true")) == 0);
        }
        else if (_tcscmp(pstrName, _T("hscrollbarstyle")) == 0) {
            EnableScrollBar(GetVerticalScrollBar() != NULL, true);
            if (GetHorizontalScrollBar()) GetHorizontalScrollBar()->ApplyAttributeList(pstrValue);
        }
        else if (_tcscmp(pstrName, _T("childpadding")) == 0) SetChildPadding(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("dragable")) == 0) SetDragable(_tcscmp(pstrValue, _T("true")) == 0);
        else if (_tcscmp(pstrName, _T("sepsize")) == 0) SetSeparatorSize(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("dragdir")) == 0) SetDragDirection(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("sepimm")) == 0) SetSepImmMode(_tcscmp(pstrValue, _T("true")) == 0);
        else CControlUI::SetAttribute(pstrName, pstrValue);
    }

    void CContainerUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
    {
        for (int it = 0; it < m_items.GetSize(); it++) {
            static_cast<CControlUI*>(m_items[it])->SetManager(pManager, this, bInit);
        }

        if (m_pVerticalScrollBar != NULL) m_pVerticalScrollBar->SetManager(pManager, this, bInit);
        if (m_pHorizontalScrollBar != NULL) m_pHorizontalScrollBar->SetManager(pManager, this, bInit);
        CControlUI::SetManager(pManager, pParent, bInit);
    }

    CControlUI* CContainerUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
    {
        // Check if this guy is valid
        if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) return NULL;
        if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) return NULL;
        if ((uFlags & UIFIND_HITTEST) != 0) {
            if (!::PtInRect(&m_rcItem, *(static_cast<LPPOINT>(pData)))) return NULL;
            if (!m_bMouseChildEnabled) {
                CControlUI* pResult = NULL;
                if (m_pVerticalScrollBar != NULL) pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
                if (pResult == NULL && m_pHorizontalScrollBar != NULL) pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
                if (pResult == NULL) pResult = CControlUI::FindControl(Proc, pData, uFlags);
                return pResult;
            }
        }

        CControlUI* pResult = NULL;
        if (m_pVerticalScrollBar != NULL) pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
        if (pResult == NULL && m_pHorizontalScrollBar != NULL) pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
        if (pResult != NULL) return pResult;

        if ((uFlags & UIFIND_ME_FIRST) != 0) {
            CControlUI* pControl = CControlUI::FindControl(Proc, pData, uFlags);
            if (pControl != NULL) return pControl;
        }
        RECT rc = m_rcItem;
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
        if ((uFlags & UIFIND_TOP_FIRST) != 0) {
            for (int it = m_items.GetSize() - 1; it >= 0; it--) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it])->FindControl(Proc, pData, uFlags);
                if (pControl != NULL) {
                    if ((uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
                        continue;
                    else
                        return pControl;
                }
            }
        }
        else {
            for (int it = 0; it < m_items.GetSize(); it++) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it])->FindControl(Proc, pData, uFlags);
                if (pControl != NULL) {
                    if ((uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
                        continue;
                    else
                        return pControl;
                }
            }
        }

        if (pResult == NULL && (uFlags & UIFIND_ME_FIRST) == 0) pResult = CControlUI::FindControl(Proc, pData, uFlags);
        return pResult;
    }

    void CContainerUI::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        RECT rcTemp = { 0 };
        if (!::IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) return;

        CRenderClip clip;
        CRenderClip::GenerateClip(hDC, rcTemp, clip);
        CControlUI::DoPaint(hDC, rcPaint);

        if (m_items.GetSize() > 0) {
            RECT rc = m_rcItem;
            rc.left += m_rcInset.left;
            rc.top += m_rcInset.top;
            rc.right -= m_rcInset.right;
            rc.bottom -= m_rcInset.bottom;
            if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
            if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

            if (!::IntersectRect(&rcTemp, &rcPaint, &rc)) {
                for (int it = 0; it < m_items.GetSize(); it++) {
                    CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                    if (!pControl->IsVisible()) continue;
                    if (!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos())) continue;
                    if (pControl->IsFloat()) {
                        if (!::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos())) continue;
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
            else {
                CRenderClip childClip;
                CRenderClip::GenerateClip(hDC, rcTemp, childClip);
                for (int it = 0; it < m_items.GetSize(); it++) {
                    CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                    if (!pControl->IsVisible()) continue;
                    if (!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos())) continue;
                    if (pControl->IsFloat()) {
                        if (!::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos())) continue;
                        CRenderClip::UseOldClipBegin(hDC, childClip);
                        pControl->DoPaint(hDC, rcPaint);
                        CRenderClip::UseOldClipEnd(hDC, childClip);
                    }
                    else {
                        if (!::IntersectRect(&rcTemp, &rc, &pControl->GetPos())) continue;
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
        }

        if (m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible()) {
            if (::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos())) {
                m_pVerticalScrollBar->DoPaint(hDC, rcPaint);
            }
        }

        if (m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible()) {
            if (::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos())) {
                m_pHorizontalScrollBar->DoPaint(hDC, rcPaint);
            }
        }
    }

    void CContainerUI::SetFloatPos(int iIndex)
    {
        // 因为CControlUI::SetPos对float的操作影响，这里不能对float组件添加滚动条的影响
        if (iIndex < 0 || iIndex >= m_items.GetSize()) return;

        CControlUI* pControl = static_cast<CControlUI*>(m_items[iIndex]);

        if (!pControl->IsVisible()) return;
        if (!pControl->IsFloat()) return;

        SIZE szXY = pControl->GetFixedXY();
        SIZE sz = { pControl->GetFixedWidth(), pControl->GetFixedHeight() };
        RECT rcCtrl = { 0 };
        if (szXY.cx >= 0) {
            rcCtrl.left = m_rcItem.left + szXY.cx;
            rcCtrl.right = m_rcItem.left + szXY.cx + sz.cx;
        }
        else {
            rcCtrl.left = m_rcItem.right + szXY.cx - sz.cx;
            rcCtrl.right = m_rcItem.right + szXY.cx;
        }
        if (szXY.cy >= 0) {
            rcCtrl.top = m_rcItem.top + szXY.cy;
            rcCtrl.bottom = m_rcItem.top + szXY.cy + sz.cy;
        }
        else {
            rcCtrl.top = m_rcItem.bottom + szXY.cy - sz.cy;
            rcCtrl.bottom = m_rcItem.bottom + szXY.cy;
        }
        if (pControl->IsRelativePos())
        {
            TRelativePosUI tRelativePos = pControl->GetRelativePos();
            SIZE szParent = { m_rcItem.right - m_rcItem.left,m_rcItem.bottom - m_rcItem.top };
            if (tRelativePos.szParent.cx != 0)
            {
                int nIncrementX = szParent.cx - tRelativePos.szParent.cx;
                int nIncrementY = szParent.cy - tRelativePos.szParent.cy;
                rcCtrl.left += (nIncrementX*tRelativePos.nMoveXPercent / 100);
                rcCtrl.top += (nIncrementY*tRelativePos.nMoveYPercent / 100);
                rcCtrl.right = rcCtrl.left + sz.cx + (nIncrementX*tRelativePos.nZoomXPercent / 100);
                rcCtrl.bottom = rcCtrl.top + sz.cy + (nIncrementY*tRelativePos.nZoomYPercent / 100);
            }
            pControl->SetRelativeParentSize(szParent);
        }
        pControl->SetPos(rcCtrl);
    }

    void CContainerUI::ProcessScrollBar(RECT rc, int cxRequired, int cyRequired)
    {
        if (NULL != m_pHorizontalScrollBar)
        {
            RECT hrcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight() };
            ProcessScroll(m_pHorizontalScrollBar, cxRequired, rc.right - rc.left, hrcScrollBarPos);
        }

        if (NULL != m_pVerticalScrollBar)
        {
            RECT vrcScrollBarPos = { rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom };
            ProcessScroll(m_pVerticalScrollBar, cyRequired, rc.bottom - rc.top, vrcScrollBarPos);
        }
    }

    void CContainerUI::ProcessScroll(CScrollBarUI* scrollBar, int iRequired, int  iSize, const RECT& rcScrollBarPos)
    {
        if (scrollBar == NULL) return;

        if (iRequired > iSize && !scrollBar->IsVisible()) {
            scrollBar->SetVisible(true);
            scrollBar->SetScrollRange(iRequired - iSize);
            scrollBar->SetScrollPos(0);
            m_bScrollProcess = true;
            SetPos(m_rcItem);
            m_bScrollProcess = false;
            return;
        }
        // No scrollbar required
        if (!scrollBar->IsVisible()) return;

        // Scroll not needed anymore?
        int iScroll = iRequired - iSize;
        if (iScroll <= 0 && !m_bScrollProcess) {
            scrollBar->SetVisible(false);
            scrollBar->SetScrollPos(0);
            scrollBar->SetScrollRange(0);
            SetPos(m_rcItem);
        }
        else
        {
            scrollBar->SetPos(rcScrollBarPos);

            if (scrollBar->GetScrollRange() != iScroll) {
                int iScrollPos = scrollBar->GetScrollPos();
                scrollBar->SetScrollRange(::abs(iScroll));
                if (scrollBar->GetScrollRange() == 0) {
                    scrollBar->SetVisible(false);
                    scrollBar->SetScrollPos(0);
                }
                if (iScrollPos > scrollBar->GetScrollPos()) {
                    SetPos(m_rcItem);
                }
            }
        }
    }

    bool CContainerUI::SetSubControlText(LPCTSTR pstrSubControlName, LPCTSTR pstrText)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl != NULL)
        {
            pSubControl->SetText(pstrText);
            return TRUE;
        }
        else
            return FALSE;
    }

    bool CContainerUI::SetSubControlFixedHeight(LPCTSTR pstrSubControlName, int cy)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl != NULL)
        {
            pSubControl->SetFixedHeight(cy);
            return TRUE;
        }
        else
            return FALSE;
    }

    bool CContainerUI::SetSubControlFixedWdith(LPCTSTR pstrSubControlName, int cx)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl != NULL)
        {
            pSubControl->SetFixedWidth(cx);
            return TRUE;
        }
        else
            return FALSE;
    }

    bool CContainerUI::SetSubControlUserData(LPCTSTR pstrSubControlName, LPCTSTR pstrText)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl != NULL)
        {
            pSubControl->SetUserData(pstrText);
            return TRUE;
        }
        else
            return FALSE;
    }

    DuiLib::CDuiString CContainerUI::GetSubControlText(LPCTSTR pstrSubControlName)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl == NULL)
            return _T("");
        else
            return pSubControl->GetText();
    }

    int CContainerUI::GetSubControlFixedHeight(LPCTSTR pstrSubControlName)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl == NULL)
            return -1;
        else
            return pSubControl->GetFixedHeight();
    }

    int CContainerUI::GetSubControlFixedWdith(LPCTSTR pstrSubControlName)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl == NULL)
            return -1;
        else
            return pSubControl->GetFixedWidth();
    }

    const CDuiString CContainerUI::GetSubControlUserData(LPCTSTR pstrSubControlName)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = this->FindSubControl(pstrSubControlName);
        if (pSubControl == NULL)
            return _T("");
        else
            return pSubControl->GetUserData();
    }

    CControlUI* CContainerUI::FindSubControl(LPCTSTR pstrSubControlName)
    {
        CControlUI* pSubControl = NULL;
        pSubControl = static_cast<CControlUI*>(GetManager()->FindSubControlByName(this, pstrSubControlName));
        return pSubControl;
    }

    bool CContainerUI::IsDragable() const
    {
        return m_bDragable;
    }

    void CContainerUI::SetDragable(bool bDragable)
    {
        m_bDragable = bDragable;
        if (!m_bDragable) m_uButtonState &= ~UISTATE_CAPTURED;
    }

    void CContainerUI::SetEnabled(bool bEnable)
    {
        CControlUI::SetEnabled(bEnable);
        if (!IsEnabled()) {
            m_uButtonState = 0;
        }
    }

    int CContainerUI::GetSeparatorSize() const
    {
        return m_iSepSize;
    }

    void CContainerUI::SetSeparatorSize(int iSepSize)
    {
        m_iSepSize = iSepSize;
        if (m_iSepSize <= 0)
        {
            m_uButtonState = 0;
        }
    }

    int CContainerUI::GetDragDirection() const
    {
        return m_iDragDir;
    }

    void CContainerUI::SetDragDirection(int iDragDir)
    {
        m_iDragDir = iDragDir;
    }

    void CContainerUI::SetSepImmMode(bool bImmediately)
    {
        if (m_bImmMode == bImmediately) return;
        if ((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode && m_pManager != NULL) {
            m_pManager->RemovePostPaint(this);
        }

        m_bImmMode = bImmediately;
    }

    bool CContainerUI::IsSepImmMode() const
    {
        return m_bImmMode;
    }

    UINT CContainerUI::GetControlFlags() const
    {
        if (IsEnabled() && m_bDragable) return UIFLAG_SETCURSOR;
        else return 0;
    }

    void CContainerUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode) {
            RECT& rcSeparator = GetThumbRect(true);
            CRenderEngine::DrawColor(hDC, rcSeparator, 0xAA000000);
        }
    }

    RECT CContainerUI::GetThumbRect(bool bUseNew) const
    {
        RECT rc;
        if ((m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew)
            rc = m_rcNewPos;
        else
            rc = m_rcItem;

        if (eDragTop == m_eCurrDargDir || eDragLeftTop == m_eCurrDargDir || eDragRightTop == m_eCurrDargDir)
        {
            return CDuiRect(rc.left, rc.top, rc.right, rc.top + m_iSepSize);
        }
        else if (eDragDown == m_eCurrDargDir || eDragLeftDown == m_eCurrDargDir || eDragRightDown == m_eCurrDargDir)
        {
            return CDuiRect(rc.left, rc.bottom - m_iSepSize, rc.right, rc.bottom);
        }
        else if (eDragLeft == m_eCurrDargDir)
        {
            return CDuiRect(rc.left, rc.top, rc.left + m_iSepSize, rc.bottom);
        }

        return CDuiRect(rc.right - m_iSepSize, rc.top, rc.right, rc.bottom);
    }

    bool CContainerUI::HandleDrag(TEventUI& event)
    {
        if (!m_bDragable) return false;

        if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
        {
            GetThumbRects();
			int len = m_lstThumb.GetSize();
            for (int i = 0; i < len; ++i)
            {
				ThumbInfo* info = (ThumbInfo*)m_lstThumb.GetAt(i);
                RECT& rcSeparator = info->m_rc;
                if (::PtInRect(&rcSeparator, event.ptMouse))
                {
                    m_uButtonState |= UISTATE_CAPTURED;
                    m_ptLastMouse = event.ptMouse;
                    m_rcNewPos = m_rcItem;
                    m_eCurrDargDir = info->m_eDragDir;
                    if (!m_bImmMode && m_pManager) m_pManager->AddPostPaint(this);
                    return true;
                }
            }
        }
        else if (event.Type == UIEVENT_BUTTONUP)
        {
            if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
                m_uButtonState &= ~UISTATE_CAPTURED;
                m_rcItem = m_rcNewPos;
                if (!m_bImmMode && m_pManager) m_pManager->RemovePostPaint(this);
                NeedParentUpdate();
				ClearThumbList();
                if (NULL != m_pDragCb)
                    m_pDragCb(event.ptMouse, this, m_rcItem, m_pDragCbParam);
                return true;
            }
        }
        else if (event.Type == UIEVENT_MOUSEMOVE)
        {
            if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
                LONG cx = event.ptMouse.x - m_ptLastMouse.x;
				LONG cy = event.ptMouse.y - m_ptLastMouse.y;
                m_ptLastMouse = event.ptMouse;
                RECT rc = m_rcNewPos;

                if (eDragTop == m_eCurrDargDir || eDragLeftTop == m_eCurrDargDir || eDragRightTop == m_eCurrDargDir)
                {
					if (cy > 0 && event.ptMouse.y < m_rcNewPos.top) return true;
					if (cy < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepSize) return true;
					rc.top += cy;
					if (rc.bottom - rc.top <= GetMinHeight()) {
						if (m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight()) return true;
						rc.top = rc.bottom - GetMinHeight();
					}
					if (rc.bottom - rc.top >= GetMaxHeight()) {
						if (m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight()) return true;
						rc.top = rc.bottom - GetMaxHeight();
					}
                }
                else if (eDragDown == m_eCurrDargDir || eDragLeftDown == m_eCurrDargDir || eDragRightDown == m_eCurrDargDir)
                {
					if (cy > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepSize) return true;
					if (cy < 0 && event.ptMouse.y > m_rcNewPos.bottom) return true;
					rc.bottom += cy;
					if (rc.bottom - rc.top <= GetMinHeight()) {
						if (m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight()) return true;
						rc.bottom = rc.top + GetMinHeight();
					}
					if (rc.bottom - rc.top >= GetMaxHeight()) {
						if (m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight()) return true;
						rc.bottom = rc.top + GetMaxHeight();
					}
                }
                else if (eDragLeft == m_eCurrDargDir)
                {
                    if (cx > 0 && event.ptMouse.x < m_rcNewPos.left) return true;
                    if (cx < 0 && event.ptMouse.x > m_rcNewPos.left - m_iSepSize) return true;
                    rc.left += cx;
                    if (rc.right - rc.left <= GetMinWidth()) {
                        if (m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth()) return true;
                        rc.left = rc.right - GetMinWidth();
                    }
                    if (rc.right - rc.left >= GetMaxWidth()) {
                        if (m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth()) return true;
                        rc.left = rc.right - GetMaxWidth();
                    }
                } 
                else
                {
                    if (cx > 0 && event.ptMouse.x < m_rcNewPos.right - m_iSepSize) return true;
                    if (cx < 0 && event.ptMouse.x > m_rcNewPos.right) return true;
                    rc.right += cx;
                    if (rc.right - rc.left <= GetMinWidth()) {
                        if (m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth()) return true;
                        rc.right = rc.left + GetMinWidth();
                    }
                    if (rc.right - rc.left >= GetMaxWidth()) {
                        if (m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth()) return true;
                        rc.right = rc.left + GetMaxWidth();
                    }
                }

                CDuiRect rcInvalidate = GetThumbRect(true);
                m_rcNewPos = rc;

				if (eDragTop == m_eCurrDargDir || eDragDown == m_eCurrDargDir)
				{
					m_cxyFixed.cy = m_rcNewPos.bottom - m_rcNewPos.top;
				} else if (eDragLeft == m_eCurrDargDir || eDragRight == m_eCurrDargDir)
				{
					m_cxyFixed.cx = m_rcNewPos.right - m_rcNewPos.left;
				}
				else
				{
					m_cxyFixed.cx = m_rcNewPos.right - m_rcNewPos.left;
					m_cxyFixed.cy = m_rcNewPos.bottom - m_rcNewPos.top;
				}

                if (m_bImmMode) {
                    m_rcItem = m_rcNewPos;
                    NeedParentUpdate();
                    if (NULL != m_pDragCb)
                        m_pDragCb(event.ptMouse,this, m_rcItem, m_pDragCbParam);
                }
                else {
                    rcInvalidate.Join(GetThumbRect(true));
                    rcInvalidate.Join(GetThumbRect(false));
                    if (m_pManager) m_pManager->Invalidate(rcInvalidate);
                }
                return true;
            }
        }
        else if (event.Type == UIEVENT_SETCURSOR)
        {
            if (IsEnabled())
            {
                GetThumbRects();
				int len = m_lstThumb.GetSize();
                for (int i = 0; i < len;  ++i)
                {
					ThumbInfo* info = (ThumbInfo*)m_lstThumb.GetAt(i);
                    RECT& rcSeparator = info->m_rc;
                    if (::PtInRect(&rcSeparator, event.ptMouse))
                    {
                        if (info->m_eDragDir == eDragTop || info->m_eDragDir == eDragDown)
                            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
                        else if (info->m_eDragDir == eDragLeft || info->m_eDragDir == eDragRight)
                            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
                        else if (info->m_eDragDir == eDragLeftTop || info->m_eDragDir == eDragRightDown)
                            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
                        else if (info->m_eDragDir == eDragLeftDown || info->m_eDragDir == eDragRightTop)
                            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
                        return true;
                    }
                }
            }
        }
        return false;
    }

	void CContainerUI::ClearThumbList()
	{
		int len = m_lstThumb.GetSize();
		for (int i = len; i > 0; --i)
		{
			ThumbInfo* info = (ThumbInfo*)m_lstThumb.GetAt(i-1);
			delete info;
			m_lstThumb.Remove(i - 1);
		}		
	}

    void CContainerUI::GetThumbRects()
    {
		ClearThumbList();
        if ((m_iDragDir&eDragTop) != 0)
        {
            if ((m_iDragDir&eDragLeftTop) != 0 && (m_iDragDir&eDragRightTop) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeftTop, m_rcItem.left, m_rcItem.top, m_rcItem.left + m_iSepSize, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragTop, m_rcItem.left + m_iSepSize, m_rcItem.top, m_rcItem.right - m_iSepSize, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragRightTop, m_rcItem.right - m_iSepSize, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_iSepSize));
            }
            else if ((m_iDragDir&eDragLeftTop) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeftTop, m_rcItem.left, m_rcItem.top, m_rcItem.left + m_iSepSize, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragTop, m_rcItem.left + m_iSepSize, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_iSepSize));
            }
            else if ((m_iDragDir&eDragRightTop) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragTop, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_iSepSize, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragRightTop, m_rcItem.right - m_iSepSize, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_iSepSize));
            }
            else
            {
                m_lstThumb.Add(new ThumbInfo(eDragTop, m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_iSepSize));
            }
        }

        if ((m_iDragDir&eDragDown) != 0)
        {
            if ((m_iDragDir&eDragLeftDown) != 0 && (m_iDragDir&eDragRightDown) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeftDown, m_rcItem.left, m_rcItem.bottom - m_iSepSize, m_rcItem.left + m_iSepSize, m_rcItem.bottom));
                m_lstThumb.Add(new ThumbInfo(eDragDown, m_rcItem.left + m_iSepSize, m_rcItem.bottom - m_iSepSize, m_rcItem.right - m_iSepSize, m_rcItem.bottom));
                m_lstThumb.Add(new ThumbInfo(eDragRightDown, m_rcItem.right - m_iSepSize, m_rcItem.bottom- m_iSepSize, m_rcItem.right, m_rcItem.bottom));
            }
            else if ((m_iDragDir&eDragLeftDown) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeftDown, m_rcItem.left, m_rcItem.bottom - m_iSepSize, m_rcItem.left + m_iSepSize, m_rcItem.bottom));
                m_lstThumb.Add(new ThumbInfo(eDragDown, m_rcItem.left + m_iSepSize, m_rcItem.bottom - m_iSepSize, m_rcItem.right, m_rcItem.bottom));
            }
            else if ((m_iDragDir&eDragRightDown) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragDown, m_rcItem.left, m_rcItem.bottom - m_iSepSize, m_rcItem.right - m_iSepSize, m_rcItem.bottom));
                m_lstThumb.Add(new ThumbInfo(eDragRightDown, m_rcItem.right - m_iSepSize, m_rcItem.bottom - m_iSepSize, m_rcItem.right, m_rcItem.bottom));
            }
            else
            {
                m_lstThumb.Add(new ThumbInfo(eDragDown, m_rcItem.left, m_rcItem.bottom - m_iSepSize, m_rcItem.right, m_rcItem.bottom));
            }
        }

        if ((m_iDragDir&eDragLeft) != 0)
        {
            if ((m_iDragDir&eDragLeftTop) != 0 && (m_iDragDir&eDragLeftDown) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeftTop, m_rcItem.left, m_rcItem.top, m_rcItem.left + m_iSepSize, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragLeft, m_rcItem.left, m_rcItem.top + m_iSepSize, m_rcItem.left + m_iSepSize, m_rcItem.bottom - m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragLeftDown, m_rcItem.left, m_rcItem.bottom - m_iSepSize, m_rcItem.left + m_iSepSize, m_rcItem.bottom));
            }
            else if ((m_iDragDir&eDragLeftTop) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeftTop, m_rcItem.left, m_rcItem.top, m_rcItem.left + m_iSepSize, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragLeft, m_rcItem.left, m_rcItem.top + m_iSepSize, m_rcItem.left + m_iSepSize, m_rcItem.bottom));
            }
            else if ((m_iDragDir&eDragLeftDown) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeft, m_rcItem.left, m_rcItem.top, m_rcItem.left + m_iSepSize, m_rcItem.bottom - m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragLeftDown, m_rcItem.left, m_rcItem.bottom - m_iSepSize, m_rcItem.left + m_iSepSize, m_rcItem.bottom));
            }
            else
            {
                m_lstThumb.Add(new ThumbInfo(eDragLeft, m_rcItem.left, m_rcItem.top, m_rcItem.left + m_iSepSize, m_rcItem.bottom));
            }
        }

        if ((m_iDragDir&eDragRight) != 0)
        {
            if ((m_iDragDir&eDragRightTop) != 0 && (m_iDragDir&eDragRightDown) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragRightTop, m_rcItem.right - m_iSepSize, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragRight, m_rcItem.right - m_iSepSize, m_rcItem.top + m_iSepSize, m_rcItem.right, m_rcItem.bottom - m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragRightDown, m_rcItem.right - m_iSepSize, m_rcItem.bottom - m_iSepSize, m_rcItem.right, m_rcItem.bottom));
            }
            else if ((m_iDragDir&eDragRightTop) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragRightTop, m_rcItem.right - m_iSepSize, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragRight, m_rcItem.right - m_iSepSize, m_rcItem.top + m_iSepSize, m_rcItem.right, m_rcItem.bottom));
            }
            else if ((m_iDragDir&eDragRightDown) != 0)
            {
                m_lstThumb.Add(new ThumbInfo(eDragRight, m_rcItem.right - m_iSepSize, m_rcItem.top, m_rcItem.right, m_rcItem.bottom - m_iSepSize));
                m_lstThumb.Add(new ThumbInfo(eDragRightDown, m_rcItem.right - m_iSepSize, m_rcItem.bottom - m_iSepSize, m_rcItem.right, m_rcItem.bottom));
            }
            else
            {
                m_lstThumb.Add(new ThumbInfo(eDragRight, m_rcItem.right - m_iSepSize, m_rcItem.top, m_rcItem.right, m_rcItem.bottom));
            }
        }
    }

    void CContainerUI::SetDragCb(DragCb pDragCb, void* pDragCbParam)
    {
        m_pDragCb = pDragCb;
        m_pDragCbParam = pDragCbParam;
    }

} // namespace DuiLib
