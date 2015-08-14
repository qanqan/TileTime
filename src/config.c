#include <pebble.h>
#include "config.h"

static int _version;
static int _language;
static int _box1;
static int _box2;
static int _box3;
static int _box4;
static bool _big1;
static bool _big2;
static bool _big3;
static bool _big4;
static bool _tijd;
static int _colbox1;
static int _txtbox1;
static int _colbox2;
static int _txtbox2;
static int _colbox3;
static int _txtbox3;
static int _colbox4;
static int _txtbox4;

//htoi converts a hex string to an integer
int htoi(const char *s, unsigned int *res) {
  if ('0' == s[0] && ('x' == s[1] || 'X' == s[1]))
  s += 2;
  int c;
  unsigned int rc;
  for (rc = 0; '\0' != (c = *s); s++) {
    if ( c >= 'a' && c <= 'f') {
      c = c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      c = c - 'A' + 10;
    } else if (c >= '0' && c <= '9') {
      c = c - '0';
    } else {
      return -1;
    }
    rc *= 16;
    rc += (unsigned int) c;
  }
  *res = rc;
  return 0;
}

//GColorfromHEXSTR converts an hexstring to a GColor value.
GColor GColorFromHEXSTR(char const* hexstring) {
  unsigned int x;
  if (0 == htoi(hexstring,&x)) {
    unsigned int red = (x & 0xff0000) >> 16;
    unsigned int green = (x & 0x00ff00) >> 8;
    unsigned int blue = (x & 0x0000ff) >> 0;
    return GColorFromRGB(red,green,blue);
  }
  return GColorClear;
}

int getVersion(){return _version;}
void setVersion(int value){_version = value;}

int getLanguage(){return _language;}
void setLanguage(int value){_language = value;}

int getBox1(){return _box1;}
void setBox1(int value){_box1 = value;}

int getBox2(){return _box2;}
void setBox2(int value){_box2 = value;}

int getBox3(){return _box3;}
void setBox3(int value){_box3 = value;}

int getBox4(){return _box4;}
void setBox4(int value){_box4 = value;}

bool getBig1(){return _big1;}
void setBig1(bool value){_big1 = value;}

bool getBig2(){return _big2;}
void setBig2(bool value){_big2 = value;}

bool getBig3(){return _big3;}
void setBig3(bool value){_big3 = value;}

bool getBig4(){return _big4;}
void setBig4(bool value){_big4 = value;}

bool getTijd(){return _tijd;}
void setTijd(bool value){_tijd = value;}

GColor getColorbox1(){return GColorFromHEX(_colbox1);}
int getColbox1(){return _colbox1;}
void setColbox1(int value){_colbox1 = value;}

GColor getColorbox2(){return GColorFromHEX(_colbox2);}
int getColbox2(){return _colbox2;}
void setColbox2(int value){_colbox2 = value;}

GColor getColorbox3(){return GColorFromHEX(_colbox3);}
int getColbox3(){return _colbox3;}
void setColbox3(int value){_colbox3 = value;}

GColor getColorbox4(){return GColorFromHEX(_colbox4);}
int getColbox4(){return _colbox4;}
void setColbox4(int value){_colbox4 = value;}

GColor getTxtcolorbox1(){return GColorFromHEX(_txtbox1);}
int getTxtbox1(){return _txtbox1;}
void setTxtbox1(int value){_txtbox1 = value;}

GColor getTxtcolorbox2(){return GColorFromHEX(_txtbox2);}
int getTxtbox2(){return _txtbox2;}
void setTxtbox2(int value){_txtbox2 = value;}

GColor getTxtcolorbox3(){return GColorFromHEX(_txtbox3);}
int getTxtbox3(){return _txtbox3;}
void setTxtbox3(int value){_txtbox3 = value;}

GColor getTxtcolorbox4(){return GColorFromHEX(_txtbox4);}
int getTxtbox4(){return _txtbox4;}
void setTxtbox4(int value){_txtbox4 = value;}

bool is_box_function_second_used() {
  if (_box1 == 2) return true;
  if (_box2 == 2) return true;
  if (_box3 == 2) return true;
  if (_box4 == 2) return true;
  return false;
}

// Set value for language attributes
void configInt(DictionaryIterator *iter, int key, void (*function)(int)) {
  Tuple *tuple = dict_find(iter, key);
 
  //APP_LOG(APP_LOG_LEVEL_INFO, "Handling key %d",key);
  if (tuple) {
    int tmp = tuple->value->int32;
    //APP_LOG(APP_LOG_LEVEL_INFO, "Key %d has value %d",key,tmp);
    (*function)(tmp);
    persist_write_int(key,tmp);
    //APP_LOG(APP_LOG_LEVEL_INFO, "Stored Key %d is now %d",key,(int)persist_read_int(key));
  }
}

// Set value for boolean attributes
void configBool(DictionaryIterator *iter, int key, void (*function)(bool)) {
  Tuple *tuple = dict_find(iter, key);

  //APP_LOG(APP_LOG_LEVEL_INFO, "Handling key %d",key);
  if (tuple) {
    bool tmp = tuple->value->int8;
    //APP_LOG(APP_LOG_LEVEL_INFO, "Key %d has value %s",key,tmp ? "true" : "false");
    (*function)(tmp);
    persist_write_bool(key,tmp);
    //APP_LOG(APP_LOG_LEVEL_INFO, "Stored Key %d is now %s",key,persist_read_bool(key) ? "true" : "false");
  }
}

// Handle the return values from pebble-js-app.js
void config_in_received_handler(DictionaryIterator *iter, void *context) {
  configInt(iter,LANGUAGE_PKEY,setLanguage);
  configBool(iter,TIJD_PKEY,setTijd);
  configBool(iter,BIG1_PKEY,setBig1);
  configBool(iter,BIG2_PKEY,setBig2);
  configBool(iter,BIG3_PKEY,setBig3);
  configBool(iter,BIG4_PKEY,setBig4);
  configInt(iter,COLBOX1_PKEY,setColbox1);
  configInt(iter,COLBOX2_PKEY,setColbox2);
  configInt(iter,COLBOX3_PKEY,setColbox3);
  configInt(iter,COLBOX4_PKEY,setColbox4);
  configInt(iter,TXTBOX1_PKEY,setTxtbox1);
  configInt(iter,TXTBOX2_PKEY,setTxtbox2);
  configInt(iter,TXTBOX3_PKEY,setTxtbox3);
  configInt(iter,TXTBOX4_PKEY,setTxtbox4);
  configInt(iter,BOX1_PKEY,setBox1);
  configInt(iter,BOX2_PKEY,setBox2);
  configInt(iter,BOX3_PKEY,setBox3);
  configInt(iter,BOX4_PKEY,setBox4);
}

// Set value for language attributes
void setInt(int key, int value, void (*function)(int)) {
 	if (persist_exists(key)) {
    int tmp = persist_read_int(key);
		(*function)(tmp);
	}	else {
		(*function)(value);
	}
}

// Set value for language attributes
void setBool(int key, bool value, void (*function)(bool)) {
 	if (persist_exists(key)) {
    bool tmp = persist_read_bool(key);
		(*function)(tmp);
	}	else {
		(*function)(value);
	}
}

// Set value for language attributes
void setColor(int key, char* value, void (*function)(char*)) {
 	if (persist_exists(key)) {
	  char tmp[9];
    persist_read_string(key, tmp, 9);
		(*function)(tmp);
	}	else {
		(*function)(value);
	}
}

// Housekeeping for previous versions
void cleanupStorage() {
  int key;
  
 	if (persist_exists(VERSION_PKEY)) {
    int version = persist_read_int(VERSION_PKEY);
    if (version < 26) {
      key = 0;
      do {
 	      if (persist_exists(key)) persist_delete(key);
      } while (key++ < 18);
      setVersion(26);
      persist_write_int(VERSION_PKEY,26);
    }  
    if (version < 27) {
      // RESERVED
    }                                   
  }  
}

void config_init(){
  cleanupStorage();
  setInt(LANGUAGE_PKEY,0,setLanguage);
  setInt(BOX1_PKEY,0,setBox1);
  setInt(BOX2_PKEY,1,setBox2);
  setInt(BOX3_PKEY,3,setBox3);
  setInt(BOX4_PKEY,4,setBox4);
  setBool(TIJD_PKEY,true,setTijd);
  setBool(BIG1_PKEY,true,setBig1);
  setBool(BIG2_PKEY,true,setBig2);
  setBool(BIG3_PKEY,true,setBig3);
  setBool(BIG4_PKEY,true,setBig4);
  setInt(COLBOX1_PKEY,0xFF5500,setColbox1);
  setInt(COLBOX2_PKEY,0x55AA00,setColbox2);
  setInt(COLBOX3_PKEY,0xFFAA00,setColbox3);
  setInt(COLBOX4_PKEY,0x55AAFF,setColbox4);
  setInt(TXTBOX1_PKEY,0xFFFFFF,setTxtbox1);
  setInt(TXTBOX2_PKEY,0x000000,setTxtbox2);
  setInt(TXTBOX3_PKEY,0x000000,setTxtbox3);
  setInt(TXTBOX4_PKEY,0xFFFFFF,setTxtbox4);
}


