#ifndef HE_UTILITY_HPP
#define HE_UTILITY_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <atomic>
#include <gtkmm.h>
#include "boost/lexical_cast.hpp"
#include "openfhe.h"
#include "ciphertext-ser.h"
#include "scheme/bfvrns/bfvrns-ser.h"
#include "key/key-ser.h"
#include "cryptocontext-ser.h"
#include "about.hpp"
#include "help.hpp"

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        virtual ~MainWindow();

        void closeTopWindow(Gtk::Window &win, bool quit_program);
        void vectorize_int_string(std::string str);

        std::map<uint64_t, int64_t> map_orig_ints, map_result_ints;
        std::vector<int64_t> vect_orig_ints, vect_result_ints;
        std::vector<int64_t> vect_1,vect_3,vect_5;
        std::atomic<bool> bool_input_file;
        std::atomic<uint8_t> mult_ops;

        lbcrypto::CCParams<lbcrypto::CryptoContextBFVRNS> parameters;
        lbcrypto::CryptoContextBFVRNS::ContextType crypto_context;
        lbcrypto::KeyPair<lbcrypto::DCRTPoly> key_pair;
        lbcrypto::Plaintext pt_orig_ints,pt_vect_1,pt_vect_3,pt_vect_5,pt_result_ints;
        lbcrypto::Ciphertext<lbcrypto::DCRTPoly> ct_orig_ints,ct_result_ints;

        //Menu controls
        Glib::RefPtr<Gtk::UIManager> uimanager_menu;
        Glib::RefPtr<Gtk::ActionGroup> actiongroup_menu;

        void display_text(Glib::RefPtr<Gtk::TextBuffer> &refBuffer,std::string text);

    private:
        typedef std::vector<Gtk::Window*> vectTpWin;
        vectTpWin topWins;

        //Main controls
        Gtk::VBox vbox_main;
        Gtk::Grid grid_main,grid_operations,grid_options;
        Gtk::Frame frame_input_file,frame_original_ints,frame_resultant_ints,frame_original_ciphertext,frame_resultant_ciphertext,frame_operations,frame_options;
        Gtk::Button btn_encrypt,btn_decrypt,btn_add_1,btn_add_3,btn_add_5,btn_sub_1,btn_sub_3,btn_sub_5,btn_mult_3,btn_mult_5,btn_reset,btn_quit;
        Gtk::HButtonBox hbb_operations,hbb_controls;

        Gtk::FileChooserButton fcb_input;

        Gtk::ScrolledWindow sw_oint,sw_rint;
        Gtk::TreeView tv_oint,tv_rint;
        Glib::RefPtr<Gtk::ListStore> ls_oint,ls_rint;

        class OIntColumns : public Gtk::TreeModel::ColumnRecord{
            public:
                OIntColumns()
                {add(m_col_key); add(m_col_value);}
                Gtk::TreeModelColumn<Glib::ustring> m_col_key;
                Gtk::TreeModelColumn<Glib::ustring> m_col_value;
        };
        OIntColumns m_columns_oint;

        class RIntColumns : public Gtk::TreeModel::ColumnRecord{
            public:
                RIntColumns()
                {add(m_col_key); add(m_col_value);}
                Gtk::TreeModelColumn<Glib::ustring> m_col_key;
                Gtk::TreeModelColumn<Glib::ustring> m_col_value;
        };
        RIntColumns m_columns_rint;

        Gtk::Label lbl_perf_ops;
        Gtk::ScrolledWindow sw_oct,sw_rct,sw_perf_ops;
        Gtk::TextView tv_oct,tv_rct,tv_perf_ops;

        Gtk::Label lbl_modulus,lbl_modulus_note,lbl_mult_depth,lbl_mult_depth_note;
        Gtk::SpinButton sbtn_modulus,sbtn_mult_depth;

        // GUI update functions
        void populate_orig_ints_treeview();
        void populate_result_ints_treeview();

        // Signal handlers
        void on_main_menu_help();
        void on_main_menu_about();
        void on_fcb_input_file_set();
        void on_btn_encrypt_click();
        void on_btn_decrypt_click();
        void on_btn_add_1_click();
        void on_btn_add_3_click();
        void on_btn_add_5_click();
        void on_btn_sub_1_click();
        void on_btn_sub_3_click();
        void on_btn_sub_5_click();
        void on_btn_mult_3_click();
        void on_btn_mult_5_click();
        void on_btn_reset_click();
};

#endif
