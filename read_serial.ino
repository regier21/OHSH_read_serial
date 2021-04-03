#define NUM_JOINTS 1
#define BAUD_RATE 115200
#define SERIAL_TIMEOUT 20

// The serial to read signals from. Typically Serial2 for RPi, Serial for USB (helpful for debugging)
// You must also uncomment SigSerial.begin() if this is set to be not Serial
#define SigSerial Serial

int angles[NUM_JOINTS];

bool readCommand();

void setup() {
  Serial.begin(BAUD_RATE);
  // SigSerial.begin(BAUD_RATE);
  SigSerial.setTimeout(SERIAL_TIMEOUT);
}

void loop() {
  if (readCommand()) {
    Serial.println("Angles updated");
    
  }
}

/**
 * Reads command from SigSerial. 
 * Command must be of the form <#,#,...,#>, where the
 * number of numers is at least NUM_JOINTS.
 * 
 * Returns: true if read successful, false otherwise
 * 
 * Side effects: angles modified. This may occur even if false
 * is returned, so if the old angles are needed save them before
 * calling.
 */
bool readCommand(){
  if (SigSerial.available() <= 0) return false;
  String command = SigSerial.readStringUntil('\n');
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
