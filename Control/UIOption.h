#ifndef __UIOPTION_H__
#define __UIOPTION_H__

#pragma once

namespace DuiLib
{
    enum OptionSelectedState
    {
        eOptStateNotSelected = 0,
        eOptStateSelected = 1,
        eOptStatePartialSelected = 2,
    };

	class UILIB_API COptionUI : public CButtonUI
	{
	public:
		COptionUI();
		~COptionUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

		bool Activate();
		void SetEnabled(bool bEnable = true);

		LPCTSTR GetSelectedImage();
		void SetSelectedImage(LPCTSTR pStrImage);
        void SetBGRSelectedImage(LPCTSTR pStrImage, unsigned char* bytesImg, DWORD bytesLength, int iWidth, int iHeight, bool bShare);

		LPCTSTR GetSelectedHotImage();
		void SetSelectedHotImage(LPCTSTR pStrImage);

		void SetSelectedTextColor(DWORD dwTextColor);
		DWORD GetSelectedTextColor();

		void SetSelectedBkColor(DWORD dwBkColor);
		DWORD GetSelectBkColor();    

        LPCTSTR GetSelectedDisableImage();
        void SetSelectedDisableImage(LPCTSTR pStrImage);

		LPCTSTR GetForeImage();
		void SetForeImage(LPCTSTR pStrImage);

		LPCTSTR GetGroup() const;
		void SetGroup(LPCTSTR pStrGroupName = NULL);
		bool IsSelected() const;
		virtual void Selected(bool bSelected);

		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);
        void DoEvent(TEventUI& event);
        void SetSelectedFont(int index);
        int GetSelectedFont();

        void SetSelectedBorderColor(DWORD dwSelectedBorderColor);
        virtual DWORD GetBorderColor() const;

        void  SetPartialSelected();
        bool IsPartialSelected();

        LPCTSTR GetPartialSelectedImage();
        void SetPartialSelectedImage(LPCTSTR pStrImage);

        LPCTSTR GetPartialSelectedHotImage();
        void SetPartialSelectedHotImage(LPCTSTR pStrImage);

        LPCTSTR GetPartialSelectedDisableImage();
        void SetPartialSelectedDisableImage(LPCTSTR pStrImage);

        OptionSelectedState GetSelectedState();
	protected:
        OptionSelectedState	m_iSelected; // 0 未选中 1选中 2选中部分
		CDuiString		m_sGroupName;

		DWORD			m_dwSelectedBkColor;
		DWORD			m_dwSelectedTextColor;
        DWORD			m_dwSelectedBorderColor;

		CDuiString		m_sSelectedImage;
		CDuiString		m_sSelectedHotImage;
		CDuiString		m_sForeImage;
        CDuiString		m_sSelectedDisableImage;
        int		        m_iSelectedFont;

        CDuiString		m_sPartialSelectedImage;
        CDuiString		m_sPartialSelectedHotImage;
        CDuiString		m_sPartialSelectedDisableImage;
	};

} // namespace DuiLib

#endif // __UIOPTION_H__