#include "StdAfx.h"
#include "UIGif.h"

namespace DuiLib
{

    CGifUI::CGifUI(void)
    {
        m_pGifImage = NULL;
        m_pPropertyItem = NULL;
        m_nFrameCount = 0;
        m_nFramePosition = 0;
        m_bIsAutoPlay = true;
        m_bIsAutoSize = false;
        m_bIsPlaying = false;
    }


    CGifUI::~CGifUI(void)
    {
        DeleteGif();
        m_pManager->KillTimer(this, EVENT_TIEM_ID);

    }

    LPCTSTR CGifUI::GetClass() const
    {
        return _T("GifUI");
    }

    LPVOID CGifUI::GetInterface(LPCTSTR pstrName)
    {
        if (_tcscmp(pstrName, DUI_CTR_GIF) == 0) return static_cast<CGifUI*>(this);
        return CLabelUI::GetInterface(pstrName);
    }

    void CGifUI::DoInit()
    {
        InitGifImage();
    }

    void CGifUI::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;
        if (NULL == m_pGifImage)
        {
            InitGifImage();
        }
        
        DrawFrame(hDC);
        CLabelUI::PaintText(hDC);
        CLabelUI::PaintBorder(hDC);
    }

    void CGifUI::DoEvent(TEventUI& event)
    {
        if (event.Type == UIEVENT_TIMER)
            OnTimer((UINT_PTR)event.wParam);
    }

    void CGifUI::SetVisible(bool bVisible /* = true */)
    {
        CControlUI::SetVisible(bVisible);
        if (bVisible)
            PlayGif();
        else
            StopGif();
    }

    void CGifUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if (_tcscmp(pstrName, _T("bkimage")) == 0) SetBkImage(pstrValue);
        else if (_tcscmp(pstrName, _T("autoplay")) == 0) {
            SetAutoPlay(_tcscmp(pstrValue, _T("true")) == 0);
        }
        else if (_tcscmp(pstrName, _T("autosize")) == 0) {
            SetAutoSize(_tcscmp(pstrValue, _T("true")) == 0);
        }
        else
            CLabelUI::SetAttribute(pstrName, pstrValue);
    }

    void CGifUI::SetBkImage(LPCTSTR pStrImage)
    {
        if (m_sBkImage == pStrImage || NULL == pStrImage) return;

        m_sBkImage = pStrImage;

        StopGif();
        DeleteGif();

        Invalidate();
    }

    LPCTSTR CGifUI::GetBkImage()
    {
        return m_sBkImage.GetData();
    }

    void CGifUI::SetAutoPlay(bool bIsAuto)
    {
        m_bIsAutoPlay = bIsAuto;
    }

    bool CGifUI::IsAutoPlay() const
    {
        return m_bIsAutoPlay;
    }

    void CGifUI::SetAutoSize(bool bIsAuto)
    {
        m_bIsAutoSize = bIsAuto;
    }

    bool CGifUI::IsAutoSize() const
    {
        return m_bIsAutoSize;
    }

    void CGifUI::PlayGif()
    {
        if (m_bIsPlaying || m_pGifImage == NULL)
        {
            return;
        }

        long lPause = ((long*)m_pPropertyItem->value)[m_nFramePosition] * 10;
        if (lPause == 0) lPause = 100;
        m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);

        m_bIsPlaying = true;
    }

    void CGifUI::PauseGif()
    {
        if (!m_bIsPlaying || m_pGifImage == NULL)
        {
            return;
        }

        m_pManager->KillTimer(this, EVENT_TIEM_ID);
        this->Invalidate();
        m_bIsPlaying = false;
    }

    void CGifUI::StopGif()
    {
        if (!m_bIsPlaying)
        {
            return;
        }

        m_pManager->KillTimer(this, EVENT_TIEM_ID);
        m_nFramePosition = 0;
        this->Invalidate();
        m_bIsPlaying = false;
    }

    void CGifUI::InitGifImage()
    {
        const TImageInfo* pImageInfo = m_pManager->AddGdiplusImage(GetBkImage());
        if (NULL == pImageInfo)
        {
            return;
        }
        m_pGifImage = pImageInfo->pGdiplusImage;
        UINT nCount = 0;
        nCount = m_pGifImage->GetFrameDimensionsCount();
        GUID* pDimensionIDs = new GUID[nCount];
        m_pGifImage->GetFrameDimensionsList(pDimensionIDs, nCount);
        m_nFrameCount = m_pGifImage->GetFrameCount(&pDimensionIDs[0]);
        int nSize = m_pGifImage->GetPropertyItemSize(PropertyTagFrameDelay);
        m_pPropertyItem = (Gdiplus::PropertyItem*) malloc(nSize);
        m_pGifImage->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
        delete  pDimensionIDs;
        pDimensionIDs = NULL;

        if (m_bIsAutoSize)
        {
            SetFixedWidth(m_pGifImage->GetWidth());
            SetFixedHeight(m_pGifImage->GetHeight());
        }
        if (m_bIsAutoPlay)
        {
            PlayGif();
        }
    }

    void CGifUI::DeleteGif()
    {
        if (m_pPropertyItem != NULL)
        {
            free(m_pPropertyItem);
            m_pPropertyItem = NULL;
        }
        m_nFrameCount = 0;
        m_nFramePosition = 0;
    }

    void CGifUI::OnTimer(UINT_PTR idEvent)
    {
        if (idEvent != EVENT_TIEM_ID)
            return;
        m_pManager->KillTimer(this, EVENT_TIEM_ID);
        this->Invalidate();

        m_nFramePosition = (++m_nFramePosition) % m_nFrameCount;

        long lPause = ((long*)m_pPropertyItem->value)[m_nFramePosition] * 10;
        if (lPause == 0) lPause = 100;
        m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);
    }

    void CGifUI::DrawFrame(HDC hDC)
    {
        if (NULL == hDC || NULL == m_pGifImage) return;
        GUID pageGuid = Gdiplus::FrameDimensionTime;
        Gdiplus::Graphics graphics(hDC);
        graphics.DrawImage(m_pGifImage, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
        m_pGifImage->SelectActiveFrame(&pageGuid, m_nFramePosition);
    }   

}
