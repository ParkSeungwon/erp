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
	Gtk::Button add_{"추가"}, del_{"삭제"}, right_{"->"}, left_{"<-"},
		add2_{"추가"}, del2_{"삭제"}, update_{"수정"};
	Gtk::Separator separator_{Gtk::ORIENTATION_VERTICAL};
	Gtk::HBox hb_, entry_box_, entry_box2_, button_box_, blank_[2], button_box2_;
	Gtk::VBox vb_[4];
	Gtk::Entry entry_[7], process_, entry2_[3];
	Gtk::SpinButton dose_;
	z::TreeView<int, string, string, string, string> herb_{"id", "약재", "한자", "性味", "효능"};
	z::TreeView<int, string, string, float> recipe_{"id", "약재", "법제", "용량"};
	z::TreeView<int, string, string, string> formular_{"id", "처방", "한자", "효과"};
	SqlQuery sq_;

private:
	int selected_[3] = {-1, -1, -1};//herb, recipe, formular
	void load_herb_table(), set_properties(), connect_event(), load_formular_table(),
			 load_recipe_table();
};


