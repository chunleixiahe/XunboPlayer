#include "stdafx.h"
#include "UIMenuButton.h"

namespace DuiLib
{
	CMenuButtonUI::CMenuButtonUI()
		: m_uButtonState(0)
		, m_dwHotTextColor(0)
		, m_dwPushedTextColor(0)
		, m_dwFocusedTextColor(0)
		,m_dwHotBkColor(0)
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	LPCTSTR CMenuButtonUI::GetClass() const
	{
		return _T("MenuButtonUI");
	}

	LPVOID CMenuButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_MENUBUTTON) == 0 ) return static_cast<CMenuButtonUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CMenuButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CMenuButtonUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
					Activate();
					return;
				}
			}
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() ) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER, event.wParam, event.lParam);
				Invalidate();
			}
			// return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE, event.wParam, event.lParam);
				Invalidate();
			}
			// return;
		}
		if( event.Type == UIEVENT_SETCURSOR ) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			return;
		}
		CLabelUI::DoEvent(event);
	}

	bool CMenuButtonUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		return true;
	}

	void CMenuButtonUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	//************************************
	// Method:    SetHotBkColor
	// FullName:  CButtonUI::SetHotBkColor
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD dwColor
	// Note:	  
	//************************************
	void CMenuButtonUI::SetHotBkColor( DWORD dwColor )
	{
		m_dwHotBkColor = dwColor;
	}

	//************************************
	// Method:    GetHotBkColor
	// FullName:  CButtonUI::GetHotBkColor
	// Access:    public 
	// Returns:   DWORD
	// Qualifier: const
	// Note:	  
	//************************************
	DWORD CMenuButtonUI::GetHotBkColor() const
	{
		return m_dwHotBkColor;
	}

	void CMenuButtonUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}

	DWORD CMenuButtonUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}

	void CMenuButtonUI::SetPushedTextColor(DWORD dwColor)
	{
		m_dwPushedTextColor = dwColor;
	}

	DWORD CMenuButtonUI::GetPushedTextColor() const
	{
		return m_dwPushedTextColor;
	}

	void CMenuButtonUI::SetFocusedTextColor(DWORD dwColor)
	{
		m_dwFocusedTextColor = dwColor;
	}

	DWORD CMenuButtonUI::GetFocusedTextColor() const
	{
		return m_dwFocusedTextColor;
	}

	LPCTSTR CMenuButtonUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CMenuButtonUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CMenuButtonUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CMenuButtonUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CMenuButtonUI::GetPushedImage()
	{
		return m_sPushedImage;
	}

	void CMenuButtonUI::SetPushedImage(LPCTSTR pStrImage)
	{
		m_sPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CMenuButtonUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CMenuButtonUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CMenuButtonUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CMenuButtonUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	//************************************
	// Method:    GetForeImage
	// FullName:  CButtonUI::GetForeImage
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier:
	// Note:	  
	//************************************
	LPCTSTR CMenuButtonUI::GetForeImage()
	{
		return m_sForeImage;
	}

	//************************************
	// Method:    SetForeImage
	// FullName:  CButtonUI::SetForeImage
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pStrImage
	// Note:	  
	//************************************
	void CMenuButtonUI::SetForeImage( LPCTSTR pStrImage )
	{
		m_sForeImage = pStrImage;
		Invalidate();
	}

	//************************************
	// Method:    GetHotForeImage
	// FullName:  CButtonUI::GetHotForeImage
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier:
	// Note:	  
	//************************************
	LPCTSTR CMenuButtonUI::GetHotForeImage()
	{
		return m_sHotForeImage;
	}

	//************************************
	// Method:    SetHotForeImage
	// FullName:  CButtonUI::SetHotForeImage
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pStrImage
	// Note:	  
	//************************************
	void CMenuButtonUI::SetHotForeImage( LPCTSTR pStrImage )
	{
		m_sHotForeImage = pStrImage;
		Invalidate();
	}

	SIZE CMenuButtonUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CMenuButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotforeimage")) == 0 ) SetHotForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBkColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("hottextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("pushedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("focusedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusedTextColor(clrColor);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CMenuButtonUI::PaintText(HDC hDC)
	{

		if (GetEnabledEffect())
		{
			CLabelUI::PaintText(hDC);
			return ;
		}


		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;
		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;

		if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
			clrColor = GetPushedTextColor();
		else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
			clrColor = GetHotTextColor();
		else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
			clrColor = GetFocusedTextColor();

		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, clrColor, \
			NULL, NULL, nLinks, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, clrColor, \
			m_iFont, m_uTextStyle);
	}

	void CMenuButtonUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() )
			{
				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
				else goto Label_ForeImage;
			}
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ){
					m_sPushedImage.Empty();
				}
				if( !m_sPushedForeImage.IsEmpty() )
				{
					if( !DrawImage(hDC, (LPCTSTR)m_sPushedForeImage) )
						m_sPushedForeImage.Empty();
					return;
				}
				else goto Label_ForeImage;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ){
					m_sHotImage.Empty();
				}
				if( !m_sHotForeImage.IsEmpty() ) {
					if( !DrawImage(hDC, (LPCTSTR)m_sHotForeImage) )
						m_sHotForeImage.Empty();
					return;
				}
				else goto Label_ForeImage;
			}
			else if(m_dwHotBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
				return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
				else goto Label_ForeImage;
			}
		}

		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
			else goto Label_ForeImage;
		}

		if(!m_sForeImage.IsEmpty() )
			goto Label_ForeImage;

		return;

Label_ForeImage:
		if(!m_sForeImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage) ) m_sForeImage.Empty();
		}
	}
}