#include "stdafx.h"
#include "UICheckBox.h"

namespace DuiLib
{
    CCheckBoxUI::CCheckBoxUI()
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
}
