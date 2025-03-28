#ifndef CONFIG_H
#define CONFIG_H

#define SDA_PIN 4  // D2 (GPIO4)
#define SCL_PIN 5  // D1 (GPIO5)

#define MYPORT_TX 12 // D6
#define MYPORT_RX 13 // D7


/*devices*/
const unsigned char GM = 0x00; /*Body module*/
const unsigned char SHD = 0x08; /*Sunroof Control*/
const unsigned char CDC = 0x18; /*CD Changer*/
const unsigned char FUH = 0x28; /*Radio controlled clock*/
const unsigned char CCM = 0x30; /*Check control module*/
const unsigned char GT = 0x3B; /*Graphics driver (in navigation system)*/
const unsigned char DIA = 0x3F; /*Diagnostic*/
const unsigned char FBZV = 0x40; /*Remote control central locking*/
const unsigned char GTF = 0x43; /*Graphics driver for rear screen (in navigation system)*/
const unsigned char EWS = 0x44; /*Immobiliser*/
const unsigned char CID = 0x46; /*Central information display (flip-up LCD screen)*/
const unsigned char MFL = 0x50; /*Multi function steering wheel*/
const unsigned char MM0 = 0x51; /*Mirror memory*/
const unsigned char IHK = 0x5B; /*Integrated heating and air conditioning*/
const unsigned char PDC = 0x60; /*Park distance control*/
const unsigned char ONL = 0x67; /*unknown*/
const unsigned char RAD = 0x68; /*Radio*/
const unsigned char DSP = 0x6A; /*Digital signal processing audio amplifier*/
const unsigned char SM0 = 0x72; /*Seat memory*/
const unsigned char SDRS = 0x73; /*Sirius Radio*/
const unsigned char CDCD = 0x76; /*CD changer, DIN size.*/
const unsigned char NAVE = 0x7F; /*Navigation (Europe)*/
const unsigned char IKE = 0x80; /*Instrument cluster electronics*/
const unsigned char MM1 = 0x9B; /*Mirror memory*/
const unsigned char MM2 = 0x9C; /*Mirror memory*/
const unsigned char FMID = 0xA0; /*Rear multi-info-display*/
const unsigned char ABM = 0xA4; /*Air bag module*/
const unsigned char KAM = 0xA8; /*unknown*/
const unsigned char ASP = 0xAC; /*unknown*/
const unsigned char SES = 0xB0; /*Speed recognition system*/
const unsigned char NAVJ = 0xBB; /*Navigation (Japan)*/
const unsigned char GLO = 0xBF; /*Global, broadcast address*/
const unsigned char MID = 0xC0; /*Multi-info display*/
const unsigned char TEL = 0xC8; /*Telephone*/
const unsigned char TCU = 0xCA; /*unknown (BMW Assist?)*/
const unsigned char LCM = 0xD0; /*Light control module*/
const unsigned char GTHL = 0xDA; /*unknown*/
const unsigned char IRIS = 0xE0; /*Integrated radio information system*/
const unsigned char ANZV = 0xE7; /*Front display*/
const unsigned char RLS = 0xE8; /*Rain/Light Sensor*/
const unsigned char TV = 0xED; /*Television*/
const unsigned char BMBT = 0xF0; /*On-board monitor operating part*/
const unsigned char CSU = 0xF5; /*unknown*/
const unsigned char LOC = 0xFF; /*Local*/

/*Messages*/
const unsigned char DSREQ = 0x01;/*"Device status request"*/
const unsigned char DSRED = 0x02;/*"Device status ready"*/
const unsigned char BSREQ = 0x03;/*"Bus status request"*/
const unsigned char BS = 0x04;/*"Bus status"*/
const unsigned char DRM = 0x06;/*"DIAG read memory"*/
const unsigned char DWM = 0x07;/*"DIAG write memory"*/
const unsigned char DRCD = 0x08;/*"DIAG read coding data"*/
const unsigned char DWCD = 0x09;/*"DIAG write coding data"*/
const unsigned char VC = 0x0C;/*"Vehicle control"*/

const unsigned char ISREQ = 0x10;/*"Ignition status request"*/
const unsigned char IS = 0x11;/*"Ignition status"*/
const unsigned char ISSREQ = 0x12;/*"IKE sensor status request"*/
const unsigned char ISS = 0x13;/*"IKE sensor status"*/
const unsigned char CCSREQ = 0x14;/*"Country coding status request"*/
const unsigned char CCS = 0x15;/*"Country coding status"*/
const unsigned char OREQ = 0x16;/*"Odometer request"*/
const unsigned char O = 0x17;/*"Odometer"*/
const unsigned char SR = 0x18;/*"Speed/RPM"*/
const unsigned char T = 0x19;/*"Temperature"*/
const unsigned char ITDG = 0x1A;/*"IKE text display/Gong"*/
const unsigned char ITS = 0x1B;/*"IKE text status"*/
const unsigned char G = 0x1C;/*"Gong"*/
const unsigned char TREQ = 0x1D;/*"Temperature request"*/
const unsigned char UTAD = 0x1F;/*"UTC time and date"*/

const unsigned char MT = 0x21; /*Radio Short cuts*/
const unsigned char TDC = 0x22; /*Text display confirmation*/
const unsigned char UMID = 0x23;/*"Display Text"*/
const unsigned char UANZV = 0x24;/*"Update ANZV"*/
const unsigned char OBCSU = 0x2A;/*"On-Board Computer State Update"*/
const unsigned char TI = 0x2b; /*Telephone indicators*/

const unsigned char MFLB = 0x32;/*"MFL buttons"*/
const unsigned char DSPEB = 0x34;/*"DSP Equalizer Button"*/
const unsigned char CDSREQ = 0x38;/*"CD status request"*/
const unsigned char CDS = 0x39;/*"CD status"*/
const unsigned char MFLB2 = 0x3B;/*"MFL buttons"*/
const unsigned char SDRSSREQ = 0x3D;/*"SDRS status request"*/
const unsigned char SDRSS = 0x3E;/*"SDRS status"*/

const unsigned char SOBCD = 0x40;/*"Set On-Board Computer Data"*/
const unsigned char OBCDR = 0x41;/*"On-Board Computer Data Request"*/
const unsigned char LCDC = 0x46;/* LCD Clear*/
const unsigned char BMBTB0 = 0x47;/*"BMBT buttons"*/
const unsigned char BMBTB1 = 0x48;/*"BMBT buttons"*/
const unsigned char KNOB = 0x49;/*"KNOB button"*/ /*this is for right knob turn, pressing know is BMBTB1 and ButtonMenuKnob*/
const unsigned char CC = 0x4a;/*Cassette control*/
const unsigned char CS = 0x4b;/*"Cassette Status"*/
const unsigned char RGBC = 0x4F;/*"RGB Control"*/

const unsigned char VDREQ = 0x53;/*"Vehicle data request"*/
const unsigned char VDS = 0x54;/*"Vehicle data status"*/
const unsigned char LSREQ = 0x5A;/*"Lamp status request"*/
const unsigned char LS = 0x5B;/*"Lamp Status"*/
const unsigned char ICLS = 0x5C;/*"Instrument cluster lighting status"*/

const unsigned char RSSREQ = 0x71;/*"Rain sensor status request"*/
const unsigned char RKB = 0x72;/*"Remote Key buttons"*/
const unsigned char EWSKS = 0x74;/*"EWS key status"*/
const unsigned char DWSREQ = 0x79;/*"Doors/windows status request"*/
const unsigned char DWS = 0x7A;/*"Doors/windows status"*/
const unsigned char SHDS = 0x7C;/*"SHD status"*/

const unsigned char RCL = 0xD4; /*RDS channel list*/

const unsigned char DD = 0xA0;/*"DIAG data"*/
const unsigned char CPAT = 0xA2;/*"Current position and time*/
const unsigned char CL = 0xA4;/*Current location, always 23 bytes, data has 2 byte order number and then ascii: 00 01 4F 55 4C 55 00 == 1st packet, OULU\0*/
const unsigned char ST = 0xa5; /*Screen text*/
const unsigned char TMCSREQ = 0xA7;/*"TMC status request"*/
const unsigned char NC = 0xAA;/*"Navigation Control"*/

#endif
