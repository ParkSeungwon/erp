#include"win2.h"
using namespace std;

Window2::Window2()
{
	set_title("처방전");
	add(hb_[0]);
	for(int i=0; i<5; i++) {
		hb_[0].pack_start(vb_[i]);
		if(i < 3) hb_[0].pack_start(separator_[i]);
	}

	vb_[0].pack_start(scroll_[1]);
	scroll_[1].add(patient_);
	vb_[0].pack_start(hb_[6], Gtk::PACK_SHRINK);
	hb_[6].pack_start(entry_[0]);//name
	hb_[6].pack_start(man_, Gtk::PACK_SHRINK);
	hb_[6].pack_start(sex_, Gtk::PACK_SHRINK);
	hb_[6].pack_start(woman_, Gtk::PACK_SHRINK);
	vb_[0].pack_start(birth_, Gtk::PACK_SHRINK);
	vb_[0].pack_start(entry_[1]);//tel
	vb_[0].pack_start(entry_[2]);//address
	vb_[0].pack_start(hb_[9], Gtk::PACK_SHRINK);
	hb_[9].pack_start(height_);
	hb_[9].pack_start(cm_, Gtk::PACK_SHRINK);
	hb_[9].pack_start(weight_);
	hb_[9].pack_start(kg_, Gtk::PACK_SHRINK);
	vb_[0].pack_start(hb_[1], Gtk::PACK_SHRINK);
	hb_[1].pack_start(add_);
	hb_[1].pack_start(del_);

	vb_[1].pack_start(date_);
	vb_[1].pack_start(hb_[2], Gtk::PACK_SHRINK);
	hb_[2].pack_start(add2_, Gtk::PACK_SHRINK);
	hb_[2].pack_start(del2_, Gtk::PACK_SHRINK);

	vb_[2].pack_start(symptom_, Gtk::PACK_SHRINK);
	vb_[2].pack_start(base_, Gtk::PACK_SHRINK);
	vb_[2].pack_start(prescription_);
	//vb_[2].pack_start(hb_[2], Gtk::PACK_SHRINK);
	vb_[2].pack_start(hb_[8], Gtk::PACK_SHRINK);
	vb_[2].pack_start(hb_[7], Gtk::PACK_SHRINK);
	vb_[2].pack_start(hb_[3], Gtk::PACK_SHRINK);
	hb_[3].pack_start(email_);
	hb_[3].pack_start(print_);
	hb_[7].pack_start(korean_);
	hb_[7].pack_start(multiply_);
	hb_[8].pack_start(dose_);
	hb_[8].pack_start(days_, Gtk::PACK_SHRINK);
	hb_[8].pack_start(save_, Gtk::PACK_SHRINK);

	vb_[3].pack_start(hb_[4]);
	vb_[3].pack_start(right_, Gtk::PACK_SHRINK);
	vb_[3].pack_start(left_, Gtk::PACK_SHRINK);
	vb_[3].pack_start(hb_[5]);

	vb_[4].pack_start(scroll_[0]);
	scroll_[0].add(herb_);

	sq_.connect("localhost", "zeta", "cockcodk0", "herb");
	set_properties();
	load_herb_table();
	load_patient_table();
	load_base_formular();
	show_all_children();
	connect_event();
}

void Window2::set_properties()
{
	const char *p[] = {"이름", "전화번호", "주소"};
	for(int i=0; i<3; i++) entry_[i].set_placeholder_text(p[i]);
	patient_.set_size_request(-1, 700);
	for(int i=0; i<2; i++) separator_[i].set_orientation(Gtk::ORIENTATION_VERTICAL);
	for(int i=0; i<3; i++) scroll_[i].set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	
	scroll_[1].set_size_request(-1, 700);
	symptom_.set_size_request(-1, 100);
	weight_.set_increments(1, 10);
	height_.set_increments(1, 10);
	weight_.set_range(0, 200);
	height_.set_range(0, 300);
	dose_.set_increments(1, 10);
	dose_.set_range(0, 99);
	herb_.search_column(2);
	patient_.search_column(2);
	base_.set_id_column(0);
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

void Window2::connect_event()
{
	add_.signal_clicked().connect([&]() {
		string s[3];
		for(int i=0; i<3; i++) s[i] = entry_[i].get_text();
		guint y, m, d;
		birth_.get_date(y, m, d);
		int birthday = y * 10000 + (m+1) * 100 + d;
		sq_.select("patient", "limit 1");
		sq_.insert(AutoIncrement{}, s[0], birthday, sex_.get_active() ? 1 : 0,//female 1
				s[2], s[1], weight_.get_value(), height_.get_value());
	});
	add2_.signal_clicked().connect([&]() {
		sq_.select("prescription", "limit 1");
		//sq_.insert();
	});
	patient_.add_event([&]() {
			auto v = patient_.get_selected();
			if(v.empty()) {
				selected_[0] = -1;
				return;
			}
			auto tup = patient_.get_row(v[0]);
			int id = get<0>(tup);
			selected_[0] = id;
			sq_.reconnect();
			sq_.select("patient", "where id = " + to_string(id));
			entry_[0].set_text(sq_[0]["name"].asString());
			entry_[1].set_text(sq_[0]["tel"].asString());
			entry_[2].set_text(sq_[0]["address"].asString());
			sex_.set_active(sq_[0]["sex"].asInt());
			int date = sq_[0]["birth"].asInt();
			int year = date / 10000;
			int month = (date % 10000) / 100 - 1;
			int day = date % 100;
			birth_.select_month(month, date / 10000);
			birth_.select_day(day);
			weight_.set_value(sq_[0]["weight"].asInt());
			height_.set_value(sq_[0]["height"].asInt());

			sq_.select("prescription", "where patient = " + to_string(id) + " order by date");
			for(auto row : sq_) date_.push_back(sq_[0]["id"].asInt(), sq_[0]["date"].asInt());
	});
	date_.add_event([&]() {
			auto v = date_.get_selected();
			if(v.empty()) {
				selected_[1] = -1;
				return;
			}
			auto tup = date_.get_row(v[0]);
			int id = get<0>(tup);
			selected_[1] = id;
			sq_.select("prescription", "where id = " + to_string(id));
			symptom_.get_buffer()->set_text(sq_[0]["symptom"].asString());
			dose_.set_value(sq_[0]["dose"].asInt());
			auto it = std::find(base_formular_index_.begin(), base_formular_index_.end(), 
					sq_[0]["base_formula"].asInt());
			base_.set_active(it - base_formular_index_.begin());
	});
	base_.signal_changed().connect([&]() {
			if(auto it = base_.get_active(); it) {
				auto row = *it;
				int id = row[column.id_];
				sq_.reconnect();
				string q = "select * from (select * from recipe where pres = ";
				sq_.query(q + to_string(id) + ") as tmp inner join herb on tmp.herb = herb.id;");
				sq_.fetch();
				prescription_.clear();
				for(auto row : sq_) prescription_.push_back(row["id"].asInt(), row["korean"].asString(),
						row["process"].asString(), row["weight"].asFloat());
			}
	});

}

void Window2::load_date_table() 
{
	if(selected_[0] == -1) return;
	sq_.select("prescription", "where patient = "  + to_string(selected_[0]));
	date_.clear();
	for(auto row : sq_) date_.push_back(sq_[0]["id"].asInt(), sq_[0]["date"].asInt());
}

void Window2::load_base_formular()
{
	sq_.reconnect();
	sq_.select("formular", "order by korean");

	auto ref_tree_model = Gtk::ListStore::create(column);
	base_.set_model(ref_tree_model);
	for(auto r : sq_) {
		auto iter = ref_tree_model->append();
		auto row = *iter;
		row[column.id_] = r["id"].asInt();
		row[column.name_] = r["korean"].asString();
		base_formular_index_.push_back(row[column.id_]);
	}
	base_.pack_start(column.name_);
	base_.set_active(0);
}

void Window2::load_patient_table()
{
	sq_.reconnect();
	sq_.select("patient");
	patient_.clear();
	for(auto row : sq_) {
		cout << "date" << row["date"].asString() << endl;
		patient_.push_back(row["id"].asInt(), row["name"].asString(),
			row["birth"].asInt(), row["tel"].asString());
	}
}
