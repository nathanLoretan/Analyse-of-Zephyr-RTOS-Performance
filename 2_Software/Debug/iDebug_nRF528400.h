#ifndef __DEBUG_NRF52840__
#define __DEBUG_NRF52840__

#include "../configuration.h"

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

#define NRF_RADIO_ADDR                           0x40001000
// #define NRF_RADIO                       ((NRF_RADIO_Type          *) DEBUG_NRF_RADIO)

// #define DEBUG_NRF_RADIO                           0x40001000

// nrf_radio.h nrf_radio_task_t  -> nrf_radio_task_trigger()
//                               -> nrf_radio_event_clear()

// #define NRF_RADIO_OFFSET_TASKS_TXEN              0x000
// #define NRF_RADIO_OFFSET_TASKS_RXEN              0x004
// #define NRF_RADIO_OFFSET_TASKS_START             0x008
// #define NRF_RADIO_OFFSET_TASKS_STOP              0x00C
// #define NRF_RADIO_OFFSET_TASKS_DISABLE           0x010
// #define NRF_RADIO_OFFSET_TASKS_RSSISTART         0x014
// #define NRF_RADIO_OFFSET_TASKS_RSSISTOP          0x018
// #define NRF_RADIO_OFFSET_TASKS_BCSTART           0x01C
// #define NRF_RADIO_OFFSET_TASKS_BCSTOP            0x020
// #define NRF_RADIO_OFFSET_TASKS_EDSTART           0x024
// #define NRF_RADIO_OFFSET_TASKS_EDSTOP            0x028
// #define NRF_RADIO_OFFSET_TASKS_CCASTART          0x02C
// #define NRF_RADIO_OFFSET_TASKS_CCASTOP           0x030
// #define NRF_RADIO_OFFSET_EVENTS_READY            0x100
// #define NRF_RADIO_OFFSET_EVENTS_ADDRESS          0x104
// #define NRF_RADIO_OFFSET_EVENTS_PAYLOAD          0x108
// #define NRF_RADIO_OFFSET_EVENTS_END              0x10C
// #define NRF_RADIO_OFFSET_EVENTS_DISABLED         0x110
// #define NRF_RADIO_OFFSET_EVENTS_DEVMATCH         0x114
// #define NRF_RADIO_OFFSET_EVENTS_DEVMISS          0x118
// #define NRF_RADIO_OFFSET_EVENTS_RSSIEND          0x11C
// #define NRF_RADIO_OFFSET_EVENTS_BCMATCH          0x128
// #define NRF_RADIO_OFFSET_EVENTS_CRCOK            0x130
// #define NRF_RADIO_OFFSET_EVENTS_CRCERROR         0x134
// #define NRF_RADIO_OFFSET_EVENTS_FRAMESTART       0x138
// #define NRF_RADIO_OFFSET_EVENTS_EDEND            0x13C
// #define NRF_RADIO_OFFSET_EVENTS_EDSTOPPED        0x140
// #define NRF_RADIO_OFFSET_EVENTS_CCAIDLE          0x144
// #define NRF_RADIO_OFFSET_EVENTS_CCABUSY          0x148
// #define NRF_RADIO_OFFSET_EVENTS_CCASTOPPED       0x14C
// #define NRF_RADIO_OFFSET_EVENTS_RATEBOOST        0x150
// #define NRF_RADIO_OFFSET_EVENTS_TXREADY          0x154
// #define NRF_RADIO_OFFSET_EVENTS_RXREADY          0x158
// #define NRF_RADIO_OFFSET_EVENTS_MHRMATCH         0x15C
// #define NRF_RADIO_OFFSET_SHORTS                  0x200
// #define NRF_RADIO_OFFSET_INTENSET                0x304
// #define NRF_RADIO_OFFSET_INTENCLR                0x308
// #define NRF_RADIO_OFFSET_CRCSTATUS               0x400
// #define NRF_RADIO_OFFSET_RXMATCH                 0x408
// #define NRF_RADIO_OFFSET_RXCRC                   0x40C
// #define NRF_RADIO_OFFSET_DAI                     0x410
// #define NRF_RADIO_OFFSET_PACKETPTR               0x504
// #define NRF_RADIO_OFFSET_FREQUENCY               0x508
// #define NRF_RADIO_OFFSET_TXPOWER                 0x50C
// #define NRF_RADIO_OFFSET_MODE                    0x510
// #define NRF_RADIO_OFFSET_PCNF0                   0x514
// #define NRF_RADIO_OFFSET_PCNF1                   0x518
// #define NRF_RADIO_OFFSET_BASE0                   0x51C
// #define NRF_RADIO_OFFSET_BASE1                   0x520
// #define NRF_RADIO_OFFSET_PREFIX0                 0x524
// #define NRF_RADIO_OFFSET_PREFIX1                 0x528
// #define NRF_RADIO_OFFSET_TXADDRESS               0x52C
// #define NRF_RADIO_OFFSET_RXADDRESSES             0x530
// #define NRF_RADIO_OFFSET_CRCCNF                  0x534
// #define NRF_RADIO_OFFSET_CRCPOLY                 0x538
// #define NRF_RADIO_OFFSET_CRCINIT                 0x53C
// #define NRF_RADIO_OFFSET_TIFS                    0x544
// #define NRF_RADIO_OFFSET_RSSISAMPLE              0x548
// #define NRF_RADIO_OFFSET_STATE                   0x550
// #define NRF_RADIO_OFFSET_DATAWHITEIV             0x554
// #define NRF_RADIO_OFFSET_BCC                     0x560
// #define NRF_RADIO_OFFSET_DAB(_offset)            (0x600 + 0x04 * _offset)
// #define NRF_RADIO_OFFSET_DAP(_offset)            (0x620 + 0x04 * _offset)
// #define NRF_RADIO_OFFSET_DACNF                   0x640
// #define NRF_RADIO_OFFSET_MHRMATCHCONF            0x644
// #define NRF_RADIO_OFFSET_MHRMATCHMAS             0x648
// #define NRF_RADIO_OFFSET_MODECNF0                0x650
// #define NRF_RADIO_OFFSET_SFD                     0x660
// #define NRF_RADIO_OFFSET_EDCNT                   0x664
// #define NRF_RADIO_OFFSET_EDSAMPLE                0x668
// #define NRF_RADIO_OFFSET_CCACTRL                 0x66C
// #define NRF_RADIO_OFFSET_POWER                   0xFFC
//
// #define DEBUG_TSK_TXEN             *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_TXEN))
// #define DEBUG_TSK_RXEN             *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_RXEN))
// #define DEBUG_TSK_START            *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_START))
// #define DEBUG_TSK_STOP             *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_STOP))
// #define DEBUG_TSK_DISABLE          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_DISABLE))
// #define DEBUG_TSK_RSSISTART        *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_RSSISTART))
// #define DEBUG_TSK_RSSISTOP         *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_RSSISTOP))
// #define DEBUG_TSK_BCSTART          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_BCSTART))
// #define DEBUG_TSK_BCSTOP           *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_BCSTOP))
// #define DEBUG_TSK_EDSTART          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_EDSTART))
// #define DEBUG_TSK_EDSTOP           *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_EDSTOP))
// #define DEBUG_TSK_CCASTART         *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_CCASTART))
// #define DEBUG_TSK_CCASTOP          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_TASKS_CCASTOP))
// #define DEBUG_EVT_READY            *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_READY))
// #define DEBUG_EVT_ADDRESS          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_ADDRESS))
// #define DEBUG_EVT_PAYLOAD          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_PAYLOAD))
// #define DEBUG_EVT_END              *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_END))
// #define DEBUG_EVT_DISABLED         *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_DISABLED))
// #define DEBUG_EVT_DEVMATCH         *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_DEVMATCH))
// #define DEBUG_EVT_DEVMISS          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_DEVMISS))
// #define DEBUG_EVT_RSSIEND          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_RSSIEND))
// #define DEBUG_EVT_BCMATCH          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_BCMATCH))
// #define DEBUG_EVT_CRCOK            *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_CRCOK))
// #define DEBUG_EVT_CRCERROR         *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_CRCERROR))
// #define DEBUG_EVT_FRAMESTART       *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_FRAMESTART))
// #define DEBUG_EVT_EDEND            *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_EDEND))
// #define DEBUG_EVT_EDSTOPPED        *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_EDSTOPPED))
// #define DEBUG_EVT_CCAIDLE          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_CCAIDLE))
// #define DEBUG_EVT_CCABUSY          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_CCABUSY))
// #define DEBUG_EVT_CCASTOPPED       *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_CCASTOPPED))
// #define DEBUG_EVT_RATEBOOST        *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_RATEBOOST))
// #define DEBUG_EVT_TXREADY          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_TXREADY))
// #define DEBUG_EVT_RXREADY          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_RXREADY))
// #define DEBUG_EVT_MHRMATCH         *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_EVENTS_MHRMATCH))
//
// #define DEBUG_RADIO_STATE          *((volatile uint32_t*) (DEBUG_NRF_RADIO + NRF_RADIO_OFFSET_STATE))

#define RADIO_STATE_DISABLED    0
#define RADIO_STATE_RXRU        1
#define RADIO_STATE_RX          2
#define RADIO_STATE_RXIDLE      3
#define RADIO_STATE_RXDISABLE   4
#define RADIO_STATE_TXRU        9
#define RADIO_STATE_TXIDLE      10
#define RADIO_STATE_TX          11
#define RADIO_STATE_TXDISABLE   12

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
// #define NRF_P0                          ((NRF_GPIO_Type*) DEBUG_NRF_GPIO_P0)
// #define NRF_P1                          ((NRF_GPIO_Type*) DEBUG_NRF_GPIO_P1)

#define DEBUG_NRF_GPIO_P0                       0x50000000
#define DEBUG_NRF_GPIO_P1                       0x50000300

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

#define OUT_P0               *((volatile uint32_t*) (DEBUG_NRF_GPIO_P0 + NRF_GPIO_OFFSET_OUT))
#define OUTSET_P0            *((volatile uint32_t*) (DEBUG_NRF_GPIO_P0 + NRF_GPIO_OFFSET_OUTSET))
#define OUTCLR_P0            *((volatile uint32_t*) (DEBUG_NRF_GPIO_P0 + NRF_GPIO_OFFSET_OUTCLR))
#define DIRSET_P0            *((volatile uint32_t*) (DEBUG_NRF_GPIO_P0 + NRF_GPIO_OFFSET_DIRSET))

#define OUT_P1               *((volatile uint32_t*) (DEBUG_NRF_GPIO_P1 + NRF_GPIO_OFFSET_OUT))
#define OUTSET_P1            *((volatile uint32_t*) (DEBUG_NRF_GPIO_P1 + NRF_GPIO_OFFSET_OUTSET))
#define OUTCLR_P1            *((volatile uint32_t*) (DEBUG_NRF_GPIO_P1 + NRF_GPIO_OFFSET_OUTCLR))
#define DIRSET_P1            *((volatile uint32_t*) (DEBUG_NRF_GPIO_P1 + NRF_GPIO_OFFSET_DIRSET))

//------------------------------------------------------------------------------

// typedef struct {                                    // PPI Structure
//   PPI_TASKS_CHG_Type TASKS_CHG[6];                  // Channel group tasks
//   __I  uint32_t  RESERVED0[308];
//   __IO uint32_t  CHEN;                              // Channel enable register
//   __IO uint32_t  CHENSET;                           // Channel enable set register
//   __IO uint32_t  CHENCLR;                           // Channel enable clear register
//   __I  uint32_t  RESERVED1;
//   PPI_CH_Type CH[20];                               // PPI Channel
//   __I  uint32_t  RESERVED2[148];
//   __IO uint32_t  CHG[6];                            // Description collection[0]: Channel group 0
//   __I  uint32_t  RESERVED3[62];
//   PPI_FORK_Type FORK[32];                           // Fork
// } NRF_PPI_Type;

#define DEBUG_NRF_PPI         0x4001F000

#define NRF_PPI_TASKS_CHGEN(_offset)    (0x000 + 0x08 * _offset)
#define NRF_PPI_TASKS_CHGDIS(_offset)   (0x004 + 0x08 * _offset)
#define NRF_PPI_CHEN                    0x500
#define NRF_PPI_CHENSET                 0x504
#define NRF_PPI_CHENCLR                 0x508
#define NRF_PPI_CHEEP(_offset)          (0x510 + 0x08 * _offset)
#define NRF_PPI_CHTEP(_offset)          (0x514 + 0x08 * _offset)
#define NRF_PPI_CHG(_offset)            (0x800 + 0x04 * _offset)
#define NRF_PPI_ORK(_offset)            (0x910 + 0x04 * _offset)

//------------------------------------------------------------------------------

#define DEBUG_PIN_SET(_pin, _port, _flag)       \
do {                                            \
  if(_flag) {                                   \
    (_port) ? (OUT_P1 |= (0x01 << _pin)) :      \
              (OUT_P0 |= (0x01 << _pin)); }     \
  else {                                        \
    (_port) ? (OUT_P1 &= ~(0x01 << _pin)) :     \
              (OUT_P0 &= ~(0x01 << _pin));  }   \
} while(0)

#define DEBUG_PIN_TOGGLE(_pin, _port)                           \
do {                                                            \
  if(_port) {                                                   \
    (OUT_P1 & (0x01 << _pin)) ? (OUT_P1 &= ~(0x01 << _pin)) :   \
                                (OUT_P1 |= (0x01 << _pin)); }   \
  else {                                                        \
    (OUT_P0 & (0x01 << _pin)) ? (OUT_P0 &= ~(0x01 << _pin)) :   \
                                (OUT_P0 |= (0x01 << _pin)); }   \
} while(0)

#define DEBUG_PIN_INIT(_pin, _port)   (_port) ? (DIRSET_P1 |= 1 << _pin) : \
                                                (DIRSET_P0 |= 1 << _pin);  \

#define DEBUG_PIN         13
#define DEBUG_PORT        1
#define DEBUG(_flag)      DEBUG_PIN_SET(DEBUG_PIN, DEBUG_PORT, _flag)

#define DEBUG_EXT_INT_LATENCY_PIN         31
#define DEBUG_EXT_INT_LATENCY_PORT        0
#define DEBUG_EXT_INT_LATENCY()           DEBUG_PIN_TOGGLE(DEBUG_EXT_INT_LATENCY_PIN, DEBUG_EXT_INT_LATENCY_PORT)

#define DEBUG_SOFT_INT_LATENCY_PIN        30
#define DEBUG_SOFT_INT_LATENCY_PORT       0
#define DEBUG_SOFT_INT_LATENCY()          DEBUG_PIN_TOGGLE(DEBUG_SOFT_INT_LATENCY_PIN, DEBUG_SOFT_INT_LATENCY_PORT)

// Zephyr:
//    -> isr_rx_conn()     Zephyr/subsys/bluetooth/controller/ll_sw/ctrl.c
#define DEBUG_CONN_EVT_PIN                12
#define DEBUG_CONN_EVT_PORT               1
#define DEBUG_CONN_EVT(_flag)             DEBUG_PIN_SET(DEBUG_CONN_EVT_PIN, DEBUG_CONN_EVT_PORT, _flag)

// Inspired by the State Machine from nrf52840 Datasheet p.252 Figure.35
#define RADIO_SM_DISABLED_PIN           2
#define RADIO_SM_DISABLED_PORT          1
// #define RADIO_SM_DISABLED(_flag)        DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, _flag)
#define RADIO_SM_DISABLED()         DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, DEBUG_RADIO_STATE & RADIO_STATE_DISABLED)

#define RADIO_SM_TXRU_PIN               3
#define RADIO_SM_TXRU_PORT              1
// #define RADIO_SM_TXRU(_flag)            DEBUG_PIN_SET(RADIO_SM_TXRU_PIN, RADIO_SM_TXRU_PORT, _flag)
#define RADIO_SM_TXRU()            DEBUG_PIN_SET(RADIO_SM_TXRU_PIN, RADIO_SM_TXRU_PORT, DEBUG_RADIO_STATE & RADIO_STATE_TXRU)

#define RADIO_SM_TXIDLE_PIN             4
#define RADIO_SM_TXIDLE_PORT            1
// #define RADIO_SM_TXIDLE(_flag)          DEBUG_PIN_SET(RADIO_SM_TXIDLE_PIN, RADIO_SM_TXIDLE_PORT, _flag)
#define RADIO_SM_TXIDLE()          DEBUG_PIN_SET(RADIO_SM_TXIDLE_PIN, RADIO_SM_TXIDLE_PORT, DEBUG_RADIO_STATE & RADIO_STATE_TXIDLE)

#define RADIO_SM_TX_PIN                 5
#define RADIO_SM_TX_PORT                1
// #define RADIO_SM_TX(_flag)              DEBUG_PIN_SET(RADIO_SM_RX_PIN, RADIO_SM_RX_PORT, _flag)
#define RADIO_SM_TX()              DEBUG_PIN_SET(RADIO_SM_RX_PIN, RADIO_SM_RX_PORT, DEBUG_RADIO_STATE & RADIO_STATE_TX)

#define RADIO_SM_TXDISABLE_PIN          6
#define RADIO_SM_TXDISABLE_PORT         1
// #define RADIO_SM_TXDISABLE(_flag)       DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, _flag)
#define RADIO_SM_TXDISABLE()       DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, DEBUG_RADIO_STATE & RADIO_STATE_TXDISABLE)

#define RADIO_SM_RXRU_PIN               7
#define RADIO_SM_RXRU_PORT              1
// #define RADIO_SM_RXRU(_flag)            DEBUG_PIN_SET(RADIO_SM_RXRU_PIN, RADIO_SM_RXRU_PORT, _flag)
#define RADIO_SM_RXRU()            DEBUG_PIN_SET(RADIO_SM_RXRU_PIN, RADIO_SM_RXRU_PORT, DEBUG_RADIO_STATE & RADIO_STATE_RXRU)

#define RADIO_SM_RXIDLE_PIN             8
#define RADIO_SM_RXIDLE_PORT            1
// #define RADIO_SM_RXIDLE(_flag)          DEBUG_PIN_SET(RADIO_SM_RXIDLE_PIN, RADIO_SM_RXIDLE_PORT, _flag)
#define RADIO_SM_RXIDLE()          DEBUG_PIN_SET(RADIO_SM_RXIDLE_PIN, RADIO_SM_RXIDLE_PORT, DEBUG_RADIO_STATE & RADIO_STATE_RXIDLE)

#define RADIO_SM_RX_PIN                 10
#define RADIO_SM_RX_PORT                1
// #define RADIO_SM_RX(_flag)              DEBUG_PIN_SET(RADIO_SM_RX_PIN, RADIO_SM_RX_PORT, _flag)
#define RADIO_SM_RX()              DEBUG_PIN_SET(RADIO_SM_RX_PIN, RADIO_SM_RX_PORT, DEBUG_RADIO_STATE & RADIO_STATE_RX)

#define RADIO_SM_RXDISABLE_PIN          11
#define RADIO_SM_RXDISABLE_PORT         1
// #define RADIO_SM_RXDISABLE(_flag)       DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, _flag)
#define RADIO_SM_RXDISABLE()       DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, DEBUG_RADIO_STATE & RADIO_STATE_RXDISABLE)

// Zephyr:
//    -> radio_tx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_rx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_disable()       Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
#define DEBUG_RADIO_SM()      \
do {                          \
  RADIO_SM_DISABLED()         \
  RADIO_SM_TXRU()             \
  RADIO_SM_TXIDLE()           \
  RADIO_SM_TX()               \
  RADIO_SM_TXDISABLE()        \
  RADIO_SM_RXRU()             \
  RADIO_SM_RXIDLE()           \
  RADIO_SM_RX()               \
  RADIO_SM_RXDISABLE()        \
} while(0)


// void iDebug_radio_sm();
void iDebug_init();

#endif // __DEBUG_NRF52840__
