#include"custom.h"
using namespace std;

z::Frame::Frame(string title)
{
	set_label(title);
	set_label_align(0.15, 0.5);
	set_shadow_type(Gtk::SHADOW_ETCHED_IN);
	frame_inner_box_.set_margin_top(20);
	frame_inner_box_.set_margin_bottom(20);
	frame_inner_box_.set_margin_left(20);
	frame_inner_box_.set_margin_right(20);
	frame_inner_box_.set_spacing(10);
	add(frame_inner_box_);
}

Gtk::VBox &z::Frame::inner_box()
{
	return frame_inner_box_;
}

z::Box::Box()
{
	set_spacing(20);
	set_margin_top(20);
	set_margin_bottom(20);
	set_margin_left(20);
	set_margin_right(20);
}
/*
Glib::RefPtr<Gdk::Pixbuf> z::matrix2pixbuf(cv::Mat &mat, bool convert)
{//changes matrix data format
	if(convert) cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
	auto size = mat.size();
	return Gdk::Pixbuf::create_from_data(mat.data, Gdk::COLORSPACE_RGB, 
			mat.channels() == 4, 8, size.width, size.height, (int)mat.step);
}
*/
