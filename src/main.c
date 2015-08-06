#include <pebble.h>
#include "string.h"
#include "config.h"
#include <ctype.h>
  
static bool btConnected = false;

//Define the size of the different items
#define YEAR_SIZE 4
#define DAY_SIZE 3
#define DATE_SIZE 24
#define DOW_SIZE 8
#define DATEDOW_SIZE 30
#define HOUR_SIZE 3
#define MINUTE_SIZE 3
#define SEC_SIZE 3

//Define days of the week
static const char* const DOWS[6][7] = {
  {"Zo","Ma","Di","Wo","Do","Vr","Za"},
  {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"},
  {"Do","Lu","Ma","Me","Gi","Ve","Sa"},
  {"So","Mo","Di","Mi","Do","Fr","Sa"},
  {"Di","Lu","Ma","Me","Je","Ve","Sa"},
  {"Do","Lu","Ma","Mi","Ju","Vi","Sa"}
};

//Define months of the year
static const char* const MONS[6][12] = {
  {"Jan","Feb","Maart","April","Mei","Juni","Juli","Aug","Sept","Okt","Nov","Dec"},
  {"Jan","Feb","March","April","May","June","July","Aug","Sep","Oct","Nov","Dec"},
  {"Genn","Febbr","Mar","Apr","Mag","Giugno","Luglio","Ag","Sett","Ott","Nov","Dic"},
  {"Jan","Feb","März","Apr","Mai","Jun","Jul","Aug","Sept","Okt","Nov","Dez"},
  {"Jan","Fév","Mars","Avr","Mai","Juin","Juil","Août","Sept","Okt","Nov","Dec"},
  {"Enero","Feb","Marzo","Abr","Mayo","Jun","Jul","Agosto","Sept","Okt","Nov","Dic"}   
};

//Define window
static Window *s_main_window;

// Define boxes
typedef struct {
	TextLayer *currentLayer;
	TextLayer *nextLayer;	
  int origin;
	PropertyAnimation *currentAnimation;
	PropertyAnimation *nextAnimation;
} Box;

//Define the four boxes
static Box box1;
static Box box2;
static Box box3;
static Box box4;

//Define layers
static TextLayer *s_background_layer1;
static TextLayer *s_background_layer2;
static TextLayer *s_background_layer3;
static TextLayer *s_background_layer4;

// Define fonts
static GFont s_ext_font;
static GFont s_big_font;
static GFont s_date_small;
static GFont s_date_big;
static GFont s_dow_ext;
static GFont s_dow_big;
static GFont s_dow_big_pm;
static GFont s_dow_uk_big;
static GFont s_dow_small;
static GFont s_dow_uk_small;

//Define time structure
static struct tm *t;

// Create long-lived buffer for the time
static char hour[2][HOUR_SIZE];
static char minute[2][MINUTE_SIZE];
static char seconde[2][SEC_SIZE];
static char dow[2][DOW_SIZE];
static char datedow[2][DATEDOW_SIZE];
static char date[2][DATE_SIZE];

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

//Get the different time values to be displayed
static void get_time_parameters(struct tm* t, char* hourstr, char* minutestr, char* secondstr, char* datestr, char* dowstr, char* datedowstr) {
  // Determine the time information 
  int hours = t->tm_hour;
  int minutes = t->tm_min;
  int seconds = t->tm_sec;
  int months = t->tm_mon;
  int days = t->tm_mday;
  int dow = t->tm_wday;
  int year = 1900 + t->tm_year;
  int taal = getLanguage();
  
  // Define local buffers
	memset(hourstr, 0, HOUR_SIZE);
	memset(minutestr, 0, MINUTE_SIZE);
	memset(secondstr, 0, SEC_SIZE);
	memset(datestr, 0, DATE_SIZE);
	memset(datedowstr, 0, DATEDOW_SIZE);
	memset(dowstr, 0, DOW_SIZE);
  
  if (!getUur()) {
    if (hours>12 && hours < 24) {
      hours = hours - 12; 
      snprintf(dowstr, DOW_SIZE, "%s pm",DOWS[taal][dow]);    
    } else if (hours == 24 || hours == 0) {
      hours = 12; 
      snprintf(dowstr, DOW_SIZE, "%s am",DOWS[taal][dow]);    
    } else {
      snprintf(dowstr, DOW_SIZE, "%s am",DOWS[taal][dow]);    
    }
  } else {
    snprintf(dowstr, DOW_SIZE, "%s",DOWS[taal][dow]);       
  }
  snprintf(hourstr, HOUR_SIZE, "%02d",hours);
  snprintf(minutestr, MINUTE_SIZE, "%02d",minutes);
  snprintf(secondstr, SEC_SIZE, "%02d",seconds);
  snprintf(datestr, DATE_SIZE, "%d\n%s\n%d", days,MONS[taal][months],year);
  snprintf(datedowstr, DATEDOW_SIZE, "%s %d\n%s\n%d",DOWS[taal][dow],days,MONS[taal][months],year);
}


// Animation stopped handler
static void animationStoppedHandler(struct Animation *animation, bool finished, void *context)
{
	Layer *textLayer = text_layer_get_layer((TextLayer *)context);
	GRect rect = layer_get_frame(textLayer);
	//rect.origin.x = 144;
	layer_set_frame(textLayer, rect);
}

// Animate box
static void makeAnimationsForLayers(Box* box, TextLayer* current, TextLayer* next)
{
  int from_location = layer_get_frame(text_layer_get_layer(current)).origin.x;
  int to_location = layer_get_frame(text_layer_get_layer(next)).origin.x;
  
	GRect fromRect = layer_get_frame(text_layer_get_layer(next));
	GRect toRect = fromRect;
	toRect.origin.x = from_location;
	
	box->nextAnimation = property_animation_create_layer_frame(text_layer_get_layer(next), &fromRect, &toRect);
	animation_set_duration((Animation *)box->nextAnimation, 400);
	animation_set_curve((Animation *)box->nextAnimation, AnimationCurveEaseOut);
	animation_schedule((Animation *)box->nextAnimation);
	
	GRect fromRect2 = layer_get_frame(text_layer_get_layer(current));
	GRect toRect2 = fromRect2;
	toRect2.origin.x = to_location;
	
	box->currentAnimation = property_animation_create_layer_frame(text_layer_get_layer(current), &fromRect2, &toRect2);
	animation_set_duration((Animation *)box->currentAnimation, 400);
	animation_set_curve((Animation *)box->currentAnimation, AnimationCurveEaseOut);
	
	animation_set_handlers((Animation *)box->currentAnimation, (AnimationHandlers) {
		.stopped = (AnimationStoppedHandler)animationStoppedHandler
	}, current);
	
	animation_schedule((Animation *)box->currentAnimation);
}

//Determine if te contents of the textbox has changed
static bool must_update_box(char* current, char* new) {
 	if (memcmp(current, new, strlen(new)) != 0 || (strlen(new) == 0 && strlen(current) != 0)) return true;
  return false;
}

//Determine if the currentlayer hasfocus on the watch
static bool currentlayer_has_focus(Box* box) {
  GRect rect = layer_get_frame(text_layer_get_layer(box->currentLayer));
  if (rect.origin.x == box->origin) return true;
  return false;
}

//Determine value of the box
static char* box_value(BoxFunction fun, char* hourstr, char* minutestr, char* secondstr, char* datestr, char* dowstr, char* datedowstr) {
  switch (fun) {
    case 0: return hourstr;
    case 1: return minutestr;
    case 2: return secondstr;
    case 3: return datestr;
    case 4: return dowstr;
    case 5: return " ";
    case 6: return datedowstr;
  }
  return " ";
}

// Update the value of the boxes 
static void update_box(Box* bbox, BoxFunction fun, char* hourstr, char* minutestr, char* secondstr, char* datestr, char* dowstr, char* datedowstr) {
  bool hasfocus = currentlayer_has_focus(bbox);   
	TextLayer* current = (hasfocus) ? bbox->currentLayer : bbox->nextLayer;
  TextLayer* next = (hasfocus) ? bbox->nextLayer : bbox->currentLayer;
  char* value = box_value(fun, hourstr, minutestr, secondstr, datestr, dowstr, datedowstr);   
	char *currentStr;
     
  //Update the box with the hours in it
  switch (fun) {
  case 0: 
    //Update the box with the hour value in it
    currentStr = (hasfocus) ? hour[0] : hour[1];
    if (must_update_box(currentStr, value)) {
      if (hasfocus) {
        memset(hour[1], 0, HOUR_SIZE);
        memcpy(hour[1], value, strlen(value));
        text_layer_set_text(next, hour[1]);
      } else {
        memset(hour[0], 0, HOUR_SIZE);
        memcpy(hour[0], value, strlen(value));
        text_layer_set_text(next, hour[0]);
      }
      makeAnimationsForLayers(bbox, current, next);
    }
    return;
  case 1:
    //Update the box with the minute value in it
    currentStr = (hasfocus) ? minute[0] : minute[1];
    if (must_update_box(currentStr, value)) {
      if (hasfocus) {
        memset(minute[1], 0, MINUTE_SIZE);
        memcpy(minute[1], value, strlen(value));
        text_layer_set_text(next, minute[1]);
      } else {
        memset(minute[0], 0, MINUTE_SIZE);
        memcpy(minute[0], value, strlen(value));
        text_layer_set_text(next, minute[0]);
      }
      makeAnimationsForLayers(bbox, current, next);
    }
    return;
  case 2:
    //Update the box with the seconds value in it
    currentStr = (hasfocus) ? seconde[0] : seconde[1];
    if (must_update_box(currentStr, value)) {
      if (hasfocus) {
        memset(seconde[1], 0, SEC_SIZE);
        memcpy(seconde[1], value, strlen(value));
        text_layer_set_text(next, seconde[1]);
      } else {
        memset(seconde[0], 0, SEC_SIZE);
        memcpy(seconde[0], value, strlen(value));
        text_layer_set_text(next, seconde[0]);
      }
      makeAnimationsForLayers(bbox, current, next);
    }
    return;
  case 3:
    //Update the box with the date in it
    currentStr = (hasfocus) ? date[0] : date[1];
    if (must_update_box(currentStr, value)) {
      if (hasfocus) {
        memset(date[1], 0, DATE_SIZE);
        memcpy(date[1], value, strlen(value));
        text_layer_set_text(next, date[1]);
      } else {
        memset(date[0], 0, DATE_SIZE);
        memcpy(date[0], value, strlen(value));
        text_layer_set_text(next, date[0]);
      }
      makeAnimationsForLayers(bbox, current, next);
    }
    return;
  case 4:
    //Update the box with the day of the week in it
    currentStr = (hasfocus) ? dow[0] : dow[1];
    if (must_update_box(currentStr, value)) {
      if (hasfocus) {
        memset(dow[1], 0, DOW_SIZE);
        memcpy(dow[1], value, strlen(value));
        text_layer_set_text(next, dow[1]);
      } else {
        memset(dow[0], 0, DOW_SIZE);
        memcpy(dow[0], value, strlen(value));
        text_layer_set_text(next, dow[0]);
      }
      makeAnimationsForLayers(bbox, current, next);
    }
    return;
  case 5:
    //Empty the box
    text_layer_set_text(current, " ");
    text_layer_set_text(next, " ");
    makeAnimationsForLayers(bbox, current, next);
    return;
  case 6:
    //Update the box with the date+dow in it
    currentStr = (hasfocus) ? datedow[0] : datedow[1];
    if (must_update_box(currentStr, value)) {
      if (hasfocus) {
        memset(datedow[1], 0, DATEDOW_SIZE);
        memcpy(datedow[1], value, strlen(value));
        text_layer_set_text(next, datedow[1]);
      } else {
        memset(datedow[0], 0, DATEDOW_SIZE);
        memcpy(datedow[0], value, strlen(value));
        text_layer_set_text(next, datedow[0]);
      }
      makeAnimationsForLayers(bbox, current, next);
    }
    return;
  }
}

//Updates the four tiles with the new time values 
static void update_time(struct tm *t) {
  char hourstr[HOUR_SIZE];
  char minutestr[MINUTE_SIZE];
  char secondstr[SEC_SIZE];
  char datestr[DATE_SIZE];  
  char datedowstr[DATEDOW_SIZE];
  char dowstr[DOW_SIZE];

  // Determine the time information 
  get_time_parameters(t,hourstr,minutestr,secondstr,datestr,dowstr,datedowstr);
  
  // Update screen if nessecary
  update_box(&box1, getBox1(), hourstr, minutestr, secondstr, datestr, dowstr,datedowstr);
  update_box(&box2, getBox2(), hourstr, minutestr, secondstr, datestr, dowstr,datedowstr);
  update_box(&box3, getBox3(), hourstr, minutestr, secondstr, datestr, dowstr,datedowstr);
  update_box(&box4, getBox4(), hourstr, minutestr, secondstr, datestr, dowstr,datedowstr);
}
                              
//Show the initial time after starting the app and after changing the settings.
static void initial_update_time(struct tm *t) {
  get_time_parameters(t,hour[0],minute[0],seconde[0],date[0],dow[0],datedow[0]);
  text_layer_set_text(box1.currentLayer, box_value(getBox1(),hour[0],minute[0],seconde[0],date[0],dow[0],datedow[0]));
  text_layer_set_text(box2.currentLayer, box_value(getBox2(),hour[0],minute[0],seconde[0],date[0],dow[0],datedow[0]));
  text_layer_set_text(box3.currentLayer, box_value(getBox3(),hour[0],minute[0],seconde[0],date[0],dow[0],datedow[0]));
  text_layer_set_text(box4.currentLayer, box_value(getBox4(),hour[0],minute[0],seconde[0],date[0],dow[0],datedow[0]));
}

//Time handler
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	t = tick_time;
  update_time(t);
}

//Set the default properties of the four boxes
static void default_preferences(Box* box) {
  text_layer_set_background_color(box->currentLayer,GColorClear);
  text_layer_set_background_color(box->nextLayer,GColorClear);
  text_layer_set_text_alignment(box->currentLayer,GTextAlignmentCenter);
  text_layer_set_text_alignment(box->nextLayer,GTextAlignmentCenter);
  text_layer_set_overflow_mode(box->currentLayer,GTextOverflowModeWordWrap);
  text_layer_set_overflow_mode(box->nextLayer,GTextOverflowModeWordWrap);
}

//Set the specific settings for a box given the function, the font size and the color
static void set_preferences(Box* box, BoxFunction fun, bool big, GColor kleur) {
  text_layer_set_text_color(box->currentLayer,kleur);
  text_layer_set_text_color(box->nextLayer,kleur);
  switch (fun) {
    case 0: 
      if (big) {
        text_layer_set_font(box->currentLayer,s_ext_font);  
        text_layer_set_font(box->nextLayer,s_ext_font);  
      } else {
        text_layer_set_font(box->currentLayer,s_big_font);  
        text_layer_set_font(box->nextLayer,s_big_font);          
      }
      return;
    case 1: 
      if (big) {
        text_layer_set_font(box->currentLayer,s_ext_font);  
        text_layer_set_font(box->nextLayer,s_ext_font);  
      } else {
        text_layer_set_font(box->currentLayer,s_big_font);  
        text_layer_set_font(box->nextLayer,s_big_font);          
      }
      return;
    case 2: 
      if (big) {
        text_layer_set_font(box->currentLayer,s_ext_font);  
        text_layer_set_font(box->nextLayer,s_ext_font);  
      } else {
        text_layer_set_font(box->currentLayer,s_big_font);  
        text_layer_set_font(box->nextLayer,s_big_font);          
      }
      return;
    case 3: 
      if (big) {
        text_layer_set_font(box->currentLayer,s_date_big);  
        text_layer_set_font(box->nextLayer,s_date_big);  
      } else {
        text_layer_set_font(box->currentLayer,s_date_small);  
        text_layer_set_font(box->nextLayer,s_date_small);          
      }
      return;
    case 4: 
      if (getLanguage()==1) {
        if (big) {
          text_layer_set_font(box->currentLayer,s_dow_uk_big);  
          text_layer_set_font(box->nextLayer,s_dow_uk_big);  
        } else {
          text_layer_set_font(box->currentLayer,s_dow_uk_small);  
          text_layer_set_font(box->nextLayer,s_dow_uk_small);  
        }
      } else {
        if (big) {
          if (getUur()) {
            text_layer_set_font(box->currentLayer,s_dow_ext);  
            text_layer_set_font(box->nextLayer,s_dow_ext);  
          } else {
            text_layer_set_font(box->currentLayer,s_dow_big_pm);  
            text_layer_set_font(box->nextLayer,s_dow_big_pm);              
          }
        } else {
          if (getUur()) {
            text_layer_set_font(box->currentLayer,s_dow_big);  
            text_layer_set_font(box->nextLayer,s_dow_big);              
          } else {
            text_layer_set_font(box->currentLayer,s_dow_small);  
            text_layer_set_font(box->nextLayer,s_dow_small);  
          }
        }
      }
      return;
    case 5: return;
    case 6: 
      if (big) {
        text_layer_set_font(box->currentLayer,s_date_big);  
        text_layer_set_font(box->nextLayer,s_date_big);  
      } else {
        text_layer_set_font(box->currentLayer,s_date_small);  
        text_layer_set_font(box->nextLayer,s_date_small);          
      }
      return;
  } 
}

//Stating main window of the app
static void main_window_load(Window *window) {
  // Determine window layer
  Layer *window_layer = window_get_root_layer(window);

  // Create the background of the 4 boxes
  s_background_layer1 = text_layer_create(GRect( 0, 0,72,84));
  s_background_layer2 = text_layer_create(GRect(72, 0,72,84));
  s_background_layer3 = text_layer_create(GRect( 0,84,72,84));
  s_background_layer4 = text_layer_create(GRect(72,84,72,84));

  // Define the four backgrounds of the four boxes
  text_layer_set_background_color(s_background_layer1,GColorFromHEXSTR(getColbox1()));
  text_layer_set_background_color(s_background_layer2,GColorFromHEXSTR(getColbox2()));
  text_layer_set_background_color(s_background_layer3,GColorFromHEXSTR(getColbox3()));
  text_layer_set_background_color(s_background_layer4,GColorFromHEXSTR(getColbox4()));
  
  // Add the backgrounds to the window
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer1));
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer2));
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer3));
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer4));

  // Create box1
  box1.origin = 0;
	box1.currentLayer = text_layer_create(GRect(box1.origin,4,72,80));
	box1.nextLayer = text_layer_create(GRect(-72,4,72,80));
  default_preferences(&box1);
  set_preferences(&box1,getBox1(),getBig1(),GColorFromHEXSTR(getTxtbox1()));
  
  // Create box2
  box2.origin = 72;
	box2.currentLayer = text_layer_create(GRect(box2.origin,4,72,80));
	box2.nextLayer = text_layer_create(GRect(144,4,72,80));
  default_preferences(&box2);
  set_preferences(&box2,getBox2(),getBig2(),GColorFromHEXSTR(getTxtbox2()));

  // Create box3
  box3.origin = 0;
	box3.currentLayer = text_layer_create(GRect(box3.origin,88,72,80));
	box3.nextLayer = text_layer_create(GRect(-72,88,72,80));
  default_preferences(&box3);
  set_preferences(&box3,getBox3(),getBig3(),GColorFromHEXSTR(getTxtbox3()));

  // Create box4
  box4.origin = 72;
	box4.currentLayer = text_layer_create(GRect(box4.origin,88,72,80));
	box4.nextLayer = text_layer_create(GRect(144,88,72,80));
  default_preferences(&box4);
  set_preferences(&box4,getBox4(),getBig4(),GColorFromHEXSTR(getTxtbox4()));

  // Add the four boxes to the window
	layer_add_child(window_layer, text_layer_get_layer(box1.currentLayer));
	layer_add_child(window_layer, text_layer_get_layer(box1.nextLayer));
	layer_add_child(window_layer, text_layer_get_layer(box2.currentLayer));
	layer_add_child(window_layer, text_layer_get_layer(box2.nextLayer));
	layer_add_child(window_layer, text_layer_get_layer(box3.currentLayer));
	layer_add_child(window_layer, text_layer_get_layer(box3.nextLayer));
	layer_add_child(window_layer, text_layer_get_layer(box4.currentLayer));
	layer_add_child(window_layer, text_layer_get_layer(box4.nextLayer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayers
  text_layer_destroy(s_background_layer1);
  text_layer_destroy(s_background_layer2);
  text_layer_destroy(s_background_layer3);
  text_layer_destroy(s_background_layer4);
	text_layer_destroy(box1.currentLayer);
	text_layer_destroy(box1.nextLayer);
	text_layer_destroy(box2.currentLayer);
	text_layer_destroy(box2.nextLayer);
	text_layer_destroy(box3.currentLayer);
	text_layer_destroy(box3.nextLayer);
	text_layer_destroy(box4.currentLayer);
	text_layer_destroy(box4.nextLayer);
}

// Handler for updating the settings of the boxen after the configscreen closes with save
static void in_received_handler(DictionaryIterator *iter, void *context) {
  // call autoconf_in_received_handler
	config_in_received_handler(iter, context);

  text_layer_set_background_color(s_background_layer1,GColorFromHEXSTR(getColbox1()));
  text_layer_set_background_color(s_background_layer2,GColorFromHEXSTR(getColbox2()));
  text_layer_set_background_color(s_background_layer3,GColorFromHEXSTR(getColbox3()));
  text_layer_set_background_color(s_background_layer4,GColorFromHEXSTR(getColbox4()));
  set_preferences(&box1,getBox1(),getBig1(),GColorFromHEXSTR(getTxtbox1()));
  set_preferences(&box2,getBox2(),getBig2(),GColorFromHEXSTR(getTxtbox2()));
  set_preferences(&box3,getBox3(),getBig3(),GColorFromHEXSTR(getTxtbox3()));
  set_preferences(&box4,getBox4(),getBig4(),GColorFromHEXSTR(getTxtbox4()));

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  initial_update_time(t);
}

//Bleutooth handler
static void bluetooth_connection_handler(bool connected){
	btConnected = connected;
}

//Initialization of the app
static void init() {
  // Custom Fonts
  s_ext_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_extrabold_60));
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_bold_55));
  s_dow_uk_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_medium_30));
  s_dow_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_bold_30));
  s_dow_uk_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_bold_36));
  s_dow_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_extrabold_44));
  s_dow_big_pm = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_extrabold_36));
  s_dow_ext = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_extrabold_50));
  s_date_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_bold_23));
  s_date_big = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_bold_26));

  //init the configscreen
  config_init();

  // Create main Window element and assign to pointer
  s_main_window = window_create();
  // APP_LOG(APP_LOG_LEVEL_INFO, "Main window created.");

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  app_message_register_inbox_received(in_received_handler);
  btConnected = bluetooth_connection_service_peek();
  bluetooth_connection_service_subscribe(bluetooth_connection_handler);

  // Make sure the time is displayed from the start
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  initial_update_time(t);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

// Cleanup
static void deinit() {
  // Stop config screen
  config_deinit();
  bluetooth_connection_service_unsubscribe();

  // Destroy Window
  window_destroy(s_main_window);
  // APP_LOG(APP_LOG_LEVEL_INFO, "Main window destroyed.");

  // Unload GFont
  fonts_unload_custom_font(s_ext_font);
  fonts_unload_custom_font(s_big_font);
  fonts_unload_custom_font(s_dow_ext);
  fonts_unload_custom_font(s_dow_big);
  fonts_unload_custom_font(s_dow_big_pm);
  fonts_unload_custom_font(s_dow_small);
  fonts_unload_custom_font(s_dow_uk_big);
  fonts_unload_custom_font(s_dow_uk_small);
  fonts_unload_custom_font(s_date_big);
  fonts_unload_custom_font(s_date_small);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}