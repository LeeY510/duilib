#include "StdAfx.h"
#include "UITabs.h"

#pragma warning( disable: 4251 )
namespace DuiLib
{

    CTabPaneUI::CTabPaneUI() :
         m_dwTextColor(0)
        , m_dwHotTextColor(0)
        , m_dwSelectedTextColor(0)
        , m_dwDisabledTextColor(0)

        , m_dwBkColor(0)
        , m_dwHotBkColor(0)
        , m_dwSelectedBkColor(0)

        , m_iFont(-1)
        , m_bSelected(false)
        , m_bVisibleTips(true)

        , m_pTabs(NULL)
        , m_pIcon(NULL)
        , m_pText(NULL)
        , m_pClose(NULL)
    {
        this->SetMinWidth(50);
        this->SetMaxWidth(200);
        RECT rc = { 4,0,4,0 };
        this->SetInset(rc);
        m_pIcon = new CControlUI();
        m_pIcon->SetFixedWidth(14);
        m_pIcon->SetFixedHeight(14);
        m_pText = new CLabelUI();
        m_pText->SetTextStyle(DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        m_pClose = new CButtonUI();
        m_pClose->SetFixedWidth(14);
        m_pClose->SetFixedHeight(14);
        m_pClose->SetToolTip(L"关闭");
        this->Add(m_pIcon, false);
        this->Add(m_pText, false);
        this->Add(m_pClose, false);
    }

    CTabPaneUI::~CTabPaneUI(void){

    }

    LPCTSTR CTabPaneUI::GetClass() const{
        return _T("TabPaneUI");
    }

    LPVOID	CTabPaneUI::GetInterface(LPCTSTR pstrName){
        if (_tcscmp(pstrName, DUI_CTR_TABPANE) == 0)
            return static_cast<CTabPaneUI*>(this);
        return CHorizontalLayoutUI::GetInterface(pstrName);
    }

    void	CTabPaneUI::Invalidate(){
        CHorizontalLayoutUI::Invalidate();
    }

    CDuiString CTabPaneUI::GetText() const{
        return m_pText->GetText();
    }

    void    CTabPaneUI::SetText(LPCTSTR pstrText){
        m_pText->SetText(pstrText);
        if (!m_bVisibleTips) {
            m_pIcon->SetToolTip(L"");
            m_pText->SetToolTip(L"");
        }
        else {
            m_pIcon->SetToolTip(pstrText);
            m_pText->SetToolTip(pstrText);
        }
    }

    void	CTabPaneUI::SetVisibleIcon(bool bIsVisible){
        m_pIcon->SetVisible(bIsVisible);
    }

    bool	CTabPaneUI::IsVisibleIcon(){
        return m_pIcon->IsVisible();
    }

    void   CTabPaneUI::SetIconImage(LPCTSTR pstrImage){
        m_pIcon->SetBkImage(pstrImage);
    }

    CDuiString CTabPaneUI::GetIconImage() const {
        return m_pIcon->GetBkImage();
    }

    void	CTabPaneUI::SetVisibleCloseBtn(bool bIsVisible){
        m_pClose->SetVisible(bIsVisible);
    }

    bool	CTabPaneUI::IsVisibleCloseBtn(){
        return m_pClose->IsVisible();
    }

    CDuiString CTabPaneUI::GetHotCloseBtnImage(){
        return m_pClose->GetHotImage();
    }

    void    CTabPaneUI::SetHotCloseBtnImage(LPCTSTR pStrImage){
        m_pClose->SetHotImage(pStrImage);
    }

    CDuiString CTabPaneUI::GetNormalCloseBtnImage(){
        return m_pClose->GetNormalImage();
    }

    void    CTabPaneUI::SetNormalCloseBtnImage(LPCTSTR pStrImage){
        m_pClose->SetNormalImage(pStrImage);
    }

    CDuiString CTabPaneUI::GetPushedCloseBtnImage(){
        return m_pClose->GetPushedImage();
    }

    void    CTabPaneUI::SetPushedCloseBtnImage(LPCTSTR pStrImage){
        m_pClose->SetPushedImage(pStrImage);
    }

    CControlUI* CTabPaneUI::GetIconCtrl() const{
        return m_pIcon;
    }

    CLabelUI* CTabPaneUI::GetTextCtrl() const{
        return m_pText;
    }

    CButtonUI* CTabPaneUI::GetCloseBtn() const {
        return m_pClose;
    }

    void CTabPaneUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if (_tcscmp(pstrName, _T("text")) == 0)
            SetText(pstrValue);
        else if (_tcscmp(pstrName, _T("iconattr")) == 0)
            m_pIcon->ApplyAttributeList(pstrValue);
        else if (_tcscmp(pstrName, _T("textattr")) == 0)
            m_pText->ApplyAttributeList(pstrValue);
        else if (_tcscmp(pstrName, _T("closeattr")) == 0)
            m_pClose->ApplyAttributeList(pstrValue);
        else if (_tcscmp(pstrName, _T("iconimage")) == 0)
            SetIconImage(pstrValue);
        else if (_tcscmp(pstrName, _T("visibleicon")) == 0)
            SetVisibleIcon(_tcscmp(pstrValue, _T("true")) == 0);
        else if (_tcscmp(pstrName, _T("visibleclose")) == 0)
            SetVisibleCloseBtn(_tcscmp(pstrValue, _T("true")) == 0);
        else if (_tcscmp(pstrName, _T("closenormalimage")) == 0)
            SetNormalCloseBtnImage(pstrValue);
        else if (_tcscmp(pstrName, _T("closehotimage")) == 0)
            SetHotCloseBtnImage(pstrValue);
        else if (_tcscmp(pstrName, _T("closepushedimage")) == 0)
            SetPushedCloseBtnImage(pstrValue);
        else if (_tcscmp(pstrName, _T("textcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetTextColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("hottextcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotTextColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("selectedtextcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedTextColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("bkcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetNormalBkColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("hotbkcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotBkColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("selectedbkcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedBkColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("selected")) == 0) {
            bool bSelected = _tcscmp(pstrValue, _T("true")) == 0;
            Selected(bSelected);
            if (bSelected && m_pTabs) {
                m_pTabs->PushItemSelected(this);
            }
        }            
        else if (_tcscmp(pstrName, _T("font")) == 0) SetFont(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("textpadding")) == 0) {
            RECT rcTextPadding = { 0 };
            LPTSTR pstr = NULL;
            rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
            rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
            rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
            rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
            SetTextPadding(rcTextPadding);
        }
        else if (_tcscmp(pstrName, _T("visibletips")) == 0)
            SetVisibleTips(_tcscmp(pstrValue, _T("true")) == 0);

        else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
    }

    void CTabPaneUI::PaintText(HDC hDC)
    {
        if (m_pText->GetText().IsEmpty()) return;

        int iFont = GetFont();

        if (iFont != m_pText->GetFont())
            m_pText->SetFont(iFont);

        if (!IsEnabled()) {
            if (m_dwDisabledTextColor == 0) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();            
            if(m_pText->GetTextColor() != m_dwDisabledTextColor)
                m_pText->SetTextColor(m_dwDisabledTextColor);
            return;
        }

        DWORD dwTextColor = 0;
        if ((m_uButtonState & UISTATE_SELECTED) != 0) {
            if (m_dwSelectedTextColor != 0) dwTextColor = m_dwSelectedTextColor;
            if (0 == dwTextColor) dwTextColor = m_dwTextColor;
        }
        else if ((m_uButtonState & UISTATE_HOT) != 0) {
            if (m_dwHotTextColor != 0) dwTextColor = m_dwHotTextColor;
            if (0 == dwTextColor) dwTextColor = m_dwTextColor;
        }
        else {
            dwTextColor = m_dwTextColor;
        }

        if (0 == dwTextColor) {
            dwTextColor = m_pManager->GetDefaultFontColor();
        }

        if (m_pText->GetTextColor() != dwTextColor)
            m_pText->SetTextColor(dwTextColor);
    }

    void CTabPaneUI::SetTextColor(DWORD dwTextColor) {
        m_dwTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CTabPaneUI::GetTextColor() const {
        return m_dwTextColor;
    }

    void CTabPaneUI::SetHotTextColor(DWORD dwTextColor) {
        m_dwHotTextColor = dwTextColor;
    }

    DWORD CTabPaneUI::GetHotTextColor() const{
        return m_dwHotTextColor;
    }

    void CTabPaneUI::SetSelectedTextColor(DWORD dwTextColor) {
        m_dwSelectedTextColor = dwTextColor;
    }

    DWORD CTabPaneUI::GetSelectedTextColor() const {
        return m_dwSelectedTextColor;
    }

    void CTabPaneUI::SetNormalBkColor(DWORD dwBkColor) {
        m_dwBkColor = dwBkColor;
    }

    DWORD CTabPaneUI::GetNormalBkColor() const {
        return m_dwBkColor;
    }

    void CTabPaneUI::SetHotBkColor(DWORD dwBkColor) {
        if (dwBkColor == m_dwHotBkColor) return;
        m_dwHotBkColor = dwBkColor;
    }

    DWORD CTabPaneUI::GetHotBkColor() const {
        return m_dwHotBkColor;
    }

    void CTabPaneUI::SetSelectedBkColor(DWORD dwBkColor) {
        if (dwBkColor == m_dwSelectedBkColor) return;
        m_dwSelectedBkColor = dwBkColor;
    }

    DWORD CTabPaneUI::GetSelectedBkColor() const {
        return m_dwSelectedBkColor;
    }

    bool CTabPaneUI::IsSelected() const{
        return m_bSelected;
    }

    void CTabPaneUI::Selected(bool bSelected){
        if (m_bSelected == bSelected) return;
        m_bSelected = bSelected;
        if (m_bSelected) m_uButtonState |= UISTATE_SELECTED;
        else m_uButtonState &= ~UISTATE_SELECTED;

        if (m_pManager != NULL) {
            if (!m_sGroupName.IsEmpty()) {
                if (m_bSelected) {
                    CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
                    for (int i = 0; i < aOptionGroup->GetSize(); i++) {
                        CTabPaneUI* pControl = static_cast<CTabPaneUI*>(aOptionGroup->GetAt(i));
                        if (pControl != this) {
                            pControl->Selected(false);
                        }
                    }
                    m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
                }
            }
            else {
                m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
            }
        }
        
        Invalidate();        
    }

    LPCTSTR CTabPaneUI::GetGroup() const
    {
        return m_sGroupName;
    }

    void CTabPaneUI::SetGroup(LPCTSTR pStrGroupName)
    {
        if (pStrGroupName == NULL) {
            if (m_sGroupName.IsEmpty()) return;
            m_sGroupName.Empty();
        }
        else {
            if (m_sGroupName == pStrGroupName) return;
            if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
            m_sGroupName = pStrGroupName;
        }

        if (!m_sGroupName.IsEmpty()) {
            if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
        }
        else {
            if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
        }

        Selected(m_bSelected);
    }

    void CTabPaneUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
    {
        CControlUI::SetManager(pManager, pParent, bInit);

        for (int it = 0; it < m_items.GetSize(); it++) {
            static_cast<CControlUI*>(m_items[it])->SetManager(pManager, this, bInit);
        }
        
        if (bInit && !m_sGroupName.IsEmpty()) {
            if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
        }
    }

    bool CTabPaneUI::Activate()
    {
        if (!CHorizontalLayoutUI::Activate()) return false;
        if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);        
        if (!m_sGroupName.IsEmpty()) Selected(true);
        else Selected(!m_bSelected);

        m_pTabs->PushItemSelected(this);
        return true;
    }

    void CTabPaneUI::SetEnabled(bool bEnable)
    {
        m_pText->SetEnabled(bEnable);
        m_pClose->SetEnabled(bEnable);
        CControlUI::SetEnabled(bEnable);
        if (!IsEnabled()) {
            if (m_bSelected) m_uButtonState |= UISTATE_SELECTED;
            else m_uButtonState = 0;
        }
    }

    void CTabPaneUI::DoEvent(TEventUI& event)
    {
        if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
            if (m_pParent != NULL) m_pParent->DoEvent(event);
            else CControlUI::DoEvent(event);
            return;
        }

        if (event.Type == UIEVENT_SETFOCUS)
        {
            Invalidate();
        }
        if (event.Type == UIEVENT_KILLFOCUS)
        {
            Invalidate();
        }
        if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
        {
            if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) {
                m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
                Invalidate();
            }
            return;
        }
        if (event.Type == UIEVENT_MOUSEMOVE)
        {
            if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
                if (::PtInRect(&m_rcItem, event.ptMouse)) m_uButtonState |= UISTATE_PUSHED;
                else m_uButtonState &= ~UISTATE_PUSHED;
                Invalidate();
            }
            return;
        }
        if (event.Type == UIEVENT_BUTTONUP)
        {
            if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
                if (::PtInRect(&m_rcItem, event.ptMouse)) Activate();
                m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
                Invalidate();
            }
            return;
        }
        if (event.Type == UIEVENT_MOUSEENTER)
        {
            if (IsEnabled()) {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
        }
        if (event.Type == UIEVENT_MOUSELEAVE)
        {
            if (IsEnabled()) {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
        }

        CHorizontalLayoutUI::DoEvent(event);
    }

    void CTabPaneUI::PaintStatusImage(HDC hDC)
    {
        if (NULL == m_pTabs || NULL == m_pManager) return;

        //优先绘制不可以图片
        if (!IsEnabled())
        {
            CHorizontalLayoutUI::PaintStatusImage(hDC);
            return;
        }

        DWORD dwBkColor = 0;
        if ((m_uButtonState & UISTATE_SELECTED) != 0) {
            if (m_dwSelectedBkColor != 0) dwBkColor = m_dwSelectedBkColor;
            else dwBkColor = m_pManager->GetDefaultSelectedBkColor();
        }
        else if ((m_uButtonState & UISTATE_HOT) != 0) {
            if (m_dwHotBkColor != 0) dwBkColor = m_dwHotBkColor; 
            else dwBkColor = m_dwBkColor;
        }
        else dwBkColor = m_dwBkColor;

        if (this->GetBkColor() != dwBkColor) {
            this->SetBkColor(dwBkColor);
        }
    }

    SIZE CTabPaneUI::EstimateSize(SIZE szAvailable)
    {
        if (NULL == m_pTabs || NULL == m_pManager) return CSize(0, 0);

        RECT rcInset = this->GetInset();
        int iHeight = m_pTabs->GetHeight();
        RECT rcOwnerInset = m_pTabs->GetInset();
        iHeight = iHeight - rcOwnerInset.top - rcOwnerInset.bottom;

        RECT rcText = { 0, 0, 9999, 9999};
        CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, GetText(), m_dwTextColor, GetFont(), DT_CALCRECT | DT_VCENTER | DT_SINGLELINE);
        
        int iIconWidth = 0;
        if(m_pIcon->IsVisible()) iIconWidth = m_pIcon->GetFixedWidth();

        int iCloseWidth = 0;
        if(m_pClose->IsVisible()) iCloseWidth = m_pClose->GetFixedWidth();

        RECT rcTextPadding = m_pText->GetTextPadding();

        int iWidth = rcText.right - rcText.left + iIconWidth + iCloseWidth + rcInset.left + rcInset.right + rcTextPadding.left + rcTextPadding.right;

        if (iWidth > GetMaxWidth()) {
            iWidth = GetMaxWidth();
        }

        if (iWidth < GetMinWidth()) {
            iWidth = GetMinWidth();
        }

        int iCloseHeight = m_pClose->GetFixedHeight();
        if (iCloseHeight < iHeight) {
            RECT rcClosePadding = m_pClose->GetPadding();
            if (0 == rcClosePadding.top && 0 == rcClosePadding.bottom) {
                rcClosePadding.top = (iHeight - iCloseHeight) / 2;
                m_pClose->SetPadding(rcClosePadding);
            }
        }

        int iIconHeight = m_pIcon->GetFixedHeight();
        if (iIconHeight < iHeight) {
            RECT rcIconPadding = m_pIcon->GetPadding();
            if (0 == rcIconPadding.top && 0 == rcIconPadding.bottom) {
                rcIconPadding.top = (iHeight - iIconHeight) / 2;
                m_pIcon->SetPadding(rcIconPadding);
            }
        }

        int iTextWidth = iWidth - (iIconWidth + iCloseWidth + rcInset.left + rcInset.right);
        if (iTextWidth != m_pText->GetFixedWidth()) {
            m_pText->SetFixedWidth(iTextWidth);
        }
        
        SIZE szXY = { iWidth , iHeight };
        return szXY;
    }

    void CTabPaneUI::SetOwner(CTabsUI* pOwner) {
        m_pTabs = pOwner;
    }

    void CTabPaneUI::SetFont(int index) {
        m_iFont = index;
        m_pText->SetFont(index);
    }

    int  CTabPaneUI::GetFont() const {
        return m_iFont;
    }

    void CTabPaneUI::SetTextPadding(const RECT& rcTextPadding) {
        m_pText->SetTextPadding(rcTextPadding);
    }

    RECT CTabPaneUI::GetTextPadding()const {
        return m_pText->GetTextPadding();
    }

    void CTabPaneUI::SetVisibleTips(bool bIsVisible) {
        m_bVisibleTips = bIsVisible;
        if (!m_bVisibleTips) {
            m_pIcon->SetToolTip(L"");
            m_pText->SetToolTip(L"");
        }else {
            m_pIcon->SetToolTip(m_pText->GetText());
            m_pText->SetToolTip(m_pText->GetText());
        }            
    }

    bool CTabPaneUI::IsVisibleTips() const {
        return m_bVisibleTips;
    }

    /*****************************************************************************/
    /*****************************************************************************/
    /*****************************************************************************/

    CTabsUI::CTabsUI() :
        m_uItemMinWidth(50)
        , m_uItemMaxWidth(200)
        , m_bItemIconVisible(false)
        , m_bItemCloseBtnVisible(false)
        , m_dwItemTextColor(0)
        , m_dwItemHotTextColor(0)
        , m_dwItemSelTextColor(0)

        , m_dwItemBkColor(0)
        , m_dwItemHotBkColor(0)
        , m_dwItemSelBkColor(0)

        , m_iItemfont (-1)
        , m_bItemVisibleTips(true)

        , m_pTabPaneCloseCb(NULL)
        , m_pCbParam(NULL)
    {
        ::ZeroMemory(&m_rcItemInset, sizeof(m_rcItemInset));
        ::ZeroMemory(&m_rcItemTextPadding, sizeof(m_rcItemTextPadding));        
    }

    CTabsUI::~CTabsUI() {

    }

    LPCTSTR CTabsUI::GetClass() const {
        return _T("TabsUI");
    }

    LPVOID	CTabsUI::GetInterface(LPCTSTR pstrName) {
        if (_tcscmp(pstrName, DUI_CTR_TABS) == 0) return static_cast<CTabsUI*>(this);
        return CHorizontalLayoutUI::GetInterface(pstrName);
    }

    bool CTabsUI::Add(CControlUI* pControl) {
        InitTabPane(pControl);

        return CHorizontalLayoutUI::Add(pControl);
    }

    bool CTabsUI::AddAt(CControlUI* pControl, int iIndex) {
        InitTabPane(pControl);
        return CHorizontalLayoutUI::AddAt(pControl, iIndex);
    }

    void CTabsUI::SetItemIconVisible(bool _IsVisibled) {
        m_bItemIconVisible = _IsVisibled;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetVisibleIcon(_IsVisibled);
        }
    }

    void CTabsUI::SetItemCloseBtnVisible(bool _IsVisibled) {
        m_bItemCloseBtnVisible = _IsVisibled;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetVisibleCloseBtn(_IsVisibled);
        }
    }

    void CTabsUI::SetItemMinWidth(UINT uiWidth) {
        m_uItemMinWidth = uiWidth;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetMinWidth(uiWidth);
        }
    }

    void CTabsUI::SetItemMaxWidth(UINT uiWidth) {
        m_uItemMaxWidth = uiWidth;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetMaxWidth(uiWidth);
        }
    }

    void CTabsUI::SetItemTextColor(DWORD _dwItemTextColor) {
        m_dwItemTextColor = _dwItemTextColor;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetTextColor(_dwItemTextColor);
        }
    }

    void CTabsUI::SetItemHotTextColor(DWORD _dwItemHotTextColor) {
        m_dwItemHotTextColor = _dwItemHotTextColor;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetHotTextColor(_dwItemHotTextColor);
        }
    }

    void CTabsUI::SetItemSelTextColor(DWORD _dwSelItemTextColor) {
        m_dwItemSelTextColor = _dwSelItemTextColor;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetSelectedTextColor(_dwSelItemTextColor);
        }
    }

    void CTabsUI::SetItemBkColor(DWORD _dwColor) {
        m_dwItemBkColor = _dwColor;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetNormalBkColor(_dwColor);
        }
    }

    void CTabsUI::SetItemHotBkColor(DWORD _dwColor) {
        m_dwItemHotBkColor = _dwColor;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetHotBkColor(_dwColor);
        }
    }

    void CTabsUI::SetItemSelBkColor(DWORD _dwColor) {
        m_dwItemSelBkColor = _dwColor;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetSelectedBkColor(_dwColor);
        }
    }

    void    CTabsUI::SetItemCloseBtnNormalImage(LPCTSTR pStrImage) {
        m_strItemCloseBtnNormalImage = pStrImage;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetNormalCloseBtnImage(pStrImage);
        }
    }

    void    CTabsUI::SetItemCloseBtnHotImage(LPCTSTR pStrImage) {
        m_strItemCloseBtnHotImage = pStrImage;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetHotCloseBtnImage(pStrImage);
        }
    }

    void  CTabsUI::SetItemCloseBtnPushedImage(LPCTSTR pStrImage) {
        m_strItemCloseBtnPushedImage = pStrImage;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetPushedCloseBtnImage(pStrImage);
        }
    }

    void CTabsUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
        if (_tcscmp(pstrName, _T("itemminwidth")) == 0)
            SetItemMinWidth(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("itemmaxwidth")) == 0)
            SetItemMaxWidth(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("itemiconvisible")) == 0)
            SetItemIconVisible(_tcscmp(pstrValue, _T("true")) == 0);
        else if (_tcscmp(pstrName, _T("itemclosevisible")) == 0)
            SetItemCloseBtnVisible(_tcscmp(pstrValue, _T("true")) == 0);
        else if (_tcscmp(pstrName, _T("itemtextcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemTextColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("itemhottextcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemHotTextColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("itemseltextcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemSelTextColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("itembkcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemBkColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("itemhotbkcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemHotBkColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("itemselbkcolor")) == 0) {
            if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemSelBkColor(clrColor);
        }
        else if (_tcscmp(pstrName, _T("itemclosenormalimage")) == 0)
            SetItemCloseBtnNormalImage(pstrValue);
        else if (_tcscmp(pstrName, _T("itemclosehotimage")) == 0)
            SetItemCloseBtnHotImage(pstrValue);
        else if (_tcscmp(pstrName, _T("itemclosepushedimage")) == 0)
            SetItemCloseBtnPushedImage(pstrValue);
        else if (_tcscmp(pstrName, _T("group")) == 0)
            SetGroup(pstrValue);
        else if (_tcscmp(pstrName, _T("itemfont")) == 0) SetItemFont(_ttoi(pstrValue));
        else if (_tcscmp(pstrName, _T("iteminset")) == 0) {
            RECT rcInset = { 0 };
            LPTSTR pstr = NULL;
            rcInset.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
            rcInset.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
            rcInset.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
            rcInset.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
            SetItemInset(rcInset);
        }
        else if (_tcscmp(pstrName, _T("itemtextpadding")) == 0) {
            RECT rcTextPadding = { 0 };
            LPTSTR pstr = NULL;
            rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
            rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
            rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
            rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
            SetItemTextPadding(rcTextPadding);
        }
        else if (_tcscmp(pstrName, _T("itemiconimage")) == 0)
            SetItemIconImage(pstrValue);
        else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
    }

    void CTabsUI::InitTabPane(CControlUI* pControl)
    {
        CTabPaneUI* pTabPane = (CTabPaneUI*)pControl;

        pTabPane->SetOwner(this);

        if (0 == pTabPane->GetMinWidth()) {
            pTabPane->SetMinWidth(m_uItemMinWidth);
        }

        if (0 == pTabPane->GetMaxWidth()) {
            pTabPane->SetMaxWidth(m_uItemMaxWidth);
        }

        if (m_bItemIconVisible != pTabPane->IsVisibleIcon()) {
            pTabPane->SetVisibleIcon(true);
        }

        if (m_bItemCloseBtnVisible != pTabPane->IsVisibleIcon()) {
            pTabPane->SetVisibleIcon(true);
        }

        if (0 != m_dwItemTextColor && 0 == pTabPane->GetTextColor()) {
            pTabPane->SetTextColor(m_dwItemTextColor);
        }

        if (0 != m_dwItemHotTextColor && 0 == pTabPane->GetHotTextColor()) {
            pTabPane->SetHotTextColor(m_dwItemHotTextColor);
        }

        if (0 != m_dwItemSelTextColor && 0 == pTabPane->GetSelectedTextColor()) {
            pTabPane->SetSelectedTextColor(m_dwItemSelTextColor);
        }

        if (0 != m_dwItemBkColor && 0 == pTabPane->GetNormalBkColor()) {
            pTabPane->SetNormalBkColor(m_dwItemBkColor);
        }

        if (0 != m_dwItemHotBkColor && 0 == pTabPane->GetHotBkColor()) {
            pTabPane->SetHotBkColor(m_dwItemHotBkColor);
        }

        if (0 != m_dwItemSelBkColor && 0 == pTabPane->GetSelectedBkColor()) {
            pTabPane->SetSelectedBkColor(m_dwItemSelBkColor);
        }

        if (-1 != m_iItemfont && -1 == pTabPane->GetFont()) {
            pTabPane->SetFont(m_iItemfont);
        }

        if (m_rcItemInset.left != 0 || m_rcItemInset.top != 0 || m_rcItemInset.right != 0 || m_rcItemInset.bottom != 0) {
            RECT rc = pTabPane->GetInset();
            if (rc.left == 0 && rc.top == 0 && rc.right == 0 && rc.bottom == 0) {
                pTabPane->SetInset(m_rcItemInset);
            }
        }

        if (m_rcItemTextPadding.left != 0 || m_rcItemTextPadding.top != 0 || m_rcItemTextPadding.right != 0 || m_rcItemTextPadding.bottom != 0) {
            RECT rc = pTabPane->GetTextPadding();
            if (rc.left == 0 && rc.top == 0 && rc.right == 0 && rc.bottom == 0) {
                pTabPane->SetTextPadding(m_rcItemInset);
            }
        }

        if (!m_strItemIconImage.IsEmpty() && pTabPane->GetIconImage().IsEmpty()) {
            pTabPane->SetIconImage(m_strItemIconImage);
        }

        if (!m_strItemCloseBtnNormalImage.IsEmpty() && pTabPane->GetNormalCloseBtnImage().IsEmpty()) {
            pTabPane->SetNormalCloseBtnImage(m_strItemCloseBtnNormalImage);
        }

        if (!m_strItemCloseBtnHotImage.IsEmpty() && pTabPane->GetHotCloseBtnImage().IsEmpty()) {
            pTabPane->SetHotCloseBtnImage(m_strItemCloseBtnHotImage);
        }

        if (!m_strItemCloseBtnPushedImage.IsEmpty() && pTabPane->GetPushedCloseBtnImage().IsEmpty()) {
            pTabPane->SetPushedCloseBtnImage(m_strItemCloseBtnPushedImage);
        }

        if (!m_bItemVisibleTips && pTabPane->IsVisibleTips()) {
            pTabPane->SetVisibleTips(true);
        }

        pTabPane->GetCloseBtn()->OnNotify += MakeDelegate(this, &CTabsUI::OnItemClose);

        pTabPane->SetGroup(m_strGroupName);

        m_itemOptStack.InsertAt(0, pControl);
    }

    void CTabsUI::SetGroup(LPCTSTR pstrGroupName)
    {
        m_strGroupName = pstrGroupName;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetGroup(pstrGroupName);
        }
    }

    void CTabsUI::SetItemFont(int iFont)
    {
        m_iItemfont = iFont;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetFont(iFont);
        }
    }

    void CTabsUI::SetItemInset(RECT rcInset) {
        m_rcItemInset = rcInset;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetInset(rcInset);
        }
    }

    void CTabsUI::SetItemTextPadding(RECT rcTextPadding) {
        m_rcItemTextPadding = rcTextPadding;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetTextPadding(rcTextPadding);
        }
    }

    void CTabsUI::SetItemVisibleTips(bool bIsVisible){
        m_bItemVisibleTips = bIsVisible;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetVisibleTips(bIsVisible);
        }
    }

    bool CTabsUI::OnItemClose(void* param){
        TNotifyUI* pMsg = (TNotifyUI*)param;
        if (_tcsicmp(pMsg->sType, DUI_MSGTYPE_CLICK) == 0)
        {
            CButtonUI* pClose = (CButtonUI*)pMsg->pSender;
            CTabPaneUI* pItem = (CTabPaneUI*)pClose->GetParent();
            if (m_pTabPaneCloseCb) {
                if (!m_pTabPaneCloseCb(pItem, m_pCbParam))
                    return true;
            }
            
            RemoveItem(pItem);
            return true;
        }
        return true;
    }

    void CTabsUI::SetItemIconImage(LPCTSTR pStrImage) {
        m_strItemIconImage = pStrImage;
        int nCount = this->GetCount();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CTabPaneUI* pItem = static_cast<CTabPaneUI*>(this->GetItemAt(nIndex));
            pItem->SetIconImage(pStrImage);
        }
    }

    void CTabsUI::PushItemSelected(CControlUI* pItem){
        int nCount = m_itemOptStack.GetSize();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CControlUI* pCtrl = (CControlUI*)m_itemOptStack.GetAt(nIndex);
            if (pCtrl == pItem) {
                m_itemOptStack.Remove(nIndex);
                break;
            }
        }
        m_itemOptStack.Add(pItem);
    }

    void CTabsUI::SetTabPaneCloseCb(TabPaneCloseCb pTabPaneCloseCb, void* pCbParam){
        m_pTabPaneCloseCb = pTabPaneCloseCb;
        m_pCbParam = pCbParam;
    }

    void CTabsUI::RemoveTabPane(CControlUI* pTabPane) {
        int iCount = this->GetCount();
        int i = 0;
        for (; i < iCount; ++i)
        {
            CControlUI* pItem = this->GetItemAt(i);
            if (pItem == pTabPane)
                break;
        }

        if (i >= iCount) return;
        RemoveItem((CTabPaneUI*)pTabPane);
    }

    void CTabsUI::RemoveItem(CTabPaneUI* pTabPane) {
        bool bIsSelected = pTabPane->IsSelected();
        this->Remove(pTabPane);

        if (!m_strGroupName.IsEmpty()) {
            m_pManager->RemoveOptionGroup(m_strGroupName, pTabPane);
        }

        int nCount = m_itemOptStack.GetSize();
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            CControlUI* pCtrl = (CControlUI*)m_itemOptStack.GetAt(nIndex);
            if (pCtrl == pTabPane) {
                m_itemOptStack.Remove(nIndex);
                --nCount;
                break;
            }
        }

        if (bIsSelected && nCount > 0) {
            --nCount;
            CTabPaneUI* pItem = (CTabPaneUI*)m_itemOptStack.GetAt(nCount);
            pItem->Selected(true);
        }
    }

    void CTabsUI::RemoveAll() {
        __super::RemoveAll();
        m_pManager->RemoveOptionGroup(m_strGroupName);
    }
}