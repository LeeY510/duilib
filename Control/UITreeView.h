#ifndef UITreeView_h__
#define UITreeView_h__

#include <vector>
using namespace std;

#pragma once

namespace DuiLib
{
    class CTreeViewUI;
    class CCheckBoxUI;
    class CLabelUI;
    class COptionUI;

    class UILIB_API CTreeNodeUI : public CListContainerElementUI
    {
    public:
        CTreeNodeUI(CTreeNodeUI* _ParentNode = NULL);
        ~CTreeNodeUI(void);

    public:
        LPCTSTR GetClass() const;
        LPVOID	GetInterface(LPCTSTR pstrName);
        virtual void	DoEvent(TEventUI& event);
        void	Invalidate();
        bool	Select(bool bSelect = true);

        bool	Add(CControlUI* _pTreeNodeUI);
        bool	AddAt(CControlUI* pControl, int iIndex);
        bool	Remove(CControlUI* pControl);
        void    RemoveAll();

        void	SetVisibleTag(bool _IsVisible);
        bool	GetVisibleTag();
        void	SetItemText(LPCTSTR pstrValue);
        CDuiString	GetItemText();
        void	CheckBoxSelected(bool _Selected);
        bool	IsCheckBoxSelected() const;
        bool	IsHasChild() const;
        bool	AddChildNode(CTreeNodeUI* _pTreeNodeUI);
        bool	RemoveAt(CTreeNodeUI* _pTreeNodeUI);
        void	SetParentNode(CTreeNodeUI* _pParentTreeNode);
        CTreeNodeUI* GetParentNode();
        long	GetCountChild();
        void	SetTreeView(CTreeViewUI* _CTreeViewUI);
        CTreeViewUI* GetTreeView();
        CTreeNodeUI* GetChildNode(int _nIndex);
        void	SetVisibleFolderBtn(bool _IsVisibled);
        bool	GetVisibleFolderBtn();
        void	SetVisibleCheckBtn(bool _IsVisibled);
        bool	GetVisibleCheckBtn();
        void	SetItemTextColor(DWORD _dwItemTextColor);
        DWORD	GetItemTextColor() const;
        void	SetItemHotTextColor(DWORD _dwItemHotTextColor);
        DWORD	GetItemHotTextColor() const;
        void	SetSelItemTextColor(DWORD _dwSelItemTextColor);
        DWORD	GetSelItemTextColor() const;
        void	SetSelItemHotTextColor(DWORD _dwSelHotItemTextColor);
        DWORD	GetSelItemHotTextColor() const;

        void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        CStdPtrArray GetTreeNodes();

        int			 GetTreeIndex();
        int			 GetNodeIndex();
        void	SetVisibleIcon(bool _IsVisibled);
        bool	GetVisibleIcon();
    private:
        CTreeNodeUI* GetLastNode();
        CTreeNodeUI* CalLocation(CTreeNodeUI* _pTreeNodeUI);
    public:
        CHorizontalLayoutUI*	GetTreeNodeHoriznotal() const { return pHoriz; };
        CCheckBoxUI*			GetFolderButton() const { return pFolderButton; };
        CLabelUI*				GetDottedLine() const { return pDottedLine; };
        COptionUI*			    GetCheckBox() const { return pCheckBox; };
        COptionUI*				GetItemButton() const { return pItemButton; };
        CControlUI*				GetIcon() const { return pIcon; };
    private:
        long	m_iTreeLavel;
        bool	m_bIsVisable;
        bool	m_bIsCheckBox;
        DWORD	m_dwItemTextColor;
        DWORD	m_dwItemHotTextColor;
        DWORD	m_dwSelItemTextColor;
        DWORD	m_dwSelItemHotTextColor;

        CTreeViewUI*			pTreeView;
        CHorizontalLayoutUI*	pHoriz;
        CCheckBoxUI*			pFolderButton;
        CLabelUI*				pDottedLine;
        COptionUI*			    pCheckBox;
        COptionUI*				pItemButton;
        CControlUI*             pIcon;

        CTreeNodeUI*			pParentTreeNode;

        CStdPtrArray			mTreeNodes;
    };

    class UILIB_API CTreeViewUI : public CListUI, public INotifyUI
    {
    public:
        CTreeViewUI(void);
        ~CTreeViewUI(void);

        friend class CTreeNodeUI;

    public:
        virtual LPCTSTR GetClass() const;
        virtual LPVOID	GetInterface(LPCTSTR pstrName);
        virtual bool Add(CTreeNodeUI* pControl);
        virtual bool AddAt(CTreeNodeUI* pControl, int iIndex);
        virtual bool Remove(CTreeNodeUI* pControl);
        virtual bool RemoveAt(int iIndex);
        virtual void RemoveAll();

        virtual bool OnCheckBoxChanged(void* param);
        virtual bool OnFolderChanged(void* param);
        virtual bool OnDBClickItem(void* param);
        virtual bool SetItemCheckBox(COptionUI* _CheckBox, CTreeNodeUI* _TreeNode);
        virtual void SetItemExpand(bool _Expanded, CTreeNodeUI* _TreeNode);
        virtual void Notify(TNotifyUI& msg);
        virtual void SetVisibleFolderBtn(bool _IsVisibled);
        virtual bool GetVisibleFolderBtn();
        virtual void SetVisibleCheckBtn(bool _IsVisibled);
        virtual bool GetVisibleCheckBtn();
        virtual void SetItemMinWidth(UINT _ItemMinWidth);
        virtual UINT GetItemMinWidth();
        virtual void SetItemTextColor(DWORD _dwItemTextColor);
        virtual void SetItemHotTextColor(DWORD _dwItemHotTextColor);
        virtual void SetSelItemTextColor(DWORD _dwSelItemTextColor);
        virtual void SetSelItemHotTextColor(DWORD _dwSelHotItemTextColor);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        virtual void RemoveListItem(CControlUI* pControl);

        virtual CControlUI* GetRootAt(int iIndex) const;
        virtual int GetRootIndex(CControlUI* pControl) const;
        virtual int GetRootCount() const;
    private:
        void LoopAdd(CTreeNodeUI* pControl);
        int  LoopAddAt(CTreeNodeUI* pControl, int iIndex);
    private:
        UINT m_uItemMinWidth;
        bool m_bVisibleFolderBtn;
        bool m_bVisibleCheckBtn;
        CStdPtrArray mRootNodes;
    };
}


#endif // UITreeView_h__
