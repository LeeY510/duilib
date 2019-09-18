#ifndef __UIGIF_H__
#define __UIGIF_H__

#pragma once

namespace DuiLib
{
    class CLabelUI;

#define EVENT_TIEM_ID	100

    class UILIB_API CGifUI : public CLabelUI
    {
    public:
        CGifUI(void);
        ~CGifUI(void);

        LPCTSTR	GetClass() const;
        LPVOID	GetInterface(LPCTSTR pstrName);
        void	DoInit() override;
        void	DoPaint(HDC hDC, const RECT& rcPaint) override;
        void	DoEvent(TEventUI& event) override;
        void	SetVisible(bool bVisible = true) override;
        void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
        void	SetBkImage(LPCTSTR pStrImage);
        LPCTSTR GetBkImage();

        void	SetAutoPlay(bool bIsAuto = true);
        bool	IsAutoPlay() const;
        void	SetAutoSize(bool bIsAuto = true);
        bool	IsAutoSize() const;
        void	PlayGif();
        void	PauseGif();
        void	StopGif();

    private:
        void	InitGifImage();
        void	DeleteGif();
        void    OnTimer(UINT_PTR idEvent);
        void	DrawFrame(HDC hDC);		// ����GIFÿ֡
    private:
        Gdiplus::Image	*m_pGifImage;
        UINT			m_nFrameCount;				// gifͼƬ��֡��
        UINT			m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡
        Gdiplus::PropertyItem*	m_pPropertyItem;	// ֡��֮֡����ʱ��

        CDuiString		m_sBkImage;
        bool			m_bIsAutoPlay;				// �Ƿ��Զ�����gif
        bool			m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
        bool			m_bIsPlaying;

    };
}

#endif // __UIGIF_H__
