#include"win.h"
using namespace std;

int main(int ac, char **av)
{
	auto app = Gtk::Application::create(ac, av, "");
	Window win;
	return app->run(win);
}

