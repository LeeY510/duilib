#include "StdAfx.h"
#include "UIMenu.h"

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//
ContextMenuObserver s_context_menu_observer;

CMenuUI::CMenuUI() :m_pOwnWnd(NULL)
{
	if (GetHeader() != NULL)
		GetHeader()->SetVisible(false);
}

CMenuUI::~CMenuUI()
{}

LPCTSTR CMenuUI::GetClass() const
{
    return _T("MenuUI");
}

LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, DUI_CTR_MENU) == 0 ) return static_cast<CMenuUI*>(this);
    return CListUI::GetInterface(pstrName);
}

void CMenuUI::DoEvent(TEventUI& event)
{

	return __super::DoEvent(event);
}

bool CMenuUI::Add(CControlUI* pControl)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
	if (pMenuItem == NULL)
		return false;

	for (int i = 0; i < pMenuItem->GetCount(); ++i)
	{
		if (pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
		{
			(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(false);
		}
	}
	return CListUI::Add(pControl);
}

bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
	if (pMenuItem == NULL)
		return false;

	for (int i = 0; i < pMenuItem->GetCount(); ++i)
	{
		if (pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
		{
			(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(false);
		}
	}
	return CListUI::AddAt(pControl, iIndex);
}

int CMenuUI::GetItemIndex(CControlUI* pControl) const
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
	if (pMenuItem == NULL)
		return -1;

	return __super::GetItemIndex(pControl);
}

bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
	if (pMenuItem == NULL)
		return false;

	return __super::SetItemIndex(pControl, iIndex);
}

bool CMenuUI::Remove(CControlUI* pControl)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
	if (pMenuItem == NULL)
		return false;

	return __super::Remove(pControl);
}

SIZE CMenuUI::EstimateSize(SIZE szAvailable)
{
    int cxFixed = 0;
    int cyFixed = 0;
    for( int it = 0; it < GetCount(); it++ )
	{
        CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
        if( !pControl->IsVisible() ) continue;
        SIZE sz = pControl->EstimateSize(szAvailable);
        cyFixed += sz.cy;

		if (cxFixed < sz.cx)
		{
			cxFixed = sz.cx;
		}
    }

    if (cxFixed > (m_cxyFixed.cx - (m_rcInset.left + m_rcInset.right)))
    {
        cxFixed += m_rcInset.left + m_rcInset.right;
    }

    if (cxFixed < m_cxyFixed.cx)
    {
        cxFixed = m_cxyFixed.cx;
    }

    if (m_cxyFixed.cy == 0)
    {
        cyFixed += m_rcInset.top + m_rcInset.bottom;
    }
    else
    {
        cyFixed = m_cxyFixed.cy;
    }

    if (cxFixed > szAvailable.cx) {
        cxFixed = szAvailable.cx;
    }

    if (cyFixed > szAvailable.cy) {
        cyFixed = szAvailable.cy;
    }

    return CSize(cxFixed, cyFixed);
}

void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CListUI::SetAttribute(pstrName, pstrValue);
}

void CMenuUI::SetOwnWnd(CMenuWnd *pWnd)
{
	m_pOwnWnd = pWnd;
}

CMenuWnd *CMenuUI::GetOwnWnd()
{
	return m_pOwnWnd;
}
/////////////////////////////////////////////////////////////////////////////////////
//
class CMenuBuilderCallback: public IDialogBuilderCallback
{
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, DUI_CTR_MENU) == 0)
		{
			return new CMenuUI();
		}
		else if (_tcsicmp(pstrClass, DUI_CTR_MENUELEMENT) == 0)
		{
			return new CMenuElementUI();
		}
		return NULL;
	}
};

CMenuWnd::CMenuWnd(HWND hParent):
m_hParent(hParent),
m_pOwner(NULL),
m_pLayout(),
m_xml(_T("")),
m_sType(_T("xml")),
m_pfunModifyMenu(NULL),
m_pModifyParam(NULL),
m_pMenuClick(NULL),
m_pMenuClickParam(NULL),
m_pTrigger(NULL),
m_pMeunWinEventFunc(NULL)
{}

BOOL CMenuWnd::Receive(ContextMenuParam param)
{
	switch (param.wParam)
	{
	case 1:
		Close();
		break;
	case 2:
		{
			HWND hParent = GetParent(m_hWnd);
			while (hParent != NULL)
			{
				if (hParent == param.hWnd)
				{
					Close();
					break;
				}
				hParent = GetParent(hParent);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}


void CMenuWnd::SetModifyMenuFunc( PMODIFYMENUFUNC pfunModifyMenu, void* params )
{
	m_pfunModifyMenu = pfunModifyMenu;
    m_pModifyParam = params;
}

PMODIFYMENUFUNC CMenuWnd::GetModifyMenuFunc()
{
	return m_pfunModifyMenu;
}

void* CMenuWnd::GetModifyMenuParam()
{
    return m_pModifyParam;
}

void CMenuWnd::SetMenuClickFunc(PMENUCLICK pMenuClick, void* params)
{
	m_pMenuClick = pMenuClick;
    m_pMenuClickParam = params;
}
PMENUCLICK CMenuWnd::GetMenuClickFunc()
{
	return m_pMenuClick;
}

void* CMenuWnd::GetMenuClickParam()
{
    return m_pMenuClickParam;
}

void CMenuWnd::Init(POINT point,STRINGorID xml, LPCTSTR pSkinType)
{
	Init(NULL,xml,pSkinType,point);
}

void CMenuWnd::Init(CControlUI* pTrigger, CMenuElementUI* pOwner, POINT point, STRINGorID xml, LPCTSTR pSkinType /*= _T("xml")*/)
{
	m_pTrigger = pTrigger;
	Init(pOwner, xml, pSkinType, point);
}

void CMenuWnd::Init(CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point)
{
	m_BasedPoint = point;
    m_pOwner = pOwner;
    m_pLayout = NULL;

	if (pSkinType != NULL)
		m_sType = pSkinType;
    if (NULL != pOwner) {
        CMenuUI *pMenuUI = static_cast<CMenuUI *>(pOwner->GetParent()->GetParent());
        ASSERT(pMenuUI);
        CControlUI* pTrigger = pMenuUI->GetOwnWnd()->GetTrigger();
        m_pTrigger = pTrigger;
    }

	m_xml = xml;

	s_context_menu_observer.AddReceiver(this);

    Create((m_pOwner == NULL) ? m_hParent : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());
    // HACK: Don't deselect the parent's caption
    HWND hWndParent = m_hWnd;
    while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
    ::ShowWindow(m_hWnd, SW_SHOW);
#if defined(WIN32) && !defined(UNDER_CE)
    ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
#endif	
}


LPCTSTR CMenuWnd::GetWindowClassName() const
{
    return _T("MenuWnd");
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	if( m_pOwner != NULL ) {
		for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
			if( static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)) != NULL ) {
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pOwner->GetParent());
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(false);
			}
		}
		m_pOwner->m_pWindow = NULL;
		m_pOwner->m_uButtonState &= ~ UISTATE_PUSHED;
		m_pOwner->Invalidate();
	}
    else {
        if (m_pTrigger) m_pTrigger->GetManager()->SendNotify(m_pTrigger, DUI_MSGTYPE_MENUCLOSE, 0, 0, false);
    }
    delete this;
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_CREATE ) {
        bool bShowShadow = false;
		if( m_pOwner != NULL) {
			LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
			styleValue &= ~WS_CAPTION;
			::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
			RECT rcClient;
			::GetClientRect(*this, &rcClient);
			::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
				rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

			m_pm.Init(m_hWnd);
			// The trick is to add the items to the new container. Their owner gets
			// reassigned by this operation - which is why it is important to reassign
			// the items back to the righfull owner/manager when the window closes.
			m_pLayout = new CMenuUI();
			m_pLayout->SetName(m_pOwner->GetName());
			m_pLayout->SetOwnWnd(this);
			m_pm.UseParentResource(m_pOwner->GetManager());
			m_pLayout->SetManager(&m_pm, NULL, true);
			LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(DUI_CTR_MENU);
			if( pDefaultAttributes ) {
				m_pLayout->ApplyAttributeList(pDefaultAttributes);
            }
            else {
                m_pLayout->SetBkColor(0xFFeeeeee);
                m_pLayout->SetBorderColor(0xFF85E4FF);
                m_pLayout->SetBorderSize(0);
            }
			
			m_pLayout->SetAutoDestroy(false);
			//m_pLayout->EnableScrollBar();
			for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
				if(m_pOwner->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL ){
					//(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pLayout);
					m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
				}
			}

            //添加阴影
            CShadowUI *pShadow = m_pm.GetShadow();
            m_pOwner->GetManager()->GetShadow()->CopyShadow(pShadow);
            bShowShadow = pShadow->IsShowShadow();
            pShadow->ShowShadow(false);

			m_pm.AttachDialog(m_pLayout);

			if(m_pfunModifyMenu)
                m_pfunModifyMenu(m_pLayout, m_pModifyParam);
			// Position the popup window in absolute space
			RECT rcOwner = m_pOwner->GetPos();
			RECT rc = rcOwner;	
            RECT rcInset = m_pLayout->GetInset();

#if defined(WIN32) && !defined(UNDER_CE)
			MONITORINFO oMonitor = {}; 
			oMonitor.cbSize = sizeof(oMonitor);
			::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
			CDuiRect rcWork = oMonitor.rcMonitor;
#else
			CDuiRect rcWork;
			GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
			SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
            SIZE sz = m_pLayout->EstimateSize(szAvailable);
            int cxFixed = sz.cx;
            int cyFixed = sz.cy;

			RECT rcWindow;
			GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWindow);

			rc.top = rcOwner.top - rcInset.top;
			rc.bottom = rc.top + cyFixed;
			::MapWindowRect(m_pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
			rc.left = rcWindow.right;
			rc.right = rc.left + cxFixed;

			bool bReachBottom = false;
			bool bReachRight = false;
			LONG chRightAlgin = 0;
			LONG chBottomAlgin = 0;

			RECT rcPreWindow = {0};
			ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(s_context_menu_observer);
			ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
			while( pReceiver != NULL ) {
				CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
				if( pContextMenu != NULL ) {
					GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

					bReachRight = rcPreWindow.left >= rcWindow.right;
					bReachBottom = rcPreWindow.top >= rcWindow.bottom;
					if( pContextMenu->GetHWND() == m_pOwner->GetManager()->GetPaintWindow() 
						||  bReachBottom || bReachRight )
						break;
				}
				pReceiver = iterator.next();
			}

			if (bReachBottom)
			{
				rc.bottom = rcWindow.top;
				rc.top = rc.bottom - cyFixed;
			}

			if (bReachRight)
			{
				rc.right = rcWindow.left;
				rc.left = rc.right - cxFixed;
			}

			if( rc.bottom > rcWork.bottom )
			{
				rc.bottom = rc.top;
				rc.top = rc.bottom - cyFixed;
			}

            if (rc.top < rcWork.top)
            {
                if (cyFixed >= rcWork.GetHeight()) {
                    rc.top = rcWork.top;
                    rc.bottom = rcWork.bottom;
                }
                else {
                    rc.top = rcWork.top + (rcWork.GetHeight() - cyFixed) / 2;
                    rc.bottom = rc.top + cyFixed;
                }
            }

			if (rc.right > rcWork.right)
			{
				rc.right = rcWindow.left;
				rc.left = rc.right - cxFixed;
			}			

			if (rc.left < rcWork.left)
			{
				rc.left = rcWindow.right;
				rc.right = rc.left + cxFixed;
			}

			MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
		}
		else {
			m_pm.Init(m_hWnd);

			CDialogBuilder builder;
			CMenuBuilderCallback menuCallback;

			CMenuUI* pRoot = static_cast<CMenuUI *>(builder.Create(m_xml, m_sType.GetData(), &menuCallback, &m_pm));
            //阴影
            CShadowUI *pShadow = m_pm.GetShadow();
            bShowShadow = pShadow->IsShowShadow();
            pShadow->ShowShadow(false);

            m_pm.AttachDialog(pRoot);
			pRoot->SetOwnWnd(this);
			if(m_pfunModifyMenu)
                m_pfunModifyMenu(pRoot, m_pModifyParam);
#if defined(WIN32) && !defined(UNDER_CE)
			MONITORINFO oMonitor = {}; 
			oMonitor.cbSize = sizeof(oMonitor);
			::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
			CDuiRect rcWork = oMonitor.rcMonitor;
#else
			CDuiRect rcWork;
			GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
			SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
			szAvailable = pRoot->EstimateSize(szAvailable);
			m_pm.SetInitSize(szAvailable.cx, szAvailable.cy);

			SIZE szInit = m_pm.GetInitSize();
			CDuiRect rc;
			CPoint point = m_BasedPoint;
			rc.left = point.x;
			rc.top = point.y;
			rc.right = rc.left + szInit.cx;
			rc.bottom = rc.top + szInit.cy;

			int nWidth = rc.GetWidth();
			int nHeight = rc.GetHeight();

			if (rc.right > rcWork.right)
			{
				rc.right = point.x;
				rc.left = rc.right - nWidth;
			}

			if (rc.bottom > rcWork.bottom)
			{
				rc.bottom = point.y;
				rc.top = rc.bottom - nHeight;
			}

            if (rc.top < rcWork.top)
            {
                if (nHeight >= rcWork.GetHeight()) {
                    rc.top = rcWork.top;
                    rc.bottom = rcWork.bottom;
                }
                else {
                    rc.top = rcWork.top + (rcWork.GetHeight() - nHeight) / 2;
                    rc.bottom = rc.top + nHeight;
                }
            }

			SetForegroundWindow(m_hWnd);
			MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
			SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), SWP_SHOWWINDOW);
		}
        //显示阴影
        m_pm.GetShadow()->ShowShadow(bShowShadow);
        m_pm.GetShadow()->Create(&m_pm);
		return 0;
    }
    else if( uMsg == WM_CLOSE ) {
		if( m_pOwner != NULL )
		{
			m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
			m_pOwner->SetPos(m_pOwner->GetPos());
			m_pOwner->SetFocus();
		}
	}
	else if( uMsg == WM_RBUTTONDOWN || uMsg == WM_CONTEXTMENU || uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDBLCLK )
	{
		return 0L;
	}
	else if( uMsg == WM_KILLFOCUS )
	{
		HWND hFocusWnd = (HWND)wParam;
		BOOL bInMenuWindowList = FALSE;
		ContextMenuParam param;
		param.hWnd = GetHWND();

		ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(s_context_menu_observer);
		ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
		while( pReceiver != NULL ) {
			CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
			if( pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd ) {
				bInMenuWindowList = TRUE;
				break;
			}
			pReceiver = iterator.next();
		}

		if( !bInMenuWindowList ) {
			param.wParam = 1;
//             if(m_pTrigger)
//                 m_pTrigger->GetManager()->SendNotify(m_pTrigger, DUI_MSGTYPE_MENUCLOSE, 0, 0, true);
			s_context_menu_observer.RBroadcast(param);
			return 0;
		}
	}
	else if( uMsg == WM_KEYDOWN)
	{
		if( wParam == VK_ESCAPE)
		{
			Close();
		}
	}

    if (NULL != m_pMeunWinEventFunc && NULL != m_pTrigger) {
        m_pMeunWinEventFunc(m_pTrigger, uMsg, wParam, lParam);
    }

    LRESULT lRes = 0;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMenuWnd::SetMenuWinEventFunc(PMENUWINEVENTFUNC pMeunWinEventFunc)
{
    m_pMeunWinEventFunc = pMeunWinEventFunc;
}

PMENUWINEVENTFUNC CMenuWnd::GetMenuWinEventFunc()
{
    return m_pMeunWinEventFunc;
}

/////////////////////////////////////////////////////////////////////////////////////
CMenuElementUI::CMenuElementUI():
m_pWindow(NULL)
{
	m_cxyFixed.cy = 25;
	m_bMouseChildEnabled = true;

	SetMouseChildEnabled(false);
}

CMenuElementUI::~CMenuElementUI()
{}

LPCTSTR CMenuElementUI::GetClass() const
{
	return _T("MenuElementUI");
}

LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, DUI_CTR_MENUELEMENT) == 0 ) return static_cast<CMenuElementUI*>(this);    
    return CListContainerElementUI::GetInterface(pstrName);
}

void CMenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
	CMenuElementUI::DrawItemBk(hDC, m_rcItem);
	DrawItemText(hDC, m_rcItem);
	for (int i = 0; i < GetCount(); ++i)
	{
		if (GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) == NULL)
			GetItemAt(i)->DoPaint(hDC, rcPaint);
	}
}

void CMenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
    if( m_sText.IsEmpty() ) return;

    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iTextColor = pInfo->dwHotTextColor;
    }
    if( IsSelected() ) {
        iTextColor = pInfo->dwSelectedTextColor;
    }
    if( !IsEnabled() ) {
        iTextColor = pInfo->dwDisabledTextColor;
    }
    int nLinks = 0;
    RECT rcText = rcItem;
    rcText.left += pInfo->rcTextPadding.left;
    rcText.right -= pInfo->rcTextPadding.right;
    rcText.top += pInfo->rcTextPadding.top;
    rcText.bottom -= pInfo->rcTextPadding.bottom;

    if( pInfo->bShowHtml )
        CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        NULL, NULL, nLinks, DT_SINGLELINE | pInfo->uTextStyle);
    else
        CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
}


SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
{
	SIZE cXY = {0};
	for( int it = 0; it < GetCount(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
		if( !pControl->IsVisible() ) continue;
		SIZE sz = pControl->EstimateSize(szAvailable);
		cXY.cy += sz.cy;
		if( cXY.cx < sz.cx )
			cXY.cx = sz.cx;
	}
	if(cXY.cy == 0) {
		TListInfoUI* pInfo = m_pOwner->GetListInfo();

		DWORD iTextColor = pInfo->dwTextColor;
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			iTextColor = pInfo->dwHotTextColor;
		}
		if( IsSelected() ) {
			iTextColor = pInfo->dwSelectedTextColor;
		}
		if( !IsEnabled() ) {
			iTextColor = pInfo->dwDisabledTextColor;
		}

		RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
		rcText.left += pInfo->rcTextPadding.left;
		rcText.right -= pInfo->rcTextPadding.right;
		if( pInfo->bShowHtml ) {   
			int nLinks = 0;
			CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, NULL, NULL, nLinks, DT_CALCRECT | pInfo->uTextStyle);
		}
		else {
			CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
		}
		cXY.cx = rcText.right - rcText.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right + 20;
		cXY.cy = rcText.bottom - rcText.top + pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
	}

	if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;

	int iWidth = this->GetFixedWidth();
	int iHeight = this->GetFixedHeight();

	if (iWidth > cXY.cx)
	{
		cXY.cx = iWidth;
	}

	if (iHeight > cXY.cy)
	{
		cXY.cy = iHeight;
	}

    if (cXY.cx > this->GetMaxWidth()) {
        cXY.cx = this->GetMaxWidth();
    }

	return cXY;
}

void CMenuElementUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_MOUSEENTER )
	{
        if (!IsEnabled()) return;

		CListContainerElementUI::DoEvent(event);
		if( m_pWindow ) return;
		bool hasSubMenu = false;
		for( int i = 0; i < GetCount(); ++i )
		{
			if( GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL )
			{
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetVisible(true);
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(true);

				hasSubMenu = true;
			}
		}
		if( hasSubMenu )
		{
			m_pOwner->SelectItem(GetIndex(), true);
			CreateMenuWnd();
		}
		else
		{
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 2;
			s_context_menu_observer.RBroadcast(param);
			m_pOwner->SelectItem(GetIndex(), true);
		}
		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN || (event.Type == UIEVENT_KEYDOWN && event.chKey == '\r'))
	{
		if( IsEnabled() ){
			CListContainerElementUI::DoEvent(event);

			if( m_pWindow ) return;

			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i ) {
				if( GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL ) {
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(true);

					hasSubMenu = true;
				}
			}
			if( hasSubMenu )
			{
				CreateMenuWnd();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 1;
				s_context_menu_observer.RBroadcast(param);

                CControlUI* pSender = event.pSender;
                if (NULL == pSender) {
                    pSender = this;
                }

				CMenuUI *pMenuUI = static_cast<CMenuUI *>(GetParent()->GetParent());
				ASSERT(pMenuUI);
				CControlUI* pTrigger = pMenuUI->GetOwnWnd()->GetTrigger();
				if (NULL != pTrigger)
				{
					pTrigger->GetManager()->SendNotify(pTrigger, DUI_MSGTYPE_MENUITEMCLICK, (WPARAM)pSender, 0, true);
				}
				else
				{
					PMENUCLICK menuclick = pMenuUI->GetOwnWnd()->GetMenuClickFunc();
                    if (menuclick)menuclick(pSender, pMenuUI->GetOwnWnd()->GetMenuClickParam());
				}
			}
        }
        return;
    }

    CListContainerElementUI::DoEvent(event);
}

bool CMenuElementUI::Activate()
{
	if (CListContainerElementUI::Activate() && m_bSelected)
	{
		if( m_pWindow ) return true;
		bool hasSubMenu = false;
		for (int i = 0; i < GetCount(); ++i)
		{
			if (GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
			{
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetVisible(true);
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(true);

				hasSubMenu = true;
			}
		}
		if (hasSubMenu)
		{
			CreateMenuWnd();
		}
		else
		{
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 1;
			s_context_menu_observer.RBroadcast(param);
		}

		return true;
	}
	return false;
}

CMenuWnd* CMenuElementUI::GetMenuWnd()
{
	return m_pWindow;
}

void CMenuElementUI::CreateMenuWnd()
{
	if( m_pWindow ) return;

	m_pWindow = new CMenuWnd(m_pManager->GetPaintWindow());
	ASSERT(m_pWindow);
	CMenuUI *pMenuUI = static_cast<CMenuUI *>(GetParent()->GetParent());
	ASSERT(pMenuUI);
    m_pWindow->SetModifyMenuFunc(pMenuUI->GetOwnWnd()->GetModifyMenuFunc(), pMenuUI->GetOwnWnd()->GetModifyMenuParam());
    m_pWindow->SetMenuClickFunc(pMenuUI->GetOwnWnd()->GetMenuClickFunc(), pMenuUI->GetOwnWnd()->GetMenuClickParam());
	ContextMenuParam param;
	param.hWnd = m_pManager->GetPaintWindow();
	param.wParam = 2;
	s_context_menu_observer.RBroadcast(param);

	m_pWindow->Init(static_cast<CMenuElementUI*>(this), _T(""), _T(""), CPoint());
}


} // namespace DuiLib
