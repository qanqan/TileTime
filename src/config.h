#include <pebble.h>

typedef enum {
	LANGUAGE_NL,
	LANGUAGE_EN,
	LANGUAGE_IT,
	LANGUAGE_DU,
	LANGUAGE_FR,
	LANGUAGE_SP
} LanguageValue;

typedef enum {
	FN_UUR,
	FN_MIN,
	FN_SEC,
  FN_DAT,
  FN_DATDOW,
  FN_DOW,
  FN_EMPTY
} BoxFunction;


#define LANGUAGE_PKEY 0
LanguageValue getLanguage();

#define BOX1_PKEY 10
BoxFunction getBox1();

#define BOX2_PKEY 11
BoxFunction getBox2();

#define BOX3_PKEY 12
BoxFunction getBox3();

#define BOX4_PKEY 13
BoxFunction getBox4();

#define BIG1_PKEY 14
bool getBig1();

#define BIG2_PKEY 15
bool getBig2();

#define BIG3_PKEY 16
bool getBig3();

#define BIG4_PKEY 17
bool getBig4();

#define UUR_PKEY 9
bool getUur();

#define COLBOX1_PKEY 1
char* getColbox1();

#define TXTBOX1_PKEY 5
char* getTxtbox1();

#define COLBOX2_PKEY 2
char* getColbox2();

#define TXTBOX2_PKEY 6
char* getTxtbox2();

#define COLBOX3_PKEY 3
char* getColbox3();

#define TXTBOX3_PKEY 7
char* getTxtbox3();

#define COLBOX4_PKEY 4
char* getColbox4();

#define TXTBOX4_PKEY 8
char* getTxtbox4();


void config_in_received_handler(DictionaryIterator *iter, void *context); 

void config_init();

void config_deinit();