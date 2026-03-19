// lorenz.cpp — Lorenzov atraktor, 3D animacija, WinAPI GDI
//
// Sistem enacb:
//   dx/dt = sigma * (y - x)
//   dy/dt = x * (rho - z) - y
//   dz/dt = x*y - beta*z
//
// Tipke:
//   PRESLEDEK  — pavza / nadaljuj
//   R          — ponastavi
//   B          — vklopi/izklopi drugi atraktor (metuljev efekt)
//   +/-        — hitrost simulacije
//   1/2/3      — razlicni rezimi (kaos / periodicen / rub kaosa)
//   ESC / Q    — izhod
//   Miska levo — rotacija pogleda
//   Kolesce    — zoom
//
// Kompilacija (MinGW):
//   g++ lorenz.cpp -o lorenz.exe -lgdi32 -mwindows -O2
//
// Kompilacija (MSVC):
//   cl lorenz.cpp /Fe:lorenz.exe /O2 user32.lib gdi32.lib

#define WIN32_LEAN_AND_MEAN
#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include <math.h>
#include <stdio.h>

// ------------------------------------------------------------------ //
//  Lorenzovi parametri
// ------------------------------------------------------------------ //

#define PI 3.14159265358979

double sigma = 10.0;
double rho   = 28.0;
double beta  = 8.0 / 3.0;
double dt    = 0.005;
int    steps_per_frame = 15;

// Prednastavitve rezimov
typedef struct { double sigma, rho, beta; const char* name; } Regime;
Regime regimes[] = {
    { 10.0, 28.0,    8.0/3.0, "kaos         (sigma=10, rho=28,   beta=8/3)" },
    { 10.0, 99.65,   8.0/3.0, "periodicen   (sigma=10, rho=99.65,beta=8/3)" },
    { 10.0, 24.74,   8.0/3.0, "rub kaosa    (sigma=10, rho=24.74,beta=8/3)" },
};
int cur_regime = 0;

// ------------------------------------------------------------------ //
//  Sled (circular buffer)
// ------------------------------------------------------------------ //

#define TRAIL_LEN 12000
#define COLOR_GRP 16    // segmentov na en pen

typedef struct { double x, y, z; } Vec3;

Vec3 trail1[TRAIL_LEN], trail2[TRAIL_LEN];
int  t_head  = 0;
int  t_count = 0;

Vec3 pos1, pos2;       // trenutni polozaj obeh trajektorij
int  show_butterfly = 0;

void reset() {
    pos1 = (Vec3){ 1.0,  1.0,  1.0  };
    pos2 = (Vec3){ 1.001,1.0,  1.0  };  // mikroskopska razlika!
    t_head = 0; t_count = 0;
}

// ------------------------------------------------------------------ //
//  RK4 za Lorenzov sistem
// ------------------------------------------------------------------ //

Vec3 lorenz_deriv(Vec3 p) {
    return (Vec3){
        sigma * (p.y - p.x),
        p.x * (rho - p.z) - p.y,
        p.x * p.y - beta * p.z
    };
}

Vec3 rk4(Vec3 p) {
    Vec3 k1 = lorenz_deriv(p);
    Vec3 k2 = lorenz_deriv((Vec3){
        p.x + k1.x*dt/2, p.y + k1.y*dt/2, p.z + k1.z*dt/2 });
    Vec3 k3 = lorenz_deriv((Vec3){
        p.x + k2.x*dt/2, p.y + k2.y*dt/2, p.z + k2.z*dt/2 });
    Vec3 k4 = lorenz_deriv((Vec3){
        p.x + k3.x*dt,   p.y + k3.y*dt,   p.z + k3.z*dt   });
    return (Vec3){
        p.x + dt/6*(k1.x + 2*k2.x + 2*k3.x + k4.x),
        p.y + dt/6*(k1.y + 2*k2.y + 2*k3.y + k4.y),
        p.z + dt/6*(k1.z + 2*k2.z + 2*k3.z + k4.z)
    };
}

// ------------------------------------------------------------------ //
//  3D → 2D projekcija
// ------------------------------------------------------------------ //

double angle_az = 0.5;    // azimut (rotacija okoli Z)
double angle_el = -0.4;   // elevacija (rotacija okoli X)
double scale    = 9.5;    // zoom
double auto_rot = 0.004;  // samodejno vrtenje [rad/kader]

void project(Vec3 p, int W, int H, int* sx, int* sy) {
    // Centriraj atraktor (z center ~25)
    double x = p.x, y = p.y, z = p.z - 25.0;

    // Rotacija okoli Z (azimut)
    double ca = cos(angle_az), sa = sin(angle_az);
    double x1 =  x*ca - y*sa;
    double y1 =  x*sa + y*ca;
    double z1 =  z;

    // Rotacija okoli X (elevacija)
    double ce = cos(angle_el), se = sin(angle_el);
    double y2 =  y1*ce - z1*se;
    double z2 =  y1*se + z1*ce;  // z2 ni potreben za 2D

    *sx = W/2 + (int)(x1 * scale);
    *sy = H/2 - (int)(y2 * scale);
    (void)z2;
}

// ------------------------------------------------------------------ //
//  Barvna lestvica za sled
//  starost 0.0 = najstarejse (temno modra)  →  1.0 = najnovejse (rumena)
// ------------------------------------------------------------------ //

COLORREF trail_color(float age, int is_second) {
    BYTE r, g, b;
    if (!is_second) {
        // Prva trajektorija: temno modra → cian → rumena
        if (age < 0.5f) {
            float t = age * 2.0f;
            r = (BYTE)(20  + t * 0);
            g = (BYTE)(0   + t * 170);
            b = (BYTE)(80  + t * 175);
        } else {
            float t = (age - 0.5f) * 2.0f;
            r = (BYTE)(0   + t * 255);
            g = (BYTE)(170 + t * 50);
            b = (BYTE)(255 - t * 205);
        }
    } else {
        // Druga trajektorija: temno rdeca → oranzna
        if (age < 0.5f) {
            float t = age * 2.0f;
            r = (BYTE)(60  + t * 130);
            g = (BYTE)(0   + t * 60);
            b = (BYTE)(20);
        } else {
            float t = (age - 0.5f) * 2.0f;
            r = (BYTE)(190 + t * 65);
            g = (BYTE)(60  + t * 120);
            b = (BYTE)(20);
        }
    }
    return RGB(r, g, b);
}

// ------------------------------------------------------------------ //
//  Risanje ene sledi
// ------------------------------------------------------------------ //

void draw_trail(HDC hdc, Vec3* trail, int is_second, int W, int H) {
    if (t_count < 2) return;

    HPEN cur_pen = NULL;
    COLORREF cur_col = 0;
    int started = 0;

    for (int i = 0; i < t_count - 1; i++) {
        float age = (float)i / (t_count - 1);

        int idx0 = (t_head - t_count + i     + TRAIL_LEN) % TRAIL_LEN;
        int idx1 = (t_head - t_count + i + 1 + TRAIL_LEN) % TRAIL_LEN;

        int sx0, sy0, sx1, sy1;
        project(trail[idx0], W, H, &sx0, &sy0);
        project(trail[idx1], W, H, &sx1, &sy1);

        // Menjaj pen vsakih COLOR_GRP segmentov
        if (i % COLOR_GRP == 0) {
            COLORREF nc = trail_color(age, is_second);
            if (nc != cur_col) {
                HPEN np = CreatePen(PS_SOLID, (t_count > 4000) ? 1 : 1, nc);
                HPEN op = (HPEN)SelectObject(hdc, np);
                if (cur_pen) DeleteObject(op);   // izbrisi starega
                cur_pen = np;
                cur_col = nc;
            }
        }

        MoveToEx(hdc, sx0, sy0, NULL);
        LineTo(hdc,   sx1, sy1);
        started = 1;
    }
    // Pocisti zadnji pen
    if (started) {
        HPEN def = (HPEN)GetStockObject(BLACK_PEN);
        HPEN old = (HPEN)SelectObject(hdc, def);
        DeleteObject(old);
    }
}

// ------------------------------------------------------------------ //
//  Cel kader
// ------------------------------------------------------------------ //

void draw_frame(HDC hdc, int W, int H) {
    // --- Ozadje ---
    HBRUSH bg = CreateSolidBrush(RGB(4, 4, 12));
    RECT   rc = {0, 0, W, H};
    FillRect(hdc, &rc, bg);
    DeleteObject(bg);

    // --- Sledi ---
    if (show_butterfly) draw_trail(hdc, trail2, 1, W, H);
    draw_trail(hdc, trail1, 0, W, H);

    // --- Fikstocke (ravnovesja sistema) ---
    if (rho > 1.0) {
        double s = sqrt(beta * (rho - 1.0));
        Vec3 fp1 = { s,  s, rho - 1.0};
        Vec3 fp2 = {-s, -s, rho - 1.0};
        int cx, cy;
        HPEN wp = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
        HBRUSH wb = (HBRUSH)GetStockObject(NULL_BRUSH);
        SelectObject(hdc, wp);
        SelectObject(hdc, wb);
        project(fp1, W, H, &cx, &cy);
        Ellipse(hdc, cx-5, cy-5, cx+5, cy+5);
        project(fp2, W, H, &cx, &cy);
        Ellipse(hdc, cx-5, cy-5, cx+5, cy+5);
        DeleteObject(wp);
    }

    // --- Trenutna tocka ---
    int sx, sy;
    project(pos1, W, H, &sx, &sy);
    HPEN   dp = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HBRUSH db = CreateSolidBrush(RGB(255, 255, 220));
    SelectObject(hdc, dp); SelectObject(hdc, db);
    Ellipse(hdc, sx-5, sy-5, sx+5, sy+5);
    DeleteObject(dp); DeleteObject(db);

    if (show_butterfly && t_count > 10) {
        project(pos2, W, H, &sx, &sy);
        HPEN rp = CreatePen(PS_SOLID, 1, RGB(255, 100, 50));
        HBRUSH rb = CreateSolidBrush(RGB(255, 100, 50));
        SelectObject(hdc, rp); SelectObject(hdc, rb);
        Ellipse(hdc, sx-5, sy-5, sx+5, sy+5);
        DeleteObject(rp); DeleteObject(rb);
    }

    // --- Info panel ---
    SetBkColor(hdc,   RGB(4, 4, 12));
    SetBkMode(hdc,    OPAQUE);
    HFONT fnt = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                           DEFAULT_CHARSET, 0, 0, 0, 0, "Consolas");
    HFONT fnt_b = CreateFont(17, 0, 0, 0, FW_BOLD, 0, 0, 0,
                             DEFAULT_CHARSET, 0, 0, 0, 0, "Consolas");
    SelectObject(hdc, fnt_b);

    SetTextColor(hdc, RGB(220, 200, 255));
    TextOut(hdc, 12, 10, "Lorenzov atraktor", 17);

    SelectObject(hdc, fnt);
    int lx = 12, ly = 32, dy = 17;

    char buf[128];
    // Enacbe
    SetTextColor(hdc, RGB(140, 220, 255));
    TextOut(hdc, lx, ly,      "dx/dt = sigma*(y - x)", 21);
    TextOut(hdc, lx, ly+dy,   "dy/dt = x*(rho - z) - y", 24);
    TextOut(hdc, lx, ly+2*dy, "dz/dt = x*y - beta*z", 20);

    // Parametri
    SetTextColor(hdc, RGB(200, 255, 180));
    sprintf(buf, "sigma = %.2f   rho = %.2f   beta = %.4f", sigma, rho, beta);
    TextOut(hdc, lx, ly+3*dy+6, buf, (int)strlen(buf));

    // Rezim
    SetTextColor(hdc, RGB(255, 220, 100));
    sprintf(buf, "Rezim: %s", regimes[cur_regime].name);
    TextOut(hdc, lx, ly+4*dy+6, buf, (int)strlen(buf));

    // Polozaj
    SetTextColor(hdc, RGB(180, 180, 180));
    sprintf(buf, "x=%.2f  y=%.2f  z=%.2f  (tocke: %d)", pos1.x, pos1.y, pos1.z, t_count);
    TextOut(hdc, lx, ly+5*dy+6, buf, (int)strlen(buf));

    // Metuljev efekt
    if (show_butterfly) {
        double dist = sqrt(
            (pos1.x-pos2.x)*(pos1.x-pos2.x) +
            (pos1.y-pos2.y)*(pos1.y-pos2.y) +
            (pos1.z-pos2.z)*(pos1.z-pos2.z));
        SetTextColor(hdc, RGB(255, 130, 80));
        sprintf(buf, "Metuljev efekt: razdalja = %.4f", dist);
        TextOut(hdc, lx, ly+6*dy+6, buf, (int)strlen(buf));
    }

    // Kontrole
    SetTextColor(hdc, RGB(110, 110, 130));
    ly = H - 70;
    TextOut(hdc, lx, ly,      "1/2/3: rezim   B: metuljev efekt   R: ponastavi", 47);
    TextOut(hdc, lx, ly+dy,   "+/-: hitrost   miska: rotacija   kolesce: zoom", 47);
    TextOut(hdc, lx, ly+2*dy, "PRESLEDEK: pavza   ESC: izhod", 29);

    DeleteObject(fnt);
    DeleteObject(fnt_b);
}

// ------------------------------------------------------------------ //
//  Stanje okna / animacija
// ------------------------------------------------------------------ //

int   paused      = 0;
int   dragging    = 0;
int   drag_x0, drag_y0;
double drag_az0, drag_el0;

#define TIMER_ID 1
#define TIMER_MS 20

// ------------------------------------------------------------------ //
//  WinAPI
// ------------------------------------------------------------------ //

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {

    case WM_TIMER: {
        if (paused) break;

        // Simulacija — steps_per_frame korakov RK4
        for (int i = 0; i < steps_per_frame; i++) {
            pos1 = rk4(pos1);
            pos2 = rk4(pos2);

            trail1[t_head] = pos1;
            trail2[t_head] = pos2;
            t_head = (t_head + 1) % TRAIL_LEN;
            if (t_count < TRAIL_LEN) t_count++;
        }

        // Samodejno vrtenje
        angle_az += auto_rot;

        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc; GetClientRect(hwnd, &rc);
        int W = rc.right, H = rc.bottom;

        // Dvojno medpomnjenje
        HDC     mem = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, W, H);
        SelectObject(mem, bmp);
        draw_frame(mem, W, H);
        BitBlt(hdc, 0, 0, W, H, mem, 0, 0, SRCCOPY);
        DeleteObject(bmp);
        DeleteDC(mem);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_KEYDOWN:
        if (wp == VK_ESCAPE || wp == 'Q') PostQuitMessage(0);
        if (wp == VK_SPACE)  paused = !paused;
        if (wp == 'R')       { reset(); t_count = 0; }
        if (wp == 'B')       show_butterfly = !show_butterfly;
        if (wp == VK_OEM_PLUS  || wp == VK_ADD)      { if (steps_per_frame < 100) steps_per_frame += 5; }
        if (wp == VK_OEM_MINUS || wp == VK_SUBTRACT) { if (steps_per_frame > 1)  steps_per_frame -= 5; }
        if (wp == '1' || wp == '2' || wp == '3') {
            cur_regime = wp - '1';
            sigma = regimes[cur_regime].sigma;
            rho   = regimes[cur_regime].rho;
            beta  = regimes[cur_regime].beta;
            reset();
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;

    // Rotacija z misko
    case WM_LBUTTONDOWN:
        dragging = 1;
        drag_x0  = LOWORD(lp); drag_y0  = HIWORD(lp);
        drag_az0 = angle_az;   drag_el0 = angle_el;
        SetCapture(hwnd);
        break;
    case WM_MOUSEMOVE:
        if (dragging) {
            int dx = (int)LOWORD(lp) - drag_x0;
            int dy = (int)HIWORD(lp) - drag_y0;
            angle_az = drag_az0 + dx * 0.005;
            angle_el = drag_el0 + dy * 0.005;
            // Omejimo elevacijo
            if (angle_el >  1.4) angle_el =  1.4;
            if (angle_el < -1.4) angle_el = -1.4;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;
    case WM_LBUTTONUP:
        dragging = 0;
        ReleaseCapture();
        break;

    // Zoom s kolesckom
    case WM_MOUSEWHEEL: {
        int delta = GET_WHEEL_DELTA_WPARAM(wp);
        scale *= (delta > 0) ? 1.1 : 0.9;
        if (scale < 1.0) scale = 1.0;
        if (scale > 50.0) scale = 50.0;
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }

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
    reset();

    WNDCLASS wc    = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "Lorenz";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("Lorenz",
                             "Lorenzov atraktor — kaos in metuljev efekt",
                             WS_OVERLAPPEDWINDOW,
                             80, 60, 1000, 680,
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
