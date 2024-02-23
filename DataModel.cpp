//***************************************************************************************

#include "pch.h"
#include "DataModel.h"

//***************************************************************************************

DataModel::DataModel()
{
  std::srand((unsigned int)std::time(nullptr));

  SetArraySize(100);
  CreateArray();
}

//***************************************************************************************

const wchar_t* DataModel::GetSortMethodName(int i) const
{
  if (i == 0) return L"RadixSort";
  if (i == 1) return L"BucketSort";
  if (i == 2) return L"CountingSort";
  return L"";
}

//***************************************************************************************

void DataModel::SetSortMethod(int method)
{
  m_method = (SortMethod)method;
  if (m_method != SortMethod::radix && m_method != SortMethod::bucket && m_method != SortMethod::counting)
    m_method = SortMethod::radix;
}

//***************************************************************************************

void DataModel::SetWindow(CWnd* wnd)
{
  m_wnd = wnd;
}

//***************************************************************************************

void DataModel::SetDelay(int delay)
{
  m_delay = delay;
  if (m_delay < GetDelayMin()) m_delay = GetDelayMin();
  if (m_delay > GetDelayMax()) m_delay = GetDelayMax();
}

//***************************************************************************************

void DataModel::SetValueMax(int max)
{
  m_valueMax = max;
  if (m_valueMax < m_valueMaxMin) m_valueMax = m_valueMaxMin;
  if (m_valueMax > m_valueMaxMax) m_valueMax = m_valueMaxMax;
}

//***************************************************************************************

void DataModel::SetArraySize(int n)
{
  if (n < m_arraySizeMin) n = m_arraySizeMin;
  if (n > m_arraySizeMax) n = m_arraySizeMax;

  m_values.assign(n, 0);
  m_colors.assign(n, m_defColor);

  m_hist.clear();
  m_histMax = 0;
}

//***************************************************************************************

void DataModel::SetNumBuckets(int numBuckets)
{
  m_numBuckets = numBuckets;
  if (m_numBuckets < m_numBucketsMin) m_numBuckets = m_numBucketsMin;
  if (m_numBuckets > m_numBucketsMax) m_numBuckets = m_numBucketsMax;
}

//***************************************************************************************

void DataModel::CreateArray()
{
  int m = m_valueMax - m_valueMin + 1; // кількість можливих значень = розмір гістограми

  int n = (int)m_values.size();

  //int value = m_valueMin;

  //int curr = 0;

  for (int i = 0; i < n; i++)
  {
    m_values[i] = std::rand() % m + m_valueMin;
  }

  //for (int i = 0; i < n; i++)
  //{
  //  int value2 = value + std::rand() % (m - value - m_valueMin);
  //  if (value2 >= m_valueMax)
  //  {
  //    curr = i;
  //    break;
  //  }
  //  m_values[i] = value2;
  //  value = value2;
  //}

  //for (int i = curr; i < n; i++) m_values[i] = m_valueMax;

  // Create a backup
  m_values2 = m_values;
}

//***************************************************************************************

void DataModel::RestoreArray()
{
  m_values = m_values2;
}

//***************************************************************************************

std::wstring DataModel::GetInfo(bool useCR) const
{
  int n = (int)m_values.size();
  int k = m_valueMax - m_valueMin + 1;
  int w = GetNumBits();

  std::wstring s(useCR ? L"\r" : L"");
  s += L"\n";

  std::wstring info(L"Time complexity: ");

  switch (m_method)
  {
    case SortMethod::radix:
      info += std::format(L"O(W * N) = O({:d}){}", w * n, s);
      break;

    case SortMethod::bucket:
      info += std::format(L"O(N + Sum(Ki + Ni)) = O(K + 2N) = O({:d}){}", k + 2 * n, s);
      break;

    case SortMethod::counting:
      info += std::format(L"O(K + N) = O({:d}){}", k + n, s);
      break;
  }

  n = 0;
  for (auto& v : m_values)
  {
    n = (n + 1) % 50;
    info += std::to_wstring(v) + (n==0 ? s : L"  ");
  }

  return info;
}

//***************************************************************************************

void DataModel::StartSorting()
{
  m_resumed.test_and_set();
  m_aborted = false;

  std::thread t(ThreadFunction, this);
  t.detach();
}

//***************************************************************************************

void DataModel::PauseSorting()
{
  m_resumed.clear();
}

//***************************************************************************************

void DataModel::ResumeSorting()
{
  m_resumed.test_and_set();
  m_resumed.notify_one();
}

//***************************************************************************************

void DataModel::AbortSorting()
{
  m_aborted = true;
  m_resumed.test_and_set();
  m_resumed.notify_one();
}

//***************************************************************************************

int DataModel::GetNumBits() const
{
  // шукаємо перший ненульовий біт зліва
  int b;
  for (b = 30; b >= 0; --b) if (m_valueMax & (1 << b)) break;
  return b + 1;
}

//***************************************************************************************

void DataModel::ThreadFunction(DataModel* dataModel)
{
  dataModel->Sort();
}

void DataModel::Sort()
{
  int n = (int)m_values.size();

  bool update = (int)m_values.size() <= 500;

  switch (m_method)
  {
    case SortMethod::radix:
      RadixSort(update, 0, n - 1, GetNumBits() - 1);
      break;

    case SortMethod::bucket:
      BucketSort(update);
      break;

    case SortMethod::counting:
      CountingSort(update, 0, n - 1, m_valueMin, m_valueMax);
      break;
  }

  m_colors.assign(n, m_defColor);
  m_hist.clear();
  m_histMax = 0;

  m_wnd->SendMessageW(WM_REPAINT);
  m_wnd->PostMessageW(WM_SORTING_FINISHED);
}

//***************************************************************************************

bool DataModel::RadixSort(bool update, int l, int r, int b)
{
  if (l >= r) return true;

  const COLORREF color0 = 0xffaf7f;
  const COLORREF color1 = 0xafff7f;

  for (int i = 0; i < l; ++i) m_colors[i] = m_defColor;
  for (int i = l; i <= r; ++i) m_colors[i] = 0x7fafff;
  for (int i = r + 1; i < (int)m_colors.size(); ++i) m_colors[i] = m_defColor;

  int a = 1 << b;

  int k = l;

  for (int i = l; i <= r; ++i)
  {
    if ((m_values[i] & a) == 0) std::swap(m_values[k++], m_values[i]);

    for (int j = l; j < k; ++j) m_colors[j] = color0;
    for (int j = k; j <= i; ++j) m_colors[j] = color1;
    if (update && !UpdateAndWait()) return false;
  }

  if (b > 0)
  {
    if (!RadixSort(update, l, k - 1, b - 1)) return false;
    if (!RadixSort(update, k, r, b - 1)) return false;
  }

  return true;
}

//***************************************************************************************

void DataModel::BucketSort(bool update)
{
  std::vector<COLORREF> bucketColors{0xff7fff, 0x7fffff, 0xffff7f, 0x7f7fff, 0x7fff7f, 0xff7f7f};

  std::vector<int> numBucketValues(m_numBuckets, 0); // кількість значень в кожній корзині

  int numValues = m_valueMax - m_valueMin + 1;

  if (m_numBuckets > numValues) m_numBuckets = numValues;

  int bucketWidth = numValues / m_numBuckets;

  int lastBucketWidth = numValues - (m_numBuckets - 1) * bucketWidth;

  for (int i = 0; i < (int)m_values.size(); ++i)
  {
    int curValue = m_values[i];

    // індекс корзини, до якої треба додати поточне значення
    int b = (curValue - m_valueMin) / bucketWidth;
    if (b >= m_numBuckets) b = m_numBuckets - 1;

    int k = i;
    for (int j = m_numBuckets - 1; j > b; --j)
    {
      m_values[k] = m_values[k - numBucketValues[j]];
      k -= numBucketValues[j];
    }
    m_values[k] = curValue;

    numBucketValues[b]++;

    // кольори
    int l = 0;
    for (int b = 0; b < m_numBuckets; ++b)
    {
      for (int j = l; j < l + numBucketValues[b]; ++j)
      {
        m_colors[j] = bucketColors[b % (int)bucketColors.size()];
      }
      l += numBucketValues[b];
    }

    if (update && !UpdateAndWait()) return;
  }

  // відсортувати кожну корзину
  int l = 0;
  for (int b = 0; b < m_numBuckets; ++b)
  {
    int r = l + numBucketValues[b] - 1;

    int bucketValueMin = m_valueMin + b * bucketWidth;
    int bucketValueMax = b == m_numBuckets - 1 ? m_valueMax : bucketValueMin + bucketWidth - 1;

    //if (!InsertionSort(update, l, r)) return;
    if (!CountingSort(update, l, r, bucketValueMin, bucketValueMax)) return;

    l = r + 1;
  }
}

//***************************************************************************************

bool DataModel::CountingSort(bool update, int l, int r, int valueMin, int valueMax)
{
  int m = valueMax - valueMin + 1; // кількість можливих значень = розмір гістограми

  // попередня побудова гістограми для знаходження її максимума
  m_hist.assign(m, 0);
  for (int i = l; i <= r; ++i) m_hist[m_values[i] - valueMin]++;
  m_histMax = 0;
  for (int j = 0; j < m; ++j) if (m_histMax < m_hist[j]) m_histMax = m_hist[j];
  m_hist.assign(m, 0);

  COLORREF color = m_colors[l];
  COLORREF hlColor = 0x7f7f7f;

  // вдруге побудувати гістограму
  for (int i = l; i <= r; ++i)
  {
    m_hist[m_values[i] - valueMin]++;

    m_colors[i] = hlColor;
    if (i > l) m_colors[i - 1] = color;

    if (update && !UpdateAndWait()) return false;
  }

  m_colors[r] = color;

  // побудувати відсортований масив
  int p = l;

  for (int i = 0; i < m; ++i)
  {
    int k = m_hist[i];

    for (int j = 0; j < k; ++j)
    {
      m_colors[p] = hlColor;
      if (p > l) m_colors[p - 1] = color;

      m_values[p++] = valueMin + i;

      m_hist[i]--;

      if (update && !UpdateAndWait()) return false;
    }
  }

  m_colors[r] = color;

  return true;
}

//***************************************************************************************

bool DataModel::InsertionSort(bool update, int l, int r)
{
  for (int i = l + 1; i <= r; ++i)
  {
    int key = m_values[i];

    int64_t j = i - 1;

    while (j >= l && m_values[j] > key)
    {
      m_values[j + 1] = m_values[j];
      --j;
    }

    m_values[j + 1] = key;

    if (update && !UpdateAndWait()) return false;
  }

  return true;
}

//***************************************************************************************

bool DataModel::UpdateAndWait()
{
  m_wnd->SendMessageW(WM_REPAINT);

  std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));

  m_resumed.wait(false);

  return !m_aborted;
}

//***************************************************************************************

void DataModel::SaveResultsIntoFile(const wchar_t* fileName) const
{
  std::wofstream file(fileName);
  if (file.is_open()) file << GetInfo(false);
}

//***************************************************************************************
