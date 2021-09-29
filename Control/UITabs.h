#ifndef UITabs_h__
#define UITabs_h__

#include <vector>
using namespace std;

#pragma once

namespace DuiLib
{
    class CTabsUI;

    class UILIB_API CTabPaneUI : public CHorizontalLayoutUI
    {
    public:
        CTabPaneUI();
        ~CTabPaneUI(void);

    public:
        LPCTSTR GetClass() const;
        LPVOID	GetInterface(LPCTSTR pstrName);
        void	DoEvent(TEventUI& event);
        void	Invalidate();

        CDuiString GetText() const;
        void    SetText(LPCTSTR pstrText);

        void	SetVisibleIcon(bool bIsVisible);
        bool	IsVisibleIcon();
        void    SetIconImage(LPCTSTR pstrImage);
        CDuiString GetIconImage() const;

        void	SetVisibleCloseBtn(bool bIsVisible);
        bool	IsVisibleCloseBtn();

        CDuiString GetHotCloseBtnImage();
        void    SetHotCloseBtnImage(LPCTSTR pStrImage);

        CDuiString GetNormalCloseBtnImage();
        void    SetNormalCloseBtnImage(LPCTSTR pStrImage);

        CDuiString GetPushedCloseBtnImage();
        void    SetPushedCloseBtnImage(LPCTSTR pStrImage);

        CControlUI* GetIconCtrl() const;
        CLabelUI* GetTextCtrl() const;
        CButtonUI* GetCloseBtn() const;

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        virtual void PaintText(HDC hDC);

        void SetTextColor(DWORD dwTextColor);
        DWORD GetTextColor() const;
        void SetHotTextColor(DWORD dwTextColor);
        DWORD GetHotTextColor() const;
        void SetSelectedTextColor(DWORD dwTextColor);
        DWORD GetSelectedTextColor() const;

        void SetNormalBkColor(DWORD dwBkColor);
        DWORD GetNormalBkColor() const;
        void SetHotBkColor(DWORD dwBkColor);
        DWORD GetHotBkColor() const;
        void SetSelectedBkColor(DWORD dwBkColor);
        DWORD GetSelectedBkColor() const;

        LPCTSTR GetGroup() const;
        void SetGroup(LPCTSTR pStrGroupName = NULL);
        bool IsSelected() const;
        virtual void Selected(bool bSelected);

        void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

        bool Activate();
        void SetEnabled(bool bEnable);
        void PaintStatusImage(HDC hDC);

        SIZE EstimateSize(SIZE szAvailable);
        void SetOwner(CTabsUI* pOwner);

        void SetFont(int index);
        int  GetFont() const;
        void SetTextPadding(const RECT& rcTextPadding);
        RECT GetTextPadding()const;
        void SetVisibleTips(bool bIsVisible);
        bool IsVisibleTips() const;
    private:

        bool	            m_bIconVisable;
        bool                m_bCloseBtnVisible;
        DWORD               m_dwTextColor;
        DWORD               m_dwHotTextColor;
        DWORD	            m_dwSelectedTextColor;
        DWORD               m_dwDisabledTextColor;

        DWORD               m_dwBkColor;
        DWORD               m_dwHotBkColor;
        DWORD               m_dwSelectedBkColor;

        int                 m_iFont;

        bool			    m_bSelected;
        CDuiString		    m_sGroupName;
        bool                m_bVisibleTips;

        CTabsUI*			m_pTabs;
        CControlUI*			m_pIcon;
        CLabelUI*			m_pText;
        CButtonUI*			m_pClose;
    };

    typedef bool(*TabPaneCloseCb)(CControlUI*, void*);

    class UILIB_API CTabsUI : public CHorizontalLayoutUI
    {
    public:
        CTabsUI(void);
        ~CTabsUI(void);

    public:
        virtual LPCTSTR GetClass() const;
        virtual LPVOID	GetInterface(LPCTSTR pstrName);
        virtual bool Add(CControlUI* pControl);
        virtual bool AddAt(CControlUI* pControl, int iIndex);

        void SetItemIconVisible(bool _IsVisibled);
        void SetItemCloseBtnVisible(bool _IsVisibled);

        void SetItemMinWidth(UINT uiWidth);
        void SetItemMaxWidth(UINT uiWidth);

        void SetItemTextColor(DWORD _dwItemTextColor);
        void SetItemHotTextColor(DWORD _dwItemHotTextColor);
        void SetItemSelTextColor(DWORD _dwSelItemTextColor);

        void SetItemBkColor(DWORD _dwColor);
        void SetItemHotBkColor(DWORD _dwColor);
        void SetItemSelBkColor(DWORD _dwColor);

        void SetItemCloseBtnNormalImage(LPCTSTR pStrImage);
        void SetItemCloseBtnHotImage(LPCTSTR pStrImage);        
        void SetItemCloseBtnPushedImage(LPCTSTR pStrImage);
        void SetItemInset(RECT rcInset);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        void SetGroup(LPCTSTR pstrGroupName);

        void SetItemFont(int iFont);
        void SetItemTextPadding(RECT rcTextPadding);
        void SetItemVisibleTips(bool bIsVisible);
        bool OnItemClose(void* param);
        void SetItemIconImage(LPCTSTR pStrImage);

        void PushItemSelected(CControlUI* pItem);

        void SetTabPaneCloseCb(TabPaneCloseCb pTabPaneCloseCb, void* pCbParam);
        void RemoveTabPane(CControlUI* pTabPane);
    private:
        void InitTabPane(CControlUI* pControl);
        void RemoveItem(CTabPaneUI* pTabPane);
    private:
        UINT        m_uItemMinWidth;
        UINT        m_uItemMaxWidth;
        bool        m_bItemIconVisible;
        bool        m_bItemCloseBtnVisible;
        DWORD       m_dwItemTextColor;
        DWORD       m_dwItemHotTextColor;
        DWORD       m_dwItemSelTextColor;

        DWORD       m_dwItemBkColor;
        DWORD       m_dwItemHotBkColor;
        DWORD       m_dwItemSelBkColor;

        int         m_iItemfont;
        RECT        m_rcItemInset;
        RECT        m_rcItemTextPadding;
        bool        m_bItemVisibleTips;

        CDuiString  m_strItemCloseBtnNormalImage;
        CDuiString  m_strItemCloseBtnHotImage;
        CDuiString  m_strItemCloseBtnPushedImage;
        CDuiString  m_strItemIconImage;
        CDuiString  m_strGroupName;

        CStdPtrArray m_itemOptStack;

        TabPaneCloseCb m_pTabPaneCloseCb;
        void*       m_pCbParam;
    };
}


#endif // UITabs_h__
