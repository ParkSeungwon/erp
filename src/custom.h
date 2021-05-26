#pragma once
#include<sstream>
#include<cassert>
#include<gtkmm.h>
#include<iostream>
#include<type_traits>

namespace z {

class Frame : public Gtk::Frame
{
public:
	Frame(std::string title);
	Gtk::VBox &inner_box();
protected:
	Gtk::VBox frame_inner_box_;
};

template<int Col = 1> class RadioFrame : public Frame
{
public:
	template<class ... Args> RadioFrame(std::string title, Args... labels) 
		: Frame{title}
	{
		if(Col != 1) inner_box().set_orientation(Gtk::ORIENTATION_HORIZONTAL);
		for(auto &a : vbox_) {
			inner_box().pack_start(a);
			a.set_spacing(10);
		}
		const char *l[sizeof...(Args)] = { labels... };
		for(int i=0; i<sizeof...(Args); i++) {
			auto *p = Gtk::make_managed<Gtk::RadioButton>(l[i]);
			p->set_group(grp_);
			vbox_[i % Col].pack_start(*p);
		}
	}
	Gtk::RadioButtonGroup grp_;
	Gtk::VBox vbox_[Col];
};

class Box : public Gtk::VBox
{
public:
	Box();
};

//Glib::RefPtr<Gdk::Pixbuf> matrix2pixbuf(cv::Mat &mat, bool convertRGB = true);

template<class... Args> class TreeViewBase : public Gtk::TreeView
{
public:
	void push_back(Args... args) {
		static int k = 0;
		auto row = *ref_tree_model_->append();
		row[column_.id_] = k++;
		insert_row<0>(row, args...);
	}
	std::vector<int> get_selected() {
		std::vector<int> v;
		if(multiple_) {
			for(auto path : ref_sel_->get_selected_rows()) {
				std::stringstream ss; ss << path.to_string();
				int k; ss >> k;
				v.push_back(k);
			}
		} else if(auto it = ref_sel_->get_selected(); it) {
			auto row = *it;
			v.push_back(row[column_.id_]);
		}
		return v;
	}
	void remove_selected_row() {
		if(auto it = ref_sel_->get_selected(); it) ref_tree_model_->erase(it);
	}
	std::tuple<Args...> get_row(int id) {
		std::tuple<Args...> r;
		for(auto row : ref_tree_model_->children()) 
			if(row[column_.id_] == id) row_recur<0>(r, row);
		return r;
	}
	template<int N> auto get_nth_column() {
		if constexpr(N < 0) {//return vector of hidden id column
			std::vector<int> v;
			for(auto row : ref_tree_model_->children()) v.push_back(row[column_.id_]);
			return v;
		} else {//return nth column
			using type = std::tuple_element<N, std::tuple<Args...>>::type;
			std::vector<type> v;
			for(auto row : ref_tree_model_->children()) v.push_back(row[get<N>(column_.cols_)]);
			return v;
		}
	}
	void multiple(bool tf = true) {
		ref_sel_->set_mode(tf ? Gtk::SELECTION_MULTIPLE : Gtk::SELECTION_SINGLE);
		multiple_ = tf;
	}
	void clear() {
		ref_tree_model_->clear();
	}
	template<class T> void add_event(T func) {
		get_selection()->signal_changed().connect(func);
		//signal_row_activated().connect(func);
	}
	void search_column(int n) {
		set_search_column(n);
		//set_enable_search();
	}
protected:
	struct ModelColumns : Gtk::TreeModel::ColumnRecord {
		ModelColumns() {
			add(id_);
			add_recur<0>();
		}
		std::tuple<Gtk::TreeModelColumn<Args>...> cols_;
		Gtk::TreeModelColumn<int> id_;
		template<int N> void add_recur() {
			add(get<N>(cols_));
			if constexpr(N+1 < sizeof...(Args)) add_recur<N+1>();
		}
	} column_;
	Glib::RefPtr<Gtk::ListStore> ref_tree_model_ = Gtk::ListStore::create(column_);
	Glib::RefPtr<Gtk::TreeSelection> ref_sel_ = get_selection();
	bool multiple_ = false;
private:
	template<int N, class T, class... As> void insert_row(Gtk::TreeModel::Row &row, T a, As... as) {
		row[get<N>(column_.cols_)] = a;
		if constexpr(N+1 < sizeof...(Args)) insert_row<N+1>(row, as...);
	}
	template<int N> void row_recur(std::tuple<Args...> &r, const auto &row) {
		std::get<N>(r) = row[std::get<N>(column_.cols_)];
		if constexpr(N+1 < sizeof...(Args)) row_recur<N+1>(r, row);
	}
};

template<class... Args> class TreeView : public TreeViewBase<Args...>
{
public:
	template<class... Strings> TreeView(Strings... col_name) 
		requires (sizeof...(Strings) == sizeof...(Args)) {
			this->set_model(this->ref_tree_model_);
			append_col<0>(col_name...);
	}

private:
	template<int N, class... Strings> void append_col(std::string s, Strings... args) {
		const auto &nth_column = get<N>(this->column_.cols_);
		this->append_column(s, nth_column);//editable contents
		this->get_column(N)->set_sort_column(nth_column);//sortable when click the header
		if constexpr(N+1 < sizeof...(Args)) append_col<N+1>(args...);
	}
};

template<int E, int Num, class... Args> class EditableTreeView : public TreeViewBase<Args...>
{//E : byte operation -> editable column, Num : byte operation -> numeric column 
public:
	template<class... Strings> EditableTreeView(Strings... col_name)
		requires (sizeof...(Strings) >= sizeof...(Args)) {
			this->set_model(this->ref_tree_model_);
			append_col<0>(col_name...);
	}
private:
	template<int N, class... Strings> void append_col(std::string s, std::string t, Strings... args) {
		const auto &nth_column = get<N>(this->column_.cols_);
		if constexpr((1 << N) & Num) {
			if constexpr((1 << N) & E) this->append_column_numeric_editable(s, nth_column, t);
			else this->append_column_numeric(s, nth_column, t);
			this->get_column(N)->set_sort_column(nth_column);//sortable when click the header
			if constexpr(N+1 < sizeof...(Args)) append_col<N+1>(args...);
		} else {
			if constexpr((1 << N) & E) this->append_column_editable(s, nth_column);
			else this->append_column(s, nth_column);
			this->get_column(N)->set_sort_column(nth_column);//sortable when click the header
			append_col<N+1>(t, args...);
		}
	}
	template<int N> void append_col(std::string s) {
		const auto &nth_column = get<N>(this->column_.cols_);
		if constexpr((1 << N) & E) this->append_column_editable(s, nth_column);
		else this->append_column(s, nth_column);
		this->get_column(N)->set_sort_column(nth_column);//sortable when click the header
	}
};

}
