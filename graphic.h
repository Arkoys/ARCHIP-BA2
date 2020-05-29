#ifndef GRAPHIC_HEADER_H
#define GRAPHIC_HEADER_H

#include <gtkmm/drawingarea.h>
#include "ville.h"

void dessin_Logement(double xc, double yc, double rayon,
            const Cairo::RefPtr<Cairo::Context>& cr, bool selected, bool green);
void dessin_Transport(double xc, double yc, double rayon,
            const Cairo::RefPtr<Cairo::Context>& cr, bool selected, bool green);
void dessin_Production(double xc, double yc, double rayon,
            const Cairo::RefPtr<Cairo::Context>& cr, bool selected, bool green);
void set_x_y (double &x1, double &y1, double &x2, double &y2, double &r1,
        double &r2);
void dessin_link (city* City, const Cairo::RefPtr<Cairo::Context>& cr);
void same_x(double &x1, double &y1, double &x2, double &y2, double &r1,
	double &r2);
void same_y(double &x1, double &y1, double &x2, double &y2, double &r1,
	double &r2);

bool same_links_btw_tabs(std::vector<std::vector<unsigned int*>> tab1,
			std::vector<std::vector<unsigned int*>> tab2, unsigned int i);

#endif
