//***************************************************************************************

#pragma once

//***************************************************************************************

class DataModel
{
public:

  DataModel();

  enum SortMethod { radix = 0, bucket = 1, counting = 2 };
  int GetNumSortMethods() const { return 3; }
  void SetSortMethod(int method);
  int GetSortMethod() const { return (int)m_method; }
  bool IsRadixSort() const { return m_method == SortMethod::radix; }
  bool IsBucketSort() const { return m_method == SortMethod::bucket; }
  bool IsCountingSort() const { return m_method == SortMethod::counting; }
  const wchar_t* GetSortMethodName(int i) const;

  void SetWindow(CWnd* wnd);

  void SetDelay(int delay);
  int GetDelay() const { return m_delay; }
  int GetDelayMin() const { return m_delayMin; }
  int GetDelayMax() const { return m_delayMax; }

  void SetValueMax(int valueMax);
  int GetValueMax() const { return m_valueMax; }
  int GetValueMin() const { return m_valueMin; }
  int GetValueMaxMin() const { return m_valueMaxMin; }
  int GetValueMaxMax() const { return m_valueMaxMax; }

  void SetArraySize(int n);
  int GetArraySize() const { return (int)m_values.size(); }
  int GetArraySizeMin() const { return m_arraySizeMin; }
  int GetArraySizeMax() const { return m_arraySizeMax; }
  int GetArrayElem(int i) const { return m_values.at(i); }
  COLORREF GetArrayElemColor(int i) const { return m_colors.at(i); }

  void CreateArray();
  void RestoreArray();

  int GetHistMax() const { return m_histMax; }
  int GetHistSize() const { return (int)m_hist.size(); }
  int GetHistElem(int i) const { return m_hist.at(i); }
  COLORREF GetHistColor() const { return 0xdfef4f; }

  void SetNumBuckets(int numBuckets);
  int GetNumBuckets() const { return m_numBuckets; }
  int GetNumBucketsMin() const { return m_numBucketsMin; }
  int GetNumBucketsMax() const { return m_numBucketsMax; }

  void StartSorting();
  void PauseSorting();
  void ResumeSorting();
  void AbortSorting();

  std::wstring GetInfo(bool useCR = true) const;

  void SaveResultsIntoFile(const wchar_t* fileName) const;

private:

  // ����� �����
  std::vector<int> m_values;
  std::vector<int> m_values2; // backup
  // ����� �������
  std::vector<COLORREF> m_colors;
  const COLORREF m_defColor = 0x3fdfef;

  // ��������� ���������� ����� ������ �����
  const int m_arraySizeMin = 10;
  // ������������ ���������� ����� ������ �����
  const int m_arraySizeMax = 50000;

  // ����� ����������
  SortMethod m_method = SortMethod::radix;

  // ������� ������� �����:
  // �����
  const int m_valueMin = 0;
  // ��������� ���������� ��������
  const int m_valueMaxMin = 1;
  // ������������ ���������� ��������
  const int m_valueMaxMax = 1000;
  // �������� (���� ���������� ������ �� m_valueMaxMin �� m_valueMaxMax)
  int m_valueMax = 99;

  // ��������� - ��������������� � CountingSort
  std::vector<int> m_hist;
  int m_histMax = 0;

  // ������ ������ - ��������������� � BucketSort
  int m_numBuckets = 5;
  const int m_numBucketsMin = 1;
  const int m_numBucketsMax = 20;

  // ������� ������� - RadixSort
  int GetNumBits() const;

  // �������� - ������� �������� ����������
  const int m_delayMin = 0;
  const int m_delayMax = 500;
  int m_delay = m_delayMin;

  // ����, ��� ������ ����������� (messages) ��� ��䳿 �� ����� ����� � ������ ����������
  CWnd* m_wnd = nullptr;

  // ���� ��� ��������� ������� ������� ������, ���� ������ ������ ����������
  std::atomic_flag m_resumed{};
  std::atomic_bool m_aborted{false};

  // ������� ���������� (������������ �� ������� ������)
  void Sort();
  bool RadixSort(bool update, int l, int r, int b);
  void BucketSort(bool update);
  bool CountingSort(bool update, int l, int r, int valueMin, int valueMax);
  bool InsertionSort(bool update, int l, int r);
  bool UpdateAndWait();

  // ������� ������� ������
  static void ThreadFunction(DataModel* dataModel);
};

//***************************************************************************************
