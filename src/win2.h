#include<gtkmm.h>
#include"mysqldata.h"
#include"custom.h"
using std::string;

class Window2 : public Gtk::Window
{
public:
	Window2();

protected:
	Gtk::ScrolledWindow scroll_[3];
	Gtk::Button add_{"추가"}, del_{"삭제"}, right_{"->"}, left_{"<-"},
		add2_{"추가"}, del2_{"삭제"}, update_{"수정"}, save_{"저장"}, print_{"프린트"};
	Gtk::Separator separator_[2];//{Gtk::ORIENTATION_VERTICAL};
	Gtk::HBox hb_[9];
	Gtk::TextView symptom_;
	Gtk::ComboBox base_;
	Gtk::Switch sex_;
	Gtk::Calendar birth_;
	Gtk::Label man_{"남"}, woman_{"여"}, days_{"일분"};
	Gtk::VBox vb_[5];
	Gtk::Entry entry_[3], process_;
	Gtk::SpinButton spin_[2];
	z::TreeView<int, string, int, string> patient_{"id", "이름", "생년월일", "TEL"};//710729
	z::TreeView<int, int> date_{"id", "날짜"};//202105221305
	z::TreeView<int, string, string, float> prescription_{"id", "약재", "법제", "용량"};
	z::TreeView<int, string, string, string, string> herb_{"id", "약재", "한자", "성미", "효능"};
	Gtk::CheckButton korean_{"한글"}, multiply_{"약재 분량 곱하기"};
	SqlQuery sq_;

private:
	int selected_[3] = {-1, -1, -1};//herb, recipe, formular
	void set_properties(), load_herb_table();
};



