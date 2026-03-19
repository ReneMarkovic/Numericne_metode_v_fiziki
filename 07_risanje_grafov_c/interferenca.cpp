// interferenca.cpp — Interaktivna 2D animacija interference valov, WinAPI GDI
//
// Miska:
//   Levi klik (prazno)  — dodaj izvor
//   Levi klik + vleci   — premakni izvor
//   Desni klik          — odstrani najblizji izvor
//
// Tipke:
//   UP / DOWN           — valovna dolzina lambda (+/- 5 px)
//   F / G               — frekvenca (hitrost animacije)
//   Kolesce             — lambda (hitro)
//   C                   — ponastavi na 2 privzeta izvora
//   PRESLEDEK           — pavza / nadaljuj
//   ESC / Q             — izhod

#define WIN32_LEAN_AND_MEAN
#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include <math.h>
#include <stdio.h>

// ------------------------------------------------------------------ //
//  Konstante
// ------------------------------------------------------------------ //

#define PI       3.14159265358979
#define MAX_SRC  8
#define SRC_R    10      // polmer izvora v pikslih (za klik/drag)
#define TIMER_ID 1
#define TIMER_MS 30      // ~33 fps

// ------------------------------------------------------------------ //
//  Globalno stanje
// ------------------------------------------------------------------ //

double lambda     = 80.0;    // valovna dolzina [px]
double phase      = 0.0;     // trenutna faza [rad]
double phase_step = 0.18;    // korak faze na kader (~frekvenca)
int    paused     = 0;

// Izvori — proste pike, postavljene z misko
double src_x[MAX_SRC], src_y[MAX_SRC];
int    n_src      = 0;
int    initialized = 0;    // 0 = se nismo inicializirali glede na okno

// Vlecenje
int drag_idx = -1;         // kateri izvor vlecemo (-1 = noben)

// ------------------------------------------------------------------ //
//  Privzeta inicializacija: 2 izvora na sredini okna
// ------------------------------------------------------------------ //

void init_sources(int W, int H) {
    n_src   = 2;
    src_x[0] = W / 2.0 - 110;  src_y[0] = H / 2.0;
    src_x[1] = W / 2.0 + 110;  src_y[1] = H / 2.0;
    initialized = 1;
}

// ------------------------------------------------------------------ //
//  Pomoc: najblizji izvor od tocke (mx, my)
// ------------------------------------------------------------------ //

int nearest_src(int mx, int my) {
    int best = -1;
    double best_d2 = 1e18;
    for (int i = 0; i < n_src; i++) {
        double dx = mx - src_x[i], dy = my - src_y[i];
        double d2 = dx*dx + dy*dy;
        if (d2 < best_d2) { best_d2 = d2; best = i; }
    }
    return best;
}

// ------------------------------------------------------------------ //
//  Sinusna LUT
// ------------------------------------------------------------------ //

#define SIN_N 4096
float sin_lut[SIN_N];

void build_lut() {
    for (int i = 0; i < SIN_N; i++)
        sin_lut[i] = (float)sin(2.0 * PI * i / SIN_N);
}

static inline float fast_sin(double rad) {
    int idx = (int)(rad * (SIN_N / (2.0 * PI))) & (SIN_N - 1);
    if (idx < 0) idx += SIN_N;
    return sin_lut[idx];
}

// ------------------------------------------------------------------ //
//  Barvna mapa  [-n_src, +n_src] → RGB
//  modra (min) → temna (0) → cian/bela (max)
// ------------------------------------------------------------------ //

static inline void amp_to_rgb(float amp, int ns, BYTE* R, BYTE* G, BYTE* B) {
    float t = (amp / (float)(ns > 0 ? ns : 1) + 1.0f) * 0.5f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    if (t < 0.5f) {
        float s = t * 2.0f;
        *R = 0;
        *G = (BYTE)(s * 80);
        *B = (BYTE)(60 + s * 195);
    } else {
        float s = (t - 0.5f) * 2.0f;
        *R = (BYTE)(s * 255);
        *G = (BYTE)(80 + s * 175);
        *B = 255;
    }
}

// ------------------------------------------------------------------ //
//  Risanje enega izvora
// ------------------------------------------------------------------ //

void draw_source(HDC hdc, int idx, int is_dragged) {
    int cx = (int)src_x[idx], cy = (int)src_y[idx];
    COLORREF col = is_dragged ? RGB(255, 80, 80) : RGB(255, 220, 0);
    HPEN   wp = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH wb = CreateSolidBrush(col);
    SelectObject(hdc, wp);
    SelectObject(hdc, wb);
    Ellipse(hdc, cx - SRC_R, cy - SRC_R, cx + SRC_R, cy + SRC_R);
    DeleteObject(wp);
    DeleteObject(wb);
    char lbl[4]; sprintf(lbl, "S%d", idx + 1);
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, cx - 7, cy - 7, lbl, (int)strlen(lbl));
}

// ------------------------------------------------------------------ //
//  Render — DIB piksel po piksel + overlay
// ------------------------------------------------------------------ //

void render_frame(HDC hdc_dest, int W, int H) {
    if (!initialized) init_sources(W, H);

    // --- DIB (valovno polje) ---
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = W;
    bmi.bmiHeader.biHeight      = -H;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    BYTE*   pixels = NULL;
    HBITMAP hbm    = CreateDIBSection(hdc_dest, &bmi, DIB_RGB_COLORS,
                                      (void**)&pixels, NULL, 0);
    HDC mem = CreateCompatibleDC(hdc_dest);
    SelectObject(mem, hbm);

    // Temno ozadje ce ni izvorov
    if (n_src == 0) {
        for (int i = 0; i < W * H * 4; i += 4) {
            pixels[i]=10; pixels[i+1]=10; pixels[i+2]=15; pixels[i+3]=0;
        }
    } else {
        double k = 2.0 * PI / lambda;
        for (int y = 0; y < H; y++) {
            BYTE* row = pixels + y * W * 4;
            for (int x = 0; x < W; x++) {
                float amp = 0.0f;
                for (int s = 0; s < n_src; s++) {
                    double dx = x - src_x[s], dy = y - src_y[s];
                    double r  = sqrt(dx*dx + dy*dy) + 0.5;
                    float  A  = (float)(lambda / (lambda + r * 0.3));
                    amp += A * fast_sin(k * r - phase);
                }
                BYTE R, G, B;
                amp_to_rgb(amp, n_src, &R, &G, &B);
                row[x*4+0] = B; row[x*4+1] = G;
                row[x*4+2] = R; row[x*4+3] = 0;
            }
        }
    }

    BitBlt(hdc_dest, 0, 0, W, H, mem, 0, 0, SRCCOPY);
    DeleteObject(hbm);
    DeleteDC(mem);

    // --- Krog dosega za vsak izvor (cirkularni val) ---
    HBRUSH nb = (HBRUSH)GetStockObject(NULL_BRUSH);
    SelectObject(hdc_dest, nb);
    for (int s = 0; s < n_src; s++) {
        // Pokazi fazni krog — polmer se sinhronizira s fazo
        double phase_r = (phase / (2.0 * PI)) * lambda;
        for (int ring = 0; ring < 4; ring++) {
            int rr = (int)(phase_r + ring * lambda) % (int)(lambda * 6);
            if (rr < 5) continue;
            HPEN rp = CreatePen(PS_SOLID, 1, RGB(255, 255, 100));
            SelectObject(hdc_dest, rp);
            int cx = (int)src_x[s], cy = (int)src_y[s];
            Ellipse(hdc_dest, cx-rr, cy-rr, cx+rr, cy+rr);
            DeleteObject(rp);
        }
        draw_source(hdc_dest, s, (s == drag_idx));
    }

    // --- Info panel (levo zgoraj) ---
    HFONT fnt = CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                           DEFAULT_CHARSET, 0, 0, 0, 0, "Consolas");
    HFONT fnt_b = CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0,
                             DEFAULT_CHARSET, 0, 0, 0, 0, "Consolas");
    SetBkColor(hdc_dest, RGB(8, 8, 20));
    SetBkMode(hdc_dest, OPAQUE);

    SelectObject(hdc_dest, fnt_b);
    SetTextColor(hdc_dest, RGB(220, 220, 255));
    TextOut(hdc_dest, 12, 10, "Interferenca valov", 18);

    SelectObject(hdc_dest, fnt);
    int lx = 12, ly = 30, dy = 17;
    char buf[128];

    SetTextColor(hdc_dest, RGB(160, 220, 255));
    sprintf(buf, "lambda = %.0f px   k = 2pi/lambda", lambda);
    TextOut(hdc_dest, lx, ly, buf, (int)strlen(buf));

    SetTextColor(hdc_dest, RGB(160, 255, 160));
    double freq_rel = phase_step / 0.18;
    sprintf(buf, "frekvenca = %.2f x   (F/G za spremembo)", freq_rel);
    TextOut(hdc_dest, lx, ly + dy, buf, (int)strlen(buf));

    SetTextColor(hdc_dest, RGB(255, 220, 100));
    sprintf(buf, "izvori: %d / %d   (klik=dodaj, D-klik=odstrani)", n_src, MAX_SRC);
    TextOut(hdc_dest, lx, ly + 2*dy, buf, (int)strlen(buf));

    // Razdalje med izvori
    if (n_src >= 2) {
        SetTextColor(hdc_dest, RGB(200, 200, 200));
        for (int i = 0; i < n_src - 1 && i < 3; i++) {
            double dx = src_x[i+1] - src_x[i], dy2 = src_y[i+1] - src_y[i];
            double d  = sqrt(dx*dx + dy2*dy2);
            sprintf(buf, "  d(S%d,S%d) = %.0f px   d/lambda = %.2f",
                    i+1, i+2, d, d/lambda);
            TextOut(hdc_dest, lx, ly + (3+i)*dy, buf, (int)strlen(buf));
        }
    }

    // Kontrole (spodaj levo)
    SetBkColor(hdc_dest, RGB(8, 8, 20));
    SetTextColor(hdc_dest, RGB(110, 110, 140));
    int cl = H - 58;
    TextOut(hdc_dest, lx, cl,       "Levi klik: dodaj izvor         Desni klik: odstrani", 51);
    TextOut(hdc_dest, lx, cl+dy,    "UP/DOWN: lambda   F/G: frekvenca   Kolesce: lambda", 50);
    TextOut(hdc_dest, lx, cl+2*dy,  "C: ponastavi   PRESLEDEK: pavza   ESC: izhod", 44);

    // Pavza
    if (paused) {
        SetTextColor(hdc_dest, RGB(255, 200, 50));
        TextOut(hdc_dest, W - 90, 10, "[ PAVZA ]", 9);
    }

    // Barvna legenda (desno spodaj)
    int bx = W - 135, by = H - 50;
    SetTextColor(hdc_dest, RGB(180, 180, 180));
    TextOut(hdc_dest, bx, by - 16, "Amplituda:", 10);
    for (int i = 0; i <= 40; i++) {
        float amp = (i / 20.0f - 1.0f) * (n_src > 0 ? n_src : 1);
        BYTE R, G, B;
        amp_to_rgb(amp, n_src > 0 ? n_src : 1, &R, &G, &B);
        HPEN cp = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        HBRUSH cb = CreateSolidBrush(RGB(R, G, B));
        SelectObject(hdc_dest, cp);
        SelectObject(hdc_dest, cb);
        Rectangle(hdc_dest, bx+i*3, by, bx+i*3+3, by+14);
        DeleteObject(cp); DeleteObject(cb);
    }
    SetBkMode(hdc_dest, TRANSPARENT);
    SetTextColor(hdc_dest, RGB(80, 130, 255));
    TextOut(hdc_dest, bx,      by+16, "-max", 4);
    SetTextColor(hdc_dest, RGB(200, 200, 200));
    TextOut(hdc_dest, bx+52,   by+16, "0",   1);
    SetTextColor(hdc_dest, RGB(255, 255, 255));
    TextOut(hdc_dest, bx+108,  by+16, "+max",4);

    DeleteObject(fnt);
    DeleteObject(fnt_b);
}

// ------------------------------------------------------------------ //
//  WinAPI
// ------------------------------------------------------------------ //

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    int mx = (int)LOWORD(lp);
    int my = (int)HIWORD(lp);

    switch (msg) {

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc; GetClientRect(hwnd, &rc);
        // Dvojno medpomnjenje
        HDC     mem = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        SelectObject(mem, bmp);
        render_frame(mem, rc.right, rc.bottom);
        BitBlt(hdc, 0, 0, rc.right, rc.bottom, mem, 0, 0, SRCCOPY);
        DeleteObject(bmp); DeleteDC(mem);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_TIMER:
        if (!paused) {
            phase += phase_step;
            if (phase > 200.0 * PI) phase -= 200.0 * PI;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;

    // --- Levi klik: dodaj ali zacni vlecenje ---
    case WM_LBUTTONDOWN: {
        SetCapture(hwnd);
        // Preveri ce klikamo bliznje izvoru
        int ni = nearest_src(mx, my);
        if (ni >= 0) {
            double dx = mx - src_x[ni], dy = my - src_y[ni];
            if (dx*dx + dy*dy <= (SRC_R+4)*(SRC_R+4)) {
                drag_idx = ni;   // zacni vlecenje
                break;
            }
        }
        // Dodaj nov izvor
        if (n_src < MAX_SRC) {
            src_x[n_src] = mx;
            src_y[n_src] = my;
            n_src++;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }

    case WM_MOUSEMOVE:
        if (drag_idx >= 0) {
            src_x[drag_idx] = mx;
            src_y[drag_idx] = my;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;

    case WM_LBUTTONUP:
        drag_idx = -1;
        ReleaseCapture();
        break;

    // --- Desni klik: odstrani najblizji izvor ---
    case WM_RBUTTONDOWN: {
        int ni = nearest_src(mx, my);
        if (ni >= 0 && n_src > 0) {
            // Zamenjaj z zadnjim in zmanjsaj stevec
            src_x[ni] = src_x[n_src-1];
            src_y[ni] = src_y[n_src-1];
            n_src--;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }

    // --- Kolesce: lambda ---
    case WM_MOUSEWHEEL: {
        int delta = GET_WHEEL_DELTA_WPARAM(wp);
        lambda += (delta > 0) ? 5.0 : -5.0;
        if (lambda < 10.0) lambda = 10.0;
        if (lambda > 300.0) lambda = 300.0;
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }

    case WM_KEYDOWN:
        if (wp == VK_ESCAPE || wp == 'Q')  PostQuitMessage(0);
        if (wp == VK_SPACE)                paused = !paused;
        // Lambda
        if (wp == VK_UP   && lambda < 300) { lambda += 5; }
        if (wp == VK_DOWN && lambda > 10)  { lambda -= 5; }
        // Frekvenca
        if (wp == 'F' && phase_step < 1.5) { phase_step += 0.02; }
        if (wp == 'G' && phase_step > 0.02){ phase_step -= 0.02; }
        // Ponastavi
        if (wp == 'C') {
            RECT rc; GetClientRect(hwnd, &rc);
            init_sources(rc.right, rc.bottom);
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;

    case WM_SIZE:
        if (!initialized) {
            RECT rc; GetClientRect(hwnd, &rc);
            init_sources(rc.right, rc.bottom);
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow) {
    build_lut();

    WNDCLASS wc    = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "Interferenca";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("Interferenca",
                             "Interferenca valov — interaktivna animacija",
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
