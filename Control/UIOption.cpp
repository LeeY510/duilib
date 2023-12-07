#include "stdafx.h"
#include "UIOption.h"

namespace DuiLib
{
    COptionUI::COptionUI() : m_iSelected(eOptStateNotSelected)
        , m_dwSelectedBkColor(0)
        , m_dwSelectedTextColor(0)
        , m_dwSelectedBorderColor(0)
        , m_iSelectedFont(-1)
    {
    }

    COptionUI::~COptionUI()
    {
        if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
    }

    LPCTSTR COptionUI::GetClass() const
    {
        return _T("OptionUI");
    }

    LPVOID COptionUI::GetInterface(LPCTSTR pstrName)
    {
        if (_tcscmp(pstrName, DUI_CTR_OPTION) == 0) return static_cast<COptionUI*>(this);
        return CButtonUI::GetInterface(pstrName);
    }

    void COptionUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
    {
        CControlUI::SetManager(pManager, pParent, bInit);
        if (bInit && !m_sGroupName.IsEmpty()) {
            if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
        }
    }

    LPCTSTR COptionUI::GetGroup() const
    {
        return m_sGroupName;
    }

    void COptionUI::SetGroup(LPCTSTR pStrGroupName)
    {
        if (pStrGroupName == NULL) {
            if (m_sGroupName.IsEmpty()) return;
            m_sGroupName.Empty();
        }
        else {
            if (m_sGroupName == pStrGroupName) return;
            if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
            m_sGroupName = pStrGroupName;
        }

        if (!m_sGroupName.IsEmpty()) {
            if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
        }
        else {
            if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
        }

        Selected(eOptStateSelected == m_iSelected);
    }

    bool COptionUI::IsSelected() const
    {
        return eOptStateSelected == m_iSelected;
    }

    void COptionUI::Selected(bool bSelected)
    {
        if (eOptStateSelected == m_iSelected && bSelected) return;
        if (eOptStateNotSelected == m_iSelected && !bSelected) return;

        m_iSelected = bSelected ? eOptStateSelected : eOptStateNotSelected;
        if (bSelected) m_uButtonState |= UISTATE_SELECTED;
        else m_uButtonState &= ~UISTATE_SELECTED;

        if (m_pManager != NULL) {
            if (!m_sGroupName.IsEmpty()) {
                if (bSelected) {
                    CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
                    for (int i = 0; i < aOptionGroup->GetSize(); i++) {
                        COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
                        if (pControl != this) {
                            pControl->Selected(false);
                        }
                    }
                    m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
                }
            }
            else {
                m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
            }
        }

        Invalidate();
    }

    bool COptionUI::Activate()
    {
        if (!CButtonUI::Activate()) return false;
        if (!m_sGroupName.IsEmpty()) Selected(true);
        else Selected(eOptStateSelected == m_iSelected ? false : true);

        return true;
    }

    void COptionUI::SetEnabled(bool bEnable)
    {
        CControlUI::SetEnabled(bEnable);
        if (!IsEnabled()) {
            if (eOptStateSelected == m_iSelected) m_uButtonState |= UISTATE_SELECTED;
            else m_uButtonState = 0;
        }
    }

    LPCTSTR COptionUI::GetSelectedImage()
    {
        return m_sSelectedImage;
    }

    void COptionUI::SetSelectedImage(LPCTSTR pStrImage)
    {
        m_sSelectedImage = pStrImage;
        Invalidate();
    }

    void COptionUI::SetBGRSelectedImage(LPCTSTR pStrImage, unsigned char* bytesImg, DWORD bytesLength, int iWidth, int iHeight, bool bShare)
    {
        if (!SetBGRImage(pStrImage, bytesImg, bytesLength, iWidth, iHeight, bShare))
        {
            return;
        }

        m_sSelectedImage = pStrImage;
        Invalidate();
    }

    //************************************
    // Method:    GetSelectedHotImage
    // FullName:  COptionUI::GetSelectedHotImage
    // Access:    public 
    // Returns:   LPCTSTR
    // Qualifier:
    // Node:	  
    //************************************
    LPCTSTR COptionUI::GetSelectedHotImage()
    {
        return m_sSelectedHotImage;
    }
    //************************************
    // Method:    SetSelectedHotImage
    // FullName:  COptionUI::SetSelectedHotImage
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LPCTSTR pStrImage
    // Node:	  
    //************************************
    void COptionUI::SetSelectedHotImage(LPCTSTR pStrImage)
    {
        m_sSelectedHotImage = pStrImage;
        Invalidate();
    }

    void COptionUI::SetSelectedTextColor(DWORD dwTextColor)
    {
        m_dwSelectedTextColor = dwTextColor;
    }

    DWORD COptionUI::GetSelectedTextColor()
    {
        if (m_dwSelectedTextColor == 0) m_dwSelectedTextColor = m_pManager->GetDefaultFontColor();
        return m_dwSelectedTextColor;
    }

    //************************************
    // Method:    SetSelectedBkColor
    // FullName:  COptionUI::SetSelectedBkColor
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: DWORD dwBkColor
    // Note:	  
    //************************************
    void COptionUI::SetSelectedBkColor(DWORD dwBkColor)
    {
        m_dwSelectedBkColor = dwBkColor;
    }

    //************************************
    // Method:    GetSelectBkColor
    // FullName:  COptionUI::GetSelectBkColor
    // Access:    public 
    // Returns:   DWORD
    // Qualifier:
    // Note:	  
    //************************************
    DWORD COptionUI::GetSelectBkColor()
    {
        return m_dwSelectedBkColor;
    }

    LPCTSTR COptionUI::GetForeImage()
    {
        return m_sForeImage;
    }

    void COptionUI::SetForeImage(LPCTSTR pStrImage)
    {
        m_sForeImage = pStrImage;
        Invalidate();
    }

    SIZE COptionUI::EstimateSize(SIZE szAvailable)
    {
        //if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
        return CControlUI::EstimateSize(szAvailable);
    }

    void COptionUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if (_tcscmp(pstrName, _T("group")) == 0) SetGroup(pstrValue);
        else if (_tcscmp(pstrName, _T("selected")) == 0) Selected(_tcscmp(pstrValue, _T("true")) == 0);
        else if (_tcscmp(pstrName, _T("selectedimage")) == 0) SetSelectedImage(pstrValue);
        else if (_tcscmp(pstrName, _T("selectedhotimage")) == 0) SetSelectedHotImage(pstrValue);
        else if (_tcscmp(pstrName, _T("foreimage")) == 0) SetForeImage(pstrValue);
        else if (_tcscmp(pstrName, _T("wordbreak")) == 0) {
            if (_tcscmp(pstrValue, _T("true")) == 0)
            {
                m_uTextStyle = DT_VCENTER | DT_CENTER | DT_WORDBREAK;
            }
            else m_uTextStyle &= ~DT_WORDBREAK;
        }

        else if (_tcscmp(pstrName, _T("selectedbkcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedBkColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("selectedtextcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedTextColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("selectedfont")) == 0) SetSelectedFont(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("selectedbordercolor")) == 0)
        {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedBorderColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("selecteddisableimage")) == 0) SetSelectedDisableImage(pstrValue);
        else if (_tcscmp(pstrName, _T("partialselected")) == 0) SetPartialSelected();

        else if (_tcscmp(pstrName, _T("partialselectedimage")) == 0) SetPartialSelectedImage(pstrValue);
        else if (_tcscmp(pstrName, _T("partialselectedhotimage")) == 0) SetPartialSelectedHotImage(pstrValue);
        else if (_tcscmp(pstrName, _T("partialselecteddisableimage")) == 0) SetPartialSelectedDisableImage(pstrValue);
        else CButtonUI::SetAttribute(pstrName, pstrValue);
    }

    void COptionUI::PaintStatusImage(HDC hDC)
    {
        m_uButtonState &= ~UISTATE_PUSHED;
        //优先绘制不可以图片
        if (!IsEnabled())
        {
            if (eOptStateSelected == m_iSelected && !m_sSelectedDisableImage.IsEmpty()) {
                DrawImage(hDC, (LPCTSTR)m_sSelectedDisableImage);
            }
            else if (eOptStatePartialSelected == m_iSelected && !m_sPartialSelectedDisableImage.IsEmpty())
            {
                DrawImage(hDC, (LPCTSTR)m_sPartialSelectedDisableImage);
            }
            else {
                CButtonUI::PaintStatusImage(hDC);
            }

            return;
        }

        if ((m_uButtonState & UISTATE_HOT) != 0) {
            if (eOptStateSelected == m_iSelected) {

                if (m_dwSelectedBkColor != 0) {
                    CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwSelectedBkColor));
                }

                if (!m_sSelectedHotImage.IsEmpty()) {
                    if (!DrawImage(hDC, (LPCTSTR)m_sSelectedHotImage))
                        m_sSelectedHotImage.Empty();
                    else goto Label_ForeImage;
                }
                else if (!m_sSelectedImage.IsEmpty()){
                    if (!DrawImage(hDC, (LPCTSTR)m_sSelectedImage))
                        m_sSelectedImage.Empty();
                    else goto Label_ForeImage;
                }
            }
            else if (eOptStatePartialSelected == m_iSelected)
            {
                if (!m_sPartialSelectedHotImage.IsEmpty()) {
                    if (!DrawImage(hDC, (LPCTSTR)m_sPartialSelectedHotImage))
                        m_sPartialSelectedHotImage.Empty();
                    else goto Label_ForeImage;
                }
                else if (!m_sPartialSelectedImage.IsEmpty())
                {
                    if (!DrawImage(hDC, (LPCTSTR)m_sPartialSelectedImage))
                        m_sPartialSelectedImage.Empty();
                    else goto Label_ForeImage;
                }                
            }
        }
        else
        {
            if (eOptStateSelected == m_iSelected) {
                if (m_dwSelectedBkColor != 0) {
                    CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwSelectedBkColor));
                }

                if (!m_sSelectedImage.IsEmpty()) {
                    if (!DrawImage(hDC, (LPCTSTR)m_sSelectedImage))
                        m_sSelectedImage.Empty();
                    else goto Label_ForeImage;
                }                
            }
            else if (eOptStatePartialSelected == m_iSelected && !m_sPartialSelectedImage.IsEmpty())
            {
                if (!DrawImage(hDC, (LPCTSTR)m_sPartialSelectedImage))
                    m_sPartialSelectedImage.Empty();
                else goto Label_ForeImage;
            }
        }

        CButtonUI::PaintStatusImage(hDC);

    Label_ForeImage:
        if (!m_sForeImage.IsEmpty()) {
            if (!DrawImage(hDC, (LPCTSTR)m_sForeImage)) m_sForeImage.Empty();
        }
    }

    void COptionUI::PaintText(HDC hDC)
    {
        if ((m_uButtonState & UISTATE_SELECTED) != 0)
        {
            DWORD oldTextColor = m_dwTextColor;
            if (m_dwSelectedTextColor != 0) m_dwTextColor = m_dwSelectedTextColor;

            if (m_dwTextColor == 0) m_dwTextColor = m_pManager->GetDefaultFontColor();
            if (m_dwDisabledTextColor == 0) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

            if (m_sText.IsEmpty()) return;
            int nLinks = 0;
            RECT rc = m_rcItem;
            rc.left += m_rcTextPadding.left;
            rc.right -= m_rcTextPadding.right;
            rc.top += m_rcTextPadding.top;
            rc.bottom -= m_rcTextPadding.bottom;

            int iFont = m_iSelectedFont;
            if (-1 == iFont) iFont = m_iFont;

            if (m_bShowHtml)
                CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor, \
                    NULL, NULL, nLinks, m_uTextStyle);
            else
                CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor, \
                    iFont, m_uTextStyle);

            m_dwTextColor = oldTextColor;
        }
        else
            CButtonUI::PaintText(hDC);
    }

    void COptionUI::DoEvent(TEventUI& event)
    {
        CButtonUI::DoEvent(event);
    }

    void COptionUI::SetSelectedFont(int index)
    {
        m_iSelectedFont = index;
        Invalidate();
    }
    int COptionUI::GetSelectedFont()
    {
        return m_iSelectedFont;
    }

    void COptionUI::SetSelectedBorderColor(DWORD dwSelectedBorderColor)
    {
        m_dwSelectedBorderColor = dwSelectedBorderColor;
    }

    DWORD COptionUI::GetBorderColor() const
    {
        if (m_dwSelectedBorderColor != 0 && (m_uButtonState & UISTATE_SELECTED) != 0) {
            return m_dwSelectedBorderColor;
        }
        return CButtonUI::GetBorderColor();
    }

    LPCTSTR COptionUI::GetSelectedDisableImage()
    {
        return m_sSelectedDisableImage;
    }

    void COptionUI::SetSelectedDisableImage(LPCTSTR pStrImage)
    {
        m_sSelectedDisableImage = pStrImage;
        Invalidate();
    }

    void  COptionUI::SetPartialSelected() {
        if (eOptStatePartialSelected == m_iSelected) return;

        m_iSelected = eOptStatePartialSelected;
        bool bSelected = false;

        if (bSelected) m_uButtonState |= UISTATE_SELECTED;
        else m_uButtonState &= ~UISTATE_SELECTED;

        if (m_pManager != NULL) {
            if (!m_sGroupName.IsEmpty()) {
                if (bSelected) {
                    CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
                    for (int i = 0; i < aOptionGroup->GetSize(); i++) {
                        COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
                        if (pControl != this) {
                            pControl->Selected(false);
                        }
                    }
                    m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
                }
            }
            else {
                m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
            }
        }

        Invalidate();
    }

    bool COptionUI::IsPartialSelected() {
        return eOptStatePartialSelected == m_iSelected;
    }

    LPCTSTR COptionUI::GetPartialSelectedImage() {
        return m_sPartialSelectedImage;
    }

    void COptionUI::SetPartialSelectedImage(LPCTSTR pStrImage) {
        m_sPartialSelectedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR COptionUI::GetPartialSelectedHotImage() {
        return m_sPartialSelectedHotImage;
    }

    void COptionUI::SetPartialSelectedHotImage(LPCTSTR pStrImage) {
        m_sPartialSelectedHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR COptionUI::GetPartialSelectedDisableImage() {
        return m_sPartialSelectedDisableImage;
    }

    void COptionUI::SetPartialSelectedDisableImage(LPCTSTR pStrImage) {
        m_sPartialSelectedDisableImage = pStrImage;
        Invalidate();
    }

    OptionSelectedState COptionUI::GetSelectedState() {
        return m_iSelected;
    }
}