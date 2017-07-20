#include "iDebug_nRF52840.h"

#define DEBUG_PIN_INIT(_pin, _port)   _port->DIRSET |= (1 << _pin)

#define NONE      GPIOTE_CONFIG_POLARITY_None
#define LOTOHI    GPIOTE_CONFIG_POLARITY_LoToHi
#define HITOLO    GPIOTE_CONFIG_POLARITY_HiToLo
#define TOGGLE    GPIOTE_CONFIG_POLARITY_Toggle

#define CRCOK     7
#define CRCERROR  6
#define RTXIDLE   5
#define RTX       4
#define CH3       3
#define CH2       2
#define CH1       1

static void iDebug_gpiote_config(uint8_t ch, uint8_t pin, uint8_t port, uint8_t polarity)
{
  NRF_GPIOTE->CONFIG[ch]  = (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos & GPIOTE_CONFIG_MODE_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (pin << GPIOTE_CONFIG_PSEL_Pos & GPIOTE_CONFIG_PSEL_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (port << GPIOTE_CONFIG_PORT_Pos & GPIOTE_CONFIG_PORT_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (polarity << GPIOTE_CONFIG_POLARITY_Pos & GPIOTE_CONFIG_POLARITY_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos & GPIOTE_CONFIG_OUTINIT_Msk);
}

static void iDebug_ppi_config(uint8_t ch, int8_t group, volatile uint32_t* event, volatile uint32_t* task, volatile uint32_t* fork)
{
  NRF_PPI->CHEN       |= (1 << ch);
  NRF_PPI->CH[ch].EEP  = (uint32_t) event;
  NRF_PPI->CH[ch].TEP  = (uint32_t) task;

  if(fork)  {
    NRF_PPI->FORK[ch].TEP = (uint32_t) fork;
  }

  if(group >= 0) {
    NRF_PPI->CHG[group] |= (1 << ch);
  }
}

void iDebug_init()
{
  DEBUG_PIN_INIT( DEBUG_START_PIN,
                  DEBUG_START_PORT);
  DEBUG_PIN_INIT( EXT_INT_LATENCY_PIN,
                  EXT_INT_LATENCY_PORT);
  DEBUG_PIN_INIT( BLE_CONN_EVT_PIN,
                  BLE_CONN_EVT_PORT);
  DEBUG_PIN_INIT( RADIO_DISABLED_PIN,
                  RADIO_DISABLED_PORT);
  DEBUG_PIN_INIT( RADIO_TX_PIN,
                  RADIO_TX_PORT);
  DEBUG_PIN_INIT( RADIO_RX_PIN,
                  RADIO_RX_PORT);
  DEBUG_PIN_INIT( RADIO_RTXIDLE_PIN,
                  RADIO_RTXIDLE_PORT);
  DEBUG_PIN_INIT( RADIO_RTX_PIN,
                  RADIO_RTX_PORT);
  DEBUG_PIN_INIT( RADIO_CRCOK_PIN,
                  RADIO_CRCOK_PORT);
  DEBUG_PIN_INIT( RADIO_CRCERROR_PIN,
                  RADIO_CRCERROR_PORT);

  iDebug_gpiote_config(CRCOK, RADIO_CRCOK_PIN, (RADIO_CRCOK_PORT == NRF_P0) ? 0 : 1, NONE);
  iDebug_gpiote_config(CRCERROR, RADIO_CRCERROR_PIN, (RADIO_CRCERROR_PORT == NRF_P0) ? 0 : 1, NONE);
  iDebug_gpiote_config(RTXIDLE, RADIO_RTXIDLE_PIN, (RADIO_RTXIDLE_PORT == NRF_P0) ? 0 : 1, NONE);
  iDebug_gpiote_config(RTX, RADIO_RTX_PIN, (RADIO_RTX_PORT == NRF_P0) ? 0 : 1, NONE);

  iDebug_ppi_config(19, -1, &NRF_RADIO->EVENTS_CRCOK, &NRF_GPIOTE->TASKS_SET[CRCOK], NULL);
  iDebug_ppi_config(18, -1, &NRF_RADIO->EVENTS_CRCERROR, &NRF_GPIOTE->TASKS_SET[CRCERROR], NULL);

  iDebug_ppi_config(17, -1, &NRF_RADIO->EVENTS_END, &NRF_GPIOTE->TASKS_SET[RTXIDLE], &NRF_GPIOTE->TASKS_CLR[RTX]);
  iDebug_ppi_config(16, -1, &NRF_RADIO->EVENTS_READY, &NRF_GPIOTE->TASKS_SET[RTXIDLE], &NRF_GPIOTE->TASKS_CLR[RTX]);

  iDebug_ppi_config(15, -1, &NRF_RADIO->EVENTS_ADDRESS, &NRF_GPIOTE->TASKS_SET[RTX], &NRF_GPIOTE->TASKS_CLR[RTXIDLE]);

  // #define MY_DEBUG_PIN        12
  // #define MY_DEBUG_PORT        NRF_P1
  // iDebug_gpiote_config(CH3, MY_DEBUG_PIN, (MY_DEBUG_PORT == NRF_P0) ? 0 : 1, TOGGLE);
  // iDebug_ppi_config(14, -1, &NRF_TIMER0->EVENTS_COMPARE[0], &NRF_GPIOTE->TASKS_OUT[CH3], NULL);

  // ???
  // TASKS_START
  // TASKS_RXEN
  // TASKS_TXEN

  iPrint("[INIT] DEBUG initialized\n");
}
