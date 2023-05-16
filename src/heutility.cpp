/************************************************************************
 * Homomorphic Encryption Utility - Perform operations on ciphertext
 * Copyright (c) 2023 Graham Ade
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License v3 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ************************************************************************/

#include "heutility.hpp"

using namespace std;
using namespace lbcrypto;

MainWindow::MainWindow():
vbox_main(false,2),
grid_main(),grid_operations(),grid_options(),
frame_input_file("CSV Input File"),
frame_original_ints("Original Integers"),frame_resultant_ints("Resultant Integers"),
frame_original_ciphertext("Original Ciphertext"),frame_resultant_ciphertext("Resultant Ciphertext"),
frame_operations("Mathematical Operations"),
frame_options("Homomorphic Encryption Options"),
fcb_input("Select CSV File",Gtk::FILE_CHOOSER_ACTION_OPEN),
btn_encrypt("Encrypt ↓"),btn_decrypt("Decrypt ↑"),
tv_oct(),tv_rct(),
hbb_operations(Gtk::BUTTONBOX_START,5),
btn_add_1("Add 1"),btn_add_3("Add 3"),btn_add_5("Add 5"),
btn_sub_1("Subtract 1"),btn_sub_3("Subtract 3"),btn_sub_5("Subtract 5"),
btn_mult_3("Multiply by 3"),btn_mult_5("Multiply by 5"),
lbl_perf_ops("Performed Ciphertext Operations"),
tv_perf_ops(),
lbl_modulus("Modulus"),lbl_mult_depth("Multiplicative Depth"),
lbl_modulus_note("Default: 557057"),
lbl_mult_depth_note("Note: Higher multiplicative depth results in longer computations and noise."),
hbb_controls(Gtk::BUTTONBOX_END,5),
btn_reset("Reset"),btn_quit(Gtk::Stock::QUIT),
topWins(),bool_input_file(false),mult_ops(0)
{
    set_title("Homomorphic Encryption Utility");
    set_default_size(800,600);
    set_modal(true);

    //Setting up main menu
    actiongroup_menu=Gtk::ActionGroup::create();
    //Main menu
    actiongroup_menu->add(Gtk::Action::create("HelpMenu","Help"));
    actiongroup_menu->add(Gtk::Action::create("HelpHelp","HE Utility Help"),sigc::mem_fun(*this,&MainWindow::on_main_menu_help));
    actiongroup_menu->add(Gtk::Action::create("HelpAbout","About"),sigc::mem_fun(*this,&MainWindow::on_main_menu_about));

    uimanager_menu=Gtk::UIManager::create();
    uimanager_menu->insert_action_group(actiongroup_menu);
    add_accel_group(uimanager_menu->get_accel_group());

    //Laying out actions in menubar
    Glib::ustring ui_info=
        "<ui>"
            "<menubar name='MenuBar'>"
                "<menu action='HelpMenu'>"
                    "<menuitem action='HelpHelp'/>"
                    "<menuitem action='HelpAbout'/>"
                "</menu>"
            "</menubar>"
        "</ui>";

    //Attempting to build menu
    try {
        uimanager_menu->add_ui_from_string(ui_info);
    } catch(const Glib::Error &ex) {
        std::cerr << "Building menu failed: " << ex.what();
    }
    Gtk::Widget *menubar_main = uimanager_menu->get_widget("/MenuBar");
    if(menubar_main) vbox_main.pack_start(*menubar_main,Gtk::PACK_SHRINK);
    vbox_main.pack_start(grid_main,false,true,0);

    grid_main.attach(frame_input_file,0,0,2,1);
    frame_input_file.set_margin_top(5);frame_input_file.set_margin_bottom(2);frame_input_file.set_margin_start(5);frame_input_file.set_margin_end(5);
    frame_input_file.set_hexpand(true);
    frame_input_file.add(fcb_input);
    fcb_input.set_margin_top(2);fcb_input.set_margin_bottom(2);fcb_input.set_margin_start(2);fcb_input.set_margin_end(2);

    grid_main.attach(frame_original_ints,0,1,1,1);
    frame_original_ints.set_margin_top(2);frame_original_ints.set_margin_bottom(2);frame_original_ints.set_margin_start(5);frame_original_ints.set_margin_end(5);
    frame_original_ints.add(sw_oint);
    sw_oint.set_hexpand(true);sw_oint.set_vexpand(true);
    sw_oint.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
    sw_oint.add(tv_oint);
    ls_oint=Gtk::ListStore::create(m_columns_oint);
    tv_oint.set_model(ls_oint);
    tv_oint.append_column("Key",m_columns_oint.m_col_key);tv_oint.get_column(0)->set_min_width(50);
    tv_oint.append_column("Value",m_columns_oint.m_col_value);


    grid_main.attach(frame_resultant_ints,1,1,1,1);
    frame_resultant_ints.set_margin_top(2);frame_resultant_ints.set_margin_bottom(2);frame_resultant_ints.set_margin_start(5);frame_resultant_ints.set_margin_end(5);
    frame_resultant_ints.add(sw_rint);
    sw_rint.set_hexpand(true);sw_rint.set_vexpand(true);
    sw_rint.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
    sw_rint.add(tv_rint);
    ls_rint=Gtk::ListStore::create(m_columns_rint);
    tv_rint.set_model(ls_rint);
    tv_rint.append_column("Key",m_columns_rint.m_col_key);tv_rint.get_column(0)->set_min_width(50);
    tv_rint.append_column("Value",m_columns_rint.m_col_value);


    grid_main.attach(btn_encrypt,0,2,1,1);
    btn_encrypt.set_margin_top(2);btn_encrypt.set_margin_bottom(2);btn_encrypt.set_margin_start(5);btn_encrypt.set_margin_end(5);

    grid_main.attach(btn_decrypt,1,2,1,1);
    btn_decrypt.set_margin_top(2);btn_decrypt.set_margin_bottom(2);btn_decrypt.set_margin_start(5);btn_decrypt.set_margin_end(5);
    btn_decrypt.set_sensitive(false);

    grid_main.attach(frame_original_ciphertext,0,3,1,1);
    frame_original_ciphertext.set_margin_top(2);frame_original_ciphertext.set_margin_bottom(2);frame_original_ciphertext.set_margin_start(5);frame_original_ciphertext.set_margin_end(5);
    frame_original_ciphertext.add(sw_oct);
    sw_oct.set_hexpand(true);sw_oct.set_vexpand(true);
    sw_oct.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
    sw_oct.add(tv_oct);
    tv_oct.set_editable(false);
    tv_oct.set_cursor_visible(false);

    grid_main.attach(frame_resultant_ciphertext,1,3,1,1);
    frame_resultant_ciphertext.set_margin_top(2);frame_resultant_ciphertext.set_margin_bottom(2);frame_resultant_ciphertext.set_margin_start(5);frame_resultant_ciphertext.set_margin_end(5);
    frame_resultant_ciphertext.add(sw_rct);
    sw_rct.set_hexpand(true);sw_rct.set_vexpand(true);
    sw_rct.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
    sw_rct.add(tv_rct);
    tv_rct.set_editable(false);
    tv_rct.set_cursor_visible(false);


    grid_main.attach(frame_operations,0,4,2,1);
    frame_operations.set_margin_top(2);frame_operations.set_margin_bottom(2);frame_operations.set_margin_start(5);frame_operations.set_margin_end(5);
    frame_operations.add(grid_operations);
    grid_operations.attach(hbb_operations,0,0,1,1);
    hbb_operations.set_margin_top(2);hbb_operations.set_margin_bottom(2);hbb_operations.set_margin_start(2);hbb_operations.set_margin_end(2);
    hbb_operations.add(btn_add_1);hbb_operations.add(btn_add_3);hbb_operations.add(btn_add_5);
    hbb_operations.add(btn_sub_1);hbb_operations.add(btn_sub_3);hbb_operations.add(btn_sub_5);
    hbb_operations.add(btn_mult_3);hbb_operations.add(btn_mult_5);
    btn_add_1.set_sensitive(false);btn_add_3.set_sensitive(false);btn_add_5.set_sensitive(false);
    btn_sub_1.set_sensitive(false);btn_sub_3.set_sensitive(false);btn_sub_5.set_sensitive(false);
    btn_mult_3.set_sensitive(false);btn_mult_5.set_sensitive(false);
    grid_operations.attach(lbl_perf_ops,0,1,1,1);
    grid_operations.attach(sw_perf_ops,0,2,1,1);
    sw_perf_ops.set_hexpand(true);sw_perf_ops.set_vexpand(true);
    sw_perf_ops.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
    sw_perf_ops.add(tv_perf_ops);
    tv_perf_ops.set_editable(false);
    tv_perf_ops.set_cursor_visible(false);

    grid_main.attach(frame_options,0,5,2,1);
    frame_options.set_margin_top(2);frame_options.set_margin_bottom(2);frame_options.set_margin_start(5);frame_options.set_margin_end(5);
    frame_options.add(grid_options);
    grid_options.set_margin_top(2);grid_options.set_margin_bottom(2);grid_options.set_margin_start(2);grid_options.set_margin_end(2);
    grid_options.set_column_spacing(5);grid_options.set_row_spacing(5);
    grid_options.attach(lbl_modulus,0,0,1,1);
    grid_options.attach(sbtn_modulus,1,0,1,1);
    sbtn_modulus.set_range(0,1000000000000);sbtn_modulus.set_increments(1,100);sbtn_modulus.set_value(557057);
    grid_options.attach(lbl_modulus_note,2,0,1,1);lbl_modulus_note.set_halign(Gtk::ALIGN_START);
    grid_options.attach(lbl_mult_depth,0,1,1,1);
    grid_options.attach(sbtn_mult_depth,1,1,1,1);
    sbtn_mult_depth.set_range(1,4);sbtn_mult_depth.set_increments(1,1);sbtn_mult_depth.set_value(2);
    grid_options.attach(lbl_mult_depth_note,2,1,1,1);lbl_mult_depth_note.set_halign(Gtk::ALIGN_START);

    grid_main.attach(hbb_controls,0,6,2,1);
    hbb_controls.set_margin_top(2);hbb_controls.set_margin_bottom(5);hbb_controls.set_margin_start(5);hbb_controls.set_margin_end(5);
    hbb_controls.add(btn_reset);
    hbb_controls.add(btn_quit);

    //Signal handlers
    fcb_input.signal_file_set().connect(sigc::mem_fun(*this,&MainWindow::on_fcb_input_file_set));
    btn_encrypt.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_encrypt_click));
    btn_decrypt.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_decrypt_click));
    btn_add_1.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_add_1_click));
    btn_add_3.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_add_3_click));
    btn_add_5.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_add_5_click));
    btn_sub_1.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_sub_1_click));
    btn_sub_3.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_sub_3_click));
    btn_sub_5.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_sub_5_click));
    btn_mult_3.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_mult_3_click));
    btn_mult_5.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_mult_5_click));
    btn_reset.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_btn_reset_click));
    btn_quit.signal_clicked().connect(sigc::mem_fun(*this,&Gtk::Window::close));

    add(vbox_main);
    show_all_children();
}

MainWindow::~MainWindow() {
    for(vectTpWin::iterator i=topWins.begin(); i<topWins.end(); i++) {
        (*i) -> hide();
        delete(*i);
    }
}

void MainWindow::populate_orig_ints_treeview() {
    ls_oint->clear();
    for(uint64_t a=0; a<map_orig_ints.size(); a++){
        Gtk::TreeModel::Row row=*(ls_oint->append());
        row[m_columns_oint.m_col_key]=to_string(a+1);
        row[m_columns_oint.m_col_value]=boost::lexical_cast<std::string>(map_orig_ints[a]);
    }
}

void MainWindow::populate_result_ints_treeview() {
    ls_rint->clear();
    for(uint64_t a=0; a<map_result_ints.size(); a++){
        Gtk::TreeModel::Row row=*(ls_rint->append());
        row[m_columns_rint.m_col_key]=to_string(a+1);
        row[m_columns_rint.m_col_value]=boost::lexical_cast<std::string>(map_result_ints[a]);
    }
}

void MainWindow::on_main_menu_help() {
    HelpWindow *win = new HelpWindow(*this);
    topWins.push_back(win);
    win->show();
}

void MainWindow::on_main_menu_about() {
    AboutWindow *win = new AboutWindow(*this);
    topWins.push_back(win);
    win->show();
}

void MainWindow::on_fcb_input_file_set(){
    fcb_input.set_sensitive(false);

    // Reading CSV file into string
    string filename=fcb_input.get_filename();
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    string contents;
    if(in){
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0,std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    }

//cout<<contents<<endl;
//cout<<"---------------------------------"<<endl;

    // Vectorize CSV File
    vectorize_int_string(contents);
    // Create Mapping
    for(uint64_t a=0; a<vect_orig_ints.size(); a++) map_orig_ints[a]=vect_orig_ints[a];

    // Populate GUI
    populate_orig_ints_treeview();

    // Create Add, Sub, Mult Vectors
    vect_1.clear();
    vect_3.clear();
    vect_5.clear();
    for(uint64_t b=0; b<vect_orig_ints.size(); b++){
        vect_1.push_back(1);
        vect_3.push_back(3);
        vect_5.push_back(5);
    }

    bool_input_file.store(true);
}

void MainWindow::on_btn_encrypt_click(){
if(bool_input_file.load()){
    // Deactivate option buttons
    btn_encrypt.set_sensitive(false);
    sbtn_modulus.set_sensitive(false);
    sbtn_mult_depth.set_sensitive(false);

    // Set crypto parameters
    parameters.SetPlaintextModulus(sbtn_modulus.get_value());
    parameters.SetMultiplicativeDepth(sbtn_mult_depth.get_value());

    crypto_context=GenCryptoContext(parameters);

    crypto_context->Enable(PKE);
    crypto_context->Enable(KEYSWITCH);
    crypto_context->Enable(LEVELEDSHE);

    // Generate key pair
    key_pair=crypto_context->KeyGen();

    // Generate relinearization key
    crypto_context->EvalMultKeyGen(key_pair.secretKey);

    // Generate rotation evaulation keys
//    crypto_context->EvalRotateKeyGen(key_pair.secretKey,{1,3,5,-1,3,5});

    // Make plaintext
    pt_orig_ints=crypto_context->MakePackedPlaintext(vect_orig_ints);
    pt_vect_1=crypto_context->MakePackedPlaintext(vect_1);
    pt_vect_3=crypto_context->MakePackedPlaintext(vect_3);
    pt_vect_5=crypto_context->MakePackedPlaintext(vect_5);

    // Encrypt
    ct_orig_ints=crypto_context->Encrypt(key_pair.publicKey, pt_orig_ints);
    ct_result_ints=ct_orig_ints;

    // Update original integer ciphertext
    auto refBuffer=tv_oct.get_buffer();
    display_text(refBuffer,Serial::SerializeToString(ct_orig_ints));

    // Activate operation buttons
    btn_add_1.set_sensitive(true);btn_add_3.set_sensitive(true);btn_add_5.set_sensitive(true);
    btn_sub_1.set_sensitive(true);btn_sub_3.set_sensitive(true);btn_sub_5.set_sensitive(true);
    btn_mult_3.set_sensitive(true);btn_mult_5.set_sensitive(true);
}
}

void MainWindow::on_btn_decrypt_click(){
    // Deactivate operation buttons
    btn_decrypt.set_sensitive(false);
    btn_add_1.set_sensitive(false);btn_add_3.set_sensitive(false);btn_add_5.set_sensitive(false);
    btn_sub_1.set_sensitive(false);btn_sub_3.set_sensitive(false);btn_sub_5.set_sensitive(false);
    btn_mult_3.set_sensitive(false);btn_mult_5.set_sensitive(false);

    // Decrypt
    crypto_context->Decrypt(key_pair.secretKey,ct_result_ints,&pt_result_ints);
//cout<<pt_result_ints<<endl;
    // Get plaintext values
    pt_result_ints->SetLength(vect_orig_ints.size());
    vect_result_ints=pt_result_ints->GetPackedValue();
//for(uint64_t b=0;b<vect_result_ints.size();b++) cout<<vect_result_ints[b]<<endl;

    // Create mapping
    for(uint64_t a=0; a<vect_result_ints.size(); a++) map_result_ints[a]=vect_result_ints[a];

    // Update GUI
    populate_result_ints_treeview();
}

void MainWindow::on_btn_add_1_click(){
    btn_decrypt.set_sensitive(true);
    ct_result_ints=crypto_context->EvalAdd(ct_result_ints,pt_vect_1);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Added 1 to all ciphertext values\n");
}

void MainWindow::on_btn_add_3_click(){
    btn_decrypt.set_sensitive(true);
    ct_result_ints=crypto_context->EvalAdd(ct_result_ints,pt_vect_3);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Added 3 to all ciphertext values\n");
}

void MainWindow::on_btn_add_5_click(){
    btn_decrypt.set_sensitive(true);
    ct_result_ints=crypto_context->EvalAdd(ct_result_ints,pt_vect_5);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Added 5 to all ciphertext values\n");
}

void MainWindow::on_btn_sub_1_click(){
    btn_decrypt.set_sensitive(true);
    ct_result_ints=crypto_context->EvalSub(ct_result_ints,pt_vect_1);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Subtracted 1 from all ciphertext values\n");
}

void MainWindow::on_btn_sub_3_click(){
    btn_decrypt.set_sensitive(true);
    ct_result_ints=crypto_context->EvalSub(ct_result_ints,pt_vect_3);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Subtracted 3 from all ciphertext values\n");
}

void MainWindow::on_btn_sub_5_click(){
    btn_decrypt.set_sensitive(true);
    ct_result_ints=crypto_context->EvalSub(ct_result_ints,pt_vect_5);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Subtracted 5 from all ciphertext values\n");
}

void MainWindow::on_btn_mult_3_click(){
    btn_decrypt.set_sensitive(true);
    mult_ops.store(mult_ops.load()+1);
    ct_result_ints=crypto_context->EvalMult(ct_result_ints,pt_vect_3);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Multiplied all ciphertext values by 3\n");
    if(mult_ops.load()>=sbtn_mult_depth.get_value()){
        btn_mult_3.set_sensitive(false);
        btn_mult_5.set_sensitive(false);
    }
}

void MainWindow::on_btn_mult_5_click(){
    btn_decrypt.set_sensitive(true);
    mult_ops.store(mult_ops.load()+1);
    ct_result_ints=crypto_context->EvalMult(ct_result_ints,pt_vect_5);
    auto refBuffer1=tv_rct.get_buffer();
    display_text(refBuffer1,Serial::SerializeToString(ct_result_ints));
    auto refBuffer2=tv_perf_ops.get_buffer();
    display_text(refBuffer2,"Multiplied all ciphertext values by 5\n");
    if(mult_ops.load()>=sbtn_mult_depth.get_value()){
        btn_mult_3.set_sensitive(false);
        btn_mult_5.set_sensitive(false);
    }
}

void MainWindow::on_btn_reset_click(){
    fcb_input.set_sensitive(true);
    fcb_input.unselect_all();
    bool_input_file.store(false);
    btn_encrypt.set_sensitive(true);
    btn_decrypt.set_sensitive(false);
    btn_add_1.set_sensitive(false);btn_add_3.set_sensitive(false);btn_add_5.set_sensitive(false);
    btn_sub_1.set_sensitive(false);btn_sub_3.set_sensitive(false);btn_sub_5.set_sensitive(false);
    btn_mult_3.set_sensitive(false);btn_mult_5.set_sensitive(false);
    sbtn_modulus.set_sensitive(true);
    sbtn_modulus.set_value(557057);
    sbtn_mult_depth.set_sensitive(true);
    sbtn_mult_depth.set_value(2);
    map_orig_ints.clear();
    map_result_ints.clear();
    vect_orig_ints.clear();
    vect_result_ints.clear();
    vect_1.clear();
    vect_3.clear();
    vect_5.clear();
    mult_ops.store(0);
    ls_oint->clear();
    ls_rint->clear();
    auto refBuffer1=tv_oct.get_buffer();
    refBuffer1->set_text("");
    auto refBuffer2=tv_rct.get_buffer();
    refBuffer2->set_text("");
    auto refBuffer3=tv_perf_ops.get_buffer();
    refBuffer3->set_text("");
}

void MainWindow::closeTopWindow(Gtk::Window &win, bool quit_program) {
    #if 1
        vectTpWin::iterator i=std::find(topWins.begin(),topWins.end(),&win);
    #else
        vectTpWin::iterator i=topWins.begin();
        while(i!=topWins.end()){
            if((*i)!=&win) i++;
            else break;
        }
    #endif
        if(i == topWins.end()){
            cerr << "Ref window not found" << endl;
            ::exit(1);
        }
        (*i)->hide();
        delete(*i);
        topWins.erase(i);

        //Checks if program should quit
        if(quit_program==true) Gtk::Main::quit();

        //Showing MainWindow
        MainWindow::show();
}

void MainWindow::vectorize_int_string(std::string str){
    vect_orig_ints.clear();

    //Remove all line breaks
    str.erase(std::remove(str.begin(),str.end(),'\n'),str.cend());
//cout<<str<<endl;

    size_t found_comma=str.find(",");
    string temp_str;

    while(found_comma!=std::string::npos){
//cout<<"Found Pos: "<<found_comma<<endl;
//cout<<str.substr(0,found_comma)<<endl;
        temp_str=str.substr(0,found_comma);
        vect_orig_ints.push_back(boost::lexical_cast<int64_t>(temp_str));
        str=str.substr(found_comma+1);
        found_comma=str.find(",");
        if(found_comma==std::string::npos) vect_orig_ints.push_back(boost::lexical_cast<int64_t>(str));
    }

    //Print Vector
//    for(long unsigned int b=0;b<vect_orig_ints.size();b++) cout<<vect_orig_ints[b]<<endl;
}

void MainWindow::display_text(Glib::RefPtr<Gtk::TextBuffer> &refBuffer,std::string text) {
    auto iter=refBuffer->get_iter_at_offset(0);
    iter=refBuffer->insert(iter, text);
}
