#ifndef __UICONTAINER_H__
#define __UICONTAINER_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class IContainerUI
{
public:
    virtual CControlUI* GetItemAt(int iIndex) const = 0;
    virtual int GetItemIndex(CControlUI* pControl) const  = 0;
    virtual bool SetItemIndex(CControlUI* pControl, int iIndex)  = 0;
    virtual int GetCount() const = 0;
    virtual bool Add(CControlUI* pControl) = 0;
    virtual bool AddAt(CControlUI* pControl, int iIndex)  = 0;
    virtual bool Remove(CControlUI* pControl) = 0;
    virtual bool RemoveAt(int iIndex)  = 0;
    virtual void RemoveAll() = 0;
};

/////////////////////////////////////////////////////////////////////////////////////
//
class CScrollBarUI;

enum eDragDirection {
	eDragTop = 1,
	eDragDown = 2,	
	eDragLeft = 4,	
    eDragRight = 8,
	eDragLeftTop = 16,
	eDragLeftDown = 32,
	eDragRightTop = 64,
	eDragRightDown = 128,
	eDragAll = 255,
};

struct ThumbInfo
{
	ThumbInfo() {}
	ThumbInfo(eDragDirection eDragDir, int iLeft, int iTop, int iRight, int iBottom) {
		m_eDragDir = eDragDir;
		m_rc.left = iLeft;
		m_rc.top = iTop;
		m_rc.right = iRight;
		m_rc.bottom = iBottom;
	}
	RECT m_rc;
	eDragDirection m_eDragDir;
};

class UILIB_API CContainerUI : public CControlUI, public IContainerUI
{
public:
    CContainerUI();
    virtual ~CContainerUI();	

public:
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    CControlUI* GetItemAt(int iIndex) const;
    int GetItemIndex(CControlUI* pControl) const;
    bool SetItemIndex(CControlUI* pControl, int iIndex);
    int GetCount() const;
    bool Add(CControlUI* pControl);
    bool Add(CControlUI* pControl, bool bRefresh);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    void DoEvent(TEventUI& event);
    void SetVisible(bool bVisible = true);
    void SetInternVisible(bool bVisible = true);
    void SetMouseEnabled(bool bEnable = true);

    virtual RECT GetInset() const;
    virtual void SetInset(RECT rcInset); // 设置内边距，相当于设置客户区
    virtual int GetChildPadding() const;
    virtual void SetChildPadding(int iPadding);
    virtual bool IsAutoDestroy() const;
    virtual void SetAutoDestroy(bool bAuto);
    virtual bool IsDelayedDestroy() const;
    virtual void SetDelayedDestroy(bool bDelayed);
    virtual bool IsMouseChildEnabled() const;
    virtual void SetMouseChildEnabled(bool bEnable = true);

    virtual int FindSelectable(int iIndex, bool bForward = true) const;

    virtual void SetPos(RECT rc);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	bool SetSubControlText(LPCTSTR pstrSubControlName,LPCTSTR pstrText);
	bool SetSubControlFixedHeight(LPCTSTR pstrSubControlName,int cy);
	bool SetSubControlFixedWdith(LPCTSTR pstrSubControlName,int cx);
	bool SetSubControlUserData(LPCTSTR pstrSubControlName,LPCTSTR pstrText);

	CDuiString GetSubControlText(LPCTSTR pstrSubControlName);
	int GetSubControlFixedHeight(LPCTSTR pstrSubControlName);
	int GetSubControlFixedWdith(LPCTSTR pstrSubControlName);
	const CDuiString GetSubControlUserData(LPCTSTR pstrSubControlName);
	CControlUI* FindSubControl(LPCTSTR pstrSubControlName);

    virtual SIZE GetScrollPos() const;
    virtual SIZE GetScrollRange() const;
    virtual void SetScrollPos(SIZE szPos);
    virtual void LineUp();
    virtual void LineDown();
    virtual void PageUp();
    virtual void PageDown();
    virtual void HomeUp();
    virtual void EndDown();
    virtual void LineLeft();
    virtual void LineRight();
    virtual void PageLeft();
    virtual void PageRight();
    virtual void HomeLeft();
    virtual void EndRight();
    virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
    virtual CScrollBarUI* GetVerticalScrollBar() const;
    virtual CScrollBarUI* GetHorizontalScrollBar() const;

	virtual void SetEnabled(bool bEnable = true);
	bool IsDragable() const;
	void SetDragable(bool bDragable);
	int GetSeparatorSize() const;
	void SetSeparatorSize(int iSepSize);
	int GetDragDirection() const;
	void SetDragDirection(int iDragDir);
	void SetSepImmMode(bool bImmediately);
	bool IsSepImmMode() const;
    virtual UINT GetControlFlags() const;
    virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);
    virtual RECT GetThumbRect(bool bUseNew) const;

protected:
    virtual void SetFloatPos(int iIndex);
    virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);
    virtual void ProcessScroll(CScrollBarUI* scrollBar, int iRequired, int  iSize, const RECT& rcScrollBarPos);
private:
    bool HandleDrag(TEventUI& event);
    void GetThumbRects();
	void ClearThumbList();
protected:
    CStdPtrArray m_items;
    RECT m_rcInset;
    int m_iChildPadding;
    bool m_bAutoDestroy;
    bool m_bDelayedDestroy;
    bool m_bMouseChildEnabled;
    bool m_bScrollProcess; // 防止SetPos循环调用

    CScrollBarUI* m_pVerticalScrollBar;
    CScrollBarUI* m_pHorizontalScrollBar;

	bool m_bDragable;//是否可拖动改变大小
	int  m_iSepSize;//分隔符大小
	int  m_iDragDir;//拖动方向
	POINT m_ptLastMouse;
	UINT m_uButtonState;
	bool m_bImmMode;
	RECT m_rcNewPos;
	CStdPtrArray m_lstThumb;
    eDragDirection m_eCurrDargDir;
};

} // namespace DuiLib

#endif // __UICONTAINER_H__
