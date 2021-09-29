#include "stdafx.h"
#include "UIHorizontalLayout.h"

namespace DuiLib
{
	CHorizontalLayoutUI::CHorizontalLayoutUI()
	{
	}

	LPCTSTR CHorizontalLayoutUI::GetClass() const
	{
		return _T("HorizontalLayoutUI");
	}

	LPVOID CHorizontalLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_HORIZONTALLAYOUT) == 0) return static_cast<CHorizontalLayoutUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

    void CHorizontalLayoutUI::SetPos(RECT rc)
    {
        CControlUI::SetPos(rc);
        rc = m_rcItem;

        // Adjust for inset
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        if (m_items.GetSize() == 0) {
            ProcessScrollBar(rc, 0, 0);
            return;
        }

        // Determine the minimum size
        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
            szAvailable.cy += m_pVerticalScrollBar->GetScrollRange();

        int nAdjustables = 0;
        int cxFixed = 0;
        int nEstimateNum = 0;
        for (int it1 = 0; it1 < m_items.GetSize(); it1++) {
            CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
            if (!pControl->IsVisible()) continue;
            if (pControl->IsFloat()) continue;
            SIZE sz = pControl->EstimateSize(szAvailable);
            if (sz.cx == 0) {
                nAdjustables++;
            }
            else {
                if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
                if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();
            }
            cxFixed += sz.cx + pControl->GetPadding().left + pControl->GetPadding().right;
            nEstimateNum++;
        }
        cxFixed += (nEstimateNum - 1) * m_iChildPadding;

        // Place elements
        int cxNeeded = 0;
        int cxExpand = 0;
        if (nAdjustables > 0) cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
        // Position the elements
        SIZE szRemaining = szAvailable;
        int iPosY = rc.top;
        if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
            iPosY -= m_pVerticalScrollBar->GetScrollPos();
        }
        int iPosX = rc.left;
        if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
            iPosX -= m_pHorizontalScrollBar->GetScrollPos();
        }
        int iAdjustable = 0;
        int cxFixedRemaining = cxFixed;
        for (int it2 = 0; it2 < m_items.GetSize(); it2++) {
            CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
            if (!pControl->IsVisible()) continue;
            if (pControl->IsFloat()) {
                SetFloatPos(it2);
                continue;
            }

            RECT rcPadding = pControl->GetPadding();
            szRemaining.cx -= rcPadding.left;
            SIZE sz = pControl->EstimateSize(szRemaining);
            if (sz.cx == 0) {
                iAdjustable++;
                sz.cx = cxExpand;
                // Distribute remaining to last element (usually round-off left-overs)
                if (iAdjustable == nAdjustables) {
                    sz.cx = MAX(0, szRemaining.cx - rcPadding.right - cxFixedRemaining);
                }
                if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
                if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();
            }
            else {
                if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
                if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();
                cxFixedRemaining -= sz.cx;
            }

            sz.cy = pControl->GetFixedHeight();
            if (sz.cy == 0) sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom;
            if (sz.cy < 0) sz.cy = 0;
            if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();
            if (sz.cy > pControl->GetMaxHeight()) sz.cy = pControl->GetMaxHeight();

            RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top };
            pControl->SetPos(rcCtrl);

            iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
            cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
            szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
        }
        cxNeeded += (nEstimateNum - 1) * m_iChildPadding;

        // Process the scrollbar
        ProcessScrollBar(rc, cxNeeded, 0);
    }
}
