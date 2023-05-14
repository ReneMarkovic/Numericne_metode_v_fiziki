#pragma once
struct telo {
    double x;
    double y;
    double vx;
    double vy;
    double v;
    double ax;
    double ay;
    double m;
};

double delta_x(struct telo* a, struct telo* b) {
    return a->x - b->x;
}

double delta_y(struct telo* a, struct telo* b) {
    return a->y - b->y;
}

double razdalja(struct telo* a, struct telo* b) {
    double dx = delta_x(a, b);
    double dy = delta_y(a, b);
    return sqrt(dx * dx + dy * dy);
}

double razdalja2(struct telo* a, struct telo* b) {
    double dx = delta_x(a, b);
    double dy = delta_y(a, b);
    return dx * dx + dy * dy;
}


void posodobi_vektorje_Verlet(struct telo* a, double dt) {
    double x_old=a->x;
    double y_old=a->y;
    
    a->x += a->vx * dt + 0.5 * a->ax * dt * dt;
    a->y += a->vy * dt + 0.5 * a->ay * dt * dt;

    a->vx = (a->x - x_old) / dt;
    a->vy = (a->y - y_old) / dt;
}

void posodobi_vektorje_Euler(struct telo* a, double dt) {
    a->vx += a->ax * dt;
    a->vy += a->ay * dt;
    a->x += a->vx * dt;
    a->y += a->vy * dt;
}

double v0(struct telo* a, struct telo* b) {
    double dx=a->x - b->x;
    double dy=a->y - b->y;
    double r=sqrt(dx*dx+dy*dy);
    return sqrt(G * b->m / razdalja(a, b));
}

double abs_v(struct telo* a) {
    return sqrt(a->vx * a->vx + a->vy * a->vy);
}

void doloci_sile(struct telo* object, struct telo** list, int st_objektov, double* Fx, double* Fy) {
    *Fx = 0.0;
    *Fy = 0.0;

    for (int i = 0; i < st_objektov; i++) {
        if (object != list[i]) {
            double r = razdalja(object, list[i]);
            double force = list[i]->m / (r * r);
            double dx = (list[i]->x - object->x);
            double dy = (list[i]->y - object->y);
            *Fx += force * dx / r;
            *Fy += force * dy / r;

        }
    }
}


void nastavi_atribute(struct telo* object, double x, double y, double vx, double vy, double M) {
    object->x = x;
    object->y = y;
    object->vx = vx;
    object->vy = vy;
    object->v = abs_v(object);
    object->ax = 0.0;
    object->ay = 0.0;
    object->m = M;
}

void print_info(struct telo* object){
    printf("x: %f\n", object->x);
    printf("y: %f\n", object->y);
    printf("vx: %f\n", object->vx);
    printf("vy: %f\n", object->vy);
    printf("v: %f\n", object->v);
    printf("ax: %f\n", object->ax);
    printf("ay: %f\n", object->ay);
    printf("m: %f\n", object->m/MSonce);
}
