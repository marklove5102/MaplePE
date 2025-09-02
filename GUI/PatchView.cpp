// PatchView.cpp : implementation file
//
#include "pch.h"
#include "PatchView.h"
#include "resource.h"

// PatchView dialog

IMPLEMENT_DYNAMIC(PatchView, CDialogEx)


PatchView::PatchView(IMainController* mainControllerImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PATCH_DIALOG, pParent), BaseView(L"PatchView")
{
	m_patchController = new PatchController(mainControllerImpl, this);
}

PatchView::~PatchView()
{
	delete(m_patchController);
	m_patchController = nullptr;
}

void PatchView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_ID_EDIT, m_pidEdit);
}

BOOL PatchView::OnInitDialog() {
	CDialogEx::OnInitDialog();
	this->SetHWND(this->GetSafeHwnd());
	DWORD processID = m_patchController->FindProcessID();
	m_pidEdit.ModifyStyle(0, ES_CENTER);
	m_pidEdit.SetWindowTextW(std::to_wstring(processID).c_str());
	return TRUE;
}

BEGIN_MESSAGE_MAP(PatchView, CDialogEx)
	ON_BN_CLICKED(IDC_INJECT_DLL_BUTTON, &PatchView::OnBnClickedInjectDllButton)
END_MESSAGE_MAP()


// PatchView message handlers

void PatchView::OnBnClickedInjectDllButton()
{
	// TODO: Add your control notification handler code here
	CString pidText;
	m_pidEdit.GetWindowTextW(pidText);
	try {
		std::wstring input(pidText);
		DWORD pid = static_cast<DWORD>(std::stoul(input));
		if (pid <= 0) {
			MBError(L"Invalid process id");
			return;
		}
		std::wstring err = m_patchController->InjectDLL(pid);
		if (!err.empty()) {
			MBError(err);
			return;
		}
		MBInfo(L"Inject dll ok");
	}
	catch (const std::exception e) {
		MBError(L"Invalid process id");
	}

}