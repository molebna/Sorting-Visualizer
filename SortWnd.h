//***************************************************************************************

#pragma once
#include "DataModel.h"

//***************************************************************************************

class SortWnd : public CWnd
{
public:

  SortWnd();

  void SetDataModel(const DataModel* dataModel);

private:

  const DataModel* m_dataModel = nullptr;

  void OnPaint();

  DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
