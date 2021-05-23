#include<gtkmm.h>
#include"mysqldata.h"
#include"custom.h"
using namespace std;


class Window : public Gtk::Window
{
public:
	Window();

protected:
	Gtk::ScrolledWindow scroll_[3];
	z::TreeView<int, string, string, string, string> herb_{"index", "한글", "한자", "性味", "효능"};
	Gtk::Button add_{"추가"}, del_{"삭제"};
	Gtk::HBox hb_, entry_box_, button_box_;
	Gtk::VBox vb_[4];
	Gtk::Entry entry_[4];
	z::TreeView<int, string, string, string, string> prescription_{"index", "처방", "", "", ""};
	SqlQuery sq_;

private:
	int selected_ = -1;
	void load_herb_table();
	void connect_event();
};


