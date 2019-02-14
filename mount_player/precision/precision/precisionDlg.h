
// precisionDlg.h : 头文件
//

#pragma once


// CprecisionDlg 对话框
class CprecisionDlg : public CDialog
{
// 构造
public:
	CprecisionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PRECISION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int slider;
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);
	double chord;
	double length;
	afx_msg void OnEnChangeChordEdit();
	afx_msg void OnEnChangeLengthEdit();
private:
	void display(void);
};
