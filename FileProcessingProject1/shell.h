#ifndef SHELL_H
#define SHELL_H
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include"test.h"
#include"RecordManipulation.h"

void shell();					// the first function when program start
void shell_admin();				// if user select to record manipulation, it displays
void purchase_menu();
void menu();					// shell()'s menu
void admin_menu();				// shell_admin()'s menu
#endif