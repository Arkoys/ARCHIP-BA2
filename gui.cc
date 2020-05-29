// Architecture b2
#include "gui.h"
#include "iostream"
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

static Clic_and_release pinit;
static Zoom zoom(false, 1.);

using namespace std;

MyArea::MyArea()
{
}

MyArea::~MyArea()
{
}

void MyArea::setFrame(Frame x)
{
	frame = x;
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
									Frame frame)
{
	// déplace l'origine au centre de la fenêtre
	cr->translate(frame.width/2, frame.height/2);

	// normalise la largeur et hauteur aux valeurs fournies par le cadrage
	// ET inverse la direction de l'axe Y
	double sfx(frame.width/(frame.xMax - frame.xMin));
	double sfy(-frame.height/(frame.yMax - frame.yMin));

	if (zoom.get_zoom_modified())
	cr->scale(sfx*zoom.get_zoom(),sfy*zoom.get_zoom());
	else
	cr->scale(sfx,sfy);


	// décalage au centre du cadrage
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

MyEvent::MyEvent() :
	m_Box_Main(Gtk::ORIENTATION_HORIZONTAL),
	m_Box1(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box2(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box3(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box4(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_Left(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_Right(Gtk::ORIENTATION_VERTICAL, 10),
	m_Label_General("Général"),
	m_Label_Display("Display"),
	m_Label_Editor("Editor"),
	m_Label_Informations("Informations"),
	m_Label_Zoom("zoom: x" +to_string(zoom.get_zoom())),
	m_Label_c_ENJ("ENJ: "+to_string(critere_ENJ(city::getvilleinstance()))),
	m_Label_c_CI(get_ci_label()),
	m_Label_c_MTA(get_mta_label()),
	//m_Label_c_MTA(get_mta_label()),
	m_Button_exit("exit"),
	m_Button_new("new"),
	m_Button_open("open"),
	m_Button_save("save"),
	m_ToggleButton_shortest_path("shortest_path"),
	m_Button_zoom_in("zoom_in"),
	m_Button_zoom_out("zoom_out"),
	m_Button_zoom_reset("zoom_reset"),
	m_ToggleButton_edit_link("edit_link"),
	m_RadioButton1("housing"),
	m_RadioButton2("transport"),
	m_RadioButton3("production")
	{
	// Set title and border of the window
	set_title("layout buttons");
	set_border_width(0);

	// Add outer box to the window (because the window
	// can only contain a single widget)
	add(m_Box_Main);

	// Put radio buttons 2 and 3 in the same group as 1:
	m_RadioButton2.join_group(m_RadioButton1);
	m_RadioButton3.join_group(m_RadioButton1);

	//Put the inner boxes and the separator in the outer box:
	m_Box_Main.pack_start(m_Box_Left);
	m_Box_Main.pack_start(m_Box_Right);

	m_Box_Left.pack_start(m_Label_General);
	m_Box_Left.pack_start(m_Box1);
	m_Box_Left.pack_start(m_Separator1);
	m_Box_Left.pack_start(m_Label_Display);
	m_Box_Left.pack_start(m_Box2);
	m_Box_Left.pack_start(m_Separator2);
	m_Box_Left.pack_start(m_Label_Editor);
	m_Box_Left.pack_start(m_Box3);
	m_Box_Left.pack_start(m_Separator3);
	m_Box_Left.pack_start(m_Label_Informations);
	m_Box_Left.pack_start(m_Box4);

	m_Area.set_size_request(900, 900);
	m_Box_Right.pack_start(m_Area);

	// Set the inner boxes' borders
	m_Box1.set_border_width(10);
	m_Box2.set_border_width(10);
	m_Box3.set_border_width(10);
	m_Box4.set_border_width(10);

	// exit/nex/open/save in Box1:
	m_Box1.pack_start(m_Button_exit);
	m_Box1.pack_start(m_Button_new);
	m_Box1.pack_start(m_Button_open);
	m_Box1.pack_start(m_Button_save);

	// shortest_path / zoom_in / zoom_out / zoom_reset buttons in Box2:
	m_Box2.pack_start(m_ToggleButton_shortest_path);
	m_Box2.pack_start(m_Button_zoom_in);
	m_Box2.pack_start(m_Button_zoom_out);
	m_Box2.pack_start(m_Button_zoom_reset);
	m_Box2.pack_start(m_Label_Zoom);

	// edit_link button and housing / transport / production dots in Box3:
	m_Box3.pack_start(m_ToggleButton_edit_link);
	m_Box3.pack_start(m_RadioButton1);
	m_Box3.pack_start(m_RadioButton2);
	m_Box3.pack_start(m_RadioButton3);

	m_Box4.pack_start(m_Label_c_ENJ);
	m_Box4.pack_start(m_Label_c_CI);
	m_Box4.pack_start(m_Label_c_MTA);
   // Set the first button active
	m_RadioButton1.set_active();

	// Connect the clicked signal of the button to
	// thier signal handler
	m_Button_exit.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_exit) );

	m_Button_new.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_new) );

	m_Button_open.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_open) );

	m_Button_save.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_save) );

	m_ToggleButton_shortest_path.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_shortest_path) );

	m_Button_zoom_in.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_zoom_in) );

	m_Button_zoom_out.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_zoom_out) );

	m_Button_zoom_reset.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_zoom_reset) );

	m_ToggleButton_edit_link.signal_clicked().connect(sigc::mem_fun(*this,
			 &MyEvent::on_button_clicked_edit_link) );

	// Show all children of the window
	show_all_children();
	}

MyEvent::~MyEvent()
{
}

void MyEvent::on_button_clicked_exit()
{
    hide(); //to close the application.
}

void MyEvent::save (city* City)
{
	Gtk::FileChooserDialog dialog("Please choose a file",
	Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);
	//m_Label_Info.set_text("choosing a file");
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	//m_Label_Info.set_text("Done choosing a file");
	//Handle the response:
	switch(result)
	{
	case(Gtk::RESPONSE_OK):
	{
	std::cout << "Save clicked." << std::endl;
	//Notice that this is a std::string, not a Glib::ustring.
	std::string filename = dialog.get_filename();
	ofstream file;
	file.open(dialog.get_filename()+".txt");
	unsigned int zero (0);
	unsigned int unite (1);

	file << City->get_nh()<<endl;
	for (unsigned int i(0); i<City->get_nh(); ++i)
	{
		file<<City->get_q()[i]->get_uid()<<" "<<City->get_q()[i]->get_coorx()<<" "<<
			City->get_q()[i]->get_coory()<<" "<<City->get_q()[i]->get_cap()<<endl;
	}
	file << endl<<City->get_nt()<<endl;
	for (unsigned int i(City->get_nh()); i<(City->get_nh()+City->get_nt()); ++i)
	{
		file<<City->get_q()[i]->get_uid()<<" "<<City->get_q()[i]->get_coorx()<<" "<<
			City->get_q()[i]->get_coory()<<" "<<City->get_q()[i]->get_cap()<<endl;
	}
	file <<endl<<City->get_np()<<endl;
	for (unsigned int i(City->get_nh()+City->get_nt()); i<(City->get_nh()+
					City->get_nt()+City->get_np()); ++i)
	{
		file<<City->get_q()[i]->get_uid()<<" "<<City->get_q()[i]->get_coorx()<<" "<<
			City->get_q()[i]->get_coory()<<" "<<City->get_q()[i]->get_cap()<<endl;
	}
	file<<endl<<City->get_lt().size()<<endl;
	for (unsigned int j(0); j<City->get_lt().size(); ++j)
	{
		file << *City->get_lt()[j][zero] << " " << *City->get_lt()[j][unite] << endl;
	}
	std::cout << "File created: " << filename << std::endl;
	break;
	}
	case(Gtk::RESPONSE_CANCEL):
	{
	std::cout << "Cancel clicked." << std::endl;
	break;
	}
	default:
	{
	std::cout << "Unexpected button clicked." << std::endl;
	break;
	}
	}


}

void MyEvent::on_button_clicked_save()
{
	save(city::getvilleinstance());
}

void MyEvent::on_button_clicked_new()
{
	Delete_City();
	Refresh();
	m_Label_c_ENJ.set_label("0");
	m_Label_c_CI.set_label("0");
	m_Label_c_MTA.set_label("0");
    cout << "new" << endl;
}

void MyEvent::on_button_clicked_open()
{
	cout << "open" << endl;
	Gtk::FileChooserDialog dialog("Please choose a file",
		  Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	//Handle the response:
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			std::cout << "Open clicked." << std::endl;

			//Notice that this is a std::string, not a Glib::ustring.
			std::string filename = dialog.get_filename();
			std::cout << "File selected: " <<  filename << std::endl;
			//char * file1 = dialog.get_filename();
			char * file = const_cast<char*>(filename.c_str());
			Delete_City();
			lecture(file);
			unselect_all(city::getvilleinstance());
			Refresh();
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cancel clicked." << std::endl;
			break;
		}
		default:
		{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	}
}

void MyEvent::on_button_clicked_shortest_path()
{
	m_ToggleButton_shortest_path.toggled();
    cout << "shortest path " << endl;
}

void MyEvent::on_button_clicked_zoom_in()
{
	if (zoom.get_zoom()<max_zoom)
	{
	cout << "zoom_in" << endl;
	zoom.set_zoom_modified(true);
	zoom.set_zoom(zoom.get_zoom()+delta_zoom);
	Refresh();
  }
}

void MyEvent::on_button_clicked_zoom_out()
{
	if (zoom.get_zoom()>min_zoom+min_min)
	{
	cout << "zoom_out" << endl;
	zoom.set_zoom(zoom.get_zoom()-delta_zoom);
	cout << "zoom get zooommmmmm:  " << zoom.get_zoom() << endl;
	zoom.set_zoom_modified(true);
	Refresh();
  }

}

void MyEvent::on_button_clicked_zoom_reset()
{
	if (zoom.get_zoom()!=1)
	{
	cout << "zoom_reset" << endl;
	zoom.set_zoom(unite);
	zoom.set_zoom_modified(false);
	Refresh();
  }

}

void MyEvent::on_button_clicked_edit_link()
{
	m_ToggleButton_edit_link.toggled();
	cout << "edit link" << endl ;
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Frame frame;
	frame.xMin = -1000.0;
	frame.xMax = 1000.0;
	frame.yMin = -1000.0;
	frame.yMax = 1000.0;

	frame.width=900;
	frame.height=900;

	frame.asp = ( frame.xMax-frame.xMin ) / ( frame.yMax-frame.yMin );

	setFrame(frame);
	orthographic_projection(cr, frame);

	draw_city(city::getvilleinstance(), cr);
	dessin_link(city::getvilleinstance(), cr);

	return true;
}


void MyArea::draw_city(city* City, const Cairo::RefPtr<Cairo::Context>& cr)
{

	//cout << City->get_q()[2]->get_uid() << endl;
	if (City->get_nh()!=0)
	{
		for(unsigned int i(0); i<City->get_nh(); ++i)
		{
			dessin_Logement(City->get_q()[i]->get_coorx(), City->get_q()[i]->
					get_coory(),City->get_q()[i]->get_rayon(), cr, City->get_q()[i]->
					get_selected(), City->get_q()[i]->get_green());
		}
	}
	if (City->get_nt()!=0)
	{
		for(unsigned int j(City->get_nh()); j<(City->get_nh()+City->get_nt()); ++j)
		{
			dessin_Transport(City->get_q()[j]->get_coorx(), City->get_q()[j]->
					get_coory(), City->get_q()[j]->get_rayon(), cr, City->get_q()[j]->
					get_selected(), City->get_q()[j]->get_green());
		}
	}
	if (City->get_np()!=0)
	{
		for(unsigned int k(City->get_nt()+City->get_nh()); k<(City->get_nh()+City->
			get_nt()+City->get_np()); ++k)
		{
			dessin_Production(City->get_q()[k]->get_coorx(), City->get_q()[k]->
				get_coory(),City->get_q()[k]->get_rayon(), cr, City->get_q()[k]->
				get_selected(), City->get_q()[k]->get_green());
		}
	}
}

void MyEvent::Refresh()
{
	m_Area.refresh();
	m_Label_Zoom.set_label("zoom: x" +to_string(zoom.get_zoom()));
	m_Label_c_ENJ.set_label(get_enj_label());
	m_Label_c_CI.set_label(get_ci_label());
 	m_Label_c_MTA.set_label(get_mta_label());
}

void MyArea::refresh()
{
	auto win = get_window();
	if(win)
	{
		Gdk::Rectangle r(0,0, get_allocation().get_width(), get_allocation().
					get_height());
		win->invalidate_rect(r,false);
	}
}

double critere_ENJ (city* City)
{
  unsigned int start(0);
  unsigned int c(City->get_nh()+City->get_nt()+City->get_np());
  return ((sum(City->get_nh(),start,City->get_q())-sum(c,City->get_nh(),City->get_q()))
			/sum(c, start, City->get_q()));
}

double sum (unsigned int compt,unsigned int st, vector<quartier*> qrt)
{
	double sum;
	for (unsigned int i(st); i<compt; ++i)
	{
		sum += qrt[i]->get_cap();
	}
	return sum;
}

double critere_CI (city* City)
{
	unsigned int zero (0);
	unsigned int unite (1);
	double somme(0.), capa, dist, vitesse;
	quartier* q1;
	quartier* q2;
	for (unsigned int i(0); i<City->get_lt().size();++i)
	{
		for (unsigned int j(0); j<City->get_q().size();++j)
		{
			if (*City->get_lt()[i][zero]==City->get_q()[j]->get_uid())
				q1=City->get_q()[j];
			if (*City->get_lt()[i][unite]==City->get_q()[j]->get_uid())
				q2=City->get_q()[j];
		}
		unsigned int s(1), t(1);
		for (unsigned int j(City->get_nh()); j<(City->get_nh()+City->get_nt()); ++j)
		{
		    if (City->get_q()[j]->get_uid()==q1->get_uid())
				s=zero;
		}
		for (unsigned int j(City->get_nh()); j<(City->get_nh()+City->get_nt()); ++j)
		{
			if (City->get_q()[j]->get_uid()==q2->get_uid())
				t=zero;
		}
		if (s==zero and t==zero)
			vitesse=fast_speed;
		else
			vitesse=default_speed;
		capa = min(q1->get_cap(), q2->get_cap());
		dist = dist_connexion(q1, q2);
		somme = somme + dist*capa*vitesse;
		cout << somme << endl;
	}
	return somme;
}

string get_enj_label()
{
	ostringstream oss;
	oss<< "ENJ: " << critere_ENJ(city::getvilleinstance());
	return oss.str();
}

string get_ci_label()
{
	ostringstream oss;
	oss<< "IC: " << critere_CI(city::getvilleinstance());
	return oss.str();
}

string get_mta_label()
{
	ostringstream oss;
	//oss<< "MTA: " << mta(*city::getvilleinstance());
	oss<< "MTA: " << critMTA(city::getvilleinstance());
	return oss.str();
}

double dist_connexion(quartier* q1, quartier* q2)
{
	Point P1 = {q1->get_coorx(), q1->get_coory()};
	Point P2 = {q2->get_coorx(), q2->get_coory()};
	return distance(P1, P2);
}



bool fast__speed(quartier* q1, quartier* q2)
{
	  int i(0);
	  if (q1->get_type()==2)
		++i;
	  if (q2->get_type()==2)
		++i;
	  if (i==2)
		return true;
	  else
		return false;
}

bool dead_end(unsigned int uid, std::vector<std::vector<unsigned int>> lk)
{
	int b(1);
	int zero(0);
	int unite (1);
	for(unsigned int i(0); i<lk.size(); ++i)
	{
		if (lk[i][zero] == uid)
			b=0;
		if (lk[i][unite] == uid)
			b=0;
	}
	cout << "uid: " << uid << endl;
	cout << "b: " << b << endl;
	return b;
}

double critMTA(city* City)
{
	cout << "critMTA" << endl;
	unsigned int compteur_noeudsL(0);
	double MeilleurTempsAcces(0.);
	for(size_t i(0); i < City->get_q().size(); ++i)
	{
		if((City->get_q()[i]->get_type())==1)
		{
			cout << "critMTA" << endl;
			cout << "temps d'accès pour " <<  City->get_q()[i]->get_uid() << endl;
			++compteur_noeudsL;
			MeilleurTempsAcces+=critereMTA(City, City->get_q()[i]->get_uid());
			cout << "MEilleur temps d access" << MeilleurTempsAcces << endl;
		}
    }
    MeilleurTempsAcces=MeilleurTempsAcces/compteur_noeudsL;
	cout << "final" << MeilleurTempsAcces << endl;
	return MeilleurTempsAcces;
}

double critereMTA(city* City, const unsigned int& d)
{
	double critere(0.);
	cout << "critere MTA" << endl;
	unsigned int noeudP_proche (algo_dijkstra(City, d,3));
	if (noeudP_proche==no_link)
		return infinite_time;

	for (size_t i(0); i < City->get_q().size(); ++i)
	{
		cout << "critere MTA1" << endl;
		if (noeudP_proche==(City->get_q()[i]->get_uid()))
		{
			critere += City->get_q()[i]->get_access();
			cout << "VOICI LES TIMINGS mod1:" << City->get_q()[i]->get_access()<< endl;
		}
	}

	unsigned int noeudT_proche (algo_dijkstra(City, d,2));
	if (noeudT_proche==no_link)
		return infinite_time;

	for (size_t i(0); i < City->get_q().size(); ++i)
	{
		cout << "critere MTA2" << endl;
		if (noeudT_proche==(City->get_q()[i]->get_uid()))
		{
			critere += City->get_q()[i]->get_access();
			cout << "VOICI LES TIMINGS mod2:" << City->get_q()[i]->get_access()<< endl;
		}
	}
	cout << "critere MTA3: " << critere << endl;
	return critere;
}

unsigned int algo_dijkstra (city* City, const unsigned int& d,
							unsigned int type_noeud_final)
{
	cout << "algo dijkstra" << endl;
	for (size_t i(0); i < City->get_q().size(); ++i)
	{
		City->get_q()[i]->set_in(true);
		City->get_q()[i]->set_access(infinite_time);
		City->get_q()[i]->set_parent(no_link);
		if (City->get_q()[i]->get_uid()==d)
			City->get_q()[i]->set_access(0.);
	}

	vector<unsigned int> TA;
	for (size_t i(0); i < City->get_q().size(); ++i)
		TA.push_back(City->get_q()[i]->get_uid());
	tri_selon_access(TA, City);
	unsigned int n;
	bool still_in(false);
	for (size_t i(0); i < City->get_q().size(); ++i)
	{
		if (City->get_q()[i]->get_in())
			still_in=true;
	}
	while (still_in)
	{
		cout << "boucle while" << endl;
		n = find_min_access(City, TA);
		cout << "CECI EST N: " << n << endl;
		//check si lien recherché
		for (size_t i(0); i < City->get_q().size(); ++i)
		{
			if ((City->get_q()[i]->get_uid()==n) && (City->get_q()[i]->get_type()
				==type_noeud_final))
				return n;
		}

		// vamlide le passe par le noeud n
		for (size_t i(0); i < City->get_q().size(); ++i)
		{
			if ((City->get_q()[i]->get_uid())==n)
			{
				City->get_q()[i]->set_in(false);
				if (type_noeud_final==3)
					check_liens_in_P(City, TA, i, d);
				else
					check_liens_in(City, TA, i, d);
			}
		}
		// verifie s'il reste des noeuds
		still_in=false;
		for (size_t i(0); i < City->get_q().size(); ++i)
		{
			if (City->get_q()[i]->get_in())
			{
				still_in=true;
			}
		}
	}
	return no_link;
}


unsigned int find_min_access(city* City, const std::vector<unsigned int>& TA)
{
	cout << "find min access" << endl;
	for (size_t i (0); i < TA.size(); ++i)
	{
		for (size_t j(0); j < City->get_q().size(); ++j)
		{
			if ((City->get_q()[j]->get_uid()==TA[i])&&(City->get_q()[j]->get_in()))
				return City->get_q()[j]->get_uid();
		}
	}
}

void tri_selon_access(vector<unsigned int>& uids_tries, city* City)
{
	cout << "tri selon access" << endl;
	unsigned int j, tmp;
	for (size_t i(0); i < uids_tries.size(); ++i)
	{
		tmp = uids_tries[i];
		j=i;
		while (j>0 && (find_access_from_uid(uids_tries[j-1],City)>find_access_from_uid
				(tmp,City)))
		{
			uids_tries[j]=uids_tries[j-1];
			j=j-1;
		}
		uids_tries[j]= tmp;
	}
}


double find_access_from_uid(const unsigned int& uid_, city* City)
{
	for (size_t i(0); i < City->get_q().size(); ++i)
	{
		if ((City->get_q()[i]->get_uid())==uid_)
			return City->get_q()[i]->get_access();
	}
	return infinite_time;  //instruction jamais atteinte
}

double compute_access(city* City, unsigned int* n,
                        unsigned int* lv)
{
	cout << "compute access" << endl;
	for (size_t i(0); i < City->get_q().size(); ++i)
	{
		for (size_t j(0); j < City->get_q().size(); ++j)
		{
			if (((City->get_q()[i]->get_uid())==*n) and ((City->get_q()[j]->
				get_uid())==*lv))
			{
				double dist(distance((City->get_q()[i]->get_centre()), City->get_q()[j]
					->get_centre()));
				cout << "distance" << dist << endl;
				if((City->get_q()[i]->get_type()==2) and (City->get_q()[j]->
					get_type()==2))
					return dist/fast_speed;
				else
					return dist/default_speed;
			}
		}
	}
	return infinite_time; //instruction jamais atteinte
}

void check_liens_in(city* City, vector<unsigned int>& uids_tries, unsigned int k,
					unsigned int d)
{
	cout << "check_liens" << endl;
	double alt;
	for (unsigned int i(0); i < City->get_lt().size(); ++i)
	{
		cout << "LIEN NUMERO: " << i << endl;
		if (City->get_q()[k]->get_uid()==*City->get_lt()[i][0])
		{
			if (q_of_uid(City->get_lt()[i][1], City)->get_in())
			{
				alt = City->get_q()[k]->get_access() + compute_access(City,City->
						get_lt()[i][0],City->get_lt()[i][1]);
				if ((q_of_uid(City->get_lt()[i][1], City)->get_access() > alt) &&
						!(q_of_uid(City->get_lt()[i][1], City)->get_type()==3))
				{
					q_of_uid(City->get_lt()[i][1], City)->set_access(alt);
					cout << "SET ACCESS to " << City->get_lt()[i][1] << "  " <<
							q_of_uid(City->get_lt()[i][1], City)->get_access() << endl;
					q_of_uid(City->get_lt()[i][1], City)->set_parent(*City->
						get_lt()[i][0]);
					tri_selon_access(uids_tries, City);
					cout << "alt: " << alt << endl;
				}
			}
		}
		if (City->get_q()[k]->get_uid()==*City->get_lt()[i][1])
		{
			if (q_of_uid(City->get_lt()[i][0], City)->get_in())
			{
				alt = City->get_q()[k]->get_access() + compute_access(City,City->
						get_lt()[i][1],City->get_lt()[i][0]);
				if ((q_of_uid(City->get_lt()[i][0], City)->get_access() > alt) &&
					!(q_of_uid(City->get_lt()[i][0], City)->get_type()==3))
				{
					q_of_uid(City->get_lt()[i][0], City)->set_access(alt);
					cout << "SET ACCESS to " << City->get_lt()[i][0] << "  " <<
						q_of_uid(City->get_lt()[i][0], City)->get_access() << endl;
					q_of_uid(City->get_lt()[i][0], City)->set_parent(*City->
						get_lt()[i][1]);
					tri_selon_access(uids_tries, City);
					cout << "alt: " << alt << endl;
				}
			}
		}
	}
}


void check_liens_in_P(city* City,vector<unsigned int>& uids_tries, unsigned int k,
						unsigned int d)
{
	cout << "check_liens" << endl;
	double alt;
	for (unsigned int i(0); i < City->get_lt().size(); ++i)
	{
		cout << "LIEN NUMERO: " << i << endl;
		if (City->get_q()[k]->get_uid()==*City->get_lt()[i][0])
			{
			if (q_of_uid(City->get_lt()[i][1], City)->get_in())
			{
				alt = City->get_q()[k]->get_access() + compute_access(City,City->
					get_lt()[i][0],City->get_lt()[i][1]);
				if ((q_of_uid(City->get_lt()[i][1], City)->get_access() > alt))
				{
					q_of_uid(City->get_lt()[i][1], City)->set_access(alt);
					cout << "SET ACCESS to " << City->get_lt()[i][1] << "  " <<
						q_of_uid(City->get_lt()[i][1], City)->get_access() << endl;
					q_of_uid(City->get_lt()[i][1], City)->set_parent(*City->
						get_lt()[i][0]);
					tri_selon_access(uids_tries, City);
					cout << "alt: " << alt << endl;
				}
			}
		}
		if (City->get_q()[k]->get_uid()==*City->get_lt()[i][1])
		{
			if (q_of_uid(City->get_lt()[i][0], City)->get_in())
			{
				alt = City->get_q()[k]->get_access() + compute_access(City,City->
					get_lt()[i][1],City->get_lt()[i][0]);
				if ((q_of_uid(City->get_lt()[i][0], City)->get_access() > alt))
				{
					q_of_uid(City->get_lt()[i][0], City)->set_access(alt);
					cout << "SET ACCESS to " << City->get_lt()[i][0] << "  " <<
							q_of_uid(City->get_lt()[i][0], City)->get_access() << endl;
					q_of_uid(City->get_lt()[i][0], City)->set_parent(*City->
						get_lt()[i][1]);
					tri_selon_access(uids_tries, City);
					cout << "alt: " << alt << endl;
				}
			}
		}
	}
}


// Mouse event signal handlers:
bool MyEvent::on_button_press_event(GdkEventButton * event)
{

	if(event->type == GDK_BUTTON_PRESS)
	{
		// raw mouse coordinates in the window frame
		double clic_x = event->x ;
		double clic_y = event->y ;
		cout << "mouse x = " << clic_x << "\t mouse y = " << clic_y << endl;

		// origin of the drawing area
		double origin_x = m_Area.get_allocation().get_x();
		double origin_y = m_Area.get_allocation().get_y();
		double clic_xf, clic_yf;
		clic_xf=clic_x-(origin_x)-450;
		clic_yf= -(clic_y-(origin_y)-450);
		cout << "mouse xf = " << clic_xf << "\t mouse yf = " << clic_yf << endl;
		cout << "mouse xff = " << clic_xf*2000/900 << "\t mouse yff = " <<
				clic_yf*2000/900 << endl;
		cout << "m_Area x = " << origin_x << "\t m_Area y = " << origin_y << endl;

		// get width and height of drawing area
		double width = m_Area.get_allocation().get_width();
		double height= m_Area.get_allocation().get_height();
		cout << "width  = " << width << "\t height = " << height << endl;
		// retain only mouse events located within the drawing area
		if(clic_x >= origin_x && clic_x <= origin_x + width &&
		   clic_y >= origin_y && clic_y <= origin_y + height)
		{
			// Point that we are allowed to use expressed with drawing area coord.
			Point p({clic_x - origin_x, clic_y -origin_y});
			pinit.set_cxi(clic_xf);
			pinit.set_cyi(clic_yf);
			// believe it or not ; there is no symbol to designate mouse buttons...
			if(event->button == 1) // Left mouse button
				clic(city::getvilleinstance(), clic_xf, clic_yf, origin_x, origin_y);
			if(event->button == 3) // Right mouse button
				right_clic(city::getvilleinstance(), clic_xf, clic_yf);
		}
	}
	return true;
}

void MyEvent::right_clic (city* City, double clic_x, double clic_y)
{
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		if (City->get_q()[i]->get_selected())
		{
			quartier temp8(City->get_q()[i]->get_uid(), clic_x*unframe/zoom.get_zoom(),
				clic_y*unframe/zoom.get_zoom(), City->get_q()[i]->get_cap(), City->get_q()[i]->get_type());
			if (!(node_superposition(City->get_q(), temp8)))
			{
			cout << "CLICX" << clic_x << endl;
			cout << "CLICY" << clic_y << endl;
			cout << "COOR" << City->get_q()[i]->get_coorx() << endl;
			cout << "COOR" << City->get_q()[i]->get_coory() << endl;
			City->get_q()[i]->set_coorx(clic_x*unframe/zoom.get_zoom());
			City->get_q()[i]->set_coory(clic_y*unframe/zoom.get_zoom());
			cout << "COOR" << City->get_q()[i]->get_coorx() << endl;
			cout << "COOR" << City->get_q()[i]->get_coory() << endl;
			Refresh();
		  }
		}
	}
}


void MyEvent::clic (city* City, double clic_x, double clic_y, double origin_x,
					double origin_y)
{
	double unframex((2000./900.)/zoom.get_zoom());
	double unframey((2000./900.)/zoom.get_zoom());
	cout << "dans clic" << endl;
	Point clicm = {clic_x*unframex, clic_y*unframey};
	bool t1(true), t2(true);
	if (m_ToggleButton_edit_link.get_active())
	{
		ungreen_all(City);
		for (unsigned int i(0); i<City->get_q().size();++i)
		{
			if (City->get_q()[i]->get_selected())
			{
				quartier* q;
				q=City->get_q()[i];
				clic_voisin(q, City, clicm);
				t1=false;
			}
		}
		Refresh();
		cout << "refresh" << endl;
	}

	if (t1)// (un)select or destroy a district
	{
		for (unsigned int i(0); i<City->get_q().size(); ++i)
		{
			if (distance(City->get_q()[i]->get_centre(), clicm) <= City->get_q()[i]->
				get_rayon())
			{
				if (City->get_q()[i]->get_type()==1 & m_ToggleButton_shortest_path.
					get_active())
				{
					ungreen_all(City);
					unsigned int d=City->get_q()[i]->get_uid();
					unsigned int nT_proche (algo_dijkstra(City, d,deux));
					unsigned int* nTp = new unsigned int (nT_proche);
					if (q_of_uid(nTp, City)->get_parent() != no_link)
					{
						City->get_q()[i]->set_green(true);
						q_of_uid(nTp, City)->set_green(true);
						turn_green_path(City, nTp);
					}
					unsigned int nP_proche (algo_dijkstra(City, d,trois));
					unsigned int* nPp = new unsigned int (nP_proche);
					if (q_of_uid(nPp, City)->get_parent() != no_link)
					{
						City->get_q()[i]->set_green(true);
						q_of_uid(nPp, City)->set_green(true);
						turn_green_path(City, nPp);
					}
				}
				else
					ungreen_all(City);

				cout << "selected" << endl;
				t2=false; // clic dans un cercle
				if (City->get_q()[i]->get_selected())
					delete_q(City, i);
				else
				{
					unselect_all(City);
					City->get_q()[i]->set_selected(true);
				}
				Refresh();
				cout << "refresh" << endl;
			}
		}
	}

	if(t1 and t2 and sth_n_selected(City)) // creation of a new district
	{
		ungreen_all(City);
		if (m_RadioButton1.get_active())
		{
			City->new_q(clic_x*unframex, clic_y*unframey, create_uid(City), 1);
		}
		if (m_RadioButton2.get_active())
		{
			City->new_q(clic_x*unframex, clic_y*unframey, create_uid(City), 2);
		}
		if (m_RadioButton3.get_active())
		{
			City->new_q(clic_x*unframex, clic_y*unframey, create_uid(City), 3);
		}
		cout << "ref avant" << endl;
		Refresh();
		cout << "ref apres" << endl;
	}

	if (t1 and t2)
		ungreen_all(City);
}


void turn_green_path(city* City, unsigned int* Q)
{
	vector<unsigned int*> temp6;
	temp6.push_back(Q);
	unsigned int* qq= new unsigned int (q_of_uid(Q, City)->get_parent());
	temp6.push_back(qq);
	City->pb_glt(temp6);
	unsigned int* qq1 = new unsigned int(q_of_uid(Q, City)->get_parent());
	if (!(q_of_uid(qq1, City)->get_green()))
	{
		q_of_uid(qq1, City)->set_green(true);
		turn_green_path(City, new unsigned int (q_of_uid(Q, City)->get_parent()));
	}
}

bool MyEvent::sth_n_selected(city* City)
{
	bool temp(true);
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		if (City->get_q()[i]->get_selected())
			temp=false;
	}
	return temp;
}

void MyEvent::clic_voisin(quartier* q, city* City, Point clicm)
{
	bool link_not_exist(true);
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		if (distance(City->get_q()[i]->get_centre(), clicm) <= City->get_q()[i]->
			get_rayon())
		{
			for (unsigned int j(0); j<City->get_lt().size(); ++j)
			{
				cout << "avant if" << endl;
				if ((City->get_q()[i]->get_uid()==*City->get_lt()[j][0] or City->
					get_q()[i]->get_uid()==*City->get_lt()[j][1]) &&(q->get_uid()==
					*City->get_lt()[j][0] or q->get_uid()==*City->get_lt()[j][1]))
				{
					City->lt_erase(j); //efface le lien
					link_not_exist=false;
					cout << "link_not_exist" << endl;
				}
			}
			if (link_not_exist)
			{
				if (!(maxlink(City->get_lt(), q->get_uid(), City->get_q(), City->get_nh())) &&
					!(maxlink(City->get_lt(), City->get_q()[i]->get_uid(), City->get_q(),
					City->get_nh())) && !(nl_sup()))
				{
				cout << "pour créer link" << endl;
				vector<unsigned int*> temp4;
				unsigned int* u41= new unsigned int (q->get_uid());
				unsigned int* u42= new unsigned int (City->get_q()[i]->get_uid());
				temp4.push_back(u41);
				temp4.push_back(u42);
				City->pb_lt(temp4); //crée lien
				cout << City->get_lt()[City->get_lt().size()-unite][zero] << endl;
				cout << City->get_lt()[City->get_lt().size()-unite][unite] << endl;
			  }
			}
		}
	}
}


unsigned int create_uid(city* City)
{
	unsigned int uid(1);
	bool t(true);
	while (t)
	{
		t=false;
		for (unsigned int i(0); i<City->get_q().size(); ++i)
		{
			if (uid==City->get_q()[i]->get_uid())
				t=true;
		}
		if (not t)
		{
			return uid;
		}
		++uid;
	}
	return uid;
}


bool MyEvent::on_button_release_event(GdkEventButton * event)
{
	if(event->type == GDK_BUTTON_RELEASE)
	{
		// raw mouse coordinates in the window frame
		double clic_x2 = event->x ;// - _area.get_allocation().get_x();
		double clic_y2 = event->y ;//- _area.get_allocation().get_y();
		cout << "mouse x = " << clic_x2 << "\t mouse y = " << clic_y2 << endl;

		// origin of the drawing area
		double origin_x = m_Area.get_allocation().get_x();
		double origin_y = m_Area.get_allocation().get_y();
		cout << "m_Area x = " << origin_x << "\t m_Area y = " << origin_y << endl;

		// get width and height of drawing area
		double width = m_Area.get_allocation().get_width();
		double height= m_Area.get_allocation().get_height();
		cout << "width  = " << width << "\t height = " << height << endl;

		// retain only mouse events located within the drawing area
		if(clic_x2 >= origin_x && clic_x2 <= origin_x + width &&
		   clic_y2 >= origin_y && clic_y2 <= origin_y + height)
		{
			double clic_x2f, clic_y2f;
			clic_x2f=clic_x2-(origin_x)-450;
			clic_y2f= -(clic_y2-(origin_y)-450);
			if(event->button == 1 and not clic_into_a_node(city::getvilleinstance(),
				pinit.get_cxi()*(unframe/zoom.get_zoom()), pinit.get_cyi()*
				(unframe/zoom.get_zoom())))
			{
				if (clic_x2f!=pinit.get_cxi() && clic_y2f!=pinit.get_cyi())
				{
					change_rayon(clic_x2f, clic_y2f, city::getvilleinstance(), pinit,
									origin_x, origin_y);
				}
				else
				{
					unselect_all(city::getvilleinstance());
				}
				Refresh();
			}
		}
	}
	return true;
}


void MyEvent::change_rayon(double clic_x2, double clic_y2, city* City, Clic_and_release
							pinit, double origin_x, double origin_y)
{
	Point depart({pinit.get_cxi()*((2000./900.)/zoom.get_zoom()), pinit.get_cyi()
					*((2000./900.)/zoom.get_zoom())});
	Point arrivee({clic_x2*((2000./900.)/zoom.get_zoom()), clic_y2*((2000./900.)
					/zoom.get_zoom())});

	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		if (City->get_q()[i]->get_selected())
		{
			Point centre ({City->get_q()[i]->get_coorx(),City->get_q()[i]->
				get_coory()});
			quartier temp7(City->get_q()[i]->get_uid(), City->get_q()[i]->get_coorx(),
				City->get_q()[i]->get_coory(), pow(City->get_q()[i]->get_rayon()+(distance
					(centre, arrivee)-distance(centre, depart)),2), City->get_q()[i]->get_type());
			if (!(node_superposition(City->get_q(), temp7)))
			{
				City->get_q()[i]->set_cap(pow(City->get_q()[i]->get_rayon()+
						(distance(centre, arrivee)-distance(centre, depart)),2));
				City->get_q()[i]->set_rayon(abs(City->get_q()[i]->get_rayon()+
						(distance(centre, arrivee)-distance(centre, depart))));
			}
		}
	}
}


/*
void delete_q(city* City, unsigned int i)
{
	unsigned int zero(0), unite(1);
	for (unsigned int j(0); j<Lt.size(); ++j)
	{
		cout << "1" << endl;
		if (Lt[j][zero]==tab[i]->get_uid() or Lt[j][unite]==tab[i]->get_uid())
		{
			cout << "11" << endl;
			for (unsigned int k(j); k<Lt.size()-unite; ++k)
			{ base 0Lt
				cout << "link base 1" << Lt[j][unite] << endl;
				cout << "111" << endl;
				//City->get_lt().erase(City->get_lt().begin()+j); //efface les liens
				Lt[k][zero]=City->get_lt()[k+unite][zero];
				Lt[k][unite]=City->get_lt()[k+unite][unite];
				cout << "112" << endl;
			}
			cout << "12" << endl;
			Lt.pop_back();->jj
		}
m_Area
	cout << "2" << endl;
	//tab.erase(City->get_q().begin()+i); //efface le noeud
	for (unsigned int k(i); k<tab.size()-unite; ++k)
		tab[k]=tab[k+unite];
	tab.pop_back();
	if (i<City->get_nh())
		City->set_nh(City->get_nh()-unite);
	else if (i<(City->get_nh()+City->get_nt()))
		City->set_nt(City->get_nt()-unite);
	else if (i<(City->get_nh()+City->get_nt()+City->get_np()))
		City->set_np(City->get_np()-unite);
}
}
*/
/*
unsigned int checkRadioButtons()
{
unsigned int selectedRadioButton(0);
if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(RadioButton1))==TRUE) selectedRadioButton =1;
if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(RadioButton2))==TRUE) selectedRadioButton =2;
if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(RadioButton3))==TRUE) selectedRadioButton =3;
return selectedRadioButton
}
*/

void MyEvent::delete_q(city* City, unsigned int i)
{
	vector<vector<unsigned int*>> temp2;
	for (unsigned int j(0); j<City->get_lt().size(); ++j)
	{
		if ((*City->get_lt()[j][0]!=City->get_q()[i]->get_uid())&& (*City->get_lt()[j][1]
			!=City->get_q()[i]->get_uid()))
		{
			vector<unsigned int*> temp3;
			temp3.push_back(City->get_lt()[j][0]);
			temp3.push_back(City->get_lt()[j][1]);
			temp2.push_back(temp3);
		}
	}
	City->set_L(temp2);
	vector<quartier*> temp;
	for (unsigned int j(0); j<City->get_q().size(); ++j)
	{
		if (j!=i)
		{
			temp.push_back(City->get_q()[j]);
		}
	}
	switch(City->get_q()[i]->get_type())
	{
		case 1:
			City->set_nh(City->get_nh()-1);
			break;
		case 2:
		  City->set_nt(City->get_nt()-1);
			break;
		case 3:
			City->set_np(City->get_np()-1);
			break;
	}
	City->set_D(temp);
}

bool MyEvent::clic_into_a_node(city* City, double coor_x, double coor_y)
{
	Point p({coor_x, coor_y});
	bool t1(false);
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
		double dist;
		dist=distance(City->get_q()[i]->get_centre(), p);
		if (dist <= City->get_q()[i]->get_rayon())
		t1=true;
	}
return t1;
}

/*
void check_erreurs(city* City)
{
	for (unsigned int i(0); i<City->get_q().size(); ++i)
	{
	//	node_superposition(City->get_q(), City->get_q()[i]);
	//	max_link(City->get_lt(), City->get_q()[i]->get_uid(), City->get_q(), City->get_nh());
	}
	for (unsigned int i(0); i<City->get_lt().size(); ++i)
	{
		nlink_superposition(City->get_lt()[i][0], City->get_lt()[i][1], City->get_q());
	}
}*/

bool MyEvent::on_key_press_event(GdkEventKey * key_event)
{
	if(key_event->type == GDK_KEY_PRESS)
	{
		switch(gdk_keyval_to_unicode(key_event->keyval))
		{
			case 'r':
				MyEvent::on_button_clicked_zoom_reset();
				break;
			case 'i':
				MyEvent::on_button_clicked_zoom_in();
				break;
			case 'o':
				MyEvent::on_button_clicked_zoom_out();
				break;
		}
	}

	return Gtk::Window::on_key_press_event(key_event);
}
