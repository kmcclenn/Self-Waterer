// pin and variable definitions
int moistureSensorA = 0;
int pastReadingsA[100];
int initial_counter = 0;
int motorPin = 5;
boolean drynessA;

// watering consants
int threshold = 400;
int delayTimeAfterWatering = 120000; // two minute
int waterTime = 500; // half a second
int setupTime = 100; // first 100 repetitions of loop


void setup() {
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  
  int avgValueA = getMoisture(moistureSensorA, pastReadingsA, initial_counter)[0]; // average value of past 100 sensings for moisture sensor A
  pastReadingsA = getMoisture(moistureSensorA, pastReadingsA, initial_counter)[1]; // array of past 100 moisture readings
  initial_counter++;

  // first check if the soil is dry
  if (avgValueA > threshold && !drynessA) {
    drynessA = true;
    long int start_timeA = millis(); // starts the start time
  }

  // waters plants
  water(drynessA, start_timeA, avgValueA);
  
  
}

// average function
int average(int array_to_be_averaged[]) {
  long int sum = 0;
  int len = sizeof(array_to_be_averaged) / sizeof(array_to_be_averaged[0]);

  for (int i = 0; i < len; i++) {
    sum += array_to_be_averaged[i];
  }
  int average = sum/len;
  return average;
}

// get moisture function that returns the average value of the past 100 readings and the array of the past 100 readings
int * getMoisture(int sensorPin, int previousReadings[], int initial_counter) {
  int moistureValue = analogRead(sensorPin);

  // for first 100 iterations, fill in array. After that, make array the 100 most recent readings.
  if (initial_counter < setupTime) {
    previousReadings[initial_counter] = moistureValue;
  } else {
    for (int i = 0; i < 99; i++) {
      pastReadings[i] = pastReadings[i+1];
    }
    pastReadings[99] = moistureValue;
  }

  // take average and map
  int avgValue = average(pastReadings);
  avgValue = map(avgValue, 200, 700, 0, 1000);
  avgValue = min(avgValue, 1000);
  avgValue = max(avgValue, 0);
  Serial.println(avgValue);
  
  return [avgValue, previousReadings];
}

//water function
void water(boolean dryness, int start_time, int avgValue) {
  // if the soil is dry
  if (dryness && initial_counter > setupTime) {
    // if the soil is still dry after the delay time, then water it. if it is wet again, then don't water and repeat
    if (millis() - start_time > delayTimeAfterWatering && avgValue >= threshold) {
      digitalWrite(motorPin, LOW);
      delay(waterTime);
      digitalWrite(motorPin, HIGH);
      Serial.println("Watering...");
      dryness = false;
      delay(delayTimeAfterWatering);
    } else if (millis() - start_time > delayTimeAfterWatering && avgValue < threshold && initial_counter > setupTime) {
      dryness = false;
      delay(delayTimeAfterWatering);
    }
  }
}

/////////// previous code to get moisture in loop() /////////

//int moistureValue = analogRead(moistureSensor);
//  if (initial_counter < 100) {
//    pastReadingsA[initial_counter] = moistureValue;
//  } else {
//    
//    for (int i = 0; i < 99; i++) {
//      pastReadings[i] = pastReadings[i+1];
//    }
//    pastReadings[99] = moistureValue;
//  }
//  int avgValue = average(pastReadings);
//  avgValue = map(avgValue, 200, 700, 0, 1000);
//  avgValue = min(avgValue, 1000);
//  avgValue = max(avgValue, 0);
//  Serial.println(avgValue);
//  //delay(1000);
