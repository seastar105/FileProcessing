#ifndef RECORD_MANIPULATION_H
#define RECORD_MANIPULATION_H
//#define DEBUG
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<algorithm>
#include"recfile.h"
#include"delim.h"
#include"Purchase.h"
#include"Lecture.h"
#include"Member.h"

#include"tindbuff.h"
#include"textind.h"
#include"indfile.h"
#include"Btnode.h"
#include"btree.h"

void clear_console();			// use system command to clear console (works only with windows)
void LecturePurchaseSystem();
void purchase_menu();
int record_type(string);

void record_search();
void record_insert();
void record_delete();
void record_update();

void make_memories();
void delete_memories();

void delete_member(string);
void delete_lecture(string);
void delete_purchase(string);

bool search_member(string);
bool search_lecture(string);
std::vector<int> search_purchase(string);

bool update_member(string);
bool update_lecture(string);
bool update_purchase(string);

bool search_PID(string);

bool login_window();
void admin_menu();
void general_menu(Member &);
void general_system();

bool member_information(Member &);
void search_lecture();
void purchase_manipulation(string key);

void openIndex();			// If there're no indexFiles, createIndexFile First, and Open 
bool insert_member(string key);
bool insert_lecture(string key);

void RewritePurchase();		// Rewrite Purchase .dat and .ind file after update and delete
#endif