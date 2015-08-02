#include <pebble.h>
#include "config.h"

LanguageValue _language;
LanguageValue getLanguage(){return _language;}
void setLanguage(LanguageValue value){_language = value;}


BoxFunction _box1;
BoxFunction getBox1(){return _box1;}
void setBox1(BoxFunction value){_box1 = value;}


BoxFunction _box2;
BoxFunction getBox2(){return _box2;}
void setBox2(BoxFunction value){_box2 = value;}


BoxFunction _box3;
BoxFunction getBox3(){return _box3;}
void setBox3(BoxFunction value){_box3 = value;}


BoxFunction _box4;
BoxFunction getBox4(){return _box4;}
void setBox4(BoxFunction value){_box4 = value;}


bool _big1;
bool getBig1(){return _big1;}
void setBig1(bool value){_big1 = value;}


bool _big2;
bool getBig2(){return _big2;}
void setBig2(bool value){_big2 = value;}


bool _big3;
bool getBig3(){return _big3;}
void setBig3(bool value){_big3 = value;}


bool _big4;
bool getBig4(){return _big4;}
void setBig4(bool value){_big4 = value;}


bool _uur;
bool getUur(){return _uur;}
void setUur(bool value){_uur = value;}


char _colbox1[9];
char* getColbox1(){return _colbox1;}
void setColbox1(char* value){strncpy(_colbox1, value, 9);}


char _txtbox1[9];
char* getTxtbox1(){return _txtbox1;}
void setTxtbox1(char* value){strncpy(_txtbox1, value, 9);}


char _colbox2[9];
char* getColbox2(){return _colbox2;}
void setColbox2(char* value){strncpy(_colbox2, value, 9);}


char _txtbox2[9];
char* getTxtbox2(){return _txtbox2;}
void setTxtbox2(char* value){strncpy(_txtbox2, value, 9);}


char _colbox3[9];
char* getColbox3(){return _colbox3;}
void setColbox3(char* value){strncpy(_colbox3, value, 9);}


char _txtbox3[9];
char* getTxtbox3(){return _txtbox3;}
void setTxtbox3(char* value){strncpy(_txtbox3, value, 9);}


char _colbox4[9];
char* getColbox4(){return _colbox4;}
void setColbox4(char* value){strncpy(_colbox4, value, 9);}


char _txtbox4[9];
char* getTxtbox4(){return _txtbox4;}
void setTxtbox4(char* value){strncpy(_txtbox4, value, 9);}


void config_in_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *tuple = NULL;
	
	tuple = dict_find(iter, LANGUAGE_PKEY);
	tuple ? setLanguage(tuple->value->int32) : false;
	tuple = dict_find(iter, UUR_PKEY);
	tuple ? setUur(tuple->value->int32) : false;  
	tuple = dict_find(iter, BIG1_PKEY);
	tuple ? setBig1(tuple->value->int32) : false;
	tuple = dict_find(iter, BIG2_PKEY);
	tuple ? setBig2(tuple->value->int32) : false;
	tuple = dict_find(iter, BIG3_PKEY);
	tuple ? setBig3(tuple->value->int32) : false;
	tuple = dict_find(iter, BIG4_PKEY);
	tuple ? setBig4(tuple->value->int32) : false;
	tuple = dict_find(iter, COLBOX1_PKEY);
	tuple ? setColbox1(tuple->value->cstring) : false;
	tuple = dict_find(iter, TXTBOX1_PKEY);
	tuple ? setTxtbox1(tuple->value->cstring) : false;
	tuple = dict_find(iter, COLBOX2_PKEY);
	tuple ? setColbox2(tuple->value->cstring) : false;
	tuple = dict_find(iter, TXTBOX2_PKEY);
	tuple ? setTxtbox2(tuple->value->cstring) : false;
	tuple = dict_find(iter, COLBOX3_PKEY);
	tuple ? setColbox3(tuple->value->cstring) : false;
	tuple = dict_find(iter, TXTBOX3_PKEY);
	tuple ? setTxtbox3(tuple->value->cstring) : false;
	tuple = dict_find(iter, COLBOX4_PKEY);
	tuple ? setColbox4(tuple->value->cstring) : false;
	tuple = dict_find(iter, TXTBOX4_PKEY);
	tuple ? setTxtbox4(tuple->value->cstring) : false;
	tuple = dict_find(iter, BOX1_PKEY);
	tuple ? setBox1(tuple->value->int32) : false;
	tuple = dict_find(iter, BOX2_PKEY);
	tuple ? setBox2(tuple->value->int32) : false;
	tuple = dict_find(iter, BOX3_PKEY);
	tuple ? setBox3(tuple->value->int32) : false;
	tuple = dict_find(iter, BOX4_PKEY);
	tuple ? setBox4(tuple->value->int32) : false;	
}

void config_init(){
	app_message_register_inbox_received(config_in_received_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	
	if (persist_exists(LANGUAGE_PKEY)) {
		setLanguage(persist_read_int(LANGUAGE_PKEY));
	}
	else {
		setLanguage(0);
	}
  if (persist_exists(UUR_PKEY)) {
		setUur(persist_read_bool(UUR_PKEY));
	}
	else {
		setUur(true);
	}
  if (persist_exists(BIG1_PKEY)) {
		setBig1(persist_read_bool(BIG1_PKEY));
	}
	else {
		setBig1(true);
	}
  if (persist_exists(BIG2_PKEY)) {
		setBig2(persist_read_bool(BIG2_PKEY));
	}
	else {
		setBig2(true);
	}
  if (persist_exists(BIG3_PKEY)) {
		setBig3(persist_read_bool(BIG3_PKEY));
	}
	else {
		setBig3(true);
	}
  if (persist_exists(BIG4_PKEY)) {
		setBig4(persist_read_bool(BIG4_PKEY));
	}
	else {
		setBig4(true);
	}
  if (persist_exists(COLBOX1_PKEY)) {
		persist_read_string(COLBOX1_PKEY, _colbox1, 9);
		setColbox1(_colbox1);
	}
	else {
		setColbox1("FF5500");
	}
  if (persist_exists(TXTBOX1_PKEY)) {
		persist_read_string(TXTBOX1_PKEY, _txtbox1, 9);
		setTxtbox1(_txtbox1);
	}
	else {
		setTxtbox1("FFFFFF");
	}
  if (persist_exists(COLBOX2_PKEY)) {
		persist_read_string(COLBOX2_PKEY, _colbox2, 9);
		setColbox2(_colbox2);
	}
	else {
		setColbox2("55AA00");
	}
  if (persist_exists(TXTBOX2_PKEY)) {
		persist_read_string(TXTBOX2_PKEY, _txtbox2, 9);
		setTxtbox2(_txtbox2);
	}
	else {
		setTxtbox2("000000");
	}
  if (persist_exists(COLBOX3_PKEY)) {
		persist_read_string(COLBOX3_PKEY, _colbox3, 9);
		setColbox3(_colbox3);
	}
	else {
		setColbox3("FFAA00");
	}
  if (persist_exists(TXTBOX3_PKEY)) {
		persist_read_string(TXTBOX3_PKEY, _txtbox3, 9);
		setTxtbox3(_txtbox3);
	}
	else {
		setTxtbox3("000000");
	}
  if (persist_exists(COLBOX4_PKEY)) {
		persist_read_string(COLBOX4_PKEY, _colbox4, 9);
		setColbox4(_colbox4);
	}
	else {
		setColbox4("55AAFF");
	}
  if (persist_exists(TXTBOX4_PKEY)) {
		persist_read_string(TXTBOX4_PKEY, _txtbox4, 9);
		setTxtbox4(_txtbox4);
	}
	else {
		setTxtbox4("FFFFFF");
	}
	if (persist_exists(BOX1_PKEY)) {
		setBox1(persist_read_int(BOX1_PKEY));
	}
	else {
		setBox1(0);
	}
	if (persist_exists(BOX2_PKEY)) {
		setBox2(persist_read_int(BOX2_PKEY));
	}
	else {
		setBox2(1);
	}
	if (persist_exists(BOX3_PKEY)) {
		setBox3(persist_read_int(BOX3_PKEY));
	}
	else {
		setBox3(3);
	}
	if (persist_exists(BOX4_PKEY)) {
		setBox4(persist_read_int(BOX4_PKEY));
	}
	else {
		setBox4(4);
	}
}

void config_deinit(){
	persist_read_int(LANGUAGE_PKEY) != _language ? persist_write_int(LANGUAGE_PKEY, _language) : false;
	persist_read_bool(UUR_PKEY) != _uur ? persist_write_bool(UUR_PKEY, _uur) : false;
	char tmp_colbox1[9]; 
	persist_read_string(COLBOX1_PKEY, tmp_colbox1, 9);
	if(memcmp(_colbox1, tmp_colbox1, 9) != 0)
		persist_write_string(COLBOX1_PKEY, _colbox1);
	char tmp_txtbox1[9]; 
	persist_read_string(TXTBOX1_PKEY, tmp_txtbox1, 9);
	if(memcmp(_txtbox1, tmp_txtbox1, 9) != 0)
		persist_write_string(TXTBOX1_PKEY, _txtbox1);
	char tmp_colbox2[9]; 
	persist_read_string(COLBOX2_PKEY, tmp_colbox2, 9);
	if(memcmp(_colbox2, tmp_colbox2, 9) != 0)
		persist_write_string(COLBOX2_PKEY, _colbox2);
	char tmp_txtbox2[9]; 
	persist_read_string(TXTBOX2_PKEY, tmp_txtbox2, 9);
	if(memcmp(_txtbox2, tmp_txtbox2, 9) != 0)
		persist_write_string(TXTBOX2_PKEY, _txtbox2);
	char tmp_colbox3[9]; 
	persist_read_string(COLBOX3_PKEY, tmp_colbox3, 9);
	if(memcmp(_colbox3, tmp_colbox3, 9) != 0)
		persist_write_string(COLBOX3_PKEY, _colbox3);
	char tmp_txtbox3[9]; 
	persist_read_string(TXTBOX3_PKEY, tmp_txtbox3, 9);
	if(memcmp(_txtbox3, tmp_txtbox3, 9) != 0)
		persist_write_string(TXTBOX3_PKEY, _txtbox3);
	char tmp_colbox4[9]; 
	persist_read_string(COLBOX4_PKEY, tmp_colbox4, 9);
	if(memcmp(_colbox4, tmp_colbox4, 9) != 0)
		persist_write_string(COLBOX4_PKEY, _colbox4);
	char tmp_txtbox4[9]; 
	persist_read_string(TXTBOX4_PKEY, tmp_txtbox4, 9);
	if(memcmp(_txtbox4, tmp_txtbox4, 9) != 0)
		persist_write_string(TXTBOX4_PKEY, _txtbox4);
	persist_read_int(BOX1_PKEY) != _box1 ? persist_write_int(BOX1_PKEY, _box1) : false;
	persist_read_int(BOX2_PKEY) != _box2 ? persist_write_int(BOX2_PKEY, _box2) : false;
	persist_read_int(BOX3_PKEY) != _box3 ? persist_write_int(BOX3_PKEY, _box3) : false;
	persist_read_int(BOX4_PKEY) != _box4 ? persist_write_int(BOX4_PKEY, _box4) : false;
	persist_read_bool(BIG1_PKEY) != _big1 ? persist_write_bool(BIG1_PKEY, _big1) : false;
	persist_read_bool(BIG2_PKEY) != _big2 ? persist_write_bool(BIG2_PKEY, _big2) : false;
	persist_read_bool(BIG3_PKEY) != _big3 ? persist_write_bool(BIG3_PKEY, _big3) : false;
	persist_read_bool(BIG4_PKEY) != _big4 ? persist_write_bool(BIG4_PKEY, _big4) : false;
}