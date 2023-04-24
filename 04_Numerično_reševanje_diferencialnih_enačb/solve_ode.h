#pragma once
double f(double x, double y) {
	return (5 * x * x - y) / (exp(x + y));
}

double a(double v){
	double aa;
	aa = 0.5 * S * c * rho_atm / m_z * v * v - g;
	printf("%f\t%f\n", aa, v);
	return aa;
}

double newton_euler_dynamics(double y,double dt) {
	return a(y) * dt;
}


double newton_RK4_dynamics(double v, double dt) {
	double k1, k2, k3, k4;
	k1 = a(v) * dt;
	k2 = a(v + k1 / 2) * dt;
	k3 = a(v + k2 / 2) * dt;
	k4 = a(v + k3) * dt;
	return (k1 + 2 * (k2 + k3) + k4) / 6;
}

double ode_Euler_solver(double x, double y, double h) {
	return f(x, y) * h;
}

double ode_RK4_solver(double x, double y, double h) {
	double k1, k2, k3, k4;
	k1 = f(x, y) * h;
	k2 = f(x + h / 2.0, y + k1 / 2.0) * h;
	k3 = f(x + h / 2.0, y + k2 / 2.0) * h;
	k4 = f(x + h, y + k3) * h;
	return (k1 + 2 * (k2 + k3) + k4) / 6;
}