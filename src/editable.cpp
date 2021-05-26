#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();

protected:
  //Signal handlers:

  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); add(m_col_foo); add(m_col_number); }

    Gtk::TreeModelColumn<unsigned int> m_col_id;
    Gtk::TreeModelColumn<std::string> m_col_name;
    Gtk::TreeModelColumn<bool> m_col_foo;
    Gtk::TreeModelColumn<float> m_col_number;
  };

  ModelColumns m_Columns;

  //Child widgets:
  Gtk::Box m_VBox;

  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

  //For the validated column:
  //You could also use a CellRendererSpin or a CellRendererProgress:
};

#endif //GTKMM_EXAMPLEWINDOW_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

ExampleWindow::ExampleWindow()
: m_VBox(Gtk::ORIENTATION_VERTICAL)
{
  set_title("Gtk::TreeView Editable Cells example");
  set_border_width(5);
  set_default_size(400, 200);

  add(m_VBox);

  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_TreeView);

  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_VBox.pack_start(m_ScrolledWindow);

  //Create the Tree model:
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);

  m_TreeView.append_column_editable("ID", m_Columns.m_col_id);
  m_TreeView.append_column_editable("Name", m_Columns.m_col_name);
  m_TreeView.append_column_editable("foo", m_Columns.m_col_foo);
  m_TreeView.append_column_numeric_editable("foo", m_Columns.m_col_number, "%.2f");

  //Fill the TreeView's model
  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 1;
  row[m_Columns.m_col_name] = "Billy Bob";
  row[m_Columns.m_col_foo] = true;
  row[m_Columns.m_col_number] = 10;

  show_all_children();
}

#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  ExampleWindow window;

  //Shows the window and returns when it is closed.
  return app->run(window);
}
