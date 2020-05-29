// Architecture b2
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include "gui.h"
#include "graphic.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
//export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

int main(int argc, char *argv[])
{
	if(argc != 2) erreur(LECTURE_ARG);
	lecture(argv[1]);

	int Fakeargc(1);
	auto app_gui = Gtk::Application::create(Fakeargc, argv, "org.gtkmm.example");
	MyEvent gui;


	return app_gui->run(gui);
}
