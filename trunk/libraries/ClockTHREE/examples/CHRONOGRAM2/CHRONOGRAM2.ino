/*
  Delivery ClockTHREE Word Clock App
  Display time 12:12 PM in words using english template
  Supports different modes:
  Normal
  Mode (Mode to switch modes)
  SetTime
  SetColor
  Serial

  Serial interface for interacting with computers supported.

  Justin Shaw Dec 22, 2010
  
  Licenced under Creative Commons Attribution.
  Attribution 3.0

*/

#include <avr/pgmspace.h>
#include <Wire.h>
#include <string.h>
#include "Time.h"
#include "MsTimer2.h"
#include "ClockTHREE.h"
#include "C3JR_Driver.h"
#include "SPI.h"

#include "english2_v1.h"

#include "mem_font.h"
#include "rtcBOB.h"

// debounce buttons threshold
const uint8_t DEBOUNCE_THRESH = 200;     // Two button pushes within this time frame are counted only once.
const uint16_t SERIAL_TIMEOUT_MS = 1000; // Not working as expected.  Turned off.
const uint8_t RIGHT_ID = C3JR_BASE_ADDR + 1;

uint32_t primary_display[32];
uint32_t secondary_display[32];
ClockTHREE left;                      // ClockTHREE singleton (Left had display)
C3JR_Driver right;                  // Right hand display as seen from front


// Define modes
typedef void (* CallBackPtr)(); // this is a typedef for callback funtions
inline void do_nothing(void){}  // empty call back
void clear_display(uint32_t *display){
  for(uint8_t i = 0; i < 32; i++){
    display[i] = 0;
  }
}
void fadeto(uint32_t *display, uint8_t steps){ 
  right.fadeto(display + 16, steps);
  left.fadeto(display, steps);
}

/*
 * ClockTHREE is mode driven.  A Mode is like a mini arduino program 
 * with its own setup and loop functions as well as an exit function
 * to clean up and prevent memory leeks.  Only one mode can be 
 * active at a time.  The active mode defines behaviors for events 
 * (like button pushes).  The switchmode function should be called 
 * to change modes.
 */
struct Mode{
  uint8_t id;        // Mode ID
  char sym;          // ASCII Symbol for mode (used to display mode in Mode mode)
  CallBackPtr setup; // to be called when Mode is initialized
  CallBackPtr loop;  // to be called as often as possible when mode is active
  CallBackPtr exit;  // to be called when mode is exited.
  CallBackPtr inc;   // to be called when increment button is pushed
  CallBackPtr dec;   // to be called when decrement button is pushed
  CallBackPtr mode;  // to be called when mode button is pushed
  CallBackPtr enter;   // to be called when enter button is pushed
};

/* The display memory buffer is larger than physical display for screen 
 * staging and fast display.
 */
const uint8_t N_DISPLAY = 3;

// Default display -- N_DISPLAY larger than physical display.
uint32_t *display = (uint32_t*)calloc(N_DISPLAY * N_COL + 2, sizeof(uint32_t));

// active mode pointer
Mode *mode_p;

// MODE Constants
const uint8_t N_MODE = 6;
const uint8_t N_MAIN_MODE = 3;  // main modes are accessable through Mode mode. (sub modes are not).
const uint8_t N_SUB_MODE = 3;

const uint8_t NORMAL_MODE = 0;
const uint8_t SET_TIME_MODE = 1;
const uint8_t MODE_MODE = 2;

// Sub Modes (this cannot be accessed though Mode mode selection.)
const uint8_t SECONDS_MODE = 3;
const uint8_t TEMPERATURE_MODE = 4;
const uint8_t SER_DISPLAY_MODE = 5;

// Temperature unit constants
const uint8_t DEG_C = 0;
const uint8_t DEG_F = 1;

/* last_mode_id is used for returning to previous mode.  
 * Usually best to return to NORMAL_MODE, but this allows sub-modes to return
 * to the mode that invoked them.
 */
uint8_t last_mode_id = NORMAL_MODE; // the last mode clock was in...

// indexed by mode.id
Mode Modes[N_MODE];

//Time units
typedef enum {YEAR, MONTH, DAY, HOUR, MINUTE, SECOND} unit_t;
const char* TIME_CODES = "YRMODAHRMISE";

// Begin mode declarations
Mode NormalMode = {NORMAL_MODE, 
		   'N', Normal_setup, Normal_loop, Normal_exit, 
		   Normal_inc, Normal_dec, Normal_mode, do_nothing};
Mode SecondsMode = {SECONDS_MODE, 
		    'S', Seconds_setup, Seconds_loop, Seconds_exit, 
		    Seconds_mode, Seconds_mode, Seconds_mode, Seconds_mode};
Mode SerDisplayMode = {SER_DISPLAY_MODE,
		       'X', do_nothing, SerDisplay_loop, do_nothing,
		       return_to_normal,return_to_normal,return_to_normal,
		       return_to_normal};

Mode TemperatureMode = {TEMPERATURE_MODE, 'X', 
			Temperature_setup, Temperature_loop, Temperature_exit, 
			Temperature_inc, Temperature_dec, Temperature_mode,
			Temperature_mode};
Mode SetTimeMode = {SET_TIME_MODE, 
		    'S', SetTime_setup,SetTime_loop,SetTime_exit,
		    SetTime_inc, SetTime_dec, SetTime_mode, SetTime_enter};
Mode ModeMode = {MODE_MODE, 
		 'M', Mode_setup, Mode_loop, Mode_exit, 
		 Mode_inc, Mode_dec, return_to_normal, Mode_enter};

/* Event types */
const uint8_t       NO_EVT = 0; // NONE
const uint8_t     MODE_EVT = 1; // Mode Button has been pressed
const uint8_t      INC_EVT = 2; // Increment Button has been pressed
const uint8_t      DEC_EVT = 3; // Decriment Button has been pressed
const uint8_t    ENTER_EVT = 4; // Enter Button has been pressed
const uint8_t     TICK_EVT = 5; // Second has ellapsed
const uint8_t EVENT_Q_SIZE = 5; // Max # events.

// Serial Messaging
struct MsgDef{
  uint8_t id;     // message type id

  /* n_byte holde the number of bytes or if n_byte == VAR_LENGTH, message type is variable length 
   * with message length in second byte of message. */
  uint8_t n_byte; 

  /* Function to call when this message type is recieved.
   * message content (header byte removed) will available to the call back function through
   * the global variabe "char serial_msg[]".
   */
  CallBackPtr cb; 
};

const uint8_t MAX_MSG_LEN = 100; // official: 100
// const uint16_t BAUDRATE = 57600; // official:57600 (Scott Schenker 38400)
const uint8_t SYNC_BYTE = 254;   // 0xEF; (does not seem to work!)
const uint8_t VAR_LENGTH = 255;  // 0xFF;
const uint8_t N_MSG_TYPE = 25;

// Message types
const MsgDef  NOT_USED_MSG = {0x00, 1, do_nothing};
const MsgDef  ABS_TIME_REQ = {0x01, 1, send_time};
const MsgDef  ABS_TIME_SET = {0x02, 5, Serial_time_set};
// const MsgDef     EVENT_REQ = {0x08, 2, do_nothing}; // not used
// const MsgDef     EVENT_SET = {0x09, 6, do_nothing}; // not used
const MsgDef   DISPLAY_REQ = {0x0A, 1, display_send};
const MsgDef   DISPLAY_SET = {0x0B, 65, display_set};
const MsgDef  TRIGGER_MODE = {0x0C, 1, mode_interrupt};
const MsgDef   TRIGGER_INC = {0x0D, 1, do_nothing};
const MsgDef   TRIGGER_DEC = {0x0E, 1, do_nothing};
const MsgDef TRIGGER_ENTER = {0x0F, 1, do_nothing};
const MsgDef   VERSION_REQ = {0x10, 1, do_nothing};
const MsgDef     ABOUT_REQ = {0x11, 1, do_nothing};
const MsgDef          PING = {0x12, MAX_MSG_LEN, pong}; // can make variable length.

const MsgDef          SYNC = {SYNC_BYTE, MAX_MSG_LEN, do_nothing}; // must already be in sync
const MsgDef      ERR_OUT = {0x71, VAR_LENGTH, do_nothing}; // variable length

// array to loop over when new messages arrive.
const MsgDef *MSG_DEFS[N_MSG_TYPE] = {&NOT_USED_MSG,
				      &ABS_TIME_REQ,
				      &ABS_TIME_SET,
				      //&EVENT_REQ,
				      //&EVENT_SET,
				      &DISPLAY_REQ,
				      &DISPLAY_SET,
				      &TRIGGER_MODE,
				      &TRIGGER_INC,
				      &TRIGGER_DEC,
				      &TRIGGER_ENTER,
				      &VERSION_REQ,
				      &ABOUT_REQ,
				      &PING,
				      };


// Globals
/*
 * This is a input/output buffer for serial communications.  
 * This buffer is also used for some non-serial processing to save memory.
 */
char serial_msg[MAX_MSG_LEN];
uint8_t serial_msg_len;       // Stores actual length of serial message.  Should be updated upon transmit or reciept.

/*
 * Array to store button push events or clock "TICK" events.
 */
uint8_t event_q[EVENT_Q_SIZE];
uint8_t n_evt = 0;                  // number of events awaiting processing
unsigned long last_mode_time = 0;   // for debounce
unsigned long last_inc_time = 0;    // for debounce
unsigned long last_dec_time = 0;    // for debounce
unsigned long last_enter_time = 0;    // for debounce

//Font font = Font();                 // Only font at this time.
MemFont font = MemFont();
time_t t;                           // TODO: remove this, not needed (I think)
uint8_t mode_counter;               // Used for selecting mode
unsigned long count = 0;            // Number of interations current mode has been running.
uint16_t YY;                        // current time variables.
uint8_t MM, DD, hh, mm, ss;
uint8_t ahh, amm, ass;              // time of day time (tod) variables.
boolean tick = true;                // tick is true whenever a second changes (false when change has been dealt with)
unit_t SetTime_unit = YEAR;         // Part of time being set {YEAR | MONTH | DAY | HOUR | MINUTE}
uint8_t temp_unit = DEG_C;          // Temperature display units {DEG_C | DEC_F}
uint8_t sync_msg_byte_counter = 0;  // How many byte in a row has sync byte been recieved 

// language constants
uint8_t n_minute_led;               // number of minute hack leds
uint8_t n_minute_state;             // number of minute hack states to cycle through
uint8_t n_byte_per_display;         // number of bytes used for each 5 minunte time incriment

uint8_t last_minute;
/*
 * Called when mode button is pressed
 */
void mode_interrupt(){
  unsigned long now = millis();
  if(now - last_mode_time > DEBOUNCE_THRESH){
    // add mode_press event to mode event queue
    if(n_evt < EVENT_Q_SIZE){
      event_q[n_evt++] = MODE_EVT;
    }
  }
  last_mode_time = now;
}

/*
 * Called when inc button is pressed
 */
void inc_interrupt(){
  unsigned long now = millis();
  if(now - last_inc_time > DEBOUNCE_THRESH){
    // add mode_press event to mode event queue
    if(n_evt < EVENT_Q_SIZE){
      event_q[n_evt++] = INC_EVT;
    }
  }
  last_inc_time = now;
}

/*
 * Called when dec button is pressed
 */
void dec_interrupt(){
  unsigned long now = millis();
  if(now - last_dec_time > DEBOUNCE_THRESH){
    // add mode_press event to mode event queue
    if(n_evt < EVENT_Q_SIZE){
      event_q[n_evt++] = DEC_EVT;
    }
  }
  last_dec_time = now;
}


/*
 * Called when dec button is pressed
 */
void enter_interrupt(){
  unsigned long now = millis();
  if(now - last_enter_time > DEBOUNCE_THRESH){
    // add mode_press event to mode event queue
    if(n_evt < EVENT_Q_SIZE){
      event_q[n_evt++] = ENTER_EVT;
    }
  }
  last_enter_time = now;
}

/*
 * Called when second boundary is crossed
 */
void tick_interrupt(){
  if(n_evt < EVENT_Q_SIZE){
    event_q[n_evt++] = TICK_EVT;
  }
}

/*
 * Three time systems are with varying degrees of processor use and accuracy.
 * YY, MM, DD ... are the least accurate and least procesor hungry
 * Time.h time -- year(), month() ... are kinda slow.
 * rtcBOB -- most accrate (and slowest?  need to verify this.)
 */
void update_time(){
  // update global time variables.  resync with Time.h
  left.refresh();  // keep display going.
  YY = year();
  left.refresh();
  MM = month();
  left.refresh();
  DD = day();
  left.refresh();
  hh = hour();
  left.refresh();
  mm = minute();
  left.refresh();
  ss = second();
  left.refresh();
}

// Main setup function
void setup(void){
  Serial.begin(BAUDRATE);
  Serial.println("CHRONOGRAM 1.0");
  Serial.println("WyoLum 2012");
  Wire.begin();
  left.init();
  left.setdisplay(primary_display);
  right.init(RIGHT_ID);
  
  mode_p = &NormalMode;

  // ensure mode ids are consistant.
  Modes[NORMAL_MODE] = NormalMode;
  Modes[SET_TIME_MODE] = SetTimeMode;
  //  Modes[SERIAL_MODE] = SerialMode;
  Modes[MODE_MODE] = ModeMode;

  // Sub Modes
  Modes[SECONDS_MODE] = SecondsMode;
  Modes[SER_DISPLAY_MODE] = SerDisplayMode;
  Modes[TEMPERATURE_MODE] = TemperatureMode;
  mode_p->setup();

  attachInterrupt(0, mode_interrupt, FALLING); // Does not work on C2
  attachInterrupt(1, inc_interrupt, FALLING);  // Does not work on C2
  // left.setdisplay(display); // 2x actual LED array size for staging data. // required by fade to
  left.set_column_hold(50);
  /********************************************************************************/
  // tests
#ifdef NOTDEF
  bool state = false;
  while(true){
    state = !state;
    for(uint8_t row = 0; row < 8; row++){
      for(uint8_t col = 0; col < 16; col++){
	left.setPixel(col, row, state);
	right.set_pixel(col, row, state);
	left.refresh(16);
      }
    }
  }
#endif
  /********************************************************************************/

  if(digitalRead(MODE_PIN)){
    self_test();
  }

  MsTimer2::set(1000, tick_interrupt); // 1ms period
  MsTimer2::start();


  // Set Time and Alarms
  setSyncProvider(getTime);      // RTC
  setSyncInterval(60000);      // update every minute (and on boot)
  update_time();

}

// main loop function.  Dellegate to mode_p->loop();
void loop(void){
  //check button status // C2 hack
  if(PINC & (1 << 1)){
    dec_interrupt();
  }
  if(PINB & (1 << 0)){
    enter_interrupt();
  }
  /* with auto reset, 
   * serial connection to C3 causes a reset so entering serial mode
   * manually is fuitless.  This is way better anyway.
   */
  Serial_loop();
  // process new events before calling mode loop()
  for(uint8_t i = 0; i < n_evt; i++){
    switch(event_q[i]){
    case NO_EVT:
      break;
    case MODE_EVT:
      mode_p->mode();
      break;
    case INC_EVT:
      mode_p->inc();
      break;
    case DEC_EVT:
      mode_p->dec();
      break;
    case ENTER_EVT:
      mode_p->enter();
      break;
    case TICK_EVT:
      tick = true;
      ss++;
      if(ss >= 60){
	ss %= 60;
	mm++;
	if(mm >= 60){
	  mm %= 60;
	  hh++;
	  if(hh == 24){
	    update_time();
	  }
	}
      }
    }
    event_q[i] = NO_EVT;
  }
  n_evt = 0;
  mode_p->loop(); // finally call mode_p loop()
  count++;        // counts times mode_p->loop() has run since mode start

}

// Begin Normal Mode Code 
/* 
   Initalize mode.
*/
void Normal_setup(void){
  tick = true;
  clear_display(display);
}
void Normal_loop(void) {
  tick = true;
  uint8_t word[3];                // will store start_x, start_y, length of word
  time_t spm = getTime() % 86400; // seconds past midnight
  // uint8_t m = minute();
  uint8_t m = minute();

  if(m != last_minute){

    // clear the new display
    clear_display(secondary_display);
    // left.setdisplay(secondary_display);
    // left.line(0, 0, m % 16, m % 8, 1);
    // left.setdisplay(primary_display);
    // read display for next time incement
    getdisplay(hour(), HOUR_SEQ, HOUR_WORDS, secondary_display);
    getdisplay(m, MINUTE_SEQ, MINUTE_WORDS, secondary_display);
  
    fadeto(secondary_display, 16); // 32 fade steps to new display
    last_minute = m;
  }
  count++;
  left.refresh(16);
}
/*
  Get ready for next mode.
*/
void Normal_exit(void) {
  last_minute = 61;
}
/*
  Respond to button presses.
*/
void Normal_inc(void) {
  switchmodes(SECONDS_MODE);
}
void Normal_dec(void) {
  switchmodes(TEMPERATURE_MODE);
}
void Normal_mode(void) {
  switchmodes(MODE_MODE);
}

// Sub mode of normal mode ** display seconds
void Seconds_setup(void){
  tick = true;
}
void Seconds_loop(){
  if(tick){
    tick = false;
    two_digits(ss);
  }
  left.refresh(16);
}
void Seconds_exit(void) {
}
void Seconds_mode(){
  switchmodes(last_mode_id);
}

// Sub mode of normal mode ** display temp
void Temperature_setup(void){
  if(temp_unit == DEG_C){
    // faceplate.display_word(c3, MONO, c_led);
  }
  else{
    // faceplate.display_word(c3, MONO, f_led);
  }
}
void Temperature_loop(){
  int8_t temp = getTemp();
  if(temp_unit == DEG_F){
    temp = toF(temp);
  }
  two_digits(temp);
  left.refresh(16);
}
void Temperature_exit(void) {
}
// toggle temp_unit
void Temperature_inc(){
  if(temp_unit == DEG_F){
    temp_unit = DEG_C;
  }
  else{
    temp_unit = DEG_F;
  }
}
void Temperature_dec(){
  switchmodes(last_mode_id);
}
void Temperature_mode(){
  switchmodes(last_mode_id);
}

void SerDisplay_loop(){
  left.refresh(16);
}

// Begin SetTime Mode Code 
/* 
   Initalize mode.
*/
void SetTime_setup(void){
  MsTimer2::stop(); // Ticks stop while setting time
  getTime(); // sync with rtcBOB
  SetTime_unit = YEAR;
}
void SetTime_loop(void) {
  switch(SetTime_unit){
  case YEAR:
    if(count % 200 > 75){
      left.clear();
      two_digits(YY % 100);
    }
    else{
      left.clear();
      font.getChar('Y', MONO, display + 5);  
    }
    // faceplate.display_word(c3, MONO, year_led);
    break;
  case MONTH:
    if(count % 200 > 75){
      left.clear();
      two_digits(MM);
    }
    else{
      left.clear();
      font.getChar('M', MONO, display + 5);  
    }
    // faceplate.display_word(c3, MONO, month_led);
    break;
  case DAY:
    if(count % 200 > 75){
      left.clear();
      two_digits(DD);
    }
    else{
      left.clear();
      font.getChar('D', MONO, display + 5);  
    }
    // faceplate.display_word(c3, MONO, day_led);
    break;
  case HOUR:
    if(count % 200 > 75){
      left.clear();
      two_digits(hh);
    }
    else{
      left.clear();
      font.getChar('H', MONO, display + 5);  
    }
    // faceplate.display_word(c3, MONO, hour_led);
    break;
  case MINUTE:
    if(count % 200 > 75){
      left.clear();
      two_digits(mm);
    }
    else{
      left.clear();
      font.getChar('M', MONO, display + 5);  
    }
    // faceplate.display_word(c3, MONO, minute_led);
    break;
  default:
    break;
  }
  left.refresh(16);
}
/*
  Get ready for next mode.
*/
void SetTime_exit(void) {
  setRTC(YY, MM, DD, hh, mm, ss);
  MsTimer2::start();
}
/*
  Respond to button presses.
*/
void SetTime_inc(void) {
  switch(SetTime_unit){
  case YEAR:
    YY++;
    two_digits(YY % 100);
    break;
  case MONTH:
    MM = (MM + 1) % 13;
    if(MM == 0){
      MM = 1;
    }
    two_digits(MM);
    break;
  case DAY:
    DD = (DD + 1) % (MONTHS[MM] + LEAP_YEAR(YY) + 1);
    if(DD == 0){
      DD = 1;
    }
    two_digits(DD);
    break;
  case HOUR:
    hh = (hh + 1) % 24;
    two_digits(hh);
    break;
  case MINUTE:
    mm = (mm + 1) % 60;
    ss = 0;
    two_digits(mm);
    break;
  }
}
void SetTime_dec(void) {
  switch(SetTime_unit){
  case YEAR:
    YY--;
    two_digits(YY % 100);
    break;
  case MONTH:
    MM = (MM - 1) % 13;
    if(MM == 0){
      MM = 12;
    }
    two_digits(MM);
    break;
  case DAY:
    DD = (DD - 1) % (MONTHS[MM] + LEAP_YEAR(YY) + 1);
    if(DD == 0){
      DD = 1;
    }
    two_digits(DD);
    break;
  case HOUR:
    if(hh == 1){
      hh = 23; // uint cannot go neg
    }
    else{
      hh--;
    }
    two_digits(hh);
    break;
  case MINUTE:
    if(mm == 0){
      mm = 59; // uint cannot go neg
    }
    else{
      mm--;
    }
    ss = 0;
    two_digits(mm);
    break;
  }
}
void SetTime_mode(void) {
  left.clear();
  switch(SetTime_unit){
  case YEAR:
    SetTime_unit = MONTH;
    break;
  case MONTH:
    SetTime_unit = DAY;
    break;
  case DAY:
    SetTime_unit = HOUR;
    break;
  case HOUR:
    SetTime_unit = MINUTE;
    break;
  default:
    SetTime_unit = YEAR;
    break;
  }
}

void SetTime_enter(void){
  switchmodes(NORMAL_MODE);
}

void Serial_loop(void) {
  uint8_t val;
  boolean resync_flag = true;
  if(Serial.available()){
    val = Serial.read();
    // two_digits(val);
    // find msg type
    for(uint8_t msg_i = 0; msg_i < N_MSG_TYPE; msg_i++){
      if(MSG_DEFS[msg_i]->id == val){
	if(Serial_get_msg(MSG_DEFS[msg_i]->n_byte)){
	  /*
	   * Entire payload (n_byte - 1) bytes 
	   * is stored in serial_msg: callback time.
	   */

	  MSG_DEFS[msg_i]->cb();
	  //two_digits(val);
	  //left.refresh(10000);
	}
	else{
	  // Got a sync message unexpectedly. Get ready for new message.
	  // no callback
	  // or timeout
	}
	resync_flag = false;
	break;
	// return;
      }
    }
    if(resync_flag){
      Serial_sync_wait();
    }
  }
}

void Serial_sync_wait(){
  // wait for SYNC message;
  uint8_t val;
  uint8_t n = 0;
  digitalWrite(DBG, HIGH);
  while(n < SYNC.n_byte){
    if(Serial.available()){
      val = Serial.read();
      if(val == SYNC_BYTE){
	n++;
      }
      else{
	n = 0;
      }
    }
  }
  digitalWrite(DBG, LOW);
}

// Transmit contents of PING message back to sender.
void pong(){
  for(uint8_t i=0; i < MAX_MSG_LEN - 1; i++){
    // Serial.print(serial_msg[i], BYTE);
    Serial.write(serial_msg[i]);
  }
}

void send_time(){
  char ser_data[4];
  // Serial.print(ABS_TIME_SET.id, BYTE);
  Serial.write(ABS_TIME_SET.id);
  Time_to_Serial(now(), ser_data);
  for(uint8_t i = 0; i < 4; i++){
    // Serial.print(ser_data[i], BYTE);
    Serial.write(ser_data[i]);
  }
}

void Serial_time_set(){
  setTime(Serial_to_Time(serial_msg));
  
  /* // old way
  Serial_time_t data;

  for(uint8_t i = 0; i < 4; i++){
    data.dat8[i] = serial_msg[i];
  }
  setTime(data.dat32);
  */
  YY = year();
  MM = month();
  DD = day();
  hh = hour();
  mm = minute();
  ss = second();
  setRTC(YY, MM, DD, hh, mm, ss);
}

void Serial_send_err(char *err){
  uint8_t len = strlen(err);
  // Serial.print(ERR_OUT.id, BYTE);
  Serial.write(ERR_OUT.id);
  //// Serial.print(len + 2 + MAX_MSG_LEN, BYTE);
  // Serial.print(len + 2, BYTE);
  Serial.write(len + 2);
  Serial.print(err);
  left.note(55);
  // Serial.print(serial_msg);
}

void display_send(){
  uint8_t *display_p = (uint8_t *)display;
  // Serial.print(DISPLAY_SET.id, BYTE);
  Serial.write(DISPLAY_SET.id);
  for(uint8_t i = 0; i < N_COL * sizeof(uint32_t); i++){
    // Serial.print(display_p[i], BYTE);
    Serial.write(display_p[i]);
  }
}

void display_set(){
  if(mode_p->id != SER_DISPLAY_MODE){
    switchmodes(SER_DISPLAY_MODE);
  }
  uint8_t *display_p = (uint8_t *)display;
  for(uint8_t i = 0; i < N_COL * sizeof(uint32_t); i++){
    display_p[i] = serial_msg[i];
  }
}

void trigger_mode(){
  
}

// store serial data into serial_msg staring from first byte AFTER MID
// to be clear MID is not in serial_msg
boolean Serial_get_msg(uint8_t n_byte) {
  /*
    n_byte = message length including 1 byte MID
  */
  uint16_t i = 0;
  unsigned long start_time = millis();

  uint8_t val, next;
  boolean out;

  // digitalWrite(DBG, HIGH);
  if(n_byte == VAR_LENGTH){
    // variable length message
    while(!Serial.available()){/* && 
				  ((millis() - start_time) < SERIAL_TIMEOUT_MS)){*/
      delay(1);
    }
    n_byte = Serial.peek();
  }
  while((i < n_byte - 1)){/* && 
			     ((millis() - start_time) < SERIAL_TIMEOUT_MS)){*/
    if(Serial.available()){
      val = Serial.read();
      if (val == SYNC_BYTE){
	sync_msg_byte_counter++;
	if(sync_msg_byte_counter == MAX_MSG_LEN){
	   // no other valid msg combination can have MAX_MSG_LEN sync bytes.
	   // sync msg recieved! break out, next char is start of new message
	   sync_msg_byte_counter = 0;
	   break;
	 }
       }
       else{
	 sync_msg_byte_counter = 0;
       }
       serial_msg[i++] = val;
     }
   }
   if(i == n_byte - 1){
     digitalWrite(DBG, LOW);
     out = true;
   }
   else{
    digitalWrite(DBG, HIGH);
    out = false;
  }
  return out;
}

/*
  Get ready for next mode.
*/
void Serial_exit(void) {
  Serial.end();
}
/*
  Respond to button presses.
*/
void Serial_inc(void) {
}
void Serial_dec(void) {
}
void Serial_mode(void) {
  switchmodes(NORMAL_MODE);  // or maybe just go back to NORMAL_MODE? or last_mode?
}

// Begin Mode Mode Code
void Mode_setup(void) {
  // font.getChar('M', MONO, display);
  mode_counter = 1;
  font.getChar(Modes[mode_counter].sym, MONO, display + 5);
}
void Mode_loop(void) {
  left.refresh(16);
}
void Mode_exit(void) {
}
void Mode_inc(void) {
  mode_counter++;
  mode_counter %= N_MAIN_MODE - 1; // skip ModeMode
  font.getChar(Modes[mode_counter].sym, MONO, display + 5);
}
void Mode_dec(void) {
  if(mode_counter == 0){
    mode_counter = N_MAIN_MODE - 2;// skip ModeMode
  }
  else{
    mode_counter--;
  }
  font.getChar(Modes[mode_counter].sym, MONO, display + 5);
}
void Mode_enter(void) {
  switchmodes(mode_counter);
}

void switchmodes(uint8_t new_mode_id){
  // only switch if we are not in this mode already ... or not...
  // if(new_mode_id != mode_p->id){
  last_mode_id = mode_p->id;
  left.clear(); // clear both screens
  left.setdisplay(display + N_COL); 
  left.clear(); // clear both screens
  left.setdisplay(display);
  mode_p->exit();
  mode_p = &Modes[new_mode_id];
  mode_p->setup();
  count = 0;
  // }
}

void return_to_normal(){
  switchmodes(NORMAL_MODE);
}
void two_digits(uint8_t val){
  font.getChar('0' + val / 10, MONO, display + 2);
  font.getChar('0' + val % 10, MONO, display + 9);
}

void two_digits(uint8_t val, uint32_t *display){
  font.getChar('0' + val / 10, MONO, display + 2);
  font.getChar('0' + val % 10, MONO, display + 9);
}

/*
 * Add a letter without erasing existing pixels
 */
void add_char(char letter, uint32_t* display, uint8_t col){
  uint32_t data[8];
  font.getChar(letter, MONO, data);
  for(uint8_t i = 0; i < 7; i++){
    display[col + i] |= (data[i] & 0b00000000111111111111111111111111);
  }
}
void two_letters(char* letters){
  font.getChar(letters[0], MONO, display + 2);
  font.getChar(letters[1], MONO, display + 9);
}

uint8_t my_random_value = 0;
const uint8_t prime = 31;
int my_random(uint8_t min, uint8_t max){
  my_random_value += prime;
  return (my_random_value % (max - min)) + min;
}

void too_big_fireworks(){ // will not fit special effect // need SD!
  int x, y;
  left.refresh(1800);
  //while(1){
    memset(display, 0, N_COL * N_DISPLAY * sizeof(uint32_t));
    for(int k = 0; k < 8; k++){
      if(my_random(0, 10) < 5){
	memset(display, 0, N_COL * N_DISPLAY * sizeof(uint32_t));
      }
      x = my_random(3, 13);
      y = my_random(3, 10);
      int istart = my_random(0, 10);
      left.setdisplay(display + istart * N_COL);
      // left.line(7, 11, x, y, WHITE);
      for(int i = istart; i < istart + 10; i++){
	left.setdisplay(display + i * N_COL);
	left.circle(x, y, (i + 1 - istart), COLORS[my_random(0, 8)]);
	left.circle(x, y, (i + 2 - istart), COLORS[my_random(0, 8)]);
	left.circle(x, y, (i + 3 - istart), COLORS[my_random(0, 8)]);
      }
      x = my_random(0, 16);
      y = my_random(0, 9);
      istart = my_random(0, 100);
      if(istart < 10){
	for(int i = istart; i < istart + 10; i++){
	  left.setdisplay(display + i * N_COL);
	  left.circle(x, y, (i + 1 - istart), COLORS[my_random(0, 8)]);
	  left.circle(x, y, (i + 2 - istart), COLORS[my_random(0, 8)]);
	  left.circle(x, y, (i + 3 - istart), COLORS[my_random(0, 8)]);
	}
      }
      int iter = my_random(1, 2);
      for(int j = 0; j < iter; j++){
	for(int i = 0; i < N_DISPLAY; i++){
	  left.setdisplay(display + N_COL * i);
	  left.refresh(100);
	}
      }
    }
    memset(display, 0, N_COL * N_DISPLAY * sizeof(uint32_t));
    for(int i = 0; i < N_COL * 4 + 12; i++){
      left.setdisplay(display + i);
      left.refresh(180);
    }
    //  }
}

// in is the address of the start of 4 time bytes.
time_t Serial_to_Time(char *in){
  return *((uint32_t *)in);
}

// write 4 bytes of in into char buffer out.
void Time_to_Serial(time_t in, char *out){
  time_t *out_p = (time_t *)out;
  *out_p = in;
}

/*
 * HOUR_SEQ: 60  rows of bit settings.  one for each minute of an hoour.
 * To turn on word one in ith display: 10000000000000000000000000000000
 *
 * WORDS:  3 * n + 1 bytes.  first byte is # words followed by ordered words.
 *                                    x         y       l
 *        Each word is defined by startcol, startrow, len
 */

void getword(int i, prog_char* words, uint8_t *out){
  out[0] = pgm_read_byte(words + 3 * i + 1);
  out[1] = pgm_read_byte(words + 3 * i + 2);
  out[2] = pgm_read_byte(words + 3 * i + 3);
}

/*
 * Prepare out to display ith time increment
 *   i -- 0 to 287 time increment indexa
 * out -- points to column data to prepare
 */
void getdisplay(int i, prog_char *seq, prog_char *words, uint32_t* out){
  uint8_t bits;     // holds the on off state for 8 words at a time
  uint8_t word[3];  // start columm, start row, length of the current word
  uint8_t n_byte_per_step = pgm_read_byte(seq);
  for(uint8_t j = 0; j < n_byte_per_step; j++){ // j is a byte index 

    // read the state for the next set of 8 words
    bits = pgm_read_byte(seq + 1 + (i * n_byte_per_step) + j);
    for(uint8_t k = 0; k < 8; k++){                     // k is a bit index
      if((bits >> k) & 1){                              // check to see if word is on or off
	getword(j * 8 + k, words, word);                // if on, read location and length
	for(int m=word[0]; m < word[0] + word[2]; m++){ // and display it
	  out[m] |= 1 << word[1];
	}
	left.refresh(16);
      }
    }
  } 
}


void self_test(){
  boolean state = false;
  if(!testRTC()){ // RTC Failed  Display X
    display[0] = 0b10000001;
    display[1] = 0b01000010;
    display[2] = 0b00100100;
    display[3] = 0b00011000;
    display[4] = 0b00100100;
    display[5] = 0b01000010;
    display[6] = 0b10000001;
    while(1){
      left.refresh(1000);
    }
  }
  while(true){
    state = !state;
    for(uint8_t i = 0; i < 8; i++){
      for(uint8_t j = 0; j < 16; j++){
	left.setPixel(j, i, state);
	right.set_pixel(j, i, state);
	left.refresh(100);
      }
    }
  }
}

bool testRTC(){
  bool status = true;
  uint8_t date[7];
  if(rtc_raw_read(0, 7, true, date)){
    Serial.print("DATE: ");
    // date[2], date[1], date[0], date[4], date[5], date[6]
    //      hr,     min,     sec,     day,   month,  yr;
      Serial.print(date[2], DEC);
      Serial.print(":");
      Serial.print(date[1], DEC);
      Serial.print(":");
      Serial.print(date[0], DEC);
      Serial.print("  ");

      Serial.print(date[4], DEC);
      Serial.print("/");
      Serial.print(date[5], DEC);
      Serial.print("/");
      Serial.print(date[6], DEC);
      Serial.print(".");

    Serial.println("");
  }
  else{
    Serial.print("RTC FAIL");
    status = false;
  }
  return status;
}