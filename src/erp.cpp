#include<gtkmm.h>
#include<string>
#include"custom.h"
#include"win.h"
using namespace std;
//
//class Window : public Gtk::Window
//{
//public:
//	Window() {
//		add(scroll_);
//		scroll_.add(tree_);
//		herb_.push_back(1, "감초", "甘草", "감", "효");
//		herb_.push_back(2, "나초만자라노쿠키", "甘草", "감", "효");
//		herb_.push_back(3, "갑초", "草", "sk", "효");
//		set_size_request(500,200);
//		show_all_children();
//	}
//protected:
//	Gtk::ScrolledWindow scroll_;
//	z::TreeView<int, string, string, string, string> herb_{"index", "한글", "한자", "성미", "효능"};
//	z::TreeView<int, string, string, string, string> prescription_{"index", "처방", 
//};
//
int main(int ac, char **av)
{
	auto app = Gtk::Application::create(ac, av, "");
	Window win;
	return app->run(win);
}

