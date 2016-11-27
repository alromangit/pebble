#include <pebble.h>
#include <inttypes.h>

static struct SeviBusUI {
  Window *s_main_window;
  Window *s_after_splash_window;
  TextLayer *s_splash_textlayer;
  StatusBarLayer *s_status_bar;
} ui;

const int inbox_size = 128;
const int outbox_size = 128;

void setup_status_bar() {
  ui.s_status_bar = status_bar_layer_create();
  status_bar_layer_set_colors(ui.s_status_bar, GColorWhite, GColorRed);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read tuples for data
  Tuple *latitude_tuple = dict_find(iterator, MESSAGE_KEY_LATITUDE );
  Tuple *longitude_tuple = dict_find(iterator, MESSAGE_KEY_LONGITUDE);
  
  // If all data is available, use it
  APP_LOG(APP_LOG_LEVEL_INFO, "Latitude value is %s", latitude_tuple->value->cstring);
  APP_LOG(APP_LOG_LEVEL_INFO, "Longitude value is %s", longitude_tuple->value->cstring);
  
  window_stack_remove(ui.s_main_window,true);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void show_splash_screen(Window *window) {
  Layer *splash_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(splash_layer);

  ui.s_splash_textlayer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58,52), bounds.size.w, 50));
  window_set_background_color(ui.s_main_window, GColorRed);
  text_layer_set_background_color(ui.s_splash_textlayer, GColorRed);
  text_layer_set_text(ui.s_splash_textlayer, "Sevibus");
  text_layer_set_text_color(ui.s_splash_textlayer, GColorWhite);
  text_layer_set_font(ui.s_splash_textlayer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(ui.s_splash_textlayer, GTextAlignmentCenter);
  layer_add_child(splash_layer, text_layer_get_layer(ui.s_splash_textlayer));
  
  //creating and setting status bar
  setup_status_bar();
  layer_add_child(splash_layer, status_bar_layer_get_layer(ui.s_status_bar)); 
}

static void main_window_load(Window *window) {
  show_splash_screen(window);
}

static void main_window_unload(Window *window) {

}

static void init() {
  ui.s_main_window = window_create();

  window_set_window_handlers(ui.s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(inbox_size, outbox_size);
  window_stack_push(ui.s_main_window, true);
}

static void deinit() {
  window_destroy(ui.s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
