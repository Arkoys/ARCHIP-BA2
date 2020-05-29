// Architecture b2
#include <fstream>
#include <sstream>
#include <cmath>
#include "error.h"
#include "ville.h"
using namespace std;

static city Ville;

city* city::getvilleinstance()
{return &Ville;}

unsigned int nb_houses;

// traite le fichier ligne par ligne.
void lecture( char * nom_fichier)
{
	vector<quartier*> Quartiers;
	vector<vector<unsigned int*>> Links_tab(0,vector <unsigned int*>(2));
	string line;
	bool error(false);
	ifstream fichier(nom_fichier);
	if(!fichier.fail())
	{
		while(getline(fichier >> ws,line))
		{
			if(line[0]=='#')  continue;
			decodage_ligne(line, Quartiers, Links_tab, error);
		}
		Ville.set_D(Quartiers);
		Ville.set_L(Links_tab);
		clear_it(Quartiers, Links_tab);
		Quartiers.clear();
		Links_tab.clear();
		unselect_all(city::getvilleinstance());
		ungreen_all(city::getvilleinstance());
		if (error)
		{
			Delete_City();
		}
	}
	else erreur(LECTURE_OUVERTURE);
}

// met à jour l'etat
void decodage_ligne(std::string line, vector<quartier*> &Quartiers,
	vector<vector<unsigned int*> > &Links_tab, bool &error)
{
	istringstream data(line);
	// états de l'automate de lecture
	enum Etat_lecture {NB0,HOUSING,NB1,TRANSPORT,NB2,PRODUCTION,NB3,LINKS,FIN};
	static int i(0), total(0), etat(NB0); // état initial
	unsigned int uid, uid1, uid2;
	double coorx, coory;
	unsigned int capacite;
	string name;
	switch(etat)

	{
		case NB0:
		{
			if(!(data >> total)) erreur(LECTURE_NB0);
			else i=0;
			nb_houses=total;
			Ville.set_nh(total);
			if(total==0) etat=NB1; else etat=HOUSING;
			break;
		}
		case HOUSING:
		{
			if(!(data>>uid>>coorx>>coory>> capacite)) erreur(LECTURE_HOUSING);
			else ++i;
			if(i == total) etat=NB1;
			unsigned int type1(1);
			quartier Maison(uid, coorx, coory, capacite, type1);
			if (node_error(Quartiers, Links_tab, Maison, uid))
			{
				error=true;
			}
			Quartiers.push_back(new quartier(Maison));
		//	for (int i(0); i<Quartiers.size(); ++i)
		//	{cout << Quartiers[i]->get_uid() << endl;}
			if (identical_uid(Quartiers))
			{
				error=true;
			}
			break;
		}
		case NB1:
		{
			if(!(data >> total)) erreur(LECTURE_NB1);
			else i=0;
			Ville.set_nt(total);
			if(total==0) etat=NB2; else etat=TRANSPORT;
			break;
		}
		case TRANSPORT:
		{
			if( !(data>>uid>>coorx>>coory>>capacite)) erreur(LECTURE_TRANSPORT);
			else ++i;
			if(i == total) etat=NB2;
			unsigned int type2(2);
			quartier Transport(uid, coorx, coory, capacite, type2);
			if (node_error(Quartiers, Links_tab, Transport, uid))
			{
				error=true;
			}
			Quartiers.push_back(new quartier(Transport));
			if (identical_uid(Quartiers))
			{
				error=true;
			}
			break;
		}
		case NB2:
		{
			if(!(data >> total)) erreur(LECTURE_NB2);
			else i=0;
			Ville.set_np(total);
			if(total==0) etat=NB3; else etat=PRODUCTION;
			break;
		}
		case PRODUCTION:
		{
			if(!(data>>uid>>coorx>>coory>>capacite)) erreur(LECTURE_PRODUCTION);
			else ++i;
			if(i == total) etat=NB3;
			unsigned int type3(3);
			quartier Production(uid, coorx, coory, capacite, type3);
			if (node_error(Quartiers, Links_tab, Production, uid))
			{
				error=true;
			}
			Quartiers.push_back (new quartier(Production));
			if (identical_uid(Quartiers))
			{
				error=true;
			}
			break;
		}
		case NB3:
		{
			if(!(data >> total)) erreur(LECTURE_NB3);
			else i=0;
			if(total==0) etat=NB0; else etat=LINKS;
			break;
		}
		case LINKS:
		{
			if( !(data >> uid1 >> uid2)) erreur(LECTURE_LINKS);
			else ++i;
			if(i == total) etat=NB0;
			unsigned int* u1 = new unsigned int(uid1);
			unsigned int* u2 = new unsigned int(uid2);
			if (links_error(Quartiers, Links_tab, uid1, uid2))
				{
					error=true;
				}
			Links_tab.push_back({u1, u2});
			break;
		}
		default: erreur(LECTURE_ETAT);
	}
}

// vide les tableaux
void clear_it(std::vector<quartier*> Q, std::vector <std::vector<unsigned int*>> &L)
{
	std::vector<quartier*> A;
	std::vector <std::vector<unsigned int*>> B;
	Q=A;
	L=B;
}

void Delete_City() // supprime toutes les données de la ville
{
	Ville.clear_q();
	Ville.clear_Lt();
	Ville.set_nh(0);
	Ville.set_nt(0);
	Ville.set_np(0);
}

void unselect_all(city* City)
{
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		City->get_q()[i]->set_selected(false);
	}
}

void ungreen_all(city* City)
{
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		City->get_q()[i]->set_green(false);
	}
	City->clear_green_lt();
}


void city::new_q(double coorx, double coory, unsigned int uid , unsigned typeq)
{
	std::cout << "1" << std::endl;
	quartier q(uid, coorx, coory, min_capacity, typeq);
	if (!(node_error(Ville.get_q(), Ville.get_lt(), q, uid)))
		{
			std::vector<quartier*> temp5;
			q.set_selected(false);
			q.set_green(false);
			switch (typeq)
			{
				case 1:
					for (unsigned int m(0); m<number_of_houses; ++m )
						temp5.push_back(Districts[m]);
					temp5.push_back(new quartier(q));
					for (unsigned int m(number_of_houses); m<Districts.size(); ++m )
						temp5.push_back(Districts[m]);
					Ville.set_nh(Ville.get_nh()+1);
					break;
				case 2:
					for (unsigned int m(0); m<number_of_houses+number_of_transports; ++m )
						temp5.push_back(Districts[m]);
					temp5.push_back(new quartier(q));
					for (unsigned int m(number_of_houses+number_of_transports); m<Districts.size(); ++m )
						temp5.push_back(Districts[m]);
					Ville.set_nt(Ville.get_nt()+1);
					break;
				case 3:
					for (unsigned int m(0); m<Districts.size(); ++m )
						temp5.push_back(Districts[m]);
					temp5.push_back(new quartier (q));
					Ville.set_np(Ville.get_np()+1);
					break;
		}
		Districts=temp5;
	}
}

bool links_error(vector<quartier*> Quartiers, vector<vector<unsigned int*>> Links_tab, unsigned int uid1, unsigned int uid2)
{
	if (maxlink(Links_tab, uid1, Quartiers, Ville.get_nh())
	or (maxlink(Links_tab, uid2, Quartiers, Ville.get_nh()))
	or (same_thing(uid1, uid2))
	or (same_link(Links_tab, uid1, uid2))
	or (link_does_not_exists(uid1, uid2, Quartiers)) // vacuum
	or (nlink_superposition(uid1, uid2, Quartiers))
	or (nl_sup()))
	{
		return true;
	}
	else
		return false;
}

bool node_error(vector<quartier*> Q, vector<vector<unsigned int*>> Links_tab,
	 quartier node_test, unsigned int uid_test)
{
	if ((node_superposition(Q, node_test))
	or (reserveduid (uid_test))
	or (too_little_capacity(node_test)))
	{
		return true;
	}
	return false;
}

quartier* q_of_uid(unsigned int* uid, city* City)
{
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		if (City->get_q()[i]->get_uid()==*uid)
			return City->get_q()[i];
	}
}

bool nl_sup()
{
	for (unsigned int(i); i<Ville.get_q().size(); ++i)
	{
		for (unsigned int(j); j<Ville.get_lt().size(); ++j)
		{
			if (intersection_cercle_segment(Ville.get_lt()[j][zero], Ville.get_lt()[j][unite], Ville.get_q()[i]))
				return true;
		}
	}
	return false;
}

bool intersection_cercle_segment(unsigned int* u1, unsigned int* u2,
	quartier* q3)
{
  Segment segment = {q_of_uid(u1, city::getvilleinstance())->get_centre(), q_of_uid(u2, city::getvilleinstance())->get_centre()};
  Point proj(projete_orthog (segment, q3->get_centre()));
  double dist (distance(proj, q3->get_centre()));
  if (dist <= q3->get_rayon()) {
    if((proj.x > segment.start.x and proj.x < segment.end.x) or
	   (proj.x > segment.end.x and proj.x < segment.start.x) or
	   (proj.y > segment.start.y and proj.y < segment.end.y) or
	   (proj.y > segment.end.y and proj.y < segment.start.y))
	  return true;
  }
  return false;
}

Point projete_orthog (const Segment& segment, const Point& point)
  {
    /* création d'une fonction affine d'équation y1=mx+p représentant la droite
    entre les deux points du segment : */
    double m ((segment.end.y - segment.start.y) /
              (segment.end.x - segment.start.x));
    double p (segment.start.y - m * segment.start.x);

    /*calcul de la valeur c correspondant à la constante de la droite d'équation -x-m*y2+c=0
    orthogonale au segment et passant pas le point à projeter*/
    double c ((m * point.y) + point.x);

    //résultat des solutions de l'équation y1(x) = y2(x)
    Point res;
    res.x = (c-m*p)/(1+(m*m)) ;
    res.y = m*res.x+p;

    return res;
  }
