#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c

#define BUZZER 5
#define LED_1 15
#define PB_CANCEL 34
#define PB_OK 32
#define PB_UP 33
#define PB_DOWN 35
#define DHTPIN 12


#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     0
#define UTC_OFFSET_DST 0

//Declare functions

void print_line(String text, int column, int row, int text_size);
void update_time();
void update_time_with_check_alarm(void);
void ring_alarm(int alarm_index);
void got_to_menu();
void set_time();
void set_alarm(int alarm);
void run_mode(int mode);
void check_temp();
void set_time_zone();
void view_alarms();
void delete_alarm();

//Declare objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

//global variables
float utc_offset = 0.0;
int days = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;

unsigned long timenow = 0;
unsigned long timeLast = 0;

bool alarm_enabled = true;
int n_alarms = 2;
int alarm_hours[] = {0, 1};
int alarm_minute[] = {1, 10};
bool alarm_triggered[] = {false, false};

int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

int current_mode = 0;
int max_modes = 7;
String modes[] = {"1 - Set Time", "2 - Set Alarm 1", "3 - Set Alarm 2", "4- Disable Alarms", "5- View Alarms", "6- Delete Alarms", "7- Set Time Zone"};



void print_line(String text, int column, int row, int text_size) {
  //clear OLED display
  //display.clearDisplay();

  //display a custom message
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);
  display.println(text);
  display.display();
  //delay(2000);

}

void print_time_now(void) {
  print_line(String(days), 0, 0, 2);
  print_line(":", 20, 0, 2);
  print_line(String(hours), 30, 0, 2);
  print_line(":", 50, 0, 2);
  print_line(String(minutes), 60, 0, 2);
  print_line(":", 80, 0, 2);
  print_line(String(seconds), 90, 0, 2);

}
//time update function and fetch current time from NTP Server
void update_time() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    display.clearDisplay();
    print_line("Failed to get time", 0, 0, 1);
    delay(2000);
    return;
  }

  // Extract hours, minutes, seconds, and days
  char timeHour[3];
  strftime(timeHour, 3, "%H", &timeinfo);
  hours = atoi(timeHour);

  char timeMinute[3];
  strftime(timeMinute, 3, "%M", &timeinfo);
  minutes = atoi(timeMinute);

  char timeSecond[3];
  strftime(timeSecond, 3, "%S", &timeinfo);
  seconds = atoi(timeSecond);

  char timeDay[3];
  strftime(timeDay, 3, "%d", &timeinfo);
  days = atoi(timeDay);
}



// void ring_alarm(int alarm_index) {
//   display.clearDisplay();
//   print_line("MEDICINE TIME", 0, 0, 2);

//   digitalWrite(LED_1, HIGH);

//   bool break_happened = false;

//   //Ring the buzzer
//   while (break_happened == false && digitalRead(PB_CANCEL) == HIGH && digitalRead(PB_OK) == HIGH) {
//     for (int i = 0; i < n_notes; i++) {
//       if (digitalRead(PB_CANCEL) == LOW) {
//         delay(200);
//         break_happened = true;
//         break;
//       }

//       if (digitalRead(PB_OK) == LOW) {
//         // Snooze the alarm for 5 minutes
//         delay(200);
//         break_happened = true;
//         alarm_triggered[alarm_index] = false; // Reset the triggered state
//         alarm_minute[alarm_index] += 5; // Add 5 minutes to the alarm time
//         if (alarm_minute[alarm_index] >= 60) {
//           alarm_minute[alarm_index] -= 60;
//           alarm_hours[alarm_index] = (alarm_hours[alarm_index] + 1) % 24;
//         }
//       }

//       tone(BUZZER, notes[i]);
//       delay(500);
//       noTone(BUZZER);
//       delay(2);
//     }

//   }

//   digitalWrite(LED_1, LOW);
//   display.clearDisplay();
// }

void ring_alarm(int alarm_index) {
  display.clearDisplay();
  print_line("MEDICINE TIME", 0, 0, 2);

  digitalWrite(LED_1, HIGH);

  bool break_happened = false;

  // Ring the buzzer
  while (!break_happened) {
    for (int i = 0; i < n_notes; i++) {
      if (digitalRead(PB_CANCEL) == LOW) {
        // Stop the alarm
        delay(200);
        break_happened = true;
        alarm_triggered[alarm_index] = true; // Mark alarm as triggered
        break;
      }

      if (digitalRead(PB_OK) == LOW) {
        // Snooze the alarm for 5 minutes
        delay(200);
        alarm_triggered[alarm_index] = false; // Reset the triggered state
        Serial.println("Snoozing alarm for 5 minutes");
        Serial.println(alarm_triggered[alarm_index]);
        alarm_minute[alarm_index] += 1; // Add 5 minutes to the alarm time
        if (alarm_minute[alarm_index] >= 60) {
          alarm_minute[alarm_index] -= 60;
          alarm_hours[alarm_index] = (alarm_hours[alarm_index] + 1) % 24;
        }
        break_happened = true;
        break;
      }

      tone(BUZZER, notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
    }
  }

  digitalWrite(LED_1, LOW);
  display.clearDisplay();
}

void update_time_with_check_alarm(void) {
  update_time();
  print_time_now();

  if (alarm_enabled == true) {
    for (int i = 0; i < n_alarms; i++) {
      if (alarm_triggered[i] == false && alarm_hours[i] == hours && alarm_minute[i] == minutes) {
        alarm_triggered[i] = true;
        ring_alarm(i);
        
      }
    }
  }
}

int wait_for_button_press() {

  while (true) {
    if (digitalRead(PB_UP) == LOW) {
      delay(200);
      return PB_UP;
    }

    else if (digitalRead(PB_DOWN) == LOW) {
      delay(200);
      return PB_DOWN;
    }

    else if (digitalRead(PB_OK) == LOW) {
      delay(200);
      return PB_OK;
    }

    else if (digitalRead(PB_CANCEL) == LOW) {
      delay(200);
      return PB_CANCEL;
    }

    update_time();
  }
}

void got_to_menu() {
  while (digitalRead(PB_CANCEL) == HIGH) {
    display.clearDisplay();
    print_line(modes[current_mode], 0, 0, 2);

    int pressed = wait_for_button_press();

    if (pressed == PB_UP) {
      delay(200);
      current_mode += 1;
      current_mode = current_mode % max_modes;
    }

    if (pressed == PB_DOWN) {
      delay(200);
      current_mode -= 1;

      if (current_mode < 0 ) {
        current_mode = max_modes - 1;
      }
    }

    else if (pressed == PB_OK) {
      delay(200);
      Serial.println(current_mode);
      run_mode(current_mode);
    }

    else if (pressed == PB_CANCEL) {
      delay(200);
      break;
    }
  }
}

void set_time() {
  int temp_hours = hours;

  while (true) {
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hours), 0, 0, 2);

    int pressed = wait_for_button_press();

    if (pressed == PB_UP) {
      delay(200);
      temp_hours += 1;
      temp_hours = temp_hours % 24;

    }

    if (pressed == PB_DOWN) {
      delay(200);
      temp_hours -= 1;
      if (temp_hours < 0) {
        temp_hours = 23;

      }
    }

    else if (pressed == PB_OK) {
      delay(200);
      hours = temp_hours;

    }

    else if (pressed == PB_CANCEL) {
      delay(200);
      break;
    }
  }


  int temp_minutes = minutes;

  while (true) {
    display.clearDisplay();
    print_line("Enter minutes: " + String(temp_minutes), 0, 0, 2);

    int pressed = wait_for_button_press();

    if (pressed == PB_UP) {
      delay(200);
      temp_minutes += 1;
      temp_minutes = temp_minutes % 60;

    }

    if (pressed == PB_DOWN) {
      delay(200);
      temp_minutes -= 1;
      if (temp_minutes < 0) {
        temp_minutes = 59;

      }
    }

    else if (pressed == PB_OK) {
      delay(200);
      minutes = temp_minutes;

    }

    else if (pressed == PB_CANCEL) {
      delay(200);
      break;
    }
  }

  display.clearDisplay();
  print_line("Time is set", 0, 0, 2);
  delay(2000);
}

void set_alarm (int alarm) {
  int temp_hour = alarm_hours[alarm];

  while (true) {
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour), 0, 0, 2);

    int pressed = wait_for_button_press();

    if (pressed == PB_UP) {
      delay(200);
      temp_hour += 1;
      temp_hour = temp_hour % 24;

    }

    if (pressed == PB_DOWN) {
      delay(200);
      temp_hour -= 1;
      if (temp_hour < 0) {
        temp_hour = 23;

      }
    }

    else if (pressed == PB_OK) {
      delay(200);
      alarm_hours[alarm] = temp_hour;
      alarm_triggered[alarm] = false;

    }

    else if (pressed == PB_CANCEL) {
      delay(200);
      break;
    }
  }


  int temp_minutes = alarm_minute[alarm];

  while (true) {
    display.clearDisplay();
    print_line("Enter minutes: " + String(temp_minutes), 0, 0, 2);

    int pressed = wait_for_button_press();

    if (pressed == PB_UP) {
      delay(200);
      temp_minutes += 1;
      temp_minutes = temp_minutes % 60;

    }

    if (pressed == PB_DOWN) {
      delay(200);
      temp_minutes -= 1;
      if (temp_minutes < 0) {
        temp_minutes = 59;

      }
    }

    else if (pressed == PB_OK) {
      delay(200);
      alarm_minute[alarm] = temp_minutes;

    }

    else if (pressed == PB_CANCEL) {
      delay(200);
      break;
    }

  }
}

void run_mode(int mode) {
  if (mode == 0) {
    set_time();
  }

  if (mode == 1 || mode == 2) {
    set_alarm(mode - 1);
  }

  else if (mode == 3) {
    alarm_enabled = false;
  }
  
  else if (mode == 4) {
    view_alarms();
  }

  else if (mode == 5) {
    delete_alarm();
  }

  else if (mode == 6) {
    set_time_zone();
  }

}

void check_temp() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (data.temperature > 35) {
    display.clearDisplay();
    print_line("TEMP HIGH", 0, 35, 1);
  }

  if (data.temperature < 35) {
    display.clearDisplay();
    print_line("TEMP LOW", 0, 35, 1);
  }

  if (data.humidity > 40) {
    display.clearDisplay();
    print_line("HUMIDY HIGH", 0, 45, 1);
  }

  if (data.humidity < 20) {
    display.clearDisplay();
    print_line("HUMIDITY LOW", 0, 45, 1);
  }

  if (data.temperature >32 || data.temperature < 24) {
    display.clearDisplay();
    print_line("Temperature Warning",0, 25,1);
    digitalWrite(LED_1,HIGH);
  }

  if (data.humidity >85 || data.humidity < 65) {
    display.clearDisplay();
    print_line("Humidity Warning",0, 30,1);
    digitalWrite(LED_1,HIGH);
  }

  else {
    digitalWrite(LED_1,LOW);
  }


}

// void check_temp() {
//   TempAndHumidity data = dhtSensor.getTempAndHumidity();
//   if (data.temperature > 35 && data.humidity > 40) {
//     display.clearDisplay();
//     print_line("TEMP HIGH", 0, 40, 1);
//     print_line("HUMIDY HIGH", 0, 50, 1);
//     display.clearDisplay();
//   }

//   if (data.temperature < 35 && data.humidity > 40) {
//     display.clearDisplay();
//     print_line("TEMP LOW", 0, 40, 1);
//     print_line("HUMIDY HIGH", 0, 50, 1);
//     display.clearDisplay();
//   }

//   if (data.temperature > 35 && data.humidity < 40) {
//     display.clearDisplay();
//     print_line("TEMP HIGH", 0, 40, 1);
//     print_line("HUMIDY LOW", 0, 50, 1);
//     display.clearDisplay();
//   }

//   if (data.temperature < 35 && data.humidity < 40) {
//     display.clearDisplay();
//     print_line("TEMP LOW", 0, 40, 1);
//     print_line("HUMIDY LOW", 0, 50, 1);
//     display.clearDisplay();
//   }
// }


void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(PB_DOWN, INPUT);

  dhtSensor.setup(DHTPIN, DHTesp::DHT22);

  //Initialize serial monitor and OLED display
  Serial.begin(115200);
  if (! display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  //turn on OLED display
  display.display();
  delay(500);

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    display.clearDisplay();
    print_line("Connecting to WIFI", 0, 0, 2);
  }

  display.clearDisplay();
  print_line("Connected to WIFI", 0, 0, 2);

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  display.clearDisplay();

  print_line("Welcome to Medibox!", 10, 20, 2);
  delay(2000);
  display.clearDisplay();

}


void loop() {
  // put your main code here, to run repeatedly:
  update_time_with_check_alarm();
  Serial.println("Current mode: " + String(current_mode));
  if (digitalRead(PB_OK) == LOW) {
    delay(200); //allow the pushbutton to debounce
    got_to_menu();
  }
  check_temp();
  delay(10); // this speeds up the simulation
}

// Function to set the time zone offset (supports fractional offsets)
void set_time_zone() {
  float temp_offset = utc_offset;

  while (true) {
    display.clearDisplay();
    print_line("UTC Offset: " + String(temp_offset, 1), 0, 0, 2); // Display with 1 decimal place

    int pressed = wait_for_button_press();

    if (pressed == PB_UP) {
      delay(200);
      temp_offset += 0.5; // Increment by 0.5
    }

    if (pressed == PB_DOWN) {
      delay(200);
      temp_offset -= 0.5; // Decrement by 0.5
    }

    else if (pressed == PB_OK) {
      delay(200);
      utc_offset = temp_offset;
      configTime(utc_offset * 3600, UTC_OFFSET_DST, NTP_SERVER); // Convert offset to seconds
      break;
    }

    else if (pressed == PB_CANCEL) {
      delay(200);
      break;
    }
  }

  display.clearDisplay();
  print_line("Time Zone Set", 0, 0, 2);
  delay(2000);
}


// Function to view active alarms
void view_alarms() {
  while(true){
  display.clearDisplay();
  int row = 0;
  for (int i = 0; i < n_alarms; i++) {
    if(alarm_hours[i] == -1 || alarm_minute[i] == -1) {
      continue;
    }

    print_line("Alarm " + String(i + 1) + ": " + String(alarm_hours[i]) + ":" + String(alarm_minute[i]), 0, i * 10, 1);
    row++;
  }

    // If no active alarms, display a message
    if (row == 0) {
      print_line("No Active Alarms", 0, 0, 2);
    }

  int pressed = wait_for_button_press();

  if (pressed == PB_CANCEL) {
    delay(200);
    break;
  }
  }
}

// Function to delete a specific alarm
void delete_alarm() {
  int alarm_to_delete = 0;

  while (true) {
    display.clearDisplay();
    print_line("Del Alarm: " + String(alarm_to_delete + 1), 0, 0, 2);

    int pressed = wait_for_button_press();

    if (pressed == PB_UP) {
      delay(200);
      alarm_to_delete = (alarm_to_delete + 1) % n_alarms;
    }

    if (pressed == PB_DOWN) {
      delay(200);
      alarm_to_delete = (alarm_to_delete - 1 + n_alarms) % n_alarms;
    }

    else if (pressed == PB_OK) {
      delay(200);
      alarm_hours[alarm_to_delete] = -1; // Mark alarm as deleted
      alarm_minute[alarm_to_delete] = -1;
      alarm_triggered[alarm_to_delete] = false;
      break;
    }

    else if (pressed == PB_CANCEL) {
      delay(200);
      break;
    }
  }

  display.clearDisplay();
  print_line("Alarm Deleted", 0, 0, 2);
  delay(2000);
}
