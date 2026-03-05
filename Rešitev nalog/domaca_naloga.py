
import math, sys
sys.stdout.reconfigure(encoding='utf-8')

# ── Parametri ──────────────────────────────────────────────────────────────────
phi   = math.radians(20)    # naklon klanca [rad]
alpha = math.radians(30)    # kot izstrelitve nad klancem [rad]
v0    = 100.0               # začetna hitrost [m/s]
g     = 9.81                # gravitacijski pospešek [m/s²]

# ── Funkcija in njen odvod ─────────────────────────────────────────────────────
def f(t):
    """y(t) – višina pravokotno na klanec; ničla = pristanišče"""
    return v0 * math.sin(alpha) * t - 0.5 * g * math.cos(phi) * t**2

def df(t):
    """Odvod f'(t) za Newtonovo metodo"""
    return v0 * math.sin(alpha) - g * math.cos(phi) * t

def razdalja(T):
    """Razdalja vzdolž klanca ob pristalnem času T"""
    return v0 * math.cos(alpha) * T - 0.5 * g * math.sin(phi) * T**2

# ══════════════════════════════════════════════════════════════════════════════
# 1. ANALITIČNA REŠITEV
# ══════════════════════════════════════════════════════════════════════════════
T_ana = 2 * v0 * math.sin(alpha) / (g * math.cos(phi))
R_ana = razdalja(T_ana)

print("=" * 60)
print("  ANALITIČNA REŠITEV")
print("=" * 60)
print(f"  T = 2·v₀·sin(α) / (g·cos(φ))")
print(f"    = 2·{v0}·sin(30°) / ({g}·cos(20°))")
print(f"    = {T_ana:.6f} s")
print(f"  R = v₀·cos(α)·T − ½·g·sin(φ)·T²")
print(f"    = {R_ana:.4f} m")

# ══════════════════════════════════════════════════════════════════════════════
# 2. METODA BISEKCIJE
# ══════════════════════════════════════════════════════════════════════════════
def bisekcija(a, b, eps=1e-10):
    assert f(a) * f(b) < 0, "f(a)·f(b) mora biti < 0"
    iteracije = []
    while abs(b - a) > eps:
        c = (a + b) / 2
        iteracije.append((a, b, c, f(c)))
        if f(a) * f(c) < 0:
            b = c
        else:
            a = c
    return (a + b) / 2, iteracije

# f(5) > 0, f(20) < 0 → koren je v [5, 20]
T_bis, it_bis = bisekcija(5.0, 20.0)
R_bis = razdalja(T_bis)

# ══════════════════════════════════════════════════════════════════════════════
# 3. SEKANTNA METODA
# ══════════════════════════════════════════════════════════════════════════════
def sekantna(x0, x1, eps=1e-10, max_iter=100):
    iteracije = []
    for _ in range(max_iter):
        f0, f1 = f(x0), f(x1)
        if abs(f1 - f0) < 1e-14:
            break
        x2 = x1 - f1 * (x1 - x0) / (f1 - f0)
        iteracije.append((x0, x1, x2, f(x2)))
        if abs(f(x2)) < eps:
            return x2, iteracije
        x0, x1 = x1, x2
    return x1, iteracije

# Začetni točki morata biti na nasprotnih straneh korena T≈10.8 (ne pri t=0!)
# f(8) > 0, f(12) < 0
T_sek, it_sek = sekantna(8.0, 12.0)
R_sek = razdalja(T_sek)

# ══════════════════════════════════════════════════════════════════════════════
# 4. NEWTONOVA METODA
# ══════════════════════════════════════════════════════════════════════════════
def newton(x0, eps=1e-10, max_iter=100):
    iteracije = []
    for _ in range(max_iter):
        fx  = f(x0)
        dfx = df(x0)
        if abs(dfx) < 1e-14:
            break
        x1 = x0 - fx / dfx
        iteracije.append((x0, x1, f(x1)))
        if abs(f(x1)) < eps:
            return x1, iteracije
        x0 = x1
    return x0, iteracije

# Začni v padajočem delu krivulje (t > t_max = T/2 ≈ 5.4), npr. t=12
# f(12) < 0, f'(12) < 0 → Newton gre v pravo smer (k večjemu korenu)
T_new, it_new = newton(12.0)
R_new = razdalja(T_new)

# ══════════════════════════════════════════════════════════════════════════════
# PRIMERJAVA REZULTATOV
# ══════════════════════════════════════════════════════════════════════════════
print("\n" + "=" * 60)
print("  PRIMERJAVA NUMERIČNIH METOD")
print("=" * 60)
print(f"  {'Metoda':<20} {'T [s]':>14} {'R [m]':>12} {'Iteracije':>10} {'Napaka T':>14}")
print(f"  {'-'*20} {'-'*14} {'-'*12} {'-'*10} {'-'*14}")

def vrstica(ime, T, R, n_it):
    err = abs(T - T_ana)
    print(f"  {ime:<20} {T:>14.8f} {R:>12.4f} {n_it:>10} {err:>14.2e}")

vrstica("Analitična",    T_ana, R_ana, "—")
vrstica("Bisekcija",     T_bis, R_bis, len(it_bis))
vrstica("Sekantna",      T_sek, R_sek, len(it_sek))
vrstica("Newtonova",     T_new, R_new, len(it_new))

# ── Podrobnosti iteracij ───────────────────────────────────────────────────────
print("\n" + "=" * 60)
print("  BISEKCIJA – prvih 10 iteracij")
print("=" * 60)
print(f"  {'n':>3}  {'a':>10}  {'b':>10}  {'c':>12}  {'f(c)':>14}")
for n, (a, b, c, fc) in enumerate(it_bis[:10], 1):
    print(f"  {n:>3}  {a:>10.5f}  {b:>10.5f}  {c:>12.8f}  {fc:>14.6e}")

print("\n" + "=" * 60)
print("  SEKANTNA – vse iteracije")
print("=" * 60)
print(f"  {'n':>3}  {'x₀':>12}  {'x₁':>12}  {'x₂':>12}  {'f(x₂)':>14}")
for n, (x0, x1, x2, fx2) in enumerate(it_sek, 1):
    print(f"  {n:>3}  {x0:>12.6f}  {x1:>12.6f}  {x2:>12.8f}  {fx2:>14.6e}")

print("\n" + "=" * 60)
print("  NEWTONOVA – vse iteracije")
print("=" * 60)
print(f"  {'n':>3}  {'x₀':>12}  {'x₁':>12}  {'f(x₁)':>14}")
for n, (x0, x1, fx1) in enumerate(it_new, 1):
    print(f"  {n:>3}  {x0:>12.6f}  {x1:>12.8f}  {fx1:>14.6e}")

print("\n" + "=" * 60)
print("  ZAKLJUČEK")
print("=" * 60)
print(f"  Čas pristanka:  T ≈ {T_ana:.4f} s")
print(f"  Razdalja:       R ≈ {R_ana:.2f} m")
print(f"  Bisekcija potrebuje ~{len(it_bis)} iteracij za eps=1e-10")
print(f"  Sekantna potrebuje  ~{len(it_sek)} iteracij za eps=1e-10")
print(f"  Newtonova potrebuje ~{len(it_new)} iteracij za eps=1e-10")
