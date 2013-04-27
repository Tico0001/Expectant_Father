/*
The watch face only tells you how far along your wife is in the pregnancy in terms of weeks. And the size of the baby compared to a fruit. 
This is my first watch face and my first program in C. The code could be better. I couldn't get mktime() to work so I had to come up with a different way to subtract dates.
You need to enter the approximate conception date and recompile. You can also modify the code to enter the name of your wife and future baby.

*/


#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x33, 0x5B, 0x18, 0x34, 0x16, 0x52, 0x47, 0xC9, 0xB8, 0x11, 0xE4, 0xC2, 0x20, 0x65, 0xB3, 0xF5 }
PBL_APP_INFO(MY_UUID,
             "Expectant Father", "Francisco Marambio",
             3, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

//conception date
int year_concep = 2012, month_concep =11, day_concep =30;

Window window;

TextLayer wifeLayer;
TextLayer weekLayer;
TextLayer papaLayer;
TextLayer compLayer;
TextLayer fruitLayer;


//starting at 4 weeks
char* comparisons[] = {
  "  size of a ", 
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ", 
  "  size of a ", 
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  size of a ",
  "  weight of a ",
  "  length of a ",
  "  length of a ",
  "  length of a ",//20
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  length of a ",
  "  weight of a ",
  "  length of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  weight of a ",
  "  length of a ",
  "  length of a ",
  "  weight of a ",
  "  size of a "
};

char* fruits[] = {
  "    a dot",
  "    a dot", 
  "    a dot", 
  "    a dot", 
  "    poppy seed",
  "   sesame seed",
  "    lentil",
  "    blueberry",
  "    bean",
  "    grape",
  "    kumquat",
  "    fig",
  "    lime",
  "    shrimp",
  "    lemon",
  "    apple",
  "    avocado",
  "    turnip",
  "    bell pepper",
  "    mango",
  "    Banana",//20
  "    Pomegranate",
  "    Papaya",
  "    Grapefruit",
  "    Ear of Corn",
  "    Cauliflower",
  "    Lettuce",
  "    Rutabaga",
  "    Eggplant",
  "    Butternut",
  "    Cucumber",
  "    Pineapple",
  "    Jicama",
  "    Durian",
  "    Cantaloupe",
  "    Honeydew",
  "    Big Coconut",
  "    Leek",
  "    Rhubarb",
  "Mini Watermelon",
  "    Small Pumpkin"
};


void itoa2(int num, char* buffer) {
    const char digits[10] = "0123456789";
    if(num > 99) {
        buffer[0] = '9';
        buffer[1] = '9';
        return;
    } else if(num > 9) {
        buffer[0] = digits[num / 10];
    } else {
        buffer[0] = '0';
    }
    buffer[1] = digits[num % 10];
}

void replacetxt(char str[], char* buffer) {
    for (unsigned int i = 0; i < strlen(str); i++)
    {
	buffer[i] = str[i];
    }
}

//convert date to y_day
long day_year(int d, int m, int y){
	m = (m + 9) % 12; 
	y = y - m/10; 
	return 365*y + y/4 - y/100 + y/400 + (m*306 + 5)/10 + ( d - 1 );

}


// Weeks since conception date
long get_weeks() {
	PblTm t;
	get_time(&t);

	long diff_days;
	
	diff_days = day_year(t.tm_mday, t.tm_mon+1, t.tm_year+1900) - day_year(day_concep, month_concep, year_concep);

    	return diff_days/7;
	
}

// Called once per minute
void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)t;
  (void)ctx;

  static char wifeText[] = "My wife is    weeks pregnant."; 
  static char weekText[] = "XX";
  static char papaText[] = "  The baby is the"; 
  static char compText[] = "              ";
  static char fruitText[] = "                 "; 

  int weeks = get_weeks();
  
  itoa2(get_weeks(), &weekText[0]);
  replacetxt(comparisons[weeks],&compText[0]);
  replacetxt(fruits[weeks],&fruitText[0]);

  text_layer_set_text(&wifeLayer,wifeText);
  text_layer_set_text(&weekLayer,weekText);
  text_layer_set_text(&papaLayer,papaText);
  text_layer_set_text(&compLayer,compText);
  text_layer_set_text(&fruitLayer,fruitText);
}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorWhite);

  //resource_init_current_app(&APP_RESOURCES);

  // Init the text layer used to show the weeks
  text_layer_init(&weekLayer, GRect(80, 0, 144-20 /* width */, 168-54 /* height */));
  text_layer_set_text_color(&weekLayer, GColorBlack);
  text_layer_set_background_color(&weekLayer, GColorClear);
  text_layer_set_font(&weekLayer, fonts_get_system_font(FONT_KEY_GOTHAM_34_MEDIUM_NUMBERS));

  // Init the text layer used to show the wife
  text_layer_init(&wifeLayer, GRect(5, 10, 144-20 /* width */, 168-54 /* height */));
  text_layer_set_text_color(&wifeLayer, GColorBlack);
  text_layer_set_background_color(&wifeLayer, GColorClear);
  text_layer_set_font(&wifeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Init the text layer used to show the baby
  text_layer_init(&papaLayer, GRect(0, 70, 144 /* width */, 168-54 /* height */));
  text_layer_set_text_color(&papaLayer, GColorWhite);
  text_layer_set_background_color(&papaLayer, GColorBlack);
  text_layer_set_font(&papaLayer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(&papaLayer, GTextAlignmentCenter);

  // Init the text layer used to show the comparison
  text_layer_init(&compLayer, GRect(0, 97, 144 /* width */, 168-54 /* height */));
  text_layer_set_text_color(&compLayer, GColorWhite);
  text_layer_set_background_color(&compLayer, GColorBlack);
  text_layer_set_font(&compLayer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(&compLayer, GTextAlignmentCenter);

  // Init the text layer used to show the fruit
  text_layer_init(&fruitLayer, GRect(0, 125, 144 /* width */, 168-54 /* height */));
  text_layer_set_text_color(&fruitLayer, GColorWhite);
  text_layer_set_background_color(&fruitLayer, GColorBlack);
  text_layer_set_font(&fruitLayer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(&fruitLayer, GTextAlignmentCenter);
 
  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  handle_minute_tick(ctx, NULL);

  layer_add_child(&window.layer, &wifeLayer.layer);
  layer_add_child(&window.layer, &weekLayer.layer);
  layer_add_child(&window.layer, &papaLayer.layer);
  layer_add_child(&window.layer, &compLayer.layer);
  layer_add_child(&window.layer, &fruitLayer.layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    // Handle time updates
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
