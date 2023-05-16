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

#include "help.hpp"

using namespace std;

HelpWindow::HelpWindow(MainWindow &win):
grid_main(),
frame_contents("Help Information"),
tv_contents(),
hbb_main(Gtk::BUTTONBOX_END,5),
btn_close(Gtk::Stock::CLOSE),
mw(&win)
{
    set_title("HE Utility Help");
    set_default_size(1000,600);
    set_resizable(false);
    set_modal(true);

    grid_main.attach(frame_contents,0,0,1,1);
    grid_main.set_margin_top(10);grid_main.set_margin_bottom(10);grid_main.set_margin_start(10);grid_main.set_margin_end(10);
    grid_main.set_row_spacing(10);
    frame_contents.add(sw_contents);
    sw_contents.set_hexpand(true);sw_contents.set_vexpand(true);
    sw_contents.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
    sw_contents.add(tv_contents);
    tv_contents.set_editable(false);

    grid_main.attach(hbb_main,0,1,1,1);
    hbb_main.add(btn_close);

    signal_show().connect(sigc::mem_fun(*this,&HelpWindow::on_window_show));
    btn_close.signal_clicked().connect(sigc::mem_fun(*this,&HelpWindow::on_btn_close_clicked));

    add(grid_main);
    show_all_children();
}

HelpWindow::~HelpWindow(){

}

void HelpWindow::on_window_show(){
    auto refBuffer=tv_contents.get_buffer();
//    refBuffer->set_text("

string strhelp1="\nRunning the Homomorphic Encryption Utility\n\nVersion 1.0 - 5/15/2023\n\nThe program takes an input of integers formatted as a CSV (Comma-Separated Values) file, encrypts them via homomorphic encryption,\n allows the user to perform simple addition, subtraction, or multiplication on the ciphertext, and decrypts the result.\n\n\n";
string strhelp2="Table of Contents\n\n- Loading Integer Values\n- Encrypting Integer Values\n- Performing Mathematical Operations on Ciphertext\n- Decrypting the Ciphertext\n- Resetting the Program\n\n\n";
string strhelp3="Loading Integer Values\n\nThe CSV file can be selected via the top GUI file chooser button labeled 'CSV Input File'.  This button will open a directory navigation screen\n allowing the user to select a CSV file.  The utility comes with one sample CSV file named 'Sample.csv' with 100 sample integer values.\n  It reads the integer values into mapping whereby the first integer value has a key of 1 and the last integer value has a key equal to the number\n of integers in the file.  Once loaded into the program, the integer values from the CSV file and their associated key values will be shown in the\n 'Original Integer' scroll viewport located on the left side of the application window.\n\n\n";
string strhelp4="Encrypting Integer Values\n\nTo encrypt the integers, click on the 'Encrypt ↓' button located under the 'Original Integers' viewport.  The integers will be encrypted via the SWHE\n (SomeWhat Homomorphic Encryption) scheme BFVRNS with the default or user supplied 'Homomorphic Encryption Options' set at the bottom of\n the application.  Once encrypted, a JSON object representing the ciphertext will be shown in the 'Original Ciphertext' viewport located on the left\n side of the application window.\n\n";
string strhelp5="Setting the Modulus:\n\nThe prime modulus number (q) must satisfy the condition:\n\n(q-1) / m = an integer\n\nWhereby, (q) is the prime modulus, and (m) is the cyclotomic number.\n\nNote:\nAny mathematical operations performed on the input values in the CSV file must result in a number less than half of the modulus or an overflow\n condition will occur.  Any mathematical operation performed on an input value resulting in a value greater than half the modulo (q/2) will have\n the modulus value subtracted from it, repeatedly if necessary, until the resultant value is between the values of -q/2 and q/2.  The default prime\n modulus value is ‘557057’.  Try using the smaller prime modulus value of ‘65537’ for smaller integer inputs and faster calculations.\n\n";
string strhelp6="Setting the Multiplicative Depth:\n\nIn homomorphic encryption, the multiplicative depth refers to the number of times one can perform multiplication operations on encrypted data\n without having to decrypt it. It measures the number of successive multiplications that can be carried out while preserving the encrypted form.\n\nThe multiplicative depth is typically limited in homomorphic encryption schemes. Each multiplication operation increases the depth by one, and\n exceeding a certain depth limit can lead to exponential growth in the complexity and computational requirements of performing operations. This\n limitation is known as the 'depth bootstrapping problem.'\n\nThe default value is set to a multiplicative depth of ‘2’ and can be increased to a maximum of ‘4’.\n\n\n";
string strhelp7="Performing Mathematical Operations on Ciphertext\n\nIn the middle of the application window, the user can select eight different mathematical operations to perform on the ciphertext.  All mathematical\n operations are applied uniformly across all original integer inputs.  The user can select from the following mathematical operations:\n\n- 'Add 1' - This adds 1 to all original integer values.\n- 'Add 3' - This adds 3 to all original integer values.\n- 'Add 5' - This adds 5 to all original integer values.\n- 'Subtract 1' - This subtracts 1 from all original integer values.\n- 'Subtract 3' - This subtracts 3 from all original integer values.\n- 'Subtract 5' - This subtracts 5 from all original integer values.\n- 'Multiply by 3' - This multiplies all original integer values by 3.\n- 'Multiply by 5' - This multiplies all original integer values by 5.\n\nOnce a user clicks on a mathematical operation to perform, the application runs the operation on the ciphertext and displays the resultant\n ciphertext as a JSON object in the 'Resultant Ciphertext' viewport on the right side of the application window.  Additionally, a history of all\n performed ciphertext operations are listed in the scroll window viewport located below the mathematical operation buttons.  Running multiple\n mathematical operations on the ciphertext will result in the most recent ciphertext JSON object being displayed first in the 'Resultant Ciphertext'\n viewport.  A history of the resultant ciphertexts will be shown if the user scrolls down the viewport.  The most recent resultant ciphertext is always\n displayed first.\n\nNote:\nMultiplicative operations on the ciphertext are limited by the ‘Multiplicative Depth’ option set prior to plaintext encryption.  The program will\n prevent the user from additional multiplicative operations if the multiplicative depth is reached.\n\n\n";
string strhelp8="Decrypting the Ciphertext\n\nWhen the user is done performing mathematical operations on the ciphertext, they may decrypt the resultant ciphertext via the 'Decrypt ↑' button\n located above the 'Resultant Ciphertext' viewport on the right side of the application window.  The decrypted resultant integers will be displayed\n in the 'Resultant Integer' viewport above the decrypt button.  By comparing the resultant integers to the original integers, you can validate that all\n performed mathematical operations were calculated successfully.\n\nNote:\nRefer to the 'Setting the Modulus' section for information on resultant integers which seem incongruent.  All resultant integers must fall within\n the range of -q/2 and q/2 where q is the prime modulus number used for encryption.  Apparently incongruent integer results will be found to be\n correct, but reduced by the modulo to fall within this range.\n\n\n";
string strhelp9="Resetting the Program\n\nThe user may click the 'Reset' button to reset the integer input, plaintext, and ciphertext.  Once the user clicks the reset button, they will be able\n to load a new CSV file and perform new homomorphic encryption operations on the new integer data.\n\n";

    display_text(refBuffer,strhelp9);
    display_text(refBuffer,strhelp8);
    display_text(refBuffer,strhelp7);
    display_text(refBuffer,strhelp6);
    display_text(refBuffer,strhelp5);
    display_text(refBuffer,strhelp4);
    display_text(refBuffer,strhelp3);
    display_text(refBuffer,strhelp2);
    display_text(refBuffer,strhelp1);
}

void HelpWindow::on_btn_close_clicked(){
    mw->closeTopWindow(*this,false);
}

void HelpWindow::display_text(Glib::RefPtr<Gtk::TextBuffer> &refBuffer,std::string text) {
    auto iter=refBuffer->get_iter_at_offset(0);
    iter=refBuffer->insert(iter, text);
}
