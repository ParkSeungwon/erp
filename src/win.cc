#include<gtkmm.h>
#include"win.h"
using namespace std;

Window::Window() {
	add(hb_);
	for(int i=0; i<4; i++) hb_.pack_start(vb_[i]);
	vb_[0].pack_start(scroll_[0]);
	scroll_[0].add(herb_);
	scroll_[0].set_size_request(200, 700);
	vb_[0].pack_start(entry_box_, Gtk::PACK_SHRINK);
	for(int i=0; i<3; i++) entry_box_.pack_start(entry_[i]);
	vb_[0].pack_start(entry_[3], Gtk::PACK_SHRINK);
	vb_[0].pack_start(button_box_, Gtk::PACK_SHRINK);
	button_box_.pack_start(add_);
	button_box_.pack_start(del_);

	sq_.connect("localhost", "zeta", "cockcodk0", "herb");
	load_herb_table();
	set_size_request(500,200);
	show_all_children();
	connect_event();
}

void Window::load_herb_table()
{
	sq_.reconnect();//when timeout connection lost
	sq_.select("herb");
	herb_.clear();
	for(auto row : sq_) herb_.push_back(row["id"].asInt(), row["korean"].asString(),
			row["chinese"].asString(), row["taste"].asString(), row["effect"].asString());
}

void Window::connect_event()
{
	herb_.add_event([&]() {
			auto v = herb_.get_selected();
			if(!v.empty()) selected_ = v[0];
			else selected_ = -1;
	});
	add_.signal_clicked().connect([&]() {
		string s[4];
		for(int i=0; i<4; i++) s[i] = entry_[i].get_text();
		sq_.select("herb", "limit 1");
		sq_.insert(AutoIncrement{}, s[0], s[1], s[2], s[3]);
		load_herb_table();
	});
	del_.signal_clicked().connect([&]() {
		if(selected_ == -1) return;
		auto tup = herb_.get_row(selected_);
		sq_.reconnect();
		sq_.query("delete from herb where id = " + to_string(get<0>(tup)) + ';');
		load_herb_table();
	});
}
