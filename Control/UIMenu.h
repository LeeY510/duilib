/************************************************************************/
/*  Desc  ���˵�
Author ��Skilla��QQ��848861075��
Data   ��2014. 12.22                           


��ǩ:MenuContainer���̳���CContainerUI��  �˵�����    ����interval��int���������˵���϶      keyevent��bool�����Ƿ��������¼�
��ǩ:MenuUI ���̳���CListUI��                   �˵�         ����major��bool�� ���Ƿ�Ϊ���˵�      bktrans��bool��  ���Ƿ�������͸��
��ǩ:MenuItem(�̳���CListContainerElementUI)  �˵���  ����  command(string) �˵�������id�������ظ����˵�����ʱ�᷵�أ�   extendmenu��string��    �Ӳ˵����ƣ���������ǲ˵������Ĺؼ���  
��ǩ:StaticMenuItem(�̳���CListContainerElementUI) ��̬�˵���  �������˵��ָ��ߣ������Ͳ˵����޹صĿؼ���


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
		//��ʼ��
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetMenuOwner(CMenuUI* owner);
		void SetExtendMenu(CMenuUI* pMenu);
		//��ȡ����
		CDuiString GetExtendMenuName();
		CMenuUI* GetExtendMenu();
		CDuiString GetMenuCmd();
		bool IsHaveExtend();
		bool IsExtendMenuShow();
		//��������
		void SetExtenMenuShow(bool b);
		//��Ӧ�¼�
		void DoEvent(TEventUI& event);
		void DoPaint(HDC hDC, const RECT& rcPaint);
		//����
		void ShowSubMenu();
		void HideSubMenu();

	protected:
		CDuiString      m_ExtendMenuName;        //�Ӳ˵�name
		CDuiString      m_MenuCmd;                   //�˵�����
		CMenuUI*        m_pMenuOwner;               //�˵�Owner
		CMenuUI*        m_pExtendMenu;              //�Ӳ˵�
		bool            m_bHaveExtend;               //�Ƿ����Ӳ˵�
		bool            m_bExtendMenuShow;       //�Ӳ˵��Ƿ���ʾ��
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

		//��Ӧ�����¼�
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