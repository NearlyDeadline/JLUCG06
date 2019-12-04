
// JLUCG06View.cpp: CJLUCG06View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "JLUCG06.h"
#endif

#include "JLUCG06Doc.h"
#include "JLUCG06View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJLUCG06View

IMPLEMENT_DYNCREATE(CJLUCG06View, CView)

BEGIN_MESSAGE_MAP(CJLUCG06View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTTONSTART, &CJLUCG06View::OnButtonstart)
	ON_COMMAND(ID_BUTTONSTOP, &CJLUCG06View::OnButtonstop)
END_MESSAGE_MAP()

// CJLUCG06View 构造/析构

CJLUCG06View::CJLUCG06View() noexcept
{
	// TODO: 在此处添加构造代码

}

CJLUCG06View::~CJLUCG06View()
{
}

BOOL CJLUCG06View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CJLUCG06View 绘图

void CJLUCG06View::OnDraw(CDC* pDC)
{
	CJLUCG06Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (StartMove) { //运动

	}
}


void CJLUCG06View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnTimer(nIDEvent);
}


void CJLUCG06View::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDC* pDC = GetDC();
	pDC->SetROP2(R2_NOTXORPEN);
	LineStartPoint = point;
	LineEndPoint = PointsBuffer[0];
	pDC->MoveTo(LineStartPoint);
	pDC->LineTo(LineEndPoint);
	PolygonPoints.Copy(PointsBuffer);
	PointsBuffer.RemoveAll();
	DrawType = 0;
	ReleaseDC(pDC);
	DrawCurves();
	CView::OnLButtonDblClk(nFlags, point);
}

void CJLUCG06View::DrawCurves()
{
	CDC* pDC = GetDC();
	pDC->SetROP2(R2_NOTXORPEN);
	int n = PolygonPoints.GetCount();
	CPoint mid;
	for (int i = 0; i < n; i++) {
		PointsBuffer.Add(PolygonPoints[i]);
		mid.x = (PolygonPoints[i % n].x + PolygonPoints[(i + 1) % n].x) / 2;
		mid.y = (PolygonPoints[i % n].y + PolygonPoints[(i + 1) % n].y) / 2;
		PointsBuffer.Add(mid);
	}

	CPoint* result = new CPoint[CJLUCG06View::DENSITY + 1];
	CPoint* source = new CPoint[3];
	for (int j = 1; j < n; j = j + 2) {
		source[0] = PointsBuffer[j % n];
		source[1] = PointsBuffer[(j + 1) % n];
		source[2] = PointsBuffer[(j + 2) % n];
		GetBezierPoints(source, 3, result);
		for (int i = 0; i <= CJLUCG06View::DENSITY; i++) {
			pDC->SetPixel(result[i], RGB(255, 0, 0));
		}
	}
	delete result;
	delete source;
	ReleaseDC(pDC);
}

void CJLUCG06View::OnLButtonDown(UINT nFlags, CPoint point)
{
	DrawType = 1;
	LineStartPoint = point;
	LineEndPoint = point;
	PointsBuffer.Add(point);
	CView::OnLButtonDown(nFlags, point);
}


void CJLUCG06View::OnLButtonUp(UINT nFlags, CPoint point)
{

	CView::OnLButtonUp(nFlags, point);
}


void CJLUCG06View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (DrawType == 1) {
		CDC* pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		pDC->MoveTo(LineStartPoint);
		pDC->LineTo(LineEndPoint); //重画上一条线，覆盖

		LineEndPoint = point;
		pDC->MoveTo(LineStartPoint);
		pDC->LineTo(LineEndPoint); //画本条线

		ReleaseDC(pDC);
	}
	CView::OnMouseMove(nFlags, point);
}

void CJLUCG06View::GetBezierPoints(CPoint* source, int n, CPoint* result, int density)
{
	double t = 0.0;
	double delt = 1.0 / (double)density;
	for (int i = 0; i <= density; i++) {
		result[i] = Decas(source, n, t);
		t += delt;
	}
}

CPoint CJLUCG06View::Decas(CPoint* source, int n, double t)
{
	CPoint* R = new CPoint[n + 1];
	CPoint* Q = new CPoint[n + 1];
	CPoint P0;
	for (int i = 0; i <= n; i++)
		R[i] = source[i];
	for (int m = n; m > 0; m--) {
		for (int i = 0; i <= m - 1; i++) {
			Q[i].x = R[i].x + t * (R[i + 1].x - R[i].x);
			Q[i].y = R[i].y + t * (R[i + 1].y - R[i].y);
		}
		for (int i = 0; i <= m - 1; i++) {
			R[i] = Q[i];
		}
	}
	P0 = R[0];
	delete R;
	delete Q;
	return P0;
	/*int a = 0;
	int b = 1;
	int c = 2;
	int x[3];
	x[0] = PointsBuffer[a].x - 2 * PointsBuffer[b].x + PointsBuffer[c].x;
	x[1] = -2 * PointsBuffer[a].x + 2 * PointsBuffer[b].x;
	x[2] = PointsBuffer[a].x;
	int y[3];
	y[0] = PointsBuffer[a].y - 2 * PointsBuffer[b].y + PointsBuffer[c].y;
	y[1] = -2 * PointsBuffer[a].y + 2 * PointsBuffer[b].y;
	y[2] = PointsBuffer[a].y;
	CPoint result;
	result.x = (int)(t * t * double(x[0]) + t * double(x[1]) + double(x[2]));
	result.y = (int)(t * t * double(y[0]) + t * double(y[1]) + double(y[2]));
	return result;*/
}

/*void CJLUCG06View::GetBezierPoints(CPoint* result,int index, int density)
{
	double t = 0.0;
	double delt = 1.0 / (double)density;
	for (int i = 0; i <= density; i++) {
		result[i] = Decas(t, index);
		t += delt;
	}
}


CPoint CJLUCG06View::Decas(double t, int index)
{
	int n = PointsBuffer.GetCount();
	int a = index % n;
	int b = (index + 1) % n;
	int c = (index + 2) % n;
	int x[3];
	x[0] = PointsBuffer[a].x - 2 * PointsBuffer[b].x + PointsBuffer[c].x;
	x[1] = -2 * PointsBuffer[a].x + 2 * PointsBuffer[b].x;
	x[2] = PointsBuffer[a].x;
	int y[3];
	y[0] = PointsBuffer[a].y - 2 * PointsBuffer[b].y + PointsBuffer[c].y;
	y[1] = -2 * PointsBuffer[a].y + 2 * PointsBuffer[b].y;
	y[2] = PointsBuffer[a].y;
	CPoint result;
	result.x = (int)(t * t * double(x[0]) + t * double(x[1]) + double(x[2]));
	result.y = (int)(t * t * double(y[0]) + t * double(y[1]) + double(y[2]));
	return result;
	
}*/

// CJLUCG06View 打印

BOOL CJLUCG06View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CJLUCG06View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CJLUCG06View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CJLUCG06View 诊断

#ifdef _DEBUG
void CJLUCG06View::AssertValid() const
{
	CView::AssertValid();
}

void CJLUCG06View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CJLUCG06Doc* CJLUCG06View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJLUCG06Doc)));
	return (CJLUCG06Doc*)m_pDocument;
}
#endif //_DEBUG


// CJLUCG06View 消息处理程序


void CJLUCG06View::OnButtonstart()
{
	StartMove = true;
}


void CJLUCG06View::OnButtonstop()
{
	StartMove = false;
}
