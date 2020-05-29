#ifndef CONSTANTES_HEADER_H
#define CONSTANTES_HEADER_H



constexpr double dim_max(1000.);
constexpr double dist_min(10.);
constexpr double default_speed(5.);
constexpr double fast_speed(20.);

constexpr unsigned int min_capacity(1e3);
constexpr unsigned int max_capacity(1e6);

constexpr unsigned int max_line(80);

constexpr unsigned int max_link(3);

constexpr unsigned int no_link(static_cast<unsigned>(-1));
constexpr double infinite_time(1e100);


constexpr unsigned int default_drawing_size(800);

constexpr double delta_zoom(0.2);
constexpr double max_zoom(3.0);
constexpr double min_zoom(0.2);
constexpr double min_min(0.1);

constexpr int zero(0);
constexpr int unite(1);
constexpr int deux(2);
constexpr int trois(3);


constexpr	double unframe(2000./900.);

// perso
#define DEBUG
#define NO_CENTRING
constexpr double epsil_zero(1e-1);

#endif // CONSTANTES_HEADER_H
