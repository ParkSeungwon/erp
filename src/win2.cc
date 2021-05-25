#include"win2.h"
using namespace std;

Window2::Window2()
{
	add(hb_[0]);
	for(int i=0; i<5; i++) {
		hb_[0].pack_start(vb_[i]);
		if(i < 3) hb_[0].pack_start(separator_[i]);
	}

	vb_[0].pack_start(patient_);
	vb_[0].pack_start(hb_[6], Gtk::PACK_SHRINK);
	hb_[6].pack_start(entry_[0]);//name
	hb_[6].pack_start(man_, Gtk::PACK_SHRINK);
	hb_[6].pack_start(sex_, Gtk::PACK_SHRINK);
	hb_[6].pack_start(woman_, Gtk::PACK_SHRINK);
	vb_[0].pack_start(birth_, Gtk::PACK_SHRINK);
	vb_[0].pack_start(entry_[1]);//tel
	vb_[0].pack_start(entry_[2]);//address
	vb_[0].pack_start(hb_[1], Gtk::PACK_SHRINK);
	hb_[1].pack_start(add_);
	hb_[1].pack_start(del_);

	vb_[1].pack_start(date_);
	vb_[1].pack_start(add2_, Gtk::PACK_SHRINK);

	vb_[2].pack_start(symptom_, Gtk::PACK_SHRINK);
	vb_[2].pack_start(base_, Gtk::PACK_SHRINK);
	vb_[2].pack_start(prescription_);
	vb_[2].pack_start(hb_[2], Gtk::PACK_SHRINK);
	vb_[2].pack_start(hb_[8], Gtk::PACK_SHRINK);
	vb_[2].pack_start(hb_[3], Gtk::PACK_SHRINK);
	vb_[2].pack_start(hb_[7], Gtk::PACK_SHRINK);
	vb_[2].pack_start(print_, Gtk::PACK_SHRINK);
	hb_[2].pack_start(process_);
	hb_[2].pack_start(spin_[0]);
	hb_[3].pack_start(update_);
	hb_[3].pack_start(save_);
	hb_[7].pack_start(korean_);
	hb_[7].pack_start(multiply_);
	hb_[8].pack_start(spin_[1]);
	hb_[8].pack_end(days_, Gtk::PACK_SHRINK);

	vb_[3].pack_start(hb_[4]);
	vb_[3].pack_start(right_, Gtk::PACK_SHRINK);
	vb_[3].pack_start(left_, Gtk::PACK_SHRINK);
	vb_[3].pack_start(hb_[5]);

	vb_[4].pack_start(scroll_[0]);
	scroll_[0].add(herb_);

	sq_.connect("localhost", "zeta", "cockcodk0", "herb");
	set_properties();
	load_herb_table();
	show_all_children();
}

void Window2::set_properties()
{
	const char *p[] = {"이름", "전화번호", "주소"};
	for(int i=0; i<3; i++) entry_[i].set_placeholder_text(p[i]);
	process_.set_placeholder_text("수치, 법제");
	patient_.set_size_request(-1, 700);
	for(int i=0; i<2; i++) separator_[i].set_orientation(Gtk::ORIENTATION_VERTICAL);
	for(int i=0; i<3; i++) scroll_[i].set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	
	symptom_.set_size_request(-1, 100);
	spin_[0].set_increments(0.25, 1);
	spin_[0].set_range(0, 99);
	spin_[0].set_digits(2);
	spin_[1].set_increments(1, 10);
	spin_[1].set_range(0, 99);
	herb_.search_column(2);
}

void Window2::load_herb_table()
{
	sq_.reconnect();//when timeout connection lost
	sq_.select("herb");
	herb_.clear();
	for(auto row : sq_) herb_.push_back(row["id"].asInt(), row["korean"].asString(),
			row["chinese"].asString(), row["taste"].asString(), row["effect"].asString());
	selected_[0] = -1;
}

