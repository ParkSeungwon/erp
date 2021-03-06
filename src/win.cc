#include"win.h"
using namespace std;

Window::Window() {
	set_title("한방 처방집");
	add(hb_);
	for(int i=0; i<3; i++) hb_.pack_start(vb_[i]);
	hb_.pack_start(separator_);
	hb_.pack_start(vb_[3]);
	vb_[0].pack_start(scroll_[0]);
	scroll_[0].add(herb_);
	vb_[0].pack_start(entry_box_[0], Gtk::PACK_SHRINK);
	vb_[0].pack_start(entry_box_[1], Gtk::PACK_SHRINK);
	for(int i=0; i<2; i++) entry_box_[0].pack_start(entry_[i]);
	for(int i=2; i<4; i++) entry_box_[1].pack_start(entry_[i]);
	vb_[0].pack_start(button_box_, Gtk::PACK_SHRINK);
	button_box_.pack_start(add_);
	button_box_.pack_start(del_);

	vb_[1].pack_start(blank_[0]);
	vb_[1].pack_end(blank_[1]);
	vb_[1].pack_start(right_, Gtk::PACK_SHRINK);
	vb_[1].pack_start(left_, Gtk::PACK_SHRINK);

	vb_[2].pack_start(scroll_[1]);
	scroll_[1].add(recipe_);
	vb_[2].pack_start(update_, Gtk::PACK_SHRINK);

	vb_[3].pack_start(scroll_[2]);
	scroll_[2].add(formular_);
	vb_[3].pack_start(entry_box_[2], Gtk::PACK_SHRINK);
	entry_box_[2].pack_start(entry_[4], Gtk::PACK_SHRINK);
	entry_box_[2].pack_start(entry_[5], Gtk::PACK_SHRINK);
	vb_[3].pack_start(entry_[6], Gtk::PACK_SHRINK);
	vb_[3].pack_start(button_box2_, Gtk::PACK_SHRINK);
	button_box2_.pack_start(add2_);
	button_box2_.pack_start(del2_);

	//formular_.push_back(1, "궁귀조혈음", "궁귀조혈음", "산후병");
	sq_.connect("localhost", "zeta", "cockcodk0", "herb");
	load_herb_table();
	load_formular_table();
	//set_size_request(500,200);
	set_properties();
	show_all_children();
	connect_event();
}

void Window::load_herb_table()
{
	cout << "load herb table" << endl;
	sq_.reconnect();//when timeout connection lost
	sq_.select("herb");
	herb_.clear();
	for(auto row : sq_) herb_.push_back(row["id"].asInt(), row["korean"].asString(),
			row["chinese"].asString(), row["taste"].asString(), row["effect"].asString());
	selected_[0] = -1;
}

void Window::load_formular_table()
{
	cout << "load formular " << endl;
	sq_.reconnect();
	sq_.select("formular");
	formular_.clear();
	for(auto row : sq_) formular_.push_back(row["id"].asInt(),
			row["korean"].asString(), row["chinese"].asString(), row["effect"].asString());
	selected_[2] = -1;
}

void Window::set_properties()
{
	const char *text[7] = {"한글", "한자", "성미", "효능", "처방", "한자", "효과"};
	for(int i=0; i<7; i++) entry_[i].set_placeholder_text(text[i]);
	for(int i=0; i<3; i++) scroll_[i].set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	scroll_[0].set_size_request(-1, 700);
	herb_.search_column(2);
	recipe_.search_column(2);
	formular_.search_column(2);
}

void Window::load_recipe_table(int n)
{
	cout << "load recipe table" << endl;
	if(do_not_update_recipe_table_) return;
	sq_.reconnect();
	string q = "select * from (select * from recipe where pres = ";
	sq_.query(q + to_string(n) + ") as tmp inner join herb on tmp.herb = herb.id;");
	sq_.fetch();
	recipe_.clear();
	for(auto row : sq_) recipe_.push_back(row["id"].asInt(), row["korean"].asString(),
			row["process"].asString(), row["weight"].asFloat());
	selected_[1] = -1;
}

void Window::connect_event()
{
	herb_.add_event([&]() {
			cout << "herb event" << endl;
			if(auto v = herb_.get_selected(); !v.empty()) {
				herb_selected_row_ = herb_.get_row(v[0]);
				selected_[0] = get<0>(herb_selected_row_);
				selected_id_[0] = v[0];
			} else {
				selected_[0] = -1;
				selected_id_[0] = -1;
			}
	});
	formular_.add_event([&]() {
			cout << "formular select event" << endl;
			if(auto v = formular_.get_selected(); !v.empty()) {
				formular_selected_row_ = formular_.get_row(v[0]);
				selected_[2] = get<0>(formular_selected_row_);
				selected_id_[2] = v[0];
				load_recipe_table(selected_[2]);
			} else {
				selected_[2] = -1;
				selected_id_[2] = -1;
			}
	});
	add_.signal_clicked().connect([&]() {
		string s[4];
		for(int i=0; i<4; i++) s[i] = entry_[i].get_text();
		sq_.reconnect();
		sq_.select("herb", "limit 1");
		sq_.insert(AutoIncrement{}, s[0], s[1], s[2], s[3]);
		load_herb_table();
	});
	add2_.signal_clicked().connect([&]() {
			string s[3];
			for(int i=0; i<3; i++) s[i] = entry_[4 + i].get_text();
			sq_.reconnect();
			sq_.select("formular", "limit 1");
			sq_.insert(AutoIncrement{}, s[0], s[1], s[2]);
			do_not_update_recipe_table_ = true;
			load_formular_table();
			do_not_update_recipe_table_ = false;
			recipe_.clear();
	});
	del_.signal_clicked().connect([&]() {
		if(selected_[0] == -1) return;
		sq_.reconnect();
		sq_.query("delete from herb where id = " + to_string(selected_[0]) + ';');
		load_herb_table();
	});
	del2_.signal_clicked().connect([&]() {
			if(selected_[2] == -1) return;
			sq_.reconnect();
			sq_.query("delete from formular where id = " + to_string(selected_[2]) + ';');
			//sq_.query("delete from recipe where pres = " + to_string(get<0>(tup)) + ';');
			do_not_update_recipe_table_ = true;
			load_formular_table();
			do_not_update_recipe_table_ = false;
			recipe_.clear();
	});
	right_.signal_clicked().connect([&]() {
			if(selected_[0] == -1 || selected_[2] == -1) return;
			auto v = recipe_.get_nth_column<0>();
			if(std::find(v.begin(), v.end(), selected_[0]) == v.end()) 
				recipe_.push_back(selected_[0], get<1>(herb_selected_row_), "", 4);
	});
	left_.signal_clicked().connect([&]() {
			recipe_.remove_selected_row();
	});
	update_.signal_clicked().connect([&]() {
			if(selected_[2] == -1) return;
			sq_.reconnect();
			sq_.query("delete from recipe where pres = " + to_string(selected_[2]) + ';');
			sq_.select("recipe", "limit 1");
			for(int n : recipe_.get_nth_column<-1>()) {
				auto tup = recipe_.get_row(n);
				sq_.insert(selected_[2], get<0>(tup), get<2>(tup), get<3>(tup));
			}
	});
}
