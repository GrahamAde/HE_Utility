#ifndef HE_UTILITY_ABOUT_HPP
#define HE_UTILITY_ABOUT_HPP

#include "heutility.hpp"

class MainWindow;

class AboutWindow : public Gtk::Window {
    public:
        AboutWindow(MainWindow &win);
        virtual ~AboutWindow();
    private:
        Gtk::Grid grid_main;
        Gtk::VBox vbox_main;
        Gtk::Frame frame_program_name;
        Gtk::Label lbl_program_description,lbl_program_copyright_author,lbl_program_license;
        Gtk::HButtonBox hbb_main;
        Gtk::Button btn_close;

        MainWindow *mw;

        void on_btn_close_clicked();
};

#endif
