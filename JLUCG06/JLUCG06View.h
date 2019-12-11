
// JLUCG06View.h: CJLUCG06View 类的接口
//

#pragma once


class CJLUCG06View : public CView
{
protected: // 仅从序列化创建
	CJLUCG06View() noexcept;
	DECLARE_DYNCREATE(CJLUCG06View)

// 特性
public:
	CJLUCG06Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CJLUCG06View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
private:
	CArray<CPoint, CPoint&> PolygonPoints;
	CArray<CPoint, CPoint&> PointsBuffer;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	CPoint LineStartPoint;
	CPoint LineEndPoint;
	int DrawType = 0;
	bool StartMove = false;
	int TimerID;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	static const int DENSITY = 500;
private:
	//void GetBezierPoints(CPoint* result,int index, int density = CJLUCG06View::DENSITY);
	//CPoint Decas(double t,int index);
	void GetBezierPoints(CPoint* source, int n, CPoint* result, int density = CJLUCG06View::DENSITY);
	CPoint Decas(CPoint* source, int n, double t);
	void DrawCurves(COLORREF color);
	bool* IsOpposite = nullptr;
	bool FinishDrawPolygon = false;
public:
	afx_msg void OnButtonstart();
	afx_msg void OnButtonstop();
private:
	// 将PolygonPoints点的下标转化为逻辑坐标
	int GetIndex(int i);
};

#ifndef _DEBUG  // JLUCG06View.cpp 中的调试版本
inline CJLUCG06Doc* CJLUCG06View::GetDocument() const
   { return reinterpret_cast<CJLUCG06Doc*>(m_pDocument); }
#endif

