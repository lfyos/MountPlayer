
// precisionDlg.h : ͷ�ļ�
//

#pragma once


// CprecisionDlg �Ի���
class CprecisionDlg : public CDialog
{
// ����
public:
	CprecisionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PRECISION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
