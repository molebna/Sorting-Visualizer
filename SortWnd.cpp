//***************************************************************************************

#include "pch.h"
#include "SortWnd.h"

//***************************************************************************************

BEGIN_MESSAGE_MAP(SortWnd, CWnd)
  ON_WM_PAINT()
END_MESSAGE_MAP()

//***************************************************************************************

SortWnd::SortWnd()
{
  // Register window class
  const wchar_t* wndClassName = L"SortWindow";
  WNDCLASS wndcls{};
  HINSTANCE instance = ::AfxGetInstanceHandle();
  if (!::GetClassInfoW(instance, wndClassName, &wndcls))
  {
    wndcls = {};
    wndcls.style = CS_DBLCLKS;
    wndcls.lpfnWndProc = ::DefWindowProcW;
    wndcls.hInstance = instance;
    wndcls.hCursor = ::LoadCursorW(NULL, IDC_ARROW);
    wndcls.lpszClassName = wndClassName;
    ::AfxRegisterClass(&wndcls);
  }
}

//***************************************************************************************

void SortWnd::SetDataModel(const DataModel* dataModel)
{
  m_dataModel = dataModel;
}

//***************************************************************************************

void SortWnd::OnPaint()
{
  CPaintDC paintDC(this);

  RECT clientRect;
  GetClientRect(&clientRect);
  int client_rect_w = clientRect.right - clientRect.left;
  int client_rect_h = clientRect.bottom - clientRect.top;

  CDC memDC;
  memDC.CreateCompatibleDC(NULL);
  BITMAPINFOHEADER header{};
  header.biSize = sizeof(BITMAPINFOHEADER);
  header.biPlanes = 1;
  header.biBitCount = 24;
  header.biWidth = client_rect_w;
  header.biHeight = client_rect_h;
  unsigned char* bits;
  HBITMAP bitmap = ::CreateDIBSection(NULL, (BITMAPINFO*)&header, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
  HGDIOBJ origBitmap = memDC.SelectObject(bitmap);
  HGDIOBJ origBrush = memDC.SelectStockObject(DC_BRUSH);
  HGDIOBJ origPen = memDC.SelectStockObject(DC_PEN);

  COLORREF color = RGB(255,255,255); // white
  memDC.SetDCBrushColor(color);
  memDC.SetDCPenColor(color);
  memDC.Rectangle(&clientRect);

  int n = (int)m_dataModel->GetArraySize();

  if (n <= 300) memDC.SetDCPenColor(RGB(0,0,0)); // black

  RECT rect1 = clientRect;
  rect1.top = rect1.bottom - 3 * (rect1.bottom - rect1.top) / 4;
  int w1 = rect1.right - rect1.left;
  int h1 = rect1.bottom - rect1.top;

  RECT rect2 = clientRect;
  rect2.bottom = rect1.top - 10;
  int w2 = rect2.right - rect2.left;
  int h2 = rect2.bottom - rect2.top;

  // data

  int min = m_dataModel->GetValueMin();
  int max = m_dataModel->GetValueMax();

  double w = (double)(w1 - 1) / n;

  if (min < max)
  {
    for (int i = 0; i < n; ++i)
    {
      COLORREF color = m_dataModel->GetArrayElemColor(i);

      memDC.SetDCBrushColor(color);
      if (n > 300) memDC.SetDCPenColor(color);

      int l = std::lround(i * w);
      int r = std::lround((i + 1) * w) + 1;
      int b = rect1.bottom;
      int t = b - h1 * (m_dataModel->GetArrayElem(i) - min + 1) / (max - min + 1);
      RECT rect{ l, t, r, b };

      memDC.Rectangle(&rect);
    }
  }

  // histogram
  int m = (int)m_dataModel->GetHistSize();
  if (m > 0)
  {
    w = (double)(w2 - 1) / m;

    min = 0;
    max = m_dataModel->GetHistMax();

    if (min < max)
    {
      memDC.SetDCBrushColor(m_dataModel->GetHistColor());

      for (int i = 0; i < m; ++i)
      {
        int l = std::lround(i * w);
        int r = std::lround((i + 1) * w) + 1;
        int b = rect2.bottom;
        int t = b - h2 * (m_dataModel->GetHistElem(i) - min) / (max - min);
        RECT rect{ l, t, r, b };

        memDC.Rectangle(&rect);
      }
    }
  }

  paintDC.BitBlt(0, 0, client_rect_w, client_rect_h, &memDC, 0, 0, SRCCOPY);

  memDC.SelectObject(origPen);
  memDC.SelectObject(origBrush);
  memDC.SelectObject(origBitmap);
    ::DeleteObject(bitmap);
}

//***************************************************************************************
