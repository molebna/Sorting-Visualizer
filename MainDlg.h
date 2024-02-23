//***************************************************************************************

#pragma once
#include "DataModel.h"
#include "SortWnd.h"

//***************************************************************************************

class MainDlg : public CDialog
{
public:

  MainDlg(CWnd* parent);

private:

  DataModel m_dataModel;

  SortWnd m_sortWindow;

  CButton m_createButton;
  CButton m_restoreButton;
  CComboBox m_sortComboBox;
  CButton m_startButton;
  CButton m_pauseButton;
  CButton m_resumeButton;
  CButton m_abortButton;
  CEdit m_arraySizeEdit;
  CSliderCtrl m_delaySlider;
  CEdit m_maxEdit;
  CEdit m_numBucketsEdit;
  CEdit m_infoEdit;
  CButton m_saveButton;

  int m_arraySize = 0;
  int m_valueMax = 0;
  int m_numBuckets = 0;

  virtual void DoDataExchange(CDataExchange* dx);
  virtual BOOL OnInitDialog();

  void OnCreateButtonClicked();
  void OnRestoreButtonClicked();
  void OnStartButtonClicked();
  void OnPauseButtonClicked();
  void OnResumeButtonClicked();
  void OnAbortButtonClicked();
  void OnSaveButtonClicked();
  LRESULT OnRepaintMessage(WPARAM wParam, LPARAM lParam);
  LRESULT OnSortingFinishedMessage(WPARAM wParam, LPARAM lParam);
  void OnHScroll(UINT code, UINT pos, CScrollBar* scrollBar);

  DECLARE_MESSAGE_MAP()
};

//***************************************************************************************

