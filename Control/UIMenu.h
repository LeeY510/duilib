#ifndef __UIMENU_H__
#define __UIMENU_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "../Utils/observer_impl_base.hpp"

#pragma warning( disable: 4251 )

namespace DuiLib {
class CMenuUI;
class CListUI;
class CMenuWnd;
class CMenuElementUI;
class CListContainerElementUI;
/////////////////////////////////////////////////////////////////////////////////////
//
struct ContextMenuParam
{
	// 1: remove all
	// 2: remove the sub menu
	WPARAM wParam;
	HWND hWnd;
};

enum MenuAlignment
{
	eMenuAlignment_Left = 1 << 1,
	eMenuAlignment_Top = 1 << 2,
	eMenuAlignment_Right = 1 << 3,
	eMenuAlignment_Bottom = 1 << 4,
};

typedef class ObserverImpl<BOOL, ContextMenuParam> ContextMenuObserver;
typedef class ReceiverImpl<BOOL, ContextMenuParam> ContextMenuReceiver;
typedef void (*PMODIFYMENUFUNC)(CMenuUI *, void*);
typedef void (*PMENUCLICK)(CControlUI*, void*);
typedef void (*PMENUWINEVENTFUNC)(CControlUI*, UINT , WPARAM , LPARAM);
extern ContextMenuObserver s_context_menu_observer;

class UILIB_API CMenuUI : public CListUI
{
public:
	CMenuUI();
	~CMenuUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoEvent(TEventUI& event);
    virtual bool Add(CControlUI* pControl);
    virtual bool AddAt(CControlUI* pControl, int iIndex);
    virtual int GetItemIndex(CControlUI* pControl) const;
    virtual bool SetItemIndex(CControlUI* pControl, int iIndex);
    virtual bool Remove(CControlUI* pControl);

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetOwnWnd(CMenuWnd *pWnd);
	CMenuWnd *GetOwnWnd();
private:
	CMenuWnd *m_pOwnWnd;
};

/////////////////////////////////////////////////////////////////////////////////////

class UILIB_API CMenuWnd : public CWindowWnd,public ContextMenuReceiver
{
	friend CMenuElementUI;
public:
	CMenuWnd(HWND hParent = NULL);
  
	void Init(POINT point,STRINGorID xml, LPCTSTR pSkinType = _T("xml"));
	void Init(CControlUI* pTrigger, CMenuElementUI* pOwner, POINT point, STRINGorID xml, LPCTSTR pSkinType = _T("xml"));

	///> 用于动态修改Menu
	void SetModifyMenuFunc(PMODIFYMENUFUNC pfunModifyMenu, void* params);
	PMODIFYMENUFUNC GetModifyMenuFunc();
    void* GetModifyMenuParam();

	///> 用于响应Menu事件
	void SetMenuClickFunc(PMENUCLICK pMenuClick, void* params);
	PMENUCLICK GetMenuClickFunc();
    void* GetMenuClickParam();

	CControlUI* GetTrigger()
	{
		return m_pTrigger;
	}

    void SetMenuWinEventFunc(PMENUWINEVENTFUNC pMeunWinEventFunc);
    PMENUWINEVENTFUNC GetMenuWinEventFunc();
protected:
	void Init(CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point);
    LPCTSTR GetWindowClassName() const;
    void OnFinalMessage(HWND hWnd);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL Receive(ContextMenuParam param);

protected:
	HWND m_hParent;
	POINT m_BasedPoint;
	STRINGorID m_xml;
	CDuiString m_sType;
    CPaintManagerUI m_pm;
    CMenuElementUI* m_pOwner;
    CMenuUI* m_pLayout;
	CControlUI* m_pTrigger;
	PMODIFYMENUFUNC m_pfunModifyMenu;
    void* m_pModifyParam;
	PMENUCLICK	m_pMenuClick;
    void* m_pMenuClickParam;
    PMENUWINEVENTFUNC m_pMeunWinEventFunc;
};

class UILIB_API CMenuElementUI : public CListContainerElementUI
{
	friend CMenuWnd;
public:
    CMenuElementUI();
	~CMenuElementUI();

protected:
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    void DoPaint(HDC hDC, const RECT& rcPaint);
	void DrawItemText(HDC hDC, const RECT& rcItem);
	SIZE EstimateSize(SIZE szAvailable);
	bool Activate();
	void DoEvent(TEventUI& event);
	CMenuWnd* GetMenuWnd();
	void CreateMenuWnd();

protected:
	CMenuWnd* m_pWindow;
};

} // namespace DuiLib

#endif // __UIMENU_H__
