#ifndef __DEBUG_NRF52840__
#define __DEBUG_NRF52840__

// TODO: Question
//
// *(volatile uint32_t* xxx) = *(volatile uint32_t* yyy); does xxx changed if yyy changes ?  GLOBAL
//
// uint32_t x = y | x; is it possible to declare ?  GLOBAL

// /**
//   * @brief 2.4 GHz Radio (RADIO)
//   */
// typedef struct {                               // RADIO Structure
//   uint32_t  TASKS_TXEN;                        // Enable RADIO in TX mode
//   uint32_t  TASKS_RXEN;                        // Enable RADIO in RX mode
//   uint32_t  TASKS_START;                       // Start RADIO
//   uint32_t  TASKS_STOP;                        // Stop RADIO
//   uint32_t  TASKS_DISABLE;                     // Disable RADIO
//   uint32_t  TASKS_RSSISTART;                   // Start the RSSI and take one single sample of the receive signal strength.
//   uint32_t  TASKS_RSSISTOP;                    // Stop the RSSI measurement
//   uint32_t  TASKS_BCSTART;                     // Start the bit counter
//   uint32_t  TASKS_BCSTOP;                      // Stop the bit counter
//   uint32_t  TASKS_EDSTART;                     // Start the Energy Detect measurement used in IEEE 802.15.4 mode
//   uint32_t  TASKS_EDSTOP;                      // Stop the Energy Detect measurement
//   uint32_t  TASKS_CCASTART;                    // Start the Clear Channel Assessment used in IEEE 802.15.4 mode
//   uint32_t  TASKS_CCASTOP;                     // Stop the Clear Channel Assessment
//   uint32_t  RESERVED0[51];
//   uint32_t  EVENTS_READY;                      // RADIO has ramped up and is ready to be started
//   uint32_t  EVENTS_ADDRESS;                    // Address sent or received
//   uint32_t  EVENTS_PAYLOAD;                    // Packet payload sent or received
//   uint32_t  EVENTS_END;                        // Packet sent or received
//   uint32_t  EVENTS_DISABLED;                   // RADIO has been disabled
//   uint32_t  EVENTS_DEVMATCH;                   // A device address match occurred on the last received packet
//   uint32_t  EVENTS_DEVMISS;                    // No device address match occurred on the last received packet
//   uint32_t  EVENTS_RSSIEND;                    // Sampling of receive signal strength complete.
//   uint32_t  RESERVED1[2];
//   uint32_t  EVENTS_BCMATCH;                    // Bit counter reached bit count value.
//   uint32_t  RESERVED2;
//   uint32_t  EVENTS_CRCOK;                      // Packet received with CRC ok
//   uint32_t  EVENTS_CRCERROR;                   // Packet received with CRC error
//   uint32_t  EVENTS_FRAMESTART;                 // IEEE 802.15.4 length field received
//   uint32_t  EVENTS_EDEND;                      // Sampling of Energy Detection complete. A new ED sample is ready for readout from the RADIO.EDSAMPLE register
//   uint32_t  EVENTS_EDSTOPPED;                  // The sampling of Energy Detection has stopped
//   uint32_t  EVENTS_CCAIDLE;                    // Wireless medium in idle - clear to send
//   uint32_t  EVENTS_CCABUSY;                    // Wireless medium busy - do not send
//   uint32_t  EVENTS_CCASTOPPED;                 // The CCA has stopped
//   uint32_t  EVENTS_RATEBOOST;                  // Ble_LR CI field received, receive mode is changed from Ble_LR125Kbit to Ble_LR500Kbit.
//   uint32_t  EVENTS_TXREADY;                    // RADIO has ramped up and is ready to be started TX path
//   uint32_t  EVENTS_RXREADY;                    // RADIO has ramped up and is ready to be started RX path
//   uint32_t  EVENTS_MHRMATCH;                   // MAC Header match found.
//   uint32_t  RESERVED3[40];
//   uint32_t  SHORTS;                            // Shortcut register
//   uint32_t  RESERVED4[64];
//   uint32_t  INTENSET;                          // Enable interrupt
//   uint32_t  INTENCLR;                          // Disable interrupt
//   uint32_t  RESERVED5[61];
//   uint32_t  CRCSTATUS;                         // CRC status
//   uint32_t  RESERVED6;
//   uint32_t  RXMATCH;                           // Received address
//   uint32_t  RXCRC;                             // CRC field of previously received packet
//   uint32_t  DAI;                               // Device address match index
//   uint32_t  RESERVED7[60];
//   uint32_t  PACKETPTR;                         // Packet pointer
//   uint32_t  FREQUENCY;                         // Frequency
//   uint32_t  TXPOWER;                           // Output power
//   uint32_t  MODE;                              // Data rate and modulation
//   uint32_t  PCNF0;                             // Packet configuration register 0
//   uint32_t  PCNF1;                             // Packet configuration register 1
//   uint32_t  BASE0;                             // Base address 0
//   uint32_t  BASE1;                             // Base address 1
//   uint32_t  PREFIX0;                           // Prefixes bytes for logical addresses 0-3
//   uint32_t  PREFIX1;                           // Prefixes bytes for logical addresses 4-7
//   uint32_t  TXADDRESS;                         // Transmit address select
//   uint32_t  RXADDRESSES;                       // Receive address select
//   uint32_t  CRCCNF;                            // CRC configuration
//   uint32_t  CRCPOLY;                           // CRC polynomial
//   uint32_t  CRCINIT;                           // CRC initial value
//   uint32_t  RESERVED8;
//   uint32_t  TIFS;                              // Inter Frame Spacing in us
//   uint32_t  RSSISAMPLE;                        // RSSI sample
//   uint32_t  RESERVED9;
//   uint32_t  STATE;                             // Current radio state
//   uint32_t  DATAWHITEIV;                       // Data whitening initial value
//   uint32_t  RESERVED10[2];
//   uint32_t  BCC;                               // Bit counter compare
//   uint32_t  RESERVED11[39];
//   uint32_t  DAB[8];                            // Description collection[0]: Device address base segment 0
//   uint32_t  DAP[8];                            // Description collection[0]: Device address prefix 0
//   uint32_t  DACNF;                             // Device address match configuration
//   uint32_t  MHRMATCHCONF;                      // Search Pattern Configuration
//   uint32_t  MHRMATCHMAS;                       // Pattern mask
//   uint32_t  RESERVED12;
//   uint32_t  MODECNF0;                          // Radio mode configuration register 0
//   uint32_t  RESERVED13[3];
//   uint32_t  SFD;                               // IEEE 802.15.4 Start of Frame Delimiter
//   uint32_t  EDCNT;                             // IEEE 802.15.4 Energy Detect Loop Count
//   uint32_t  EDSAMPLE;                          // IEEE 802.15.4 Energy Detect Level
//   uint32_t  CCACTRL;                           // IEEE 802.15.4 Clear Channel Assessment Control
//   uint32_t  RESERVED14[611];
//   uint32_t  POWER;                             // Peripheral power control
// } NRF_RADIO_Type;

// #define NRF_RADIO                       ((NRF_RADIO_Type          *) NRF_RADIO_BASE)

#define NRF_RADIO_BASE                           0x40001000

#define NRF_RADIO_OFFSET_TASKS_TXEN              0x000
#define NRF_RADIO_OFFSET_TASKS_RXEN              0x004
#define NRF_RADIO_OFFSET_TASKS_START             0x008
#define NRF_RADIO_OFFSET_TASKS_STOP              0x00C
#define NRF_RADIO_OFFSET_TASKS_DISABLE           0x010
#define NRF_RADIO_OFFSET_TASKS_RSSISTART         0x014
#define NRF_RADIO_OFFSET_TASKS_RSSISTOP          0x018
#define NRF_RADIO_OFFSET_TASKS_BCSTART           0x01C
#define NRF_RADIO_OFFSET_TASKS_BCSTOP            0x020
#define NRF_RADIO_OFFSET_TASKS_EDSTART           0x024
#define NRF_RADIO_OFFSET_TASKS_EDSTOP            0x028
#define NRF_RADIO_OFFSET_TASKS_CCASTART          0x02C
#define NRF_RADIO_OFFSET_TASKS_CCASTOP           0x030
#define NRF_RADIO_OFFSET_EVENTS_READY            0x100
#define NRF_RADIO_OFFSET_EVENTS_ADDRESS          0x104
#define NRF_RADIO_OFFSET_EVENTS_PAYLOAD          0x108
#define NRF_RADIO_OFFSET_EVENTS_END              0x10C
#define NRF_RADIO_OFFSET_EVENTS_DISABLED         0x110
#define NRF_RADIO_OFFSET_EVENTS_DEVMATCH         0x114
#define NRF_RADIO_OFFSET_EVENTS_DEVMISS          0x118
#define NRF_RADIO_OFFSET_EVENTS_RSSIEND          0x11C
#define NRF_RADIO_OFFSET_EVENTS_BCMATCH          0x128
#define NRF_RADIO_OFFSET_EVENTS_CRCOK            0x130
#define NRF_RADIO_OFFSET_EVENTS_CRCERROR         0x134
#define NRF_RADIO_OFFSET_EVENTS_FRAMESTART       0x138
#define NRF_RADIO_OFFSET_EVENTS_EDEND            0x13C
#define NRF_RADIO_OFFSET_EVENTS_EDSTOPPED        0x140
#define NRF_RADIO_OFFSET_EVENTS_CCAIDLE          0x144
#define NRF_RADIO_OFFSET_EVENTS_CCABUSY          0x148
#define NRF_RADIO_OFFSET_EVENTS_CCASTOPPED       0x14C
#define NRF_RADIO_OFFSET_EVENTS_RATEBOOST        0x150
#define NRF_RADIO_OFFSET_EVENTS_TXREADY          0x154
#define NRF_RADIO_OFFSET_EVENTS_RXREADY          0x158
#define NRF_RADIO_OFFSET_EVENTS_MHRMATCH         0x15C
#define NRF_RADIO_OFFSET_SHORTS                  0x200
#define NRF_RADIO_OFFSET_INTENSET                0x304
#define NRF_RADIO_OFFSET_INTENCLR                0x308
#define NRF_RADIO_OFFSET_CRCSTATUS               0x400
#define NRF_RADIO_OFFSET_RXMATCH                 0x408
#define NRF_RADIO_OFFSET_RXCRC                   0x40C
#define NRF_RADIO_OFFSET_DAI                     0x410
#define NRF_RADIO_OFFSET_PACKETPTR               0x504
#define NRF_RADIO_OFFSET_FREQUENCY               0x508
#define NRF_RADIO_OFFSET_TXPOWER                 0x50C
#define NRF_RADIO_OFFSET_MODE                    0x510
#define NRF_RADIO_OFFSET_PCNF0                   0x514
#define NRF_RADIO_OFFSET_PCNF1                   0x518
#define NRF_RADIO_OFFSET_BASE0                   0x51C
#define NRF_RADIO_OFFSET_BASE1                   0x520
#define NRF_RADIO_OFFSET_PREFIX0                 0x524
#define NRF_RADIO_OFFSET_PREFIX1                 0x528
#define NRF_RADIO_OFFSET_TXADDRESS               0x52C
#define NRF_RADIO_OFFSET_RXADDRESSES             0x530
#define NRF_RADIO_OFFSET_CRCCNF                  0x534
#define NRF_RADIO_OFFSET_CRCPOLY                 0x538
#define NRF_RADIO_OFFSET_CRCINIT                 0x53C
#define NRF_RADIO_OFFSET_TIFS                    0x544
#define NRF_RADIO_OFFSET_RSSISAMPLE              0x548
#define NRF_RADIO_OFFSET_STATE                   0x550
#define NRF_RADIO_OFFSET_DATAWHITEIV             0x554
#define NRF_RADIO_OFFSET_BCC                     0x560
#define NRF_RADIO_OFFSET_DAB(_offset)            (0x600 + 0x04 * _offset)
#define NRF_RADIO_OFFSET_DAP(_offset)            (0x620 + 0x04 * _offset)
#define NRF_RADIO_OFFSET_DACNF                   0x640
#define NRF_RADIO_OFFSET_MHRMATCHCONF            0x644
#define NRF_RADIO_OFFSET_MHRMATCHMAS             0x648
#define NRF_RADIO_OFFSET_MODECNF0                0x650
#define NRF_RADIO_OFFSET_SFD                     0x660
#define NRF_RADIO_OFFSET_EDCNT                   0x664
#define NRF_RADIO_OFFSET_EDSAMPLE                0x668
#define NRF_RADIO_OFFSET_CCACTRL                 0x66C
#define NRF_RADIO_OFFSET_POWER                   0xFFC

#define TASKS_TXEN              *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_TXEN))
#define TASKS_RXEN              *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_RXEN))
#define TASKS_START             *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_START))
#define TASKS_STOP              *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_STOP))
#define TASKS_DISABLE           *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_DISABLE))
#define TASKS_RSSISTART         *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_RSSISTART))
#define TASKS_RSSISTOP          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_RSSISTOP))
#define TASKS_BCSTART           *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_BCSTART))
#define TASKS_BCSTOP            *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_BCSTOP))
#define TASKS_EDSTART           *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_EDSTART))
#define TASKS_EDSTOP            *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_EDSTOP))
#define TASKS_CCASTART          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_CCASTART))
#define TASKS_CCASTOP           *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_TASKS_CCASTOP))
#define EVENTS_READY            *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_READY))
#define EVENTS_ADDRESS          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_ADDRESS))
#define EVENTS_PAYLOAD          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_PAYLOAD))
#define EVENTS_END              *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_END))
#define EVENTS_DISABLED         *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_DISABLED))
#define EVENTS_DEVMATCH         *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_DEVMATCH))
#define EVENTS_DEVMISS          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_DEVMISS))
#define EVENTS_RSSIEND          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_RSSIEND))
#define EVENTS_BCMATCH          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_BCMATCH))
#define EVENTS_CRCOK            *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_CRCOK))
#define EVENTS_CRCERROR         *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_CRCERROR))
#define EVENTS_FRAMESTART       *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_FRAMESTART))
#define EVENTS_EDEND            *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_EDEND))
#define EVENTS_EDSTOPPED        *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_EDSTOPPED))
#define EVENTS_CCAIDLE          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_CCAIDLE))
#define EVENTS_CCABUSY          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_CCABUSY))
#define EVENTS_CCASTOPPED       *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_CCASTOPPED))
#define EVENTS_RATEBOOST        *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_RATEBOOST))
#define EVENTS_TXREADY          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_TXREADY))
#define EVENTS_RXREADY          *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_RXREADY))
#define EVENTS_MHRMATCH         *((volatile uint32_t*) (NRF_RADIO_BASE + NRF_RADIO_OFFSET_EVENTS_MHRMATCH))

//------------------------------------------------------------------------------

// typedef struct {                               // GPIO Structure
//   uint32_t  RESERVED0[321];
//   uint32_t  OUT;                               // Write GPIO port
//   uint32_t  OUTSET;                            // Set individual bits in GPIO port
//   uint32_t  OUTCLR;                            // Clear individual bits in GPIO port
//   uint32_t  IN;                                // Read GPIO port
//   uint32_t  DIR;                               // Direction of GPIO pins
//   uint32_t  DIRSET;                            // DIR set register
//   uint32_t  DIRCLR;                            // DIR clear register
//   uint32_t  LATCH;                             // Latch register indicating what GPIO pins that have met the criteria set in the PIN_CNF[n].SENSE registers
//   uint32_t  DETECTMODE;                        // Select between default DETECT signal behaviour and LDETECT mode
//   uint32_t  RESERVED1[118];
//   uint32_t  PIN_CNF[32];                       // Description collection[0]: Configuration of GPIO pins
// } NRF_GPIO_Type;
//
// #define NRF_P0                          ((NRF_GPIO_Type*) NRF_P0_BASE)
// #define NRF_P1                          ((NRF_GPIO_Type*) NRF_P1_BASE)

#define NRF_P0_BASE                       0x50000000
#define NRF_P1_BASE                       0x50000300

#define NRF_GPIO_OFFSET_OUT               0x504
#define NRF_GPIO_OFFSET_OUTSET            0x508
#define NRF_GPIO_OFFSET_OUTCLR            0x50C
#define NRF_GPIO_OFFSET_IN                0x510
#define NRF_GPIO_OFFSET_DIR               0x514
#define NRF_GPIO_OFFSET_DIRSET            0x518
#define NRF_GPIO_OFFSET_DIRCLR            0x51C
#define NRF_GPIO_OFFSET_LATCH             0x520
#define NRF_GPIO_OFFSET_DETECTMODE        0x524
#define NRF_GPIO_OFFSET_PIN_CNF(_offset)  (0x700 + 0x04 * _offset)

#define OUTSET_P0            *((volatile uint32_t*) (NRF_P0_BASE + NRF_GPIO_OFFSET_OUTSET))
#define OUTCLR_P0            *((volatile uint32_t*) (NRF_P0_BASE + NRF_GPIO_OFFSET_OUTCLR))
#define DIRSET_P0            *((volatile uint32_t*) (NRF_P0_BASE + NRF_GPIO_OFFSET_DIRSET))

#define OUTSET_P1            *((volatile uint32_t*) (NRF_P1_BASE + NRF_GPIO_OFFSET_OUTSET))
#define OUTCLR_P1            *((volatile uint32_t*) (NRF_P1_BASE + NRF_GPIO_OFFSET_OUTCLR))
#define DIRSET_P2            *((volatile uint32_t*) (NRF_P1_BASE + NRF_GPIO_OFFSET_DIRSET))

#define DEBUG_EXT_INT_LATENCY_PIN         0
#define DEBUG_EXT_INT_LATENCY_PORT        0
#define DEBUG_EXT_INT_LATENCY_STATE

#define DEBUG_SOFT_INT_LATENCY_PIN        0
#define DEBUG_SOFT_INT_LATENCY_PORT       0
#define DEBUG_SOFT_INT_LATENCY_STATE

#define DEBUG_CONN_EVT_PIN                0
#define DEBUG_CONN_EVT_PORT               0
#define DEBUG_CONN_EVT_STATE

#define DEBUG_RX_START_PIN                0
#define DEBUG_RX_START_PORT               0
#define DEBUG_RX_START_STATE

#define DEBUG_RX_STOP_PIN                 0
#define DEBUG_RX_STOP_PORT                0
#define DEBUG_RX_STOP_STATE

#define DEBUG_TX_START_PIN                0
#define DEBUG_TX_START_PORT               0
#define DEBUG_TX_START_STATE

#define DEBUG_TX_STOP_PIN                 0
#define DEBUG_TX_STOP_PORT                0
#define DEBUG_TX_STOP_STATE

#define DEBUG_STATE_TX_PIN                0
#define DEBUG_STATE_TX_PORT               0
#define DEBUG_STATE_TX_STATE

#define DEBUG_STATE_RX_PIN                0
#define DEBUG_STATE_RX_PORT               0
#define DEBUG_STATE_RX_STATE

#define DEBUG_DISABLE_PIN                 0
#define DEBUG_DISABLE_PORT                0
#define DEBUG_DISABLE_STATE

#define SET_DEBUG_PIN(_pin, _port)  (_port) ? (DIRSET_P1 |= 1 << _pin) : \
                                              (DIRSET_P0 |= 1 << _pin);  \
                                    (_port) ? (OUTCLR_P1 |= 1 << _pin) : \
                                              (OUTCLR_P0 |= 1 << _pin)   \

#define DEBUG_INIT()  do {                              \
        DIRSET_P0 = 0x0000000;                          \
        DIRSET_P1 = 0x0000000;                          \
        SET_DEBUG_PIN(DEBUG_CONN_EVT_PIN,               \
                      DEBUG_CONN_EVT_PORT);             \
        } while (0)

#endif // __DEBUG_NRF52840__
