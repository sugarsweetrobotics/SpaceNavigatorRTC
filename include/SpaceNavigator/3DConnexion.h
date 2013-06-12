#pragma once

#include <Windows.h>
#include <atlstr.h> 
#import "progid:TDxInput.Device.1" no_namespace

class C3DConnexion
{
private:
	CComPtr<ISensor> m_3DSensor;
	CComPtr<IKeyboard> m_3DKeyboard;

private:
	BOOL m_ValidFlag;

public:
	BOOL IsValid() {return m_ValidFlag;}
	HWND m_hParentWnd;
	HWND m_hWnd;
	void CreateMainWindow (HINSTANCE hInst, int x, int y, int h, int w, char *caption);

public:

	//コントローラの情報．UpdateData関数で更新すること．
    
	union {
		struct {
			//並進
			double tX;
			double tY;
			double tZ;

			//回転
			double rX;
			double rY;
			double rZ;
		};
		double data[6];
	};

   WNDCLASSEX      wndClass;
   HINSTANCE     m_hInst;

public:
	C3DConnexion(HINSTANCE hInst, HWND hWnd);
	~C3DConnexion(void);
	
public:
	HRESULT UpdateData(void);
};
