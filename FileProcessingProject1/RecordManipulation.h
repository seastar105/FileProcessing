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
int record_type();

void record_search();
void record_insert();
void record_delete();
void record_update();
void make_memories();
void delete_memories();
#endif