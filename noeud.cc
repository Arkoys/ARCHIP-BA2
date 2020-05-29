// Architecture b2
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "error.h"
#include "noeud.h"
using namespace std;



// affiche un message d'erreur puis lance le programme
void erreur(Erreur_lecture code)
{
	switch(code)
	{
		case LECTURE_ARG: cout << " nom de fichier manquant\n";  break;
		case LECTURE_OUVERTURE: cout << " problème de syntaxe\n"; break;
		case LECTURE_NB0: cout << " lecture impossible \n"; break;
		case LECTURE_NB1: cout << " lecture impossible \n"; break;
		case LECTURE_NB2: cout << " lecture impossible \n"; break;
		case LECTURE_NB3: cout << " lecture impossible \n"; break;
		case LECTURE_HOUSING: cout << " lecture impossible \n"; break;
		case LECTURE_TRANSPORT: cout << " lecture impossible \n"; break;
		case LECTURE_PRODUCTION: cout << " lecture impossible \n"; break;
		case LECTURE_LINKS: cout << " lecture impossible links \n"; break;
		case LECTURE_FIN: cout << " format non respecté \n";     break;
		case LECTURE_ETAT: cout << " etat inexistant\n";         break;
		default: cout << " erreur inconnue\n";
	}
	//exit(EXIT_FAILURE); (grisé car le prog se lance malgré les erreurs)
}

//identical uid
bool identical_uid(vector<quartier*> Quartiers)
{
	unsigned int unite(1);
	for (unsigned int i(0); i<Quartiers.size()-unite; ++i)
	{
		for (unsigned int j(unite+i); j<Quartiers.size(); ++j)
		{
			if (Quartiers[i]->get_uid()==Quartiers[j]->get_uid())
			{
				cout << error::identical_uid (Quartiers[i]->get_uid());
				return true;
			}
		}
	}
	return false;
}


//link does not exists
bool link_does_not_exists(unsigned int uid1, unsigned int uid2,
			vector<quartier*> Quartiers)
{
	bool compteur (0);
	for (unsigned int k(0); k<Quartiers.size(); ++k)
	{
		if (uid1==Quartiers[k]->get_uid())
		{
			compteur=1;
		}

	}
	if (compteur == 0)
	{
		cout << error::link_vacuum(uid1);
		return true;
	}
	compteur=0;
	for (unsigned int k(0); k<Quartiers.size(); ++k)
	{
		if (uid2==Quartiers[k]->get_uid())
		{
			compteur=1;
		}

	}
	if (compteur == 0)
	{
		cout << error::link_vacuum(uid2);
		return true;
	}
}


//multiple same link
bool same_link(vector<vector<unsigned int*>> Lt, unsigned int uidtest1,
	 		unsigned int uidtest2)
{
	unsigned int zero(0);
	unsigned int unite(1);
	for (unsigned int j(1); j<Lt.size(); ++j)
	{
		if (uidtest1==*Lt[j][zero] && uidtest2==*Lt[j][unite])
		{
			cout << error::multiple_same_link(uidtest1, uidtest2);
			return true;
		}
		if (uidtest1==*Lt[j][unite] && uidtest2==*Lt[j][zero])
		{
			cout << error::multiple_same_link(uidtest1, uidtest2);
			return true;
		}
	}
	return false;
}

//too little or too much capacity // good
bool too_little_capacity(quartier quart)
{
	if (quart.get_cap() < min_capacity)
	{
		cout << error::too_little_capacity(quart.get_cap());
		return true;
	}
	if (quart.get_cap() > max_capacity)
	{
		cout << error::too_much_capacity(quart.get_cap());
		return true;
	}
	return false;
}

// no node superposition
 bool node_superposition(vector<quartier*> quart, quartier test)
 {
 	for (unsigned int j(0); j<quart.size(); ++j)
 	{
		if (test.get_uid()!=quart[j]->get_uid())
		{
	 		double x1, y1, x2, y2;
	 		double distance;
	 		x1=test.get_coorx();
	 		y1=test.get_coory();
	 		x2=quart[j]->get_coorx();
	 		y2=quart[j]->get_coory();
	 		distance = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	 		if (distance <= (test.get_rayon()+quart[j]->get_rayon()))
	 		{
	 			cout<< error::node_node_superposition(test.get_uid(), quart[j]->get_uid());
	 			return true;
	 		}
		}
 	}
 	return false;
  }

//max link
bool maxlink(vector<vector<unsigned int*>> Lt, unsigned int uid,
		vector <quartier*> q, unsigned int nb_h)
{
	unsigned int zero (0);
	unsigned int unite (1);

	for (unsigned int j(0); j<nb_h; ++j)
	{
		unsigned int compteur (0);
		for (unsigned int i(0); i<Lt.size(); ++i)
		{
			if (*Lt[i][zero]==q[j]->get_uid() or *Lt[i][unite]==q[j]->get_uid())
			{
				if (*Lt[i][zero]==uid)
				{
					++compteur;
				}
				if (*Lt[i][unite]==uid)
				{
					++compteur;
				}
				if (compteur>2)
				{
					unsigned int uidf(uid);
					cout << error::max_link(uidf);
					return true;
				}
			}
		}
	}
	return false;
}


//node using resreve uid
bool reserveduid (unsigned int uid)
{
	if (uid==no_link)
    {
		cout << error::reserved_uid();
		return true;
    }
	return false;
}

//node link superposition
bool nlink_superposition(unsigned int uid1, unsigned int uid2,
			vector <quartier*> quart)
{
	double xuid1, xuid2;
	double yuid1, yuid2;

	for (unsigned int j(0); j<quart.size(); ++j)
	{
		if (uid1==quart[j]->get_uid())
		{
			xuid1=quart[j]->get_coorx();
			yuid1=quart[j]->get_coory();
		}
		if (uid2==quart[j]->get_uid())
		{
			xuid2=quart[j]->get_coorx();
			yuid2=quart[j]->get_coory();
		}
	}
	Point A = {xuid1, yuid1};
	Point B = {xuid2, yuid2};
	Segment seg = {A,B};

	for (unsigned int i(0); i<quart.size(); ++i)
	{
		if (quart[i]->get_uid()!=uid1 and quart[i]->get_uid()!=uid2)
		{
			if (overlapBetweenCircleSegment(quart[i]->get_cercle(), seg))
			{
				cout << error::node_link_superposition(quart[i]->get_uid());
				return true;
			}
		}
	}
	return false;
}
