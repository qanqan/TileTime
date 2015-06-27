#include <pebble.h>
#include "string.h"

#define YEAR_SIZE 4
#define DAY_SIZE 3
#define DATE_SIZE 24
#define DOW_SIZE 3
#define HOUR_SIZE 3
#define MINUTE_SIZE 3

//Define days of the week
static const char* const DOWS[] = {
  "Zo",
  "Ma",
  "Di",
  "Wo",
  "Do",
  "Vr",
  "Za"
};

//Define months of the year
static const char* const MONS[] = {
  "Jan",
  "Feb",
  "Maart",
  "April",
  "Mei",
  "Juni",
  "Juli",
  "Aug",
  "Sept",
  "Okt",
  "Nov",
  "Dec"
};

//Define window
static Window *s_main_window;

// Define boxes
typedef struct {
	TextLayer *currentLayer;
	TextLayer *nextLayer;	
  int origin;
  int border;
  int buffersize;
	PropertyAnimation *currentAnimation;
	PropertyAnimation *nextAnimation;
} Box;

static Box box1;
static Box box2;
static Box box3;
static Box box4;

//Define layers
static TextLayer *s_background_layer1;
static TextLayer *s_background_layer2;
static TextLayer *s_background_layer3;
static TextLayer *s_background_layer4;
static TextLayer *s_hour_layer;
static TextLayer *s_minute_layer;
static TextLayer *s_date_layer;
static TextLayer *s_day_layer;

// Define fonts
static GFont s_big_font;
static GFont s_small_font;

//Define time structure
static struct tm *t;

// Create a long-lived buffer
static char hour[2][HOUR_SIZE];
static char minute[2][MINUTE_SIZE];
static char dow[2][DOW_SIZE];
static char date[2][DATE_SIZE];

static void get_time_parameters(struct tm *t, char *hourstr, char *minutestr, char *datestr, char *dowstr) {
  // Determine the time information 
  int hours = t->tm_hour;
  int minutes = t->tm_min;
  int months = t->tm_mon;
  int days = t->tm_mday;
  int dow = t->tm_wday;
  int year = 1900 + t->tm_year;
  
  // Define local buffers
	memset(hourstr, 0, HOUR_SIZE);
	memset(minutestr, 0, MINUTE_SIZE);
	memset(datestr, 0, DATE_SIZE);
	memset(dowstr, 0, DOW_SIZE);
  
  snprintf(hourstr, HOUR_SIZE, "%02d",hours);
  snprintf(minutestr, MINUTE_SIZE, "%02d",minutes);
  snprintf(datestr, DATE_SIZE, "%d\n%s\n%d", days,MONS[months],year);
  snprintf(dowstr, DOW_SIZE, "%s",DOWS[dow]);
}

// Animation handler
static void animationStoppedHandler(struct Animation *animation, bool finished, void *context)
{
	Layer *textLayer = text_layer_get_layer((TextLayer *)context);
	GRect rect = layer_get_frame(textLayer);
	//rect.origin.x = 144;
	layer_set_frame(textLayer, rect);
}

// Animate box
static void makeAnimationsForLayers(Box *box, TextLayer *current, TextLayer *next)
{
	GRect fromRect = layer_get_frame(text_layer_get_layer(next));
	GRect toRect = fromRect;
	toRect.origin.x = box->origin;
	
	box->nextAnimation = property_animation_create_layer_frame(text_layer_get_layer(next), &fromRect, &toRect);
	animation_set_duration((Animation *)box->nextAnimation, 400);
	animation_set_curve((Animation *)box->nextAnimation, AnimationCurveEaseOut);
	animation_schedule((Animation *)box->nextAnimation);
	
	GRect fromRect2 = layer_get_frame(text_layer_get_layer(current));
	GRect toRect2 = fromRect2;
	toRect2.origin.x = box->border;
	
	box->currentAnimation = property_animation_create_layer_frame(text_layer_get_layer(current), &fromRect2, &toRect2);
	animation_set_duration((Animation *)box->currentAnimation, 400);
	animation_set_curve((Animation *)box->currentAnimation, AnimationCurveEaseOut);
	
	animation_set_handlers((Animation *)box->currentAnimation, (AnimationHandlers) {
		.stopped = (AnimationStoppedHandler)animationStoppedHandler
	}, current);
	
	animation_schedule((Animation *)box->currentAnimation);
}

static void update_time(struct tm *t) {
	TextLayer *next, *current;
  char hourstr[HOUR_SIZE];
  char minutestr[MINUTE_SIZE];
  char datestr[DATE_SIZE];  
  char dowstr[DOW_SIZE];
	char *currentStr,*value;
  int buffer, origin, border;
  Box *bbox;
	GRect rect;

  // Determine the time information 
  get_time_parameters(t,hourstr,minutestr,datestr,dowstr);
  
  // Update screen if nessecary
  bbox = &box1;
  value = hourstr;
  origin = bbox->origin;
  border = bbox->border;
  buffer = bbox->buffersize;
 	rect = layer_get_frame(text_layer_get_layer(bbox->currentLayer));
	currentStr = (rect.origin.x == origin) ? hour[0] : hour[1];
	if (memcmp(currentStr, value, strlen(value)) != 0 ||
		(strlen(value) == 0 && strlen(currentStr) != 0)) {
	  if (rect.origin.x == origin) {
  	  current = bbox->currentLayer;
      next = bbox->nextLayer;  
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = border;
	  	memset(hour[1], 0, buffer);
		  memcpy(hour[1], value, strlen(value));
  		text_layer_set_text(next, hour[1]);
    } else {
      rect.origin.x = border;
  	  next = bbox->currentLayer;
      current = bbox->nextLayer;      
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = origin;
  		memset(hour[0], 0, buffer);
	  	memcpy(hour[0], value, strlen(value));
		  text_layer_set_text(next, hour[0]);
    }
	  makeAnimationsForLayers(bbox, current, next);
  }

  bbox = &box2;
  value = minutestr;
  origin = bbox->origin;
  border = bbox->border;
  buffer = bbox->buffersize;
 	rect = layer_get_frame(text_layer_get_layer(bbox->currentLayer));
	currentStr = (rect.origin.x == origin) ? minute[0] : minute[1];
	if (memcmp(currentStr, value, strlen(value)) != 0 ||
		(strlen(value) == 0 && strlen(currentStr) != 0)) {
	  if (rect.origin.x == origin) {
  	  current = bbox->currentLayer;
      next = bbox->nextLayer;  
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = border;
	  	memset(minute[1], 0, buffer);
		  memcpy(minute[1], value, strlen(value));
  		text_layer_set_text(next, minute[1]);
    } else {
      rect.origin.x = border;
  	  next = bbox->currentLayer;
      current = bbox->nextLayer;      
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = origin;
  		memset(minute[0], 0, buffer);
	  	memcpy(minute[0], value, strlen(value));
		  text_layer_set_text(next, minute[0]);
    }
	  makeAnimationsForLayers(bbox, current, next);
  }

  bbox = &box3;
  value = datestr;
  origin = bbox->origin;
  border = bbox->border;
  buffer = bbox->buffersize;
 	rect = layer_get_frame(text_layer_get_layer(bbox->currentLayer));
	currentStr = (rect.origin.x == origin) ? date[0] : date[1];
	if (memcmp(currentStr, value, strlen(value)) != 0 ||
		(strlen(value) == 0 && strlen(currentStr) != 0)) {
	  if (rect.origin.x == origin) {
  	  current = bbox->currentLayer;
      next = bbox->nextLayer;  
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = border;
	  	memset(date[1], 0, buffer);
		  memcpy(date[1], value, strlen(value));
  		text_layer_set_text(next, date[1]);
    } else {
      rect.origin.x = border;
  	  next = bbox->currentLayer;
      current = bbox->nextLayer;      
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = origin;
  		memset(date[0], 0, buffer);
	  	memcpy(date[0], value, strlen(value));
		  text_layer_set_text(next, date[0]);
    }
	  makeAnimationsForLayers(bbox, current, next);
  }

  bbox = &box4;
  value = dowstr;
  origin = bbox->origin;
  border = bbox->border;
  buffer = bbox->buffersize;
 	rect = layer_get_frame(text_layer_get_layer(bbox->currentLayer));
	currentStr = (rect.origin.x == origin) ? dow[0] : dow[1];
	if (memcmp(currentStr, value, strlen(value)) != 0 ||
		(strlen(value) == 0 && strlen(currentStr) != 0)) {
	  if (rect.origin.x == origin) {
  	  current = bbox->currentLayer;
      next = bbox->nextLayer;  
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = border;
	  	memset(dow[1], 0, buffer);
		  memcpy(dow[1], value, strlen(value));
  		text_layer_set_text(next, dow[1]);
    } else {
      rect.origin.x = border;
  	  next = bbox->currentLayer;
      current = bbox->nextLayer;      
      rect = layer_get_frame(text_layer_get_layer(next));
      rect.origin.x = origin;
  		memset(dow[0], 0, buffer);
	  	memcpy(dow[0], value, strlen(value));
		  text_layer_set_text(next, dow[0]);
    }
	  makeAnimationsForLayers(bbox, current, next);
  }
}

static void initial_update_time(struct tm *t) {
  // Determine the time information 
  get_time_parameters(t,hour[0],minute[0],date[0],dow[0]);

  // Display this time on the TextLayer
  text_layer_set_text(box1.currentLayer, hour[0]);
  text_layer_set_text(box2.currentLayer, minute[0]);
  text_layer_set_text(box3.currentLayer, date[0]);
  text_layer_set_text(box4.currentLayer, dow[0]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	t = tick_time;
  update_time(t);
}

static void main_window_load(Window *window) {
  // Determine window layer
  Layer *window_layer = window_get_root_layer(window);

  // Custom Fonts
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_extrabold_60));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dosis_bold_26));

  // Create the background of the 4 boxes
  s_background_layer1 = text_layer_create(GRect( 0, 0,72,84));
  s_background_layer2 = text_layer_create(GRect(72, 0,72,84));
  s_background_layer3 = text_layer_create(GRect( 0,84,72,84));
  s_background_layer4 = text_layer_create(GRect(72,84,72,84));
  text_layer_set_background_color(s_background_layer1,GColorOrange);
  text_layer_set_background_color(s_background_layer2,GColorKellyGreen);
  text_layer_set_background_color(s_background_layer3,GColorChromeYellow);
  text_layer_set_background_color(s_background_layer4,GColorPictonBlue);
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer1));
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer2));
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer3));
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer4));

  // Create box1 - Hours
  box1.origin = 0;
  box1.border = -72;
	box1.currentLayer = text_layer_create(GRect(box1.origin,7,72,64));
	box1.nextLayer = text_layer_create(GRect(box1.border,7,72,64));
  box1.buffersize = HOUR_SIZE;
  text_layer_set_background_color(box1.currentLayer,GColorClear);
  text_layer_set_background_color(box1.nextLayer,GColorClear);
  text_layer_set_text_color(box1.currentLayer,GColorWhite);
  text_layer_set_text_color(box1.nextLayer,GColorWhite);
  text_layer_set_font(box1.currentLayer,s_big_font);  
  text_layer_set_font(box1.nextLayer,s_big_font);  
  text_layer_set_text_alignment(box1.currentLayer,GTextAlignmentCenter);
  text_layer_set_text_alignment(box1.nextLayer,GTextAlignmentCenter);
  
  // Create box2 - Minutes
  box2.origin = 72;
  box2.border = 144;
	box2.currentLayer = text_layer_create(GRect(box2.origin, 7,72,64));
	box2.nextLayer = text_layer_create(GRect(box2.border,7,72,64));
  box2.buffersize = MINUTE_SIZE;
  text_layer_set_background_color(box2.currentLayer,GColorClear);
  text_layer_set_background_color(box2.nextLayer,GColorClear);
  text_layer_set_text_color(box2.currentLayer,GColorBlack);
  text_layer_set_text_color(box2.nextLayer,GColorBlack);
  text_layer_set_font(box2.currentLayer,s_big_font);  
  text_layer_set_font(box2.nextLayer,s_big_font);  
  text_layer_set_text_alignment(box2.currentLayer,GTextAlignmentCenter);
  text_layer_set_text_alignment(box2.nextLayer,GTextAlignmentCenter);

  // Create box3 - Date
  box3.origin = 0;
  box3.border = -72;
	box3.currentLayer = text_layer_create(GRect(box3.origin,84,72,84));
	box3.nextLayer = text_layer_create(GRect(box3.border,84,72,84));
  box3.buffersize = DATE_SIZE;
  text_layer_set_background_color(box3.currentLayer,GColorClear);
  text_layer_set_background_color(box3.nextLayer,GColorClear);
  text_layer_set_text_color(box3.currentLayer,GColorBlack);
  text_layer_set_text_color(box3.nextLayer,GColorBlack);
  text_layer_set_font(box3.currentLayer,s_small_font);  
  text_layer_set_font(box3.nextLayer,s_small_font);  
  text_layer_set_text_alignment(box3.currentLayer,GTextAlignmentCenter);
  text_layer_set_text_alignment(box3.nextLayer,GTextAlignmentCenter);
  text_layer_set_overflow_mode(box3.currentLayer,GTextOverflowModeWordWrap);
  text_layer_set_overflow_mode(box3.nextLayer,GTextOverflowModeWordWrap);

  // Create box4 - Day of the week
  box4.origin = 72;
  box4.border = 144;
	box4.currentLayer = text_layer_create(GRect(box4.origin,90,72,64));
	box4.nextLayer = text_layer_create(GRect(box4.border,90,72,64));
  box4.buffersize = DOW_SIZE;
  text_layer_set_background_color(box4.currentLayer,GColorClear);
  text_layer_set_background_color(box4.nextLayer,GColorClear);
  text_layer_set_text_color(box4.currentLayer,GColorWhite);
  text_layer_set_text_color(box4.nextLayer,GColorWhite);
  text_layer_set_font(box4.currentLayer,s_big_font);  
  text_layer_set_font(box4.nextLayer,s_big_font);  
  text_layer_set_text_alignment(box4.currentLayer,GTextAlignmentCenter);
  text_layer_set_text_alignment(box4.nextLayer,GTextAlignmentCenter);
 
  // Add it as a child layer to the Window's root layer
	// Load layers
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
  // Destroy TextLayer
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

  // Unload GFont
  fonts_unload_custom_font(s_big_font);
  fonts_unload_custom_font(s_small_font);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Set background color of window
  //window_set_background_color(s_main_window, GColorRed );
  
  // Make sure the time is displayed from the start
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  initial_update_time(t);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}