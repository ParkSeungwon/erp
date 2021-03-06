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
		add2_{"추가"}, del2_{"삭제"}, save_{"저장"}, print_{"프린트"};
	Gtk::Separator separator_[2];//{Gtk::ORIENTATION_VERTICAL};
	Gtk::HBox hb_[10];
	Gtk::TextView symptom_, popup_print_;
	Gtk::ComboBox base_;
	Gtk::Switch sex_;
	Gtk::Calendar birth_;
	Gtk::Label man_{"남"}, woman_{"여"}, days_{"일분"}, cm_{"cm"}, kg_{"kg"};
	Gtk::VBox vb_[5];
	Gtk::Entry entry_[3];
	Gtk::SpinButton weight_, height_, dose_;
	z::TreeView<int, string, int, string> patient_{"id", "이름", "생년월일", "TEL"};//710729
	z::TreeView<int, int> date_{"id", "날짜"};//202105221305
	z::EditableTreeView<12, 8, int, string, string, float> prescription_{"id", "약재", "법제", "용량", "%.2f"};
	z::TreeView<int, string, string, string, string> herb_{"id", "약재", "한자", "성미", "효능"};
	Gtk::CheckButton korean_{"한글"}, multiply_{"약재 분량 곱하기"};
	SqlQuery sq_;

	struct ModelColumns : Gtk::TreeModel::ColumnRecord {
		ModelColumns() { add(id_); add(name_); }
		Gtk::TreeModelColumn<int> id_;
		Gtk::TreeModelColumn<Glib::ustring> name_;
	} column;

private:
	int selected_[3] = {-1, -1, -1};//patient, date, herb
	void set_properties(), load_herb_table(), connect_event(), load_patient_table()
		, load_date_table(), load_base_formular(), load_prescription_table(),
		load_prescription_other(), organize_widgets(), clear_prescription_other();
	std::vector<int> base_formular_index_;
	std::tuple<int, string, int, string> selected_patient_;
	std::tuple<int, string, string, string, string> selected_herb_;
	std::tuple<int, int> selected_date_;
};



