//***************************************************************************************

#include "pch.h"
#include "Resource.h"
#include "MainDlg.h"

//***************************************************************************************

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
  ON_BN_CLICKED(IDC_CREATE_BUTTON, OnCreateButtonClicked)
  ON_BN_CLICKED(IDC_RESTORE_BUTTON, OnRestoreButtonClicked)
  ON_BN_CLICKED(IDC_START_BUTTON, OnStartButtonClicked)
  ON_BN_CLICKED(IDC_PAUSE_BUTTON, OnPauseButtonClicked)
  ON_BN_CLICKED(IDC_RESUME_BUTTON, OnResumeButtonClicked)
  ON_BN_CLICKED(IDC_ABORT_BUTTON, OnAbortButtonClicked)
  ON_BN_CLICKED(IDC_SAVE_BUTTON, OnSaveButtonClicked)
  ON_MESSAGE(WM_REPAINT, OnRepaintMessage)
  ON_MESSAGE(WM_SORTING_FINISHED, OnSortingFinishedMessage)
  ON_WM_HSCROLL()
END_MESSAGE_MAP()

//***************************************************************************************

MainDlg::MainDlg(CWnd* parent) : CDialog(IDD_MAIN_DIALOG, parent)
{
  // ������� sortWindow, ��� � dataModel, ����� ����� ����� ��� ��� �����������
  m_sortWindow.SetDataModel(&m_dataModel);

  // ������� dataModel, ����� ���� ���� �� ����� ����������� - ����� ���������� ����
  m_dataModel.SetWindow(this);
}

//***************************************************************************************

void MainDlg::DoDataExchange(CDataExchange* dx)
{
  CDialog::DoDataExchange(dx);

  DDX_Control(dx, IDC_SORT_WINDOW, m_sortWindow);
  DDX_Control(dx, IDC_CREATE_BUTTON, m_createButton);
  DDX_Control(dx, IDC_RESTORE_BUTTON, m_restoreButton);
  DDX_Control(dx, IDC_SORT_COMBOBOX, m_sortComboBox);
  DDX_Control(dx, IDC_START_BUTTON, m_startButton);
  DDX_Control(dx, IDC_PAUSE_BUTTON, m_pauseButton);
  DDX_Control(dx, IDC_RESUME_BUTTON, m_resumeButton);
  DDX_Control(dx, IDC_ABORT_BUTTON, m_abortButton);
  DDX_Control(dx, IDC_DELAY_SLIDER, m_delaySlider);
  DDX_Control(dx, IDC_ARRAY_SIZE_EDIT, m_arraySizeEdit);
  DDX_Control(dx, IDC_MAX_EDIT, m_maxEdit);
  DDX_Control(dx, IDC_NUM_BUCKETS_EDIT, m_numBucketsEdit);
  DDX_Control(dx, IDC_INFO_EDIT, m_infoEdit);
  DDX_Control(dx, IDC_SAVE_BUTTON, m_saveButton);

  // Here DDX_Text function manages the transfer of int data between
  // an edit control and an int data member of the dialog box
  DDX_Text(dx, IDC_ARRAY_SIZE_EDIT, m_arraySize);
  DDX_Text(dx, IDC_MAX_EDIT, m_valueMax);
  DDX_Text(dx, IDC_NUM_BUCKETS_EDIT, m_numBuckets);

  DDV_MinMaxInt(dx, m_arraySize, m_dataModel.GetArraySizeMin(), m_dataModel.GetArraySizeMax());
  DDV_MinMaxInt(dx, m_valueMax, m_dataModel.GetValueMaxMin(), m_dataModel.GetValueMaxMax());
  DDV_MinMaxInt(dx, m_numBuckets, m_dataModel.GetNumBucketsMin(), m_dataModel.GetNumBucketsMax());
}

//***************************************************************************************

BOOL MainDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  HICON icon = ::AfxGetApp()->LoadIconW(IDR_MAIN_ICON);
  SetIcon(icon, TRUE);
  SetIcon(icon, FALSE);

  m_arraySize = m_dataModel.GetArraySize();

  m_valueMax = m_dataModel.GetValueMax();

  for (int i = 0; i < m_dataModel.GetNumSortMethods(); ++i)
  {
    m_sortComboBox.AddString(m_dataModel.GetSortMethodName(i));
  }
  m_sortComboBox.SetCurSel(m_dataModel.GetSortMethod());

  m_delaySlider.SetLineSize(0);
  m_delaySlider.SetPageSize(0);
  m_delaySlider.SetRange(m_dataModel.GetDelayMin(), m_dataModel.GetDelayMax());
  m_delaySlider.SetPos(m_dataModel.GetDelay());

  m_numBuckets = m_dataModel.GetNumBuckets();

  // ������������ �������� ���������� ����, ��� ���� ���������� �������� ��������
  UpdateData(FALSE);

  // ���������� ����� � ���������� ������
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  return TRUE;
}

//***************************************************************************************

void MainDlg::OnCreateButtonClicked()
{
  // �������� ����� ������, ���� �������� �� ����� ���������� � ���������� ����
  if (!UpdateData(TRUE)) return;

  // ���������� ��� ����� ������, ���� �������� �� ����� ����������
  m_dataModel.SetArraySize(m_arraySize);
  m_dataModel.SetValueMax(m_valueMax);
  m_dataModel.SetSortMethod(m_sortComboBox.GetCurSel());

  // ����������� ����� �����
  m_dataModel.CreateArray();

  // ���������� ����� � ���������� ������
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  // ���������� ����� � ���������� ������
  m_sortWindow.Invalidate(FALSE);
  m_sortWindow.UpdateWindow();
}

//***************************************************************************************

void MainDlg::OnRestoreButtonClicked()
{
  // �������� �����
  m_dataModel.RestoreArray();

  // ���������� ����� � ���������� ������
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  // ���������� ����� � ���������� ������
  m_sortWindow.Invalidate(FALSE);
  m_sortWindow.UpdateWindow();
}

//***************************************************************************************

void MainDlg::OnStartButtonClicked()
{
  // �������� ������� ������ �� ����� ���������� � ���������� ����
  if (!UpdateData(TRUE)) return;

  // ���������� ��� ������� ������ �� ����� ����������
  m_dataModel.SetNumBuckets(m_numBuckets);
  m_dataModel.SetSortMethod(m_sortComboBox.GetCurSel());

  // ������������ ���� ������, ��� ����� �� �����-�����
  m_arraySizeEdit.EnableWindow(FALSE);
  m_maxEdit.EnableWindow(FALSE);
  m_sortComboBox.EnableWindow(FALSE);
  m_createButton.EnableWindow(FALSE);
  m_startButton.EnableWindow(FALSE);
  m_restoreButton.EnableWindow(FALSE);
  m_pauseButton.EnableWindow(TRUE);
  m_abortButton.EnableWindow(TRUE);

  // ��������� ����������
  m_dataModel.StartSorting();
}

//***************************************************************************************

void MainDlg::OnPauseButtonClicked()
{
  m_pauseButton.EnableWindow(FALSE);

  // ��������� ���� ���������� ��� ����������� ����������� ������������ ������
  m_dataModel.PauseSorting();

  m_resumeButton.EnableWindow(TRUE);
}

//***************************************************************************************

void MainDlg::OnResumeButtonClicked()
{
  m_resumeButton.EnableWindow(FALSE);

  // ��������� ���� ���������� ��� ����������� ����������� ������
  m_dataModel.ResumeSorting();

  m_pauseButton.EnableWindow(TRUE);
}

//***************************************************************************************

void MainDlg::OnAbortButtonClicked()
{
  // ��������� ���� ���������� ��� ����������� ������������� ���������� ������
  m_dataModel.AbortSorting();
}

//***************************************************************************************

void MainDlg::OnSaveButtonClicked()
{
  // �������� ��'� ����� ��� ���������� ���������� �� ��������� ������������ ���������� ����
  // Windows ��� ��������/���������� �����
  CFileDialog dlg(FALSE, L"txt", L"", OFN_OVERWRITEPROMPT, L"Text files (*.txt)|*.txt||", this);
  if (dlg.DoModal() != IDOK) return;

  // �������� ����
  m_dataModel.SaveResultsIntoFile((const wchar_t*)dlg.GetPathName());
}

//***************************************************************************************
// ��� ���������� ������ ������-����������� (�� dataModel)
LRESULT MainDlg::OnRepaintMessage(WPARAM wParam, LPARAM lParam)
{
  // ������� sortWindow ������������ ���� (sortWindow ������ ����������� WM_PAINT)
  m_sortWindow.Invalidate(FALSE);
  m_sortWindow.UpdateWindow();

  // ���������� ����� ����� � infoEdit ����
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  return 0;
}

//***************************************************************************************
// Գ������ ����������� �� dataModel ����� ����������� ������ ������� ������ (������ ����������)
LRESULT MainDlg::OnSortingFinishedMessage(WPARAM wParam, LPARAM lParam)
{
  // ������������ ���� ������, ��� ����� �� �����-�����
  m_arraySizeEdit.EnableWindow(TRUE);
  m_maxEdit.EnableWindow(TRUE);
  m_sortComboBox.EnableWindow(TRUE);
  m_createButton.EnableWindow(TRUE);
  m_restoreButton.EnableWindow(TRUE);
  m_startButton.EnableWindow(TRUE);
  m_pauseButton.EnableWindow(FALSE);
  m_resumeButton.EnableWindow(FALSE);
  m_abortButton.EnableWindow(FALSE);

  return 0;
}

//***************************************************************************************
// �������� ���������� �� ��������� ��� ����������� �������� (��������)
void MainDlg::OnHScroll(UINT code, UINT pos, CScrollBar* scrollBar)
{
  if (scrollBar == (CScrollBar*)&m_delaySlider)
  {
    switch (code)
    {
    //case SB_LINELEFT: break;
    //case SB_LINERIGHT: break;
    //case SB_PAGELEFT: break;
    //case SB_PAGERIGHT: break;
    //case SB_THUMBPOSITION: break;
    case SB_THUMBTRACK:
    m_dataModel.SetDelay(pos);
    break;
    //case SB_ENDSCROLL: break;
    }
  }
  CDialog::OnHScroll(code, pos, scrollBar);
}

//***************************************************************************************
