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

#include "about.hpp"

using namespace std;

AboutWindow::AboutWindow(MainWindow &win):
grid_main(),
frame_program_name("Homomorphic Encryption Utility"),
vbox_main(false,2),
lbl_program_description("Encrypt 64-bit integer values, perform mathematical operations on ciphertext, and decrypt the results.\nUses the OpenFHE library. <openfhe.org>"),
lbl_program_copyright_author("(c) 2023 Graham Ade\n"),lbl_program_license("License: GNU General Public License v3\nSee the LICENSE file for more info."),
hbb_main(Gtk::BUTTONBOX_END,5),
btn_close(Gtk::Stock::CLOSE),
mw(&win)
{
    set_title("About");
    set_default_size(200,100);
    set_modal(true);

    grid_main.attach(frame_program_name,0,0,1,1);
    grid_main.set_margin_top(10);grid_main.set_margin_bottom(10);grid_main.set_margin_start(10);grid_main.set_margin_end(10);
    grid_main.set_row_spacing(10);
    frame_program_name.add(vbox_main);
    vbox_main.set_margin_top(5);vbox_main.set_margin_bottom(5);vbox_main.set_margin_start(5);vbox_main.set_margin_end(5);
    vbox_main.pack_start(lbl_program_description,Gtk::PACK_SHRINK);
    vbox_main.pack_start(lbl_program_copyright_author,Gtk::PACK_SHRINK);
    vbox_main.pack_start(lbl_program_license,Gtk::PACK_SHRINK);
    grid_main.attach(hbb_main,0,1,1,1);
    hbb_main.add(btn_close);

    btn_close.signal_clicked().connect(sigc::mem_fun(*this,&AboutWindow::on_btn_close_clicked));

    add(grid_main);
    show_all_children();
}

AboutWindow::~AboutWindow(){

}

void AboutWindow::on_btn_close_clicked() {
    mw->closeTopWindow(*this,false);
}
