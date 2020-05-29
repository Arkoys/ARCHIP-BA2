#include "graphic.h"
#include <cairomm/context.h>
#include <cmath>


void dessin_Logement(double xc, double yc, double rayon,
		const Cairo::RefPtr<Cairo::Context>& cr, bool selected, bool green)
{
	if (selected)
	cr->set_source_rgb(0.92, 0.0, 0.0);
	else
	{
		if (green)
			cr->set_source_rgb(0.0, 1.0, 0.0);
		else
			cr->set_source_rgb(0.0, 0.0, 0.0);
	}
	cr->set_line_width(1.0);
	cr->arc(xc, yc, rayon, 0.0, 2.0 * M_PI); // full circle
	cr->stroke();
}

void dessin_Transport(double xc, double yc, double rayon,
		const Cairo::RefPtr<Cairo::Context>& cr, bool selected, bool green)
{
	if (selected)
	cr->set_source_rgb(0.92, 0.0, 0.0);
	else
	{
		if (green)
			cr->set_source_rgb(0.0, 1.0, 0.0);
		else
			cr->set_source_rgb(0.0, 0.0, 0.0);
	}
	//std::cout << "selected trans " << selected << std::endl;
	double deux(2.);
	cr->set_line_width(1.0);
	cr->arc(xc, yc, rayon, 0.0, 2.0 * M_PI); // full circle
	cr->stroke();
	cr->move_to(xc, yc+rayon);
	cr->line_to(xc, yc-rayon);
	cr->stroke();
	cr->move_to(xc+rayon, yc);
	cr->line_to(xc-rayon, yc);
	cr->stroke();
	cr->move_to(xc-(sqrt(deux)*rayon/deux), yc+sqrt(deux)*rayon/deux);
	cr->line_to(xc+(sqrt(deux)*rayon/deux), yc-sqrt(deux)*rayon/deux);
	cr->stroke();
	cr->move_to(xc+sqrt(deux)*rayon/deux, yc+sqrt(deux)*rayon/deux);
	cr->line_to(xc-sqrt(deux)*rayon/deux, yc-sqrt(deux)*rayon/deux);
	cr->stroke();
}

void dessin_Production(double xc, double yc, double rayon,
		const Cairo::RefPtr<Cairo::Context>& cr, bool selected, bool green)
{
	if (selected)
	cr->set_source_rgb(0.92, 0.0, 0.0);
	else
	{
		if (green)
			cr->set_source_rgb(0.0, 1.0, 0.0);
		else
			cr->set_source_rgb(0.0, 0.0, 0.0);
	}
	//std::cout << "selected prod " << selected << std::endl;
	int trois(3);
	int quatre (4);
	int huit (8);
	cr->set_line_width(1.0);
	cr->arc(xc, yc, rayon, 0.0, 2.0 * M_PI); // full circle
	cr->stroke();
	cr->move_to(xc+rayon*trois/quatre, yc-rayon/huit);
	cr->line_to(xc+rayon*trois/quatre, yc+rayon/huit);
	cr->stroke();
	cr->move_to(xc+rayon*trois/quatre, yc+rayon/huit);
	cr->line_to(xc-rayon*trois/quatre, yc+rayon/huit);
	cr->stroke();
	cr->move_to(xc-rayon*trois/quatre, yc+rayon/huit);
	cr->line_to(xc-rayon*trois/quatre, yc-rayon/huit);
	cr->stroke();
	cr->move_to(xc-rayon*trois/quatre, yc-rayon/huit);
	cr->line_to(xc+rayon*trois/quatre, yc-rayon/huit);
	cr->stroke();
}

void dessin_link (city* City, const Cairo::RefPtr<Cairo::Context>& cr)
{
	//std::cout << "dessine lien" << std::endl;
	unsigned int zero(0), unite(1);
	double x1, y1, x2, y2, r1, r2;
	cr->set_source_rgb(0.0, 0.0, 0.0);
	for (unsigned int i(0); i<City->get_lt().size();++i)
	{
		for(unsigned int j(0); j<City->get_glt().size(); ++j)
		{
			if (same_links_btw_tabs(City->get_lt(), City->get_glt(), i))
				cr->set_source_rgb(0.0, 1.0, 0.0);
			else
				cr->set_source_rgb(0.0, 0.0, 0.0);
		}
			for (unsigned int j(0); j<City->get_q().size();++j)
			{
				if (*City->get_lt()[i][zero]==City->get_q()[j]->get_uid())
				{
					x1=City->get_q()[j]->get_coorx();
					y1=City->get_q()[j]->get_coory();
					r1=City->get_q()[j]->get_rayon();
				}
				if (*City->get_lt()[i][unite]==City->get_q()[j]->get_uid())
				{
					x2=City->get_q()[j]->get_coorx();
					y2=City->get_q()[j]->get_coory();
					r2=City->get_q()[j]->get_rayon();
				}
			}
			if (x1==x2)
			{
				same_x(x1, y1, x2, y2, r1, r2);
			}
			if (y1==y2)
			{
				same_y(x1, y1, x2, y2, r1, r2);
			}
			else
			{
				set_x_y (x1, y1, x2, y2, r1, r2);
			}
			cr->set_line_width(1.0);
			cr->move_to (x1, y1);
			cr->line_to (x2, y2);
			cr->stroke ();
	}
}
void same_x(double &x1, double &y1, double &x2, double &y2, double &r1,
					  double &r2)
{
	if (y2>=y1)
	{
	  y1=y1+r1;
	  y2=y2-r2;
	}
	else
	{
	  y1=y1-r1;
	  y2=y2+r2;
	}
}
void same_y (double &x1, double &y1, double &x2, double &y2, double &r1,
					  double &r2)
{
	if (x2>=x1)
	{
		x1=x1+r1;
		x2=x2-r2;
	}
	else
	{
		x1=x1-r1;
		x2=x2+r2;
	}
}

void set_x_y (double &x1, double &y1, double &x2, double &y2, double &r1,
					  double &r2)
{
	double angle;
	angle=atan((abs(y2-y1))/(abs(x2-x1)));
	if (x2>x1 and y2>y1)
	{
		x1=x1+r1*cos(angle);
		y1=y1+r1*sin(angle);
		x2=x2-r2*cos(angle);
		y2=y2-r2*sin(angle);
	}
	if (x2<x1 and y2>y1)
	{
		x1=x1-r1*cos(angle);
		y1=y1+r1*sin(angle);
		x2=x2+r2*cos(angle);
		y2=y2-r2*sin(angle);
	}
	if (x2<x1 and y2<y1)
	{
		x1=x1-r1*cos(angle);
		y1=y1-r1*sin(angle);
		x2=x2+r2*cos(angle);
		y2=y2+r2*sin(angle);
	}
	if (x2>x1 and y2<y1)
	{
		x1=x1+r1*cos(angle);
		y1=y1-r1*sin(angle);
		x2=x2-r2*cos(angle);
		y2=y2+r2*sin(angle);
	}
}

bool same_links_btw_tabs(std::vector<std::vector<unsigned int*>> tab1,
			std::vector<std::vector<unsigned int*>> tab2, unsigned int i)
{
	bool s(false);
		for (unsigned int j(0); j<tab2.size(); ++j)
			{
				if (*tab1[i][zero]==*tab2[j][zero] && *tab1[i][unite]==*tab2[j][unite])
					s=true;
				if (*tab1[i][zero]==*tab2[j][unite] && *tab1[i][unite]==*tab2[j][zero])
					s=true;
			}
		return s;
}
