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
        void	DrawFrame(HDC hDC);		// 绘制GIF每帧
    private:
        Gdiplus::Image	*m_pGifImage;
        UINT			m_nFrameCount;				// gif图片总帧数
        UINT			m_nFramePosition;			// 当前放到第几帧
        Gdiplus::PropertyItem*	m_pPropertyItem;	// 帧与帧之间间隔时间

        CDuiString		m_sBkImage;
        bool			m_bIsAutoPlay;				// 是否自动播放gif
        bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
        bool			m_bIsPlaying;

    };
}

#endif // __UIGIF_H__
