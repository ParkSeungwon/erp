#include"win2.h"
using namespace std;

int main(int ac, char **av)
{
	auto app = Gtk::Application::create(ac, av, "");
	Window2 win;
	return app->run(win);
}


