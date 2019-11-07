// ReVoL 2018 Primer Template
// ccreate.h
void setwf(HWND W,HFONT F){SendMessage(W,WM_SETFONT,(WPARAM)F,0);}
void setwp(HWND W,WNDPROC F){SetWindowLong(W,GWL_WNDPROC,(LONG)F);}
HWND CCreatePaintBox(HINSTANCE M,HWND P,int L,int T,int W,int H,WNDPROC F){HWND w=CreateWindow("STATIC",0,0x50800000,L,T,W,H,P,0,M,0); if(w){setwp(w,F);}return w;}
HWND CCreateStatic(HINSTANCE M,HWND P,int L,int T,int W,int H,WNDPROC F){HWND w=CreateWindow("STATIC",0,0x50000000,L,T,W,H,P,0,M,0); if(w){setwp(w,F);}return w;}
HWND CCreateLabel(HINSTANCE M,HWND P,int L,int T,int W,int H,HFONT F,const char* t){HWND w=CreateWindow("STATIC",t,0x50000000,L,T,W,H,P,0,M,0); if(W){setwf(w,F);}return w;}
HWND CCreateButton3P(HINSTANCE M, HWND P, int L, int T, int W, int H, int id, int rid, int s) { HWND w = CreateWindow("BUTTON", 0, s, L, T, W, H, P, (HMENU)id, M, 0); if (!w)return 0;HICON i=LoadIcon(M, MAKEINTRESOURCE(rid)); SendMessage(w, BM_SETIMAGE, IMAGE_ICON, LPARAM(i)); return w; }
