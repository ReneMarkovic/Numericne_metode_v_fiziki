// draw.cpp — Animiran graf v C++ z Windows GDI (brez zunanjih knjiznic)
//
// Tipke:  PRESLEDEK = pavza/nadaljuj   R = ponovi   +/- = korak dt   ESC = izhod
//
// Kompilacija (MinGW):
//   g++ draw.cpp -o draw.exe -lgdi32 -mwindows
//
// Kompilacija (MSVC):
//   cl draw.cpp /Fe:draw.exe user32.lib gdi32.lib
//
// Zaženi:
//   draw.exe

#define WIN32_LEAN_AND_MEAN
#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include <math.h>
#include <stdio.h>

// ------------------------------------------------------------------ //
//  Fizikalni parametri
// ------------------------------------------------------------------ //

#define PI    3.14159265358979
#define G     10.0
#define RHO_Z 10.0
#define RHO_A 1.29
#define R     0.05
#define CD    1.0
#define S_    (4.0 * PI * R * R)
#define VOL   (4.0 / 3.0 * PI * R * R * R)
#define M_    (RHO_Z * VOL)
#define K_    (0.5 * RHO_A * CD * S_ / M_)

double accel(double v)              { return G - K_ * v * v; }
double v_terminal()                 { return sqrt(G / K_); }
double v_exact(double t)            { double vT = v_terminal(); return vT * tanh((G/vT)*t); }
double euler_step(double v, double dt) { return v + accel(v) * dt; }
double rk4_step(double v, double dt) {
    double k1 = accel(v)           * dt;
    double k2 = accel(v + k1/2)   * dt;
    double k3 = accel(v + k2/2)   * dt;
    double k4 = accel(v + k3)     * dt;
    return v + (k1 + 2*k2 + 2*k3 + k4) / 6.0;
}

// ------------------------------------------------------------------ //
//  Simulacija
// ------------------------------------------------------------------ //

#define MAX_PTS 2000
int    N_pts;
double t_pts[MAX_PTS], v_eu[MAX_PTS], v_rk[MAX_PTS], v_an[MAX_PTS];
double VT, DT = 1.0;
double T_MAX = 60.0;

// Animacija
int  anim_step = 0;   // koliko točk je že narisanih
int  paused    = 0;   // 1 = pavza
#define TIMER_ID  1
#define TIMER_MS  60  // ms med kadri (~16 fps)

void simulate() {
    VT = v_terminal();
    double ve = 0, vr = 0;
    N_pts = 0;
    for (double t = 0; t <= T_MAX && N_pts < MAX_PTS; t += DT, N_pts++) {
        t_pts[N_pts] = t;
        v_eu[N_pts]  = ve;
        v_rk[N_pts]  = vr;
        v_an[N_pts]  = v_exact(t);
        ve = euler_step(ve, DT);
        vr = rk4_step(vr, DT);
    }
}

// ------------------------------------------------------------------ //
//  Graf — koordinatni sistem
// ------------------------------------------------------------------ //

typedef struct { int l, r, t, b; } Pad;  // robovi v pikslih

// Pretvori podatkovne koordinate v piksle
int px(double t, RECT rc, Pad p) {
    double gw = rc.right - rc.left - p.l - p.r;
    return (int)(rc.left + p.l + (t / T_MAX) * gw);
}
int py(double v, RECT rc, Pad p) {
    double gh = rc.bottom - rc.top - p.t - p.b;
    double vmax = VT * 1.1;
    return (int)(rc.bottom - p.b - (v / vmax) * gh);
}

// ------------------------------------------------------------------ //
//  Risanje
// ------------------------------------------------------------------ //

void draw_graph(HDC hdc, RECT rc) {
    Pad p = {70, 20, 50, 55};
    double vmax = VT * 1.1;

    // --- Ozadje ---
    HBRUSH bg = CreateSolidBrush(RGB(22, 22, 30));
    FillRect(hdc, &rc, bg);
    DeleteObject(bg);

    // --- Mreza ---
    HPEN grid_pen = CreatePen(PS_SOLID, 1, RGB(55, 55, 65));
    SelectObject(hdc, grid_pen);
    SetTextColor(hdc, RGB(160, 160, 160));
    SetBkMode(hdc, TRANSPARENT);

    HFONT small_font = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                                  DEFAULT_CHARSET, 0, 0, 0, 0, "Consolas");
    SelectObject(hdc, small_font);

    // Vodoravne crte (os v)
    for (int v = 0; v <= (int)(vmax); v += 5) {
        int y = py(v, rc, p);
        MoveToEx(hdc, px(0, rc, p), y, NULL);
        LineTo(hdc,   px(T_MAX, rc, p), y);
        char buf[16]; sprintf(buf, "%d", v);
        TextOut(hdc, px(0, rc, p) - 38, y - 7, buf, (int)strlen(buf));
    }
    // Navpicne crte (os t)
    for (int t = 0; t <= (int)T_MAX; t += 10) {
        int x = px(t, rc, p);
        MoveToEx(hdc, x, py(0, rc, p), NULL);
        LineTo(hdc,   x, py(vmax, rc, p));
        char buf[16]; sprintf(buf, "%ds", t);
        TextOut(hdc, x - 10, py(0, rc, p) + 8, buf, (int)strlen(buf));
    }
    DeleteObject(grid_pen);

    // --- Terminalna hitrost (crtkano) ---
    HPEN vt_pen = CreatePen(PS_DASH, 1, RGB(200, 180, 50));
    SelectObject(hdc, vt_pen);
    int y_vt = py(VT, rc, p);
    MoveToEx(hdc, px(0, rc, p), y_vt, NULL);
    LineTo(hdc,   px(T_MAX, rc, p), y_vt);
    char vt_buf[32]; sprintf(vt_buf, "vT = %.1f m/s", VT);
    SetTextColor(hdc, RGB(200, 180, 50));
    TextOut(hdc, px(T_MAX, rc, p) - 90, y_vt - 18, vt_buf, (int)strlen(vt_buf));
    DeleteObject(vt_pen);

    // --- Krivulje (do anim_step) ---
    int  draw_to = (anim_step < N_pts) ? anim_step : N_pts - 1;

    struct { double* ys; COLORREF col; int width; } curves[] = {
        { v_eu, RGB(255,  80,  50), 2 },   // Euler    — rdeca
        { v_rk, RGB( 50, 220,  80), 2 },   // RK4      — zelena
        { v_an, RGB( 80, 160, 255), 3 },   // eksaktna — modra
    };

    for (int ci = 0; ci < 3; ci++) {
        HPEN pen = CreatePen(PS_SOLID, curves[ci].width, curves[ci].col);
        SelectObject(hdc, pen);
        MoveToEx(hdc, px(t_pts[0], rc, p), py(curves[ci].ys[0], rc, p), NULL);
        for (int i = 1; i <= draw_to; i++)
            LineTo(hdc, px(t_pts[i], rc, p), py(curves[ci].ys[i], rc, p));
        DeleteObject(pen);

        // Premikajoča se pikica na koncu krivulje
        if (draw_to >= 0 && anim_step < N_pts) {
            int cx = px(t_pts[draw_to], rc, p);
            int cy = py(curves[ci].ys[draw_to], rc, p);
            HBRUSH dot_br = CreateSolidBrush(curves[ci].col);
            HPEN   dot_pn = CreatePen(PS_SOLID, 1, curves[ci].col);
            SelectObject(hdc, dot_pn);
            SelectObject(hdc, dot_br);
            Ellipse(hdc, cx - 5, cy - 5, cx + 5, cy + 5);
            DeleteObject(dot_br);
            DeleteObject(dot_pn);
        }
    }

    // --- Info panel (trenutne vrednosti) ---
    if (draw_to >= 0) {
        char info[128];
        sprintf(info, "t = %.1f s     Euler = %.3f     RK4 = %.3f     eksaktna = %.3f",
                t_pts[draw_to], v_eu[draw_to], v_rk[draw_to], v_an[draw_to]);
        SelectObject(hdc, small_font);
        SetTextColor(hdc, RGB(200, 200, 200));
        TextOut(hdc, px(0, rc, p), py(0, rc, p) + 22, info, (int)strlen(info));

        if (paused) {
            SetTextColor(hdc, RGB(255, 200, 50));
            TextOut(hdc, px(T_MAX, rc, p) - 60, py(vmax, rc, p) - 30,
                    "[ PAVZA ]", 9);
        }
        if (anim_step >= N_pts) {
            SetTextColor(hdc, RGB(100, 255, 100));
            TextOut(hdc, px(T_MAX, rc, p) - 70, py(vmax, rc, p) - 30,
                    "[ KONEC ]", 9);
        }
    }

    // --- Okvir ---
    HPEN frame_pen = CreatePen(PS_SOLID, 2, RGB(120, 120, 140));
    SelectObject(hdc, frame_pen);
    MoveToEx(hdc, px(0, rc, p),     py(0,    rc, p), NULL);
    LineTo(hdc,   px(T_MAX, rc, p), py(0,    rc, p));
    LineTo(hdc,   px(T_MAX, rc, p), py(vmax, rc, p));
    LineTo(hdc,   px(0, rc, p),     py(vmax, rc, p));
    LineTo(hdc,   px(0, rc, p),     py(0,    rc, p));
    DeleteObject(frame_pen);

    // --- Naslov ---
    HFONT title_font = CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0,
                                  DEFAULT_CHARSET, 0, 0, 0, 0, "Consolas");
    SelectObject(hdc, title_font);
    SetTextColor(hdc, RGB(230, 230, 230));
    char title[64];
    sprintf(title, "Prosti pad z zracnim uporom  (dt = %.2f s)", DT);
    TextOut(hdc, px(0, rc, p), py(vmax, rc, p) - 30, title, (int)strlen(title));
    DeleteObject(title_font);

    // --- Legenda ---
    SelectObject(hdc, small_font);
    struct { COLORREF col; const char* label; } leg[] = {
        { RGB( 80,160,255), "Analiticna  v(t) = vT*tanh(g/vT*t)" },
        { RGB( 50,220, 80), "RK4"                                  },
        { RGB(255, 80, 50), "Euler"                                 },
    };
    int lx = px(T_MAX, rc, p) - 240;
    int ly = py(vmax, rc, p) + 5;
    for (int i = 0; i < 3; i++) {
        HPEN lp = CreatePen(PS_SOLID, 2, leg[i].col);
        SelectObject(hdc, lp);
        MoveToEx(hdc, lx,      ly + i*20 + 7, NULL);
        LineTo(hdc,   lx + 22, ly + i*20 + 7);
        DeleteObject(lp);
        SetTextColor(hdc, leg[i].col);
        TextOut(hdc, lx + 28, ly + i*20, leg[i].label, (int)strlen(leg[i].label));
    }

    // --- Oznaki osi ---
    SetTextColor(hdc, RGB(180, 180, 180));
    SelectObject(hdc, small_font);
    TextOut(hdc, rc.left + 5, (py(0,rc,p) + py(vmax,rc,p))/2, "v [m/s]", 7);
    TextOut(hdc, (px(0,rc,p) + px(T_MAX,rc,p))/2 - 15, py(0,rc,p) + 35, "t [s]", 5);

    DeleteObject(small_font);
}

// ------------------------------------------------------------------ //
//  WinAPI callbacks
// ------------------------------------------------------------------ //

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Dvojno medpomnjenje — prepreci utripanje
        RECT rc; GetClientRect(hwnd, &rc);
        HDC  mem = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        SelectObject(mem, bmp);

        draw_graph(mem, rc);

        BitBlt(hdc, 0, 0, rc.right, rc.bottom, mem, 0, 0, SRCCOPY);
        DeleteObject(bmp);
        DeleteDC(mem);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_TIMER:
        if (!paused && anim_step < N_pts) {
            anim_step++;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;
    case WM_KEYDOWN:
        if (wp == VK_ESCAPE || wp == 'Q') PostQuitMessage(0);
        // Presledek — pavza / nadaljuj
        if (wp == VK_SPACE) { paused = !paused; }
        // R — ponovi animacijo
        if (wp == 'R') { anim_step = 0; paused = 0; }
        // +/- — sprememba koraka dt
        if (wp == VK_OEM_PLUS  || wp == VK_ADD) {
            if (DT > 0.1) { DT *= 0.5; simulate(); anim_step = 0; }
        }
        if (wp == VK_OEM_MINUS || wp == VK_SUBTRACT) {
            if (DT < 8.0) { DT *= 2.0; simulate(); anim_step = 0; }
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case WM_SIZE:
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow) {
    simulate();

    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "GrafODE";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("GrafODE", "ODE Graf — Euler vs RK4",
                             WS_OVERLAPPEDWINDOW,
                             100, 100, 960, 620,
                             NULL, NULL, hInst, NULL);
    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);
    SetTimer(hwnd, TIMER_ID, TIMER_MS, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
