#include "stdafx.h"
#include "UICheckBox.h"

namespace DuiLib
{
    CCheckBoxUI::CCheckBoxUI():m_bPartialSelected(false)
    {

    }

	LPCTSTR CCheckBoxUI::GetClass() const
	{
		return _T("CheckBoxUI");
	}

	void CCheckBoxUI::SetCheck(bool bCheck)
	{
		Selected(bCheck);
	}

	bool  CCheckBoxUI::GetCheck() const
	{
		return IsSelected();
	}

    void  CCheckBoxUI::SetPartialSelected(bool bPartialSelected) {
        if (m_bPartialSelected != bPartialSelected) {
            m_bPartialSelected = bPartialSelected;
            Invalidate();
        }         
    }

    bool CCheckBoxUI::IsPartialSelected() {
        return m_bPartialSelected;
    }

    void  CCheckBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
        if (_tcscmp(pstrName, _T("partialselected")) == 0) SetPartialSelected(_tcscmp(pstrValue, _T("true")) == 0);

        else if (_tcscmp(pstrName, _T("partialselectedimage")) == 0) SetPartialSelectedImage(pstrValue);
        else if (_tcscmp(pstrName, _T("partialselectedhotimage")) == 0) SetPartialSelectedHotImage(pstrValue);
        else if (_tcscmp(pstrName, _T("partialselecteddisableimage")) == 0) SetPartialSelectedDisableImage(pstrValue);
        else COptionUI::SetAttribute(pstrName, pstrValue);
    }

    LPCTSTR CCheckBoxUI::GetPartialSelectedImage() {
        return m_sPartialSelectedImage;
    }

    void CCheckBoxUI::SetPartialSelectedImage(LPCTSTR pStrImage) {
        m_sPartialSelectedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CCheckBoxUI::GetPartialSelectedHotImage() {
        return m_sPartialSelectedHotImage;
    }

    void CCheckBoxUI::SetPartialSelectedHotImage(LPCTSTR pStrImage) {
        m_sPartialSelectedHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CCheckBoxUI::GetPartialSelectedDisableImage() {
        return m_sPartialSelectedDisableImage;
    }

    void CCheckBoxUI::SetPartialSelectedDisableImage(LPCTSTR pStrImage) {
        m_sPartialSelectedDisableImage = pStrImage;
        Invalidate();
    }

    void CCheckBoxUI::PaintStatusImage(HDC hDC) {

        if (m_bPartialSelected) {
            if (!IsEnabled())
            {
                if (!m_sPartialSelectedDisableImage.IsEmpty()) {
                    DrawImage(hDC, (LPCTSTR)m_sPartialSelectedDisableImage);
                } 
            }
            else if ((m_uButtonState & UISTATE_HOT) != 0) {
                if (!m_sPartialSelectedHotImage.IsEmpty())
                {
                    if (!DrawImage(hDC, (LPCTSTR)m_sPartialSelectedHotImage))  m_sPartialSelectedHotImage.Empty();
                }                
            }
            else  {
                if (!m_sPartialSelectedImage.IsEmpty())
                {
                    if (!DrawImage(hDC, (LPCTSTR)m_sPartialSelectedImage))  m_sPartialSelectedImage.Empty();
                }
            }

            return;
        }

        COptionUI::PaintStatusImage(hDC);
    }

    void CCheckBoxUI::Selected(bool bSelected) {
        m_bPartialSelected = false;
        COptionUI::Selected(bSelected);
    }
}
