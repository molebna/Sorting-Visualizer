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
  // Вказати sortWindow, хто є dataModel, тобто звідки брати дані для відображення
  m_sortWindow.SetDataModel(&m_dataModel);

  // Вказати dataModel, якому вікну воно має слати повідомлення - цьому діалоговому вікну
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

  // Проапдейтити елементи діалогового вікна, щоб вони показували початкові значення
  UpdateData(FALSE);

  // відобразити масив в текстовому вигляді
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  return TRUE;
}

//***************************************************************************************

void MainDlg::OnCreateButtonClicked()
{
  // Отримати розмір масиву, макс значення та метод сортування з діалогового вікна
  if (!UpdateData(TRUE)) return;

  // Встановити нові розмір масиву, макс значення та метод сортування
  m_dataModel.SetArraySize(m_arraySize);
  m_dataModel.SetValueMax(m_valueMax);
  m_dataModel.SetSortMethod(m_sortComboBox.GetCurSel());

  // згенерувати новий масив
  m_dataModel.CreateArray();

  // відобразити масив в текстовому вигляді
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  // відобразити масив в графічному вигляді
  m_sortWindow.Invalidate(FALSE);
  m_sortWindow.UpdateWindow();
}

//***************************************************************************************

void MainDlg::OnRestoreButtonClicked()
{
  // відновити масив
  m_dataModel.RestoreArray();

  // відобразити масив в текстовому вигляді
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  // відобразити масив в графічному вигляді
  m_sortWindow.Invalidate(FALSE);
  m_sortWindow.UpdateWindow();
}

//***************************************************************************************

void MainDlg::OnStartButtonClicked()
{
  // Отримати кількість корзин та метод сортування з діалогового вікна
  if (!UpdateData(TRUE)) return;

  // Встановити нові кількість корзин та метод сортування
  m_dataModel.SetNumBuckets(m_numBuckets);
  m_dataModel.SetSortMethod(m_sortComboBox.GetCurSel());

  // Проапдейтити стан кнопок, едіт боксів та комбо-боксу
  m_arraySizeEdit.EnableWindow(FALSE);
  m_maxEdit.EnableWindow(FALSE);
  m_sortComboBox.EnableWindow(FALSE);
  m_createButton.EnableWindow(FALSE);
  m_startButton.EnableWindow(FALSE);
  m_restoreButton.EnableWindow(FALSE);
  m_pauseButton.EnableWindow(TRUE);
  m_abortButton.EnableWindow(TRUE);

  // Розпочати сортування
  m_dataModel.StartSorting();
}

//***************************************************************************************

void MainDlg::OnPauseButtonClicked()
{
  m_pauseButton.EnableWindow(FALSE);

  // Повідомити потік сортування про необхідність тимчасового призупинення роботи
  m_dataModel.PauseSorting();

  m_resumeButton.EnableWindow(TRUE);
}

//***************************************************************************************

void MainDlg::OnResumeButtonClicked()
{
  m_resumeButton.EnableWindow(FALSE);

  // Повідомити потік сортування про необхідність продовження роботи
  m_dataModel.ResumeSorting();

  m_pauseButton.EnableWindow(TRUE);
}

//***************************************************************************************

void MainDlg::OnAbortButtonClicked()
{
  // Повідомити потік сортування про необхідність неочікуваного завершення роботи
  m_dataModel.AbortSorting();
}

//***************************************************************************************

void MainDlg::OnSaveButtonClicked()
{
  // Отримати ім'я файлу для збереження інформації за допомогою стандартного діалогового вікна
  // Windows для відкриття/збереження файлів
  CFileDialog dlg(FALSE, L"txt", L"", OFN_OVERWRITEPROMPT, L"Text files (*.txt)|*.txt||", this);
  if (dlg.DoModal() != IDOK) return;

  // Зберегти файл
  m_dataModel.SaveResultsIntoFile((const wchar_t*)dlg.GetPathName());
}

//***************************************************************************************
// При надходженні такого кастом-повідомлення (від dataModel)
LRESULT MainDlg::OnRepaintMessage(WPARAM wParam, LPARAM lParam)
{
  // Змусити sortWindow перемалювати себе (sortWindow отримає повідомлення WM_PAINT)
  m_sortWindow.Invalidate(FALSE);
  m_sortWindow.UpdateWindow();

  // Встановити новий текст в infoEdit бокс
  m_infoEdit.SetWindowTextW(m_dataModel.GetInfo().c_str());

  return 0;
}

//***************************************************************************************
// Фінальне повідомлення від dataModel перед завершенням роботи другого потоку (потоку сортування)
LRESULT MainDlg::OnSortingFinishedMessage(WPARAM wParam, LPARAM lParam)
{
  // Проапдейтити стан кнопок, едіт боксів та комбо-боксу
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
// Обробник повідомлень від прокрутки для регулювання швидкості (затримки)
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
