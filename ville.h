// Architecture b2
#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H
#include <vector>
#include "noeud.h"

// prototypes

bool links_error(std::vector<quartier*> Quartiers, std::vector<std::vector
	<unsigned int*>> Links_tab, unsigned int uid1, unsigned int uid2);
bool node_error(std::vector<quartier*> Q, std::vector<std::vector<unsigned int*>> Links_tab,
	quartier node_test, unsigned int uid_test);
void lecture(char * nom_fichier);
void clear_it(std::vector<quartier*> Q, std::vector <std::vector<unsigned int*>> &L);

class city
{
private:
	std::vector <std::vector<unsigned int*>> Links_tab;
	std::vector <quartier*> Districts;
	std::vector <std::vector<unsigned int*>> green_links;
	unsigned int number_of_houses;
	unsigned int number_of_transports;
	unsigned int number_of_productions;
public:
	city() {}
	std::vector<quartier*> get_q()
		{return Districts;}
	std::vector<std::vector<unsigned int*>> get_lt()
		{return Links_tab;}
	std::vector<std::vector<unsigned int*>> get_glt()
		{return green_links;}
	unsigned int get_nh()
		{return number_of_houses;}
	unsigned int get_nt()
		{return number_of_transports;}
	unsigned int get_np()
		{return number_of_productions;}
	void set_D(std::vector<quartier*> Q)
		{Districts = Q;}
	void set_L(std::vector<std::vector<unsigned int*>> Lt)
		{Links_tab=Lt;}
	void set_nh(unsigned int n)
		{number_of_houses=n;}
	void set_nt(unsigned int n)
		{number_of_transports=n;}
	void set_np(unsigned int n)
		{number_of_productions=n;}
	static city* getvilleinstance();
	void clear_q()
		{Districts.clear();}
	void clear_Lt()
		{Links_tab.clear();}
	void clear_green_lt()
		{green_links.clear();}
	void pb_lt(std::vector<unsigned int*> x)
		{Links_tab.push_back(x);}
	void pb_glt(std::vector<unsigned int*> x)
	  {green_links.push_back(x);}
	void lt_erase(unsigned int x)
	  {Links_tab.erase(Links_tab.begin()+x);}
	void new_q(double coorx, double coory, unsigned int uid , unsigned typeq);
};

bool nl_sup();
quartier* q_of_uid(unsigned int* uid, city* City);
void Delete_City();
void unselect_all(city* City);
void ungreen_all(city* City);

bool intersection_cercle_segment(
	unsigned int* u1, unsigned int* u2, quartier* q3);
Point projete_orthog (const Segment& segment, const Point& point);

#endif
