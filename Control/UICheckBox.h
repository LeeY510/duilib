#ifndef __UICHECKBOX_H__
#define __UICHECKBOX_H__

#pragma once

namespace DuiLib
{
	/// ����ͨ�ĵ�ѡ��ť�ؼ���ֻ���ǡ������ֽ��
	/// ������COptionUI��ֻ��ÿ��ֻ��һ����ť���ѣ�����Ϊ�գ������ļ�Ĭ�����Ծ�����
	/// <CheckBox name="CheckBox" value="height='20' align='left' textpadding='24,0,0,0' normalimage='file='sys_check_btn.png' source='0,0,20,20' dest='0,0,20,20'' selectedimage='file='sys_check_btn.png' source='20,0,40,20' dest='0,0,20,20'' disabledimage='file='sys_check_btn.png' source='40,0,60,20' dest='0,0,20,20''"/>

	class UILIB_API CCheckBoxUI : public COptionUI
	{
	public:
        CCheckBoxUI();
		LPCTSTR GetClass() const;

		void SetCheck(bool bCheck);
		bool GetCheck() const;

        void SetPartialSelected(bool bPartialSelected);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        LPCTSTR GetPartialSelectedImage();
        void SetPartialSelectedImage(LPCTSTR pStrImage);

        LPCTSTR GetPartialSelectedHotImage();
        void SetPartialSelectedHotImage(LPCTSTR pStrImage);

        LPCTSTR GetPartialSelectedDisableImage();
        void SetPartialSelectedDisableImage(LPCTSTR pStrImage);

        virtual void PaintStatusImage(HDC hDC);
        virtual void Selected(bool bSelected);
    protected:
        bool			m_bPartialSelected;
    
        CDuiString		m_sPartialSelectedImage;
        CDuiString		m_sPartialSelectedHotImage;
        CDuiString		m_sPartialSelectedDisableImage;
	};
}

#endif // __UICHECKBOX_H__
