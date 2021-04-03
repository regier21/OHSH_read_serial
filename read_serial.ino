#define NUM_JOINTS 1

// The serial to read signals from. Typically Serial2 for RPi, Serial for USB (helpful for debugging)
// Make sure to uncomment SigSerial.begin in the setup if not using Serial, and to comment it otherwise
#define SigSerial Serial

int angles[NUM_JOINTS];

bool parseCommand(String&);

void setup() {
  Serial.begin(115200);
  //SigSerial.begin(115200);
}

void loop() {
  if (SigSerial.available() > 0) {
    Serial.println("Reading...");
    
    // read the incoming command:
    // TODO: Set quick timeout in case missed newline
    String str = SigSerial.readStringUntil('\n');
    if(parseCommand(str)){
      Serial.println("Angles parsed successfully. Last angle:" + String(angles[NUM_JOINTS-1]));
    } else{
      Serial.println("Failed to parse command");
    }
    
  }
}

bool parseCommand(String& command){
  if(!command.startsWith("<") || !command.endsWith(">")) return false;
  const char* input = command.c_str() + 1; //Skip opening '<'
  char* endPtr;
  int joint = 0;
  while (joint < NUM_JOINTS) {
    int angle = (int) strtol(input, &endPtr, 10);
    if(endPtr == input){
      Serial.println("Failed to parse number");
      return false;
    }
    angles[joint] = angle;
    input = endPtr + 1; //Skip the comma
    joint += 1;
  }
  return true;
}

/** Pi code:
 *  
 *  import pigpio
 *  pi = pigpio.pi()
 *  ser = pi.serial_open("/dev/serial0", 115200)
 *  pi.serial_write(ser, "Hello world")
 *  pi.serial_close(ser)
 */
