#include <pebble.h>

#define VERSION_PKEY 19
#define LANGUAGE_PKEY 20
#define COLBOX1_PKEY 21
#define COLBOX2_PKEY 22
#define COLBOX3_PKEY 23
#define COLBOX4_PKEY 24
#define TXTBOX1_PKEY 25
#define TXTBOX2_PKEY 26
#define TXTBOX3_PKEY 27
#define TXTBOX4_PKEY 28
#define TIJD_PKEY 29
#define BOX1_PKEY 30
#define BOX2_PKEY 31
#define BOX3_PKEY 32
#define BOX4_PKEY 33
#define BIG1_PKEY 34
#define BIG2_PKEY 35
#define BIG3_PKEY 36
#define BIG4_PKEY 37

/* typedef enum {
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
} BoxFunction;*/

int getLanguage();
int getBox1();
int getBox2();
int getBox3();
int getBox4();
bool getBig1();
bool getBig2();
bool getBig3();
bool getBig4();
bool getTijd();
GColor getColorbox1();
GColor getColorbox2();
GColor getColorbox3();
GColor getColorbox4();
GColor getTxtcolorbox1();
GColor getTxtcolorbox2();
GColor getTxtcolorbox3();
GColor getTxtcolorbox4();

void config_in_received_handler(DictionaryIterator *iter, void *context); 
void config_init();
bool is_box_function_second_used();
