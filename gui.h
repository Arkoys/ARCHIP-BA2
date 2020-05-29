// Architecture b2
#ifndef GTKMM_EXAMPLE_LAYOUTBUTTONS_H
#define GTKMM_EXAMPLE_LAYOUTBUTTONS_H

#include <gtkmm.h>
#include "graphic.h"
#include <cairomm/context.h>

void turn_green_path(city* City, unsigned int* Q);

class Clic_and_release
{
private:
	double clic_x_init;
	double clic_y_init;
	double clic_x_end;
	double clic_y_end;
public:
	void set_cxi(double x)
		{clic_x_init=x;}
	void set_cyi(double x)
		{clic_y_init=x;}
	double get_cxi()
		{return clic_x_init;}
	double get_cyi()
		{return clic_y_init;}
};

unsigned int create_uid(city* City);
unsigned int checkRadioButtons();
double critMTA(city* City);
double critereMTA(city* City, const unsigned int& d);
double find_access_from_uid(const unsigned int& uid_, city* City);
unsigned int algo_dijkstra (city* City, const unsigned int& d,
		unsigned int type_noeud_final);
unsigned int find_min_access(city* City, const std::vector<unsigned int>& TA);
void tri_selon_access(std::vector<unsigned int>& uids_tries, city* City);
double compute_access(city* City, unsigned int* n, unsigned int* lv);
void check_liens_in(city* City, std::vector<unsigned int>& uids_tries, unsigned int k,
		unsigned int d);
void check_liens_in_P(city* City, std::vector<unsigned int>& uids_tries,unsigned int k,
		unsigned int d);
double critere_ENJ(city* City);
double critere_CI(city* City);
double mta(city* City);
bool dead_end(unsigned int uid, std::vector<std::vector<unsigned int>> lk);
double searsch_into_nodep(quartier* q, city* City, double time_to_production);
double searsch_into_nodet(quartier* q, city* City, double time_to_transport);
double searsch_chain_tr(quartier* q, std::vector<std::vector<unsigned int>> lk,
		city* City, double &tim, double timetest, double &time_to_transport,
		unsigned int uidf);
double searsch_chain_p(quartier* q, std::vector<std::vector<unsigned int>> lk,
		city* City, double &tim, double timetest, double &time_to_production,
		unsigned int uidf);
bool fast__speed(quartier* q1, quartier* q2);

std::string get_enj_label();
std::string get_ci_label();
std::string get_mta_label();
double sum(unsigned int compt, unsigned int st, std::vector<quartier*> qrt);
double dist_connexion(quartier* quart1, quartier* quart2);


struct Frame // Framing and window parameters
{
	double xMin; // frame parameters
	double xMax;
	double yMin;
	double yMax;
	double asp;  // frame aspect ratio
	int height;  // window height
	int width;   // window width
};

class Zoom
{
public:
	Zoom(bool zm, double z)
			:zoom_modified(zm), zoom(z)
	{}
	double get_zoom()
	{return zoom;}
	bool get_zoom_modified()
	{return zoom_modified;}
	void set_zoom(double z)
	{zoom=z;}
	void set_zoom_modified(double zm)
	{zoom_modified=zm;}
private:
	double zoom;
	bool zoom_modified;
};

class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();
	virtual ~MyArea();
	void setFrame(Frame x);
	void refresh();
protected:
	//Override default signal handler:
	void draw_city(city* Ville, const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
	Frame frame;
};

class MyEvent : public Gtk::Window
{
public:
	MyEvent();
	virtual ~MyEvent();

protected:
	//Signal handlers:
	void save(city* City);
	void on_button_clicked_exit();
	void on_button_clicked_new();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_shortest_path();
	void on_button_clicked_zoom_in();
	void on_button_clicked_zoom_out();
	void on_button_clicked_zoom_reset();
	void on_button_clicked_edit_link();
	bool on_button_press_event(GdkEventButton * event);
	bool on_button_release_event(GdkEventButton * event);
	void delete_q(city* City, unsigned int i);
	//  bool on_button_release_event(GdkEventButton * event);
	void clic (city* City, double clic_x, double clic_y, double origin_x,
			double origin_y);
	void clic_voisin(quartier* q, city* City, Point clicm);
	void right_clic(city* City, double clic_x, double clic_y);
	void change_rayon(double clic_x2,double clic_y2, city* City,Clic_and_release pinit,
			double origin_x, double origin_y);
	bool sth_n_selected(city* City);
	bool clic_into_a_node(city* City, double coor_x, double coor_y);
	bool on_key_press_event(GdkEventKey * key_event);
	void Refresh();

	// interface components
	//Child widgets:
	Gtk::Box m_Box_Main, m_Box_Left, m_Box_Right, m_Box1, m_Box2, m_Box3, m_Box4;
	MyArea m_Area;
	Gtk::Separator m_Separator1, m_Separator2, m_Separator3;
	Gtk::Label m_Label_General;
	Gtk::Label m_Label_Display;
	Gtk::Label m_Label_Editor;
	Gtk::Label m_Label_Informations;
	Gtk::Label m_Label_Zoom;

	Gtk::Label m_Label_c_ENJ;
	Gtk::Label m_Label_c_CI;
	Gtk::Label m_Label_c_MTA;
	Gtk::Button m_Button_exit;
	Gtk::Button m_Button_new;
	Gtk::Button m_Button_open;
	Gtk::Button m_Button_save;
	Gtk::ToggleButton m_ToggleButton_shortest_path;
	Gtk::Button m_Button_zoom_in;
	Gtk::Button m_Button_zoom_out;
	Gtk::Button m_Button_zoom_reset;
	Gtk::ToggleButton m_ToggleButton_edit_link;
	Gtk::RadioButton m_RadioButton1, m_RadioButton2, m_RadioButton3;
};



double min_coorx(city City);
double min_coory(city City);
double max_coory(city City);
double max_coorx(city City);

#endif
