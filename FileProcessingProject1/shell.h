#ifndef SHELL_H
#define SHELL_H
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include"test.h"

void shell();					// the first function when program start
void shell_admin();				// if user select to record manipulation, it displays
void clear_console();			// use system command to clear console (works only with windows)
void menu();					// shell()'s menu
void admin_menu();				// shell_admin()'s menu
#endif