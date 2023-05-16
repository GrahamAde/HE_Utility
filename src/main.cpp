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

int main(int argc, char *argv[]) {
    //Opening main window
    Gtk::Main kit(argc,argv);
    MainWindow mainWindow;
    kit.run(mainWindow);
    return 0;
}
