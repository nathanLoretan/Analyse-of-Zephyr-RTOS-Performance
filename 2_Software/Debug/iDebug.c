#include "iDebug.h"

#if ENABLE_MEASUREMENT

#define DEBUG_PIN_INIT(_pin, _port)   _port->DIRSET |= (1 << _pin)

#define NONE      GPIOTE_CONFIG_POLARITY_None
#define LOTOHI    GPIOTE_CONFIG_POLARITY_LoToHi
#define HITOLO    GPIOTE_CONFIG_POLARITY_HiToLo
#define TOGGLE    GPIOTE_CONFIG_POLARITY_Toggle

#define CRCOK     7
#define RX        (CRCOK - 1)
#define TX        (RX - 1)
#define RTX       (TX - 1)
#define CH3       (RTX - 1)
#define CH2       (CH3 - 1)
#define CH1       (CH2 - 1)

static void _gpiote_config(uint8_t ch, uint8_t pin, uint8_t port, uint8_t polarity)
{
  NRF_GPIOTE->CONFIG[ch]  = (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos & GPIOTE_CONFIG_MODE_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (pin << GPIOTE_CONFIG_PSEL_Pos & GPIOTE_CONFIG_PSEL_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (port << GPIOTE_CONFIG_PORT_Pos & GPIOTE_CONFIG_PORT_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (polarity << GPIOTE_CONFIG_POLARITY_Pos & GPIOTE_CONFIG_POLARITY_Msk);
  NRF_GPIOTE->CONFIG[ch] |= (GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos & GPIOTE_CONFIG_OUTINIT_Msk);
}

static void _ppi_config(uint8_t ch, int8_t group, volatile uint32_t* event, volatile uint32_t* task, volatile uint32_t* fork)
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
  DEBUG_PIN_INIT( RADIO_TX_PIN,
                  RADIO_TX_PORT);
  DEBUG_PIN_INIT( RADIO_RX_PIN,
                  RADIO_RX_PORT);
  DEBUG_PIN_INIT( RADIO_RTX_PIN,
                  RADIO_RTX_PORT);
  DEBUG_PIN_INIT( RADIO_CRCOK_PIN,
                  RADIO_CRCOK_PORT);
  DEBUG_PIN_INIT( BLE_ERROR_PIN,
                  BLE_ERROR_PORT);
  DEBUG_PIN_INIT( BLE_NOTIFY_PIN,
                  BLE_NOTIFY_PORT);
  DEBUG_PIN_INIT( BLE_INDICATE_PIN,
                  BLE_INDICATE_PORT);

  _gpiote_config(CRCOK, RADIO_CRCOK_PIN, (RADIO_CRCOK_PORT == NRF_P0) ? 0 : 1, NONE);
  _gpiote_config(TX,    RADIO_TX_PIN,    (RADIO_TX_PORT == NRF_P0)    ? 0 : 1, NONE);
  _gpiote_config(RX,    RADIO_RX_PIN,    (RADIO_RX_PORT == NRF_P0)    ? 0 : 1, NONE);
  _gpiote_config(RTX,   RADIO_RTX_PIN,   (RADIO_RTX_PORT == NRF_P0)   ? 0 : 1, NONE);


#ifdef 	ZEPHYR_USED
  _ppi_config(19, -1, &NRF_RADIO->EVENTS_CRCOK,   &NRF_GPIOTE->TASKS_SET[CRCOK], NULL);
  _ppi_config(18, -1, &NRF_RADIO->EVENTS_TXREADY, &NRF_GPIOTE->TASKS_SET[TX], &NRF_GPIOTE->TASKS_CLR[RX]);
  _ppi_config(17, -1, &NRF_RADIO->EVENTS_RXREADY, &NRF_GPIOTE->TASKS_SET[RX], &NRF_GPIOTE->TASKS_CLR[TX]);
#endif

#ifdef BAREMETALE_USED  // Channel 19 - 17 used by the softdevice
  _ppi_config(12, -1, &NRF_RADIO->EVENTS_CRCOK,   &NRF_GPIOTE->TASKS_SET[CRCOK], NULL);
  _ppi_config(11, -1, &NRF_RADIO->EVENTS_TXREADY, &NRF_GPIOTE->TASKS_SET[TX], &NRF_GPIOTE->TASKS_CLR[RX]);
  _ppi_config(10, -1, &NRF_RADIO->EVENTS_RXREADY, &NRF_GPIOTE->TASKS_SET[RX], &NRF_GPIOTE->TASKS_CLR[TX]);
#endif

  _ppi_config(16, -1, &NRF_RADIO->EVENTS_END,      &NRF_GPIOTE->TASKS_CLR[RTX], NULL);
  _ppi_config(15, -1, &NRF_RADIO->EVENTS_ADDRESS,  &NRF_GPIOTE->TASKS_SET[RTX], &NRF_GPIOTE->TASKS_CLR[CRCOK]);
  _ppi_config(14, -1, &NRF_RADIO->EVENTS_DISABLED, &NRF_GPIOTE->TASKS_CLR[RX],  &NRF_GPIOTE->TASKS_CLR[TX]);
  _ppi_config(13, -1, &NRF_RADIO->EVENTS_DISABLED, &NRF_GPIOTE->TASKS_CLR[RTX], &NRF_GPIOTE->TASKS_CLR[CRCOK]);

  DEBUG_START();
  iPrint("[INIT] DEBUG initialized\n");
}

#else // !ENABLE_MEASUREMENT

void iDebug_init()
{
  iPrint("/!\\ DEBUG not enabled\n");
}

#endif  // ENABLE_MEASUREMENT
