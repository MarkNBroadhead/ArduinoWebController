#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 176);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

const int PIN_1 = 0;
const int PIN_2 = 1;
const int PIN_3 = 2;
const int PIN_4 = 3;
const int MOTOR_DELAY = 500;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_3, OUTPUT);   
  pinMode(PIN_4, OUTPUT);   
}

#define BUFSIZ 100  //defines the buffer size.  100 gives plenty of room.  reduce size if more ram is needed.


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  char clientline[BUFSIZ];
  int index = 0;

  if (client) {
    Serial.println("new request");
    boolean current_line_is_blank = true;
    index = 0;
    bool haveRead = false;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\n' && c != '\r') {
          clientline[index] = c;
          index++;
          if (index >= BUFSIZ)
            index = BUFSIZ -1;
          continue;
        }
        clientline[index] = 0;
        
        // Send Homepage
        if (strstr(clientline, "GET /") != 0) {
          if (strstr(clientline, "GET / ") != 0) {  //If you are going to the homepage, the filename is set to the rootFileName
            // filename = rootFileName;
          }
          if (strstr(clientline, "GET /") != 0) {
            // if (!filename) filename = clientline + 5;  //gets rid of the GET / in the filename
            (strstr(clientline, " HTTP"))[0] = 0;  //gets rid of everything from HTTP to the end.
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<form action = '/pin1' method = 'post'><button>Pin 1</button></form>");
            client.println("<form action = '/pin2' method = 'post'><button>Pin 2</button></form>");
            client.println("<form action = '/pin3' method = 'post'><button>Pin 3</button></form>");
            client.println("<form action = '/pin4' method = 'post'><button>Pin 4</button></form>");
          } 
          else {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<h2>File Not Found!</h2>");
          }
          break;
        }
      }
      if (!client.available() && !haveRead) {
        Serial.println(clientline);
        // Get posted data
        if (strstr(clientline, "POST /") != 0) {
          if (strstr(clientline, "pin1") != 0) {
            moveMotor(PIN_1, MOTOR_DELAY);
          }
          else if (strstr(clientline, "pin2") != 0) {
            moveMotor(PIN_2, MOTOR_DELAY);
          }
          else if (strstr(clientline, "pin3") != 0) {
            moveMotor(PIN_3, MOTOR_DELAY);
          }
          else if (strstr(clientline, "pin4") != 0) {
            moveMotor(PIN_4, MOTOR_DELAY);
          }
        }
        haveRead = true;
        client.stop();
        Serial.println("client disconnected");
      }
    }

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void moveMotor(int pin, int delayTime) {
  digitalWrite(pin, HIGH);
  delay(delayTime);
  digitalWrite(pin, LOW);
  delay(delayTime);
}
