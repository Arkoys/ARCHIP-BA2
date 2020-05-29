// Architecture b2
#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H
#include <vector>
#include "tools.h"


// symboles et type pour codes d'erreurs
enum Erreur_lecture {LECTURE_ARG, LECTURE_OUVERTURE,
					 LECTURE_NB0, LECTURE_HOUSING,
					 LECTURE_NB1, LECTURE_TRANSPORT,
					 LECTURE_NB2, LECTURE_PRODUCTION,
					 LECTURE_NB3, LECTURE_LINKS,
					 LECTURE_FIN, LECTURE_ETAT};

class quartier
{
public:
	quartier() {}
	quartier(unsigned int uid, double cx, double cy, unsigned int cap, unsigned int t)
			:uidq(uid), coorx(cx), coory(cy), rayon(sqrt(cap)), capacite(cap), type(t)
	{}
Cercle get_cercle()
	{return cercle;}
Point get_centre()
	{return centre;;}
double get_coorx()
	{return coorx;}
double get_coory()
	{return coory;}
unsigned int get_uid()
	{return uidq;}
double get_rayon()
	{return rayon;}
unsigned int get_cap()
	{return capacite;}
unsigned int get_type()
	{return type;}
void set_coorx(double x)
	{coorx=x;}
void set_coory(double y)
	{coory=y;}
void set_in(bool x)
	{in = x;}
void set_selected(bool x)
	{selected = x;}
void set_access(double a)
	{access = a;}
void set_parent(unsigned int p)
	{parent=p;}
void set_index(unsigned int i)
	{index = i;}
void set_rayon(double r)
  {rayon = r;}
void set_cap(unsigned int c)
	{capacite=c;}
void set_green(bool g)
	{green=g;}
bool get_in()
	{return in;}
bool get_selected()
	{return selected;}
double get_access()
	{return access;}
unsigned int get_parent()
	{return parent;}
unsigned int get_index()
	{return index;}
bool get_green()
	{return green;}
private:
	double coorx;
	double coory;
	Point centre{coorx, coory};
	double rayon;
	Cercle cercle{centre, rayon};
	unsigned int uidq;
	unsigned int capacite;
	int type;
	bool in;
	bool selected;
	double access;
	unsigned int parent;
	unsigned int index;
	bool green;
};

// prototypes


void decodage_ligne(std::string line, std::vector<quartier*> &Quartiers,
	std::vector<std::vector<unsigned int*> > &Links_tab, bool &error);
void erreur(Erreur_lecture code);

bool node_superposition(std::vector<quartier*> quart, quartier test);
bool nlink_superposition(unsigned int uid1, unsigned int uid2,
						 std::vector <quartier*> quart);
bool maxlink(std::vector<std::vector<unsigned int*>> Lt, unsigned int uid,
			 std::vector<quartier*> q, unsigned int nb_h);
bool identical_uid(std::vector<quartier*> Quartiers);
bool link_does_not_exists(unsigned int uid1, unsigned int uid2,
						  std::vector<quartier*> Quartiers);
bool same_link(std::vector<std::vector<unsigned int*>> Lt, unsigned int uidtest1,
			   unsigned int uidtest2);
bool too_little_capacity(quartier quart);
bool same_thing(unsigned int thing1, unsigned int thing2);
void nlink_superposition();
bool reserveduid (unsigned int uid);

#endif
