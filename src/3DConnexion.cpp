#include "3dconnexion.h"

C3DConnexion::C3DConnexion(void)
{
	HRESULT hr;
	CComPtr<IUnknown> p3DxDevice;
	CComPtr<ISimpleDevice> p3DxSimpleDevice;
	m_ValidFlag = FALSE;

    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	hr = p3DxDevice.CoCreateInstance(__uuidof(Device));
	if (SUCCEEDED(hr))
	{
		hr = p3DxDevice.QueryInterface(&p3DxSimpleDevice);
		if (SUCCEEDED(hr)) {
			m_3DSensor    = p3DxSimpleDevice->Sensor;
			m_3DKeyboard  = p3DxSimpleDevice->Keyboard;
			p3DxSimpleDevice->LoadPreferences(_T("3DMouse Controller"));
			p3DxSimpleDevice->Connect();
			m_ValidFlag = TRUE;
		}
	}
	CreateMainWindow(0, 0, 100, 100, "3DConnexion.cpp");
}

C3DConnexion::~C3DConnexion(void)
{
	CComPtr<ISimpleDevice> p3DxDevice;
	if (m_3DSensor)
	{
		m_3DSensor->get_Device((IDispatch**)&p3DxDevice);
		m_3DSensor.Release();
	}

	if (m_3DKeyboard)
		m_3DKeyboard.Release();

	if (p3DxDevice)
	{
		// Disconnect it from the driver
		p3DxDevice->Disconnect();
		p3DxDevice.Release();
	}
}

HRESULT C3DConnexion::UpdateData(void)
{
	MSG msg; 

	if(m_3DSensor){
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) { 
			GetMessage (&msg,NULL,0,0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CComPtr<IAngleAxis> pRotation   = m_3DSensor->Rotation;
		CComPtr<IVector3D> pTranslation = m_3DSensor->Translation;

		tX = pTranslation->X;
		tY = pTranslation->Y;
		tZ = pTranslation->Z;

		rX = pRotation->X * pRotation->Angle;
		rY = pRotation->Y * pRotation->Angle;
		rZ = pRotation->Z * pRotation->Angle;

		pRotation.Release();
		pTranslation.Release();
		return S_OK;
	}
	return E_FAIL;
}

LRESULT 
WINAPI MouseMessageProc( HWND hWnd, unsigned msg, WPARAM wParam,
                           LPARAM lParam );

void 
C3DConnexion::CreateMainWindow (int x, int y, int h, int w, char *caption)

{
   WNDCLASS      wndClass;
   HINSTANCE     hInst;

   hInst = ::GetModuleHandle(NULL);
   
   wndClass.style         = CS_HREDRAW | CS_VREDRAW ;
   wndClass.lpfnWndProc   = (WNDPROC) MouseMessageProc ;
   wndClass.cbClsExtra    = 8;
   wndClass.cbWndExtra    = 0;
   wndClass.hInstance     = hInst;
   wndClass.hIcon         = NULL;
   wndClass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName  = NULL ;
   wndClass.lpszClassName = "3DConnexion.cpp";
   RegisterClass (&wndClass) ;

   m_hWnd = CreateWindow ( "3DConnection.cpp",
	   caption,
	   WS_OVERLAPPEDWINDOW, 
	   x, y, w, h,
	   NULL, NULL,
	   hInst,
	   NULL);   
   ShowWindow(m_hWnd, SW_HIDE);
   UpdateWindow(m_hWnd);
}

LRESULT WINAPI MouseMessageProc( HWND hWnd,
								unsigned msg,
								WPARAM wParam,
								LPARAM lParam)
{
	switch ( msg ) {
	case WM_ACTIVATEAPP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_PAINT:
	case WM_CLOSE:
		break;
	case WM_DESTROY :
		PostQuitMessage (0);
		return (0);
	}
	return DefWindowProc ( hWnd, msg, wParam, lParam );
} 



