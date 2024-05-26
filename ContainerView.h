
// ContainerView.h: интерфейс класса CContainerView
//

#pragma once


class CContainerView : public CView
{
protected: // создать только из сериализации
	CContainerView() noexcept;
	DECLARE_DYNCREATE(CContainerView)

// Атрибуты
public:
	CTextDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CContainerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в ContainerView.cpp
inline CTextDoc* CContainerView::GetDocument() const
   { return reinterpret_cast<CTextDoc*>(m_pDocument); }
#endif

