#ifndef RECORD_MANIPULATION_H
#define RECORD_MANIPULATION_H

#include<iostream>
#include<map>
#include<vector>
#include<string>
#include"recfile.h"
#include"delim.h"
#include"Purchase.h"
#include"Lecture.h"
#include"Member.h"

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

bool search_member(string);
bool search_lecture(string);
vector<int> search_purchase(string);

bool update_member(string);
bool update_lecture(string);
bool update_purchase(string);

bool search_PID(string);
#endif