/************************************************************************/
/*  Desc  ：菜单
Author ：Skilla（QQ：848861075）
Data   ：2014. 12.22                           


标签:MenuContainer（继承自CContainerUI）  菜单容器    属性interval（int）：各级菜单间隙      keyevent（bool）：是否开启键盘事件
标签:MenuUI （继承自CListUI）                   菜单         属性major（bool） ：是否为主菜单      bktrans（bool）  ：是否开启窗口透明
标签:MenuItem(继承自CListContainerElementUI)  菜单项  属性  command(string) 菜单项命令id，不可重复（菜单结束时会返回）   extendmenu（string）    子菜单名称（这个属性是菜单级联的关键）  
标签:StaticMenuItem(继承自CListContainerElementUI) 静态菜单项  （包括菜单分割线，其他和菜单项无关的控件）


*/
/************************************************************************/
#ifndef  __UIMENU_H__
#define __UIMENU_H__

#pragma once
namespace DuiLib {

	struct MenuReturn
	{
		bool    bExit;
		CDuiString command;
	};

	class CMenuUI;
	class UILIB_API CMenuElementUI : public CListContainerElementUI
	{
	public:
		CMenuElementUI();
		//初始化
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetMenuOwner(CMenuUI* owner);
		void SetExtendMenu(CMenuUI* pMenu);
		//获取属性
		CDuiString GetExtendMenuName();
		CMenuUI* GetExtendMenu();
		CDuiString GetMenuCmd();
		bool IsHaveExtend();
		bool IsExtendMenuShow();
		//设置属性
		void SetExtenMenuShow(bool b);
		//响应事件
		void DoEvent(TEventUI& event);
		void DoPaint(HDC hDC, const RECT& rcPaint);
		//功能
		void ShowSubMenu();
		void HideSubMenu();

	protected:
		CDuiString      m_ExtendMenuName;        //子菜单name
		CDuiString      m_MenuCmd;                   //菜单命令
		CMenuUI*        m_pMenuOwner;               //菜单Owner
		CMenuUI*        m_pExtendMenu;              //子菜单
		bool            m_bHaveExtend;               //是否含有子菜单
		bool            m_bExtendMenuShow;       //子菜单是否显示了
	};


	class UILIB_API CStaticMenuElementUI :public CListContainerElementUI
	{
	public:
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoPaint(HDC hDC, const RECT& rcPaint);
	};

	class CMenuUI;
	class CMenuWnd :public CWindowWnd
	{
	public:
		CMenuWnd(void);
		~CMenuWnd(void);
		virtual LPCTSTR GetWindowClassName() const;
		LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
		LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual void OnFinalMessage(HWND hWnd);
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void BindControl(CControlUI* pControl);
		void UnBindControl(CControlUI* pControl);
		void ShowMenuWnd(bool b);
		void AddNotifier(INotifyUI* pNotify);
		void SetMenuOwner(CMenuUI* pOwner);
		void SetBktrans(bool b);

	protected:
		CPaintManagerUI m_PaintManager;
		CMenuUI*          m_pOwner;
	};

	class CDuiMenu;
	class UILIB_API CMenuUI :public CListUI, public INotifyUI
	{
	public:
		CMenuUI();
		~CMenuUI();
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetSuperMenu(CMenuUI* pSuper);
		void SetOwnerTree(CDuiMenu* pMenuTree);
		CDuiMenu* GetOwnerTree();

		void InitMenu(HWND ownerWnd);
		bool Add(CControlUI* pControl);

		bool  IsMajor();
		bool IsExtenMenuShow();
		CMenuUI* GetCurExtendMenu();
		CMenuWnd* GetMenuWnd();


		void ShowMenu(POINT p);
		void HideMenu();
		void OnItemHover(CControlUI* pControl);
		void OnItemLeave(CControlUI* pControl);
		void OnMenuItemClick(CControlUI* pControl);
		void Notify(TNotifyUI& msg) {}


		virtual void OnItemHot(CMenuElementUI* pMenuItem);
		virtual void OnItemUnHot(CMenuElementUI* pMenuItem);

		void SetCurItem(CMenuElementUI* pMenuItem);
		CMenuElementUI* GetCurItem();

		void ShowSubMenu(CMenuElementUI* pMenuItem);
		CMenuElementUI* GetNextItem();
		CMenuElementUI* GetPrevItem();

		//响应键盘事件
		void OnVkLeft();
		void OnVkRight();
		void OnVkUp();
		void OnVkDown();
		void OnVkReturn();

	protected:
		bool            m_bMajor;
		bool            m_bSubMenuShow;
		CMenuUI*    m_pCurSubMenu;
		CMenuWnd* m_pMenuWnd;
		CDuiMenu*   m_pOwnerTree;
		CMenuElementUI*  m_pCurItem;
		CMenuUI*    m_pSuperMenu;
	};


	class UILIB_API CMenuContainer : public CContainerUI
	{
	public:
		CMenuContainer();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		int GetInterval();
		bool IsKeyEvent()
		{
			return m_bKeyEvent;
		}
	protected:
		int  m_nInterval;
		bool m_bKeyEvent;
	};


	class UILIB_API CDuiMenu : public IDialogBuilderCallback
	{
	public:
		CDuiMenu(LPCTSTR xml);
		~CDuiMenu();
		CControlUI* CreateControl(LPCTSTR pstrClass);
		LPCTSTR TrackPopupMenu(HWND hwndParent, int x, int y);
		void DestroyMenu();
		bool IsMenuWnd(HWND hwnd);
		int   RunMenu();
		void ExitMenu(LPCTSTR cmd);
		void SetKeyEventMenu(CMenuUI* pMenu);
		CMenuContainer* GetMenuContainer();
	protected:
		void LoadFromXml(LPCTSTR xmlName);
		CMenuUI* FindMenuByName(LPCTSTR pName);

		CDuiString                m_xmlName;
		HWND                      m_hWndOwner;
		CMenuUI*                  m_pMajorMenu;
		CMenuUI*                  m_pKeyEvent;
		CMenuContainer*           m_menus;
		bool                      m_bMenusExsit;
		MenuReturn                m_menuRet;
	};

}
#endif