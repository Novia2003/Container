
// ContainerView.cpp: реализация класса CContainerView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Container.h"
#endif

#include "TextDoc.h"
#include "ContainerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CContainerView

IMPLEMENT_DYNCREATE(CContainerView, CView)

BEGIN_MESSAGE_MAP(CContainerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CContainerView

CContainerView::CContainerView() noexcept
{
	// TODO: добавьте код создания

}

CContainerView::~CContainerView()
{
}

BOOL CContainerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CContainerView

void CContainerView::OnDraw(CDC* pDC)
{
	CTextDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	RECT r;
	this->GetClientRect(&r);

	pDC->DrawText(pDoc->GetText(), &r, 0);

	// TODO: добавьте здесь код отрисовки для собственных данных
}

void CContainerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CContainerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CContainerView

#ifdef _DEBUG
void CContainerView::AssertValid() const
{
	CView::AssertValid();
}

void CContainerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTextDoc* CContainerView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextDoc)));
	return (CTextDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CContainerView
