#ifndef HE_UTILITY_HELP_HPP
#define HE_UTILITY_HELP_HPP

#include "heutility.hpp"

class MainWindow;

class HelpWindow : public Gtk::Window {
    public:
        HelpWindow(MainWindow &win);
        virtual ~HelpWindow();

        void display_text(Glib::RefPtr<Gtk::TextBuffer> &refBuffer,std::string text);
    private:
        Gtk::Grid grid_main;
        Gtk::Frame frame_contents;
        Gtk::ScrolledWindow sw_contents;
        Gtk::TextView tv_contents;
        Gtk::HButtonBox hbb_main;
        Gtk::Button btn_close;

        MainWindow *mw;

        void on_window_show();
        void on_btn_close_clicked();
};

#endif
