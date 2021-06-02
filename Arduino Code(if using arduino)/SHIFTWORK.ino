int BAUDRATE = 57600;
int I = 0;

// checksum variables
byte payloadChecksum = 0;
byte CalculatedChecksum;
byte checksum = 0;              //data type byte stores an 8-bit unsigned number, from 0 to 255
int payloadLength = 0;
byte payloadData[170] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;
boolean brainwave = false;
void setup() {
  Serial.begin(57600);       // Bluetooth
  //delay(500);
  Serial.begin(57600);      // software serial
  //delay(500);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  //pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  delay(2000);
  //digitalWrite(13, HIGH);
  //delay(5000);
  digitalWrite(10, LOW);
}
byte ReadOneByte() {
  byte ByteRead;
  // Wait until there is data
  while (!Serial.available());
  //Get the number of bytes (characters) available for reading from the serial port.
  //This is data that's already arrived and stored in the serial receive buffer (which holds 64 bytes)
  ByteRead = Serial.read();

  return ByteRead; // read incoming serial data
}

unsigned long delta_wave = 0;
unsigned long theta_wave = 0;
unsigned long low_alpha_wave = 0;
unsigned long high_alpha_wave = 0;
unsigned long low_beta_wave = 0;
unsigned long high_beta_wave = 0;
unsigned long low_gamma_wave = 0;
unsigned long mid_gamma_wave = 0;
unsigned long l = 0;
unsigned long m = 0;
unsigned long r = 0;
unsigned long sum = 0;
unsigned long data[8][3];
int k = 0;
int rawValue = 0;
int rawSize = 0;
int blink_strength1 = 0;
int blink_strength2 = 0;
int blink_strength3 = 0;
int raw, b, B, _a, A;
bool on1 = false;
int ledcnt=0;

/*String dataLabel1 = "poorQuality";
  String dataLabel2 = "Attention";
  String dataLabel3 = "Meditation";
  String dataLabel4 = "Delta";
  String dataLabel5 = "Theta";
  String dataLabel6 = "lowAlpha";
  String dataLabel7 = "highAlpha";
  String dataLabel8 = "lowBeta";
  String dataLabel9 = "highBeta";
  String dataLabel10 = "lowGamma";
  String dataLabel11 = "midGamma";
  bool label = true;*/



void read_waves(int i) {
  k = 0;
  delta_wave = read_3byte_int(i, k);
  i += 3; k += 1;
  theta_wave = read_3byte_int(i, k);
  i += 3; k += 1;
  low_alpha_wave = read_3byte_int(i, k);
  i += 3; k += 1;
  high_alpha_wave = read_3byte_int(i, k);
  i += 3; k += 1;
  low_beta_wave = read_3byte_int(i, k);
  i += 3; k += 1;
  high_beta_wave = read_3byte_int(i, k);
  i += 3; k += 1;
  low_gamma_wave = read_3byte_int(i, k);
  i += 3; k += 1;
  mid_gamma_wave = read_3byte_int(i, k);
  i += 3, k += 1;
}

unsigned long read_3byte_int(int i, int k) {
  //Serial.println("Read......................................................");
  l = 0; m = 0; r = 0;
  l = payloadData[i];
  m = payloadData[i + 1];
  r = payloadData[i + 2];
  data[k][0] = l;
  data[k][1] = m;
  data[k][2] = r;

  //sum=(l*65536)+(m*256)+(r);
  //return sum;*/
  return ((l << 16) + (m << 8) + r);
  //return ((payloadData[i] << 16) + (payloadData[i+1] << 8) + payloadData[i+2]);
}

void loop() {
  // Look for sync bytes
  // Byte order: 0xAA, 0xAA, payloadLength, payloadData,
  // Checksum (sum all the bytes of payload, take lowest 8 bits, then bit inverse on lowest
  if (ReadOneByte() == 0xAA) {
    if (ReadOneByte() == 0xAA) {
      payloadLength = ReadOneByte();
      if (payloadLength > 169) //Payload length can not be greater than 169
        return;
      payloadChecksum = 0;
      //////////////Clear Payload

      ///////////////////////////
      for (int i = 0; i < payloadLength; i++) {     //loop until payload length is complete
        payloadData[i] = ReadOneByte();             //Read payload
        payloadChecksum += payloadData[i];
      }
      checksum = ReadOneByte();                     //Read checksum byte from stream
      payloadChecksum = 255 - payloadChecksum;      //Take ones compliment of generated checksum
      if (checksum == payloadChecksum) {
        // poorQuality = 200;
        attention = 0;
        meditation = 0;
      }
      brainwave = false;
      for (int i = 0; i < payloadLength; i++) { // Parse the payload
        switch (payloadData[i]) {
          case 02:
            i++;
            poorQuality = payloadData[i];
            bigPacket = true;
            break;
          case 04:
            i++;
            attention = payloadData[i];

            break;
          case 05:
            i++;
            meditation = payloadData[i];
            break;
          case 0x16:
            //Serial.print(payloadData[i]);
            //Serial.print(",");
            i++;
            I = i;
            blink_strength1 = payloadData[I];
            I++;
            blink_strength2 = payloadData[I];
            I++;
            blink_strength3 = payloadData[I];

          //Serial.print(blink_strength1);
          //Serial.print(",");
          //Serial.print(blink_strength2);
          //Serial.print(",");
          //Serial.print(blink_strength3);
          //Serial.print(",");
          //Serial.print(millis());
          //Serial.println();
          case 0x80:
            i++;
            rawSize = payloadData[i];
            //B = i;
            //A = i;


            /*a = (int)payloadData[++B] << 8;
              b = payloadData[++B];
              _a = payloadData[++A];
              Serial.print(_a);
              Serial.print(",");
              Serial.print(a);
              Serial.print(",");
              Serial.print(b);*/
            /*raw=payloadData[++B];
              b=payloadData[++B];*/
            //raw=payloadData[++B]*256 + payloadData[++B];
            // if (raw>=32768)
            // {
            //  raw=raw-65536;
            //  }
            /*Serial.print(raw,DEC);
              Serial.print(":");
              Serial.print(b,DEC);
              Serial.print("  =");*/
            ////////////////////////////////////// rawValue = ((int)payloadData[++i] << 8) | payloadData[++i];
            //Serial.print(rawSize);
            //Serial.print(",");
            //////////////////////////////////////////// Serial.print(rawValue);
            ////////////////////////////////////////////  Serial.print(",");
            ////////////////////////////////////////////   Serial.print(millis());
            //////////////////////////////////////////////  Serial.print(",");
            //////////////////////////////////////////  Serial.print(poorQuality);

            /////////////////////////////////////////////// Serial.println();

            // i = i + 3;
            break;
          case 0x83:                         // ASIC EEG POWER INT
            i++;
            brainwave = true;
            byte vlen = payloadData[i];
            // Serial.print(vlen, DEC);
            //  Serial.println();
            //read_waves(i + 1);
            i += vlen; // i = i + vlen
            ///////////PRINT WAVES////////////////
            /*if (brainwave && attention > 0) {
              Serial.print(attention);
              Serial.print(",");
              Serial.print(meditation);
              Serial.print(",");
              Serial.print(delta_wave, DEC);
              Serial.print(",");
              Serial.print(theta_wave, DEC);
              Serial.print(",");
              Serial.print(low_alpha_wave, DEC);
              Serial.print(",");
              Serial.print(high_alpha_wave, DEC);
              Serial.print(",");
              Serial.print(low_beta_wave, DEC);
              Serial.print(",");
              Serial.print(high_beta_wave, DEC);
              Serial.print(",");
              Serial.print(low_gamma_wave, DEC);
              Serial.print(",");
              Serial.print(mid_gamma_wave, DEC);
              Serial.println();

              }*/
            break;
        }                                 // switch
      }
      // for loop of one packet
      if (attention > 0 && brainwave) {
        Serial.print(blink_strength1);
        Serial.print(",");
        Serial.print(millis() / 1000);
        Serial.println();
        
        if (blink_strength1 > 10 )
        {
          // on1=true;
          ledcnt=ledcnt+1;
          Serial.print("LEDcnt");
          Serial.println(ledcnt);
          if (ledcnt % 2 == 0) {
            Serial.println("ON");
            digitalWrite(13, HIGH);
          }
          else {
              Serial.println("OFF");
              digitalWrite(13, LOW);
            }
        }
      }
     
      /*if (attention > 0 && brainwave) {
        Serial.print(blink_strength);
        Serial.print(",");
        Serial.print(attention);
        Serial.print(",");
        Serial.print(millis());
        Serial.println();

        //digitalWrite(11,HIGH);
        //digitalWrite(12,HIGH);
        }*/

    }
  }
}
