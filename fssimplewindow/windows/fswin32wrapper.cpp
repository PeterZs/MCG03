#pragma warning(disable:4996)

#if defined(_WIN32_WINNT) && _WIN32_WINNT<0x0500
#undef _WIN32_WINNT
#endif

#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x500
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>
#include <direct.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "fssimplewindow.h"
#include "fswin32keymap.h"

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

static FsWin32KeyMapper fsKeyMapper;



#include "fssimplewindowinternal.h"

static FsSimpleWindowInternal fsWin32Internal;
static void (*IntervalCallBack)(void *)=NULL;
static void *IntervalCallBackParameter=NULL;

void FsRegisterIntervalCallBack(void (*callback)(void *),void *param)
{
	IntervalCallBack=callback;
	IntervalCallBackParameter=param;
}


FsSimpleWindowInternal::FsSimpleWindowInternal()
{
	hWnd=NULL;
	hRC=NULL;
	hPlt=NULL;
	hDC=NULL;
}

const FsSimpleWindowInternal *FsGetSimpleWindowInternal(void)
{
	return &fsWin32Internal;
}


static LONG WINAPI WindowFunc(HWND wnd,UINT msg,WPARAM wp,LPARAM lp);
static void YsSetPixelFormat(HDC dc);
static HPALETTE YsCreatePalette(HDC dc);
static void InitializeOpenGL(HWND wnd);



// For OpenGL set up >>
static int doubleBuffer=0;
// For OpenGL set up <<


class FsMouseEventLog
{
public:
	int eventType;
	int lb,mb,rb;
	int mx,my;
	unsigned int shift,ctrl;
};


#define NKEYBUF 256
static int keyBuffer[NKEYBUF];
static int nKeyBufUsed=0;
static int charBuffer[NKEYBUF];
static int nCharBufUsed=0;
static int nMosBufUsed=0;
static FsMouseEventLog mosBuffer[NKEYBUF];

static int exposure=0;


#define WINSTYLE WS_OVERLAPPED|WS_CAPTION|WS_VISIBLE|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX

static const char *WINCLASS="FsSimpleWindow_MainWindowClass";
static const char *WINNAME="MainWindow";

void FsOpenWindow(int x0,int y0,int wid,int hei,int useDoubleBuffer)
{
	FsOpenWindow(x0,y0,wid,hei,useDoubleBuffer,NULL);
}

void FsOpenWindow(int x0,int y0,int wid,int hei,int useDoubleBuffer,const char windowName[])
{
	if(NULL!=fsWin32Internal.hWnd)
	{
		MessageBoxA(fsWin32Internal.hWnd,"Error! Window already exists.","Error!",MB_OK);
		exit(1);
	}

	// Note 2012/03/08 RegisterClassW and CreateWindowW doesn't seem to work.
	WNDCLASSA wc;
	HINSTANCE inst=GetModuleHandleA(NULL);

	wc.style=CS_OWNDC|CS_BYTEALIGNWINDOW;
	wc.lpfnWndProc=(WNDPROC)WindowFunc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=(HINSTANCE)inst;
	wc.hIcon=LoadIconA(inst,"MAINICON");
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName=NULL;
	wc.lpszClassName=WINCLASS;
	if(0!=RegisterClassA(&wc))
	{
		doubleBuffer=useDoubleBuffer;


		RECT rc;
		rc.left  =x0;
		rc.top   =y0;
		rc.right =(unsigned long)(x0+wid-1);
		rc.bottom=(unsigned long)(y0+hei-1);
		AdjustWindowRect(&rc,WINSTYLE,FALSE);
		wid  =rc.right-rc.left+1;
		hei  =rc.bottom-rc.top+1;

#ifdef _UNICODE
		// What's the point of using CreateWindowA?  Another weird Microsoft logic here.
		static wchar_t buf[256];
		const char *windowNameA=(const char *)(NULL!=windowName ? windowName : WINNAME);
		for(int i=0; i<255 && 0!=windowNameA[i]; ++i)
		{
			buf[i]=windowNameA[i];
			buf[i+1]=0;
		}
		const char *windowNameUsed=(const char *)buf;
#else
		const char *windowNameUsed=(const char *)(NULL!=windowName ? windowName : WINNAME);
#endif

		fsWin32Internal.hWnd=CreateWindowA(WINCLASS,windowNameUsed,WINSTYLE,x0,y0,wid,hei,NULL,NULL,inst,NULL);
		if(NULL!=fsWin32Internal.hWnd)
		{
			InitializeOpenGL(fsWin32Internal.hWnd);

			ShowWindow(fsWin32Internal.hWnd,SW_SHOWNORMAL);
			UpdateWindow(fsWin32Internal.hWnd);

			FsPassedTime();  // Reset Timer
		}
		else
		{
			MessageBoxA(NULL,"Error! Could not open a window.","Error!",MB_OK);
			exit(1);
		}
	}
}

void FsCloseWindow(void)
{
	DestroyWindow(fsWin32Internal.hWnd);
	fsWin32Internal.hWnd=NULL;
	fsWin32Internal.hRC=NULL;
	fsWin32Internal.hPlt=NULL;
	fsWin32Internal.hDC=NULL;
}

void FsGetWindowSize(int &wid,int &hei)
{
	RECT rect;
	GetClientRect(fsWin32Internal.hWnd,&rect);
	wid=rect.right;
	hei=rect.bottom;
}

void FsGetWindowPosition(int &x0,int &y0)
{
	POINT pnt={0,0};
	ClientToScreen(fsWin32Internal.hWnd,&pnt);

	RECT rect;
	GetClientRect(fsWin32Internal.hWnd,&rect);
	rect.left+=pnt.x;
	rect.top+=pnt.y;

	AdjustWindowRect(&rect,WINSTYLE,FALSE);

	x0=rect.left;
	y0=rect.top;
}

void FsPollDevice(void)
{
	MSG msg;
	while(PeekMessage(&msg,fsWin32Internal.hWnd,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void FsSleep(int ms)
{
	if(ms>0)
	{
		Sleep(ms);
	}
}

long long int FsPassedTime(void)
{
	static long long int lastTick;
	long long int tick;

	static int first=1;
	if(1==first)
	{
		lastTick=FsSubSecondTimer();
		first=0;
	}

	tick=FsSubSecondTimer();
	long long passed=tick-lastTick;
	lastTick=tick;

	return passed;
}

long long int FsSubSecondTimer(void)
{
	// If I give up on Windows XP, I can use GetTickCount64.

	long long int clk=GetTickCount();

	static long long int lastValue=0;
	static long long int base=0;

	if(clk<lastValue) // Underflow.  49.7 days have passed.
	{
		base+=0x100000000LL;
	}
	lastValue=clk;

	static int first=1;
	static long long int t0=0;
	if(1==first)
	{
		t0=base+clk;
		first=0;
	}

	return base+clk-t0;
}

int FsInkey(void)
{
	if(nKeyBufUsed>0)
	{
		int i,keyCode;
		keyCode=keyBuffer[0];
		nKeyBufUsed--;
		for(i=0; i<nKeyBufUsed; i++)
		{
			keyBuffer[i]=keyBuffer[i+1];
		}
		return keyCode;
	}
	return 0;
}

int FsInkeyChar(void)
{
	if(nCharBufUsed>0)
	{
		int i,asciiCode;
		asciiCode=charBuffer[0];
		nCharBufUsed--;
		for(i=0; i<nCharBufUsed; i++)
		{
			charBuffer[i]=charBuffer[i+1];
		}
		return asciiCode;
	}
	return 0;
}

int FsGetKeyState(int fsKeyCode)
{
	const int vk=fsKeyMapper.FsKeyToVk(fsKeyCode);
	return (GetKeyState(vk)&0x8000)!=0;
}

int FsCheckWindowExposure(void)
{
	const int ret=exposure;
	exposure=0;
	return ret;
}

void FsGetMouseState(int &lb,int &mb,int &rb,int &mx,int &my)
{
	POINT cur;
	GetCursorPos(&cur);
	ScreenToClient(fsWin32Internal.hWnd,&cur);

	mx=cur.x;
	my=cur.y;

	lb=((GetKeyState(VK_LBUTTON)&0x8000)!=0 ? 1 : 0);
	mb=((GetKeyState(VK_MBUTTON)&0x8000)!=0 ? 1 : 0);
	rb=((GetKeyState(VK_RBUTTON)&0x8000)!=0 ? 1 : 0);
}

int FsGetMouseEvent(int &lb,int &mb,int &rb,int &mx,int &my)
{
	if(0<nMosBufUsed)
	{
		int eventType=mosBuffer[0].eventType;
		mx=mosBuffer[0].mx;
		my=mosBuffer[0].my;
		lb=mosBuffer[0].lb;
		mb=mosBuffer[0].mb;
		rb=mosBuffer[0].rb;

		int i;
		for(i=0; i<nMosBufUsed-1; i++)
		{
			mosBuffer[i]=mosBuffer[i+1];
		}
		nMosBufUsed--;

		return eventType;
	}
	else
	{
		FsGetMouseState(lb,mb,rb,mx,my);
		return FSMOUSEEVENT_NONE;
	}
}

void FsSwapBuffers(void)
{
	if(0==doubleBuffer)
	{
		MessageBoxA(NULL,"Error! FsSwapBuffers used in the single-buffered mode.","Error!",MB_OK);
		exit(1);
	}

	HDC hDC;
	glFlush();
	hDC=wglGetCurrentDC();
	SwapBuffers(hDC);
}

static LONG WINAPI WindowFunc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	switch(msg)
	{
	case WM_QUERYNEWPALETTE:
	case WM_PALETTECHANGED:
		if(NULL!=fsWin32Internal.hPlt)
		{
			SelectPalette(fsWin32Internal.hDC,fsWin32Internal.hPlt,FALSE);
			RealizePalette(fsWin32Internal.hDC);
		}
		return DefWindowProc(hWnd,msg,wp,lp);
	case WM_CREATE:
		fsWin32Internal.hDC=GetDC(hWnd);
		YsSetPixelFormat(fsWin32Internal.hDC);
		fsWin32Internal.hRC=wglCreateContext(fsWin32Internal.hDC);
		wglMakeCurrent(fsWin32Internal.hDC,fsWin32Internal.hRC);
		if(0==doubleBuffer)
		{
			glDrawBuffer(GL_FRONT);
		}
		InitializeOpenGL(hWnd);
		break;
	case WM_SIZE:
		wglMakeCurrent(fsWin32Internal.hDC,fsWin32Internal.hRC);
		break;
	case WM_PAINT:
		wglMakeCurrent(fsWin32Internal.hDC,fsWin32Internal.hRC);
		exposure=1;
		if(NULL!=IntervalCallBack)
		{
			(*IntervalCallBack)(IntervalCallBackParameter);
		}
		return DefWindowProc(hWnd,msg,wp,lp);
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		exit(1);
		break;
	case WM_MOUSEWHEEL:
		{
			int step;
			step=HIWORD(wp);
			if(step>=0x8000)
			{
				step-=0x10000;
			}
			step/=WHEEL_DELTA;
			if(step>0)
			{
				while(step>0)
				{
					if(nKeyBufUsed<NKEYBUF)
					{
						keyBuffer[nKeyBufUsed++]=FSKEY_WHEELUP;
					}
					step--;
				}
			}
			else if(step<0)
			{
				while(step<0)
				{
					if(nKeyBufUsed<NKEYBUF)
					{
						keyBuffer[nKeyBufUsed++]=FSKEY_WHEELDOWN;
					}
					step++;
				}
			}
		}
		break;
	case WM_SYSCOMMAND:
		if((wp&(~0x0f))==SC_KEYMENU && (('0'<=lp && lp<'9') || ('A'<=lp && lp<='Z') || ('a'<=lp && lp<='z')))
		{
			// Prevent beep.
			return 0;
		}
		return DefWindowProc(hWnd,msg,wp,lp);
	case WM_SYSKEYDOWN:
		if((lp & (1<<29))!=0 && // Alt
		  (wp==VK_MENU ||
		   wp==VK_OEM_1 ||
		   wp==VK_OEM_PLUS ||
		   wp==VK_OEM_COMMA ||
		   wp==VK_OEM_MINUS ||
		   wp==VK_OEM_PERIOD ||
		   wp==VK_OEM_2 ||
		   wp==VK_OEM_3 ||
		   wp==VK_OEM_4 ||
		   wp==VK_OEM_5 ||
		   wp==VK_OEM_6 ||
		   wp==VK_OEM_7 ||
		   wp==VK_OEM_8 ||
#ifdef VK_OEM_AX
		   wp==VK_OEM_AX ||
#endif
		   wp==VK_OEM_102 ||
		   wp=='0' ||
		   wp=='1' ||
		   wp=='2' ||
		   wp=='3' ||
		   wp=='4' ||
		   wp=='5' ||
		   wp=='6' ||
		   wp=='7' ||
		   wp=='8' ||
		   wp=='9' ||
		   wp=='A' ||
		   wp=='B' ||
		   wp=='C' ||
		   wp=='D' ||
		   wp=='E' ||
		   wp=='F' ||
		   wp=='G' ||
		   wp=='H' ||
		   wp=='I' ||
		   wp=='J' ||
		   wp=='K' ||
		   wp=='L' ||
		   wp=='M' ||
		   wp=='N' ||
		   wp=='O' ||
		   wp=='P' ||
		   wp=='Q' ||
		   wp=='R' ||
		   wp=='S' ||
		   wp=='T' ||
		   wp=='U' ||
		   wp=='V' ||
		   wp=='W' ||
		   wp=='X' ||
		   wp=='Y' ||
		   wp=='Z' ||
		   wp==VK_ESCAPE ||
		   wp==VK_F1 ||
		   wp==VK_F2 ||
		   wp==VK_F3 ||
		   /* wp==VK_F4 || */
		   wp==VK_F5 ||
		   wp==VK_F6 ||
		   wp==VK_F7 ||
		   wp==VK_F8 ||
		   wp==VK_F9 ||
		   wp==VK_F10 ||
		   wp==VK_F11 ||
		   wp==VK_F12 ||
		   wp==VK_RETURN ||
		   wp==VK_NUMLOCK ||
		   wp==VK_NUMPAD0 ||
		   wp==VK_NUMPAD1 ||
		   wp==VK_NUMPAD2 ||
		   wp==VK_NUMPAD3 ||
		   wp==VK_NUMPAD4 ||
		   wp==VK_NUMPAD5 ||
		   wp==VK_NUMPAD6 ||
		   wp==VK_NUMPAD7 ||
		   wp==VK_NUMPAD8 ||
		   wp==VK_NUMPAD9 ||
		   wp==VK_DECIMAL ||
		   wp==VK_DIVIDE ||
		   wp==VK_MULTIPLY ||
		   wp==VK_SUBTRACT ||
		   wp==VK_ADD))
		{
			int keyCode;
			keyCode=fsKeyMapper.VkToFsKey(wp);
			if(keyCode!=0 && nKeyBufUsed<NKEYBUF)
			{
				keyBuffer[nKeyBufUsed++]=keyCode;
			}
			return 0;
		}
		return DefWindowProc(hWnd,msg,wp,lp);
	case WM_SYSKEYUP:
		return 0;
	case WM_KEYDOWN:
		if(nKeyBufUsed<NKEYBUF)
		{
			int keyCode;
			keyCode=fsKeyMapper.VkToFsKey(wp);
			if(keyCode!=0)
			{
				keyBuffer[nKeyBufUsed++]=keyCode;
			}
		}
		break;
	case WM_CHAR:
		if(nCharBufUsed<NKEYBUF)
		{
			charBuffer[nCharBufUsed++]=wp;
		}
		break;
	case WM_ERASEBKGND:
		return 1;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		if(nMosBufUsed<NKEYBUF)
		{
			int eventType;
			switch(msg)
			{
			default:
				eventType=FSMOUSEEVENT_NONE;
				break;
			case WM_LBUTTONDOWN:
				eventType=FSMOUSEEVENT_LBUTTONDOWN;
				break;
			case WM_LBUTTONUP:
				eventType=FSMOUSEEVENT_LBUTTONUP;
				break;
			case WM_MBUTTONDOWN:
				eventType=FSMOUSEEVENT_MBUTTONDOWN;
				break;
			case WM_MBUTTONUP:
				eventType=FSMOUSEEVENT_MBUTTONUP;
				break;
			case WM_RBUTTONDOWN:
				eventType=FSMOUSEEVENT_RBUTTONDOWN;
				break;
			case WM_RBUTTONUP:
				eventType=FSMOUSEEVENT_RBUTTONUP;
				break;
			case WM_MOUSEMOVE:
				eventType=FSMOUSEEVENT_MOVE;
				break;
			}

			int lb=((wp & MK_LBUTTON)!=0);
			int mb=((wp & MK_MBUTTON)!=0);
			int rb=((wp & MK_RBUTTON)!=0);
			unsigned int shift=((wp & MK_SHIFT)!=0);
			unsigned int ctrl=((wp & MK_CONTROL)!=0);
			int mx=LOWORD(lp);
			int my=HIWORD(lp);

			if(eventType==FSMOUSEEVENT_MOVE &&
			   0<nMosBufUsed &&
			   mosBuffer[nMosBufUsed-1].eventType==FSMOUSEEVENT_MOVE &&
			   mosBuffer[nMosBufUsed-1].lb==lb &&
			   mosBuffer[nMosBufUsed-1].mb==mb &&
			   mosBuffer[nMosBufUsed-1].rb==rb &&
			   mosBuffer[nMosBufUsed-1].shift==shift &&
			   mosBuffer[nMosBufUsed-1].ctrl==ctrl)
			{
				mosBuffer[nMosBufUsed-1].mx=mx;
				mosBuffer[nMosBufUsed-1].my=my;
				break;
			}

			mosBuffer[nMosBufUsed].eventType=eventType;
			mosBuffer[nMosBufUsed].lb=lb;
			mosBuffer[nMosBufUsed].mb=mb;
			mosBuffer[nMosBufUsed].rb=rb;
			mosBuffer[nMosBufUsed].shift=shift;
			mosBuffer[nMosBufUsed].ctrl=ctrl;
			mosBuffer[nMosBufUsed].mx=mx;
			mosBuffer[nMosBufUsed].my=my;
			nMosBufUsed++;
		}
		break;

	default:
		return DefWindowProc(hWnd,msg,wp,lp);
	}
	return 1;
}

static void YsSetPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int pfm;
	pfm=ChoosePixelFormat(hDC,&pfd);
	if(pfm!=0)
	{
		if(SetPixelFormat(hDC,pfm,&pfd)!=FALSE)
		{
			fsWin32Internal.hPlt=YsCreatePalette(hDC);
			SelectPalette(hDC,fsWin32Internal.hPlt,FALSE);
			RealizePalette(hDC);
			return;
		}
	}

	MessageBoxA(NULL,"Error In YsSetPixelFormat.",NULL,MB_OK);
	exit(1);
}

static unsigned char YsPalVal(unsigned long n,unsigned bit,unsigned sft)
{
	unsigned long msk;
	n>>=sft;
	msk=(1<<bit)-1;
	n&=msk;
	return (unsigned char)(n*255/msk);
}

/* ? lp=LocalAlloc(LMEM_FIXED,sizeof(LOGPALETTE)+n*sizeof(PALETTEENTRY)); */
/* ? LocalFree(lp); */

static HPALETTE YsCreatePalette(HDC dc)
{
	HPALETTE neo;
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE *lp;
    int n,i;

    n=GetPixelFormat(dc);
    DescribePixelFormat(dc,n,sizeof(PIXELFORMATDESCRIPTOR),&pfd);

    if(pfd.dwFlags & PFD_NEED_PALETTE)
    {
        n=1<<pfd.cColorBits;
        lp=(LOGPALETTE *)malloc(sizeof(LOGPALETTE)+n*sizeof(PALETTEENTRY));
        lp->palVersion=0x300;
        lp->palNumEntries=(WORD)n;
        for (i=0; i<n; i++)
        {
            lp->palPalEntry[i].peRed  =YsPalVal(i,pfd.cRedBits,pfd.cRedShift);
            lp->palPalEntry[i].peGreen=YsPalVal(i,pfd.cGreenBits,pfd.cGreenShift);
            lp->palPalEntry[i].peBlue =YsPalVal(i,pfd.cBlueBits,pfd.cBlueShift);
            lp->palPalEntry[i].peFlags=0;
        }

        neo=CreatePalette(lp);
		free(lp);
	    return neo;
    }
	return NULL;
}

static void InitializeOpenGL(HWND wnd)
{
	RECT rect;

    GetClientRect(wnd,&rect);

    glClearColor(1.0F,1.0F,1.0F,0.0F);
    glClearDepth(1.0F);
	glDisable(GL_DEPTH_TEST);

	glViewport(0,0,rect.right,rect.bottom);

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,(float)rect.right-1,(float)rect.bottom-1,0,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_FLAT);
	glPointSize(1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3ub(0,0,0);
}

void FsChangeToProgramDir(void)
{
	char fn[MAX_PATH];
	GetModuleFileNameA(NULL,fn,MAX_PATH);

	const int l=strlen(fn);
	int i;
	for(i=l-1; i>=0; i--)
	{
		if(fn[i]=='\\' || fn[i]=='/')
		{
			fn[i]=0;
			break;
		}
	}

	_chdir(fn);
}



