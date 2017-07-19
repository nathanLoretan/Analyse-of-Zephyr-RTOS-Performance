#include "iDebug_nRF52840.h"

// typedef enum {
// RADIO_STATE_DISABLED    = 0,
// RADIO_STATE_RXRU        = 1,
// RADIO_STATE_RXIDLE      = 2,
// RADIO_STATE_RX          = 3,
// RADIO_STATE_RXDISABLE   = 4,
// RADIO_STATE_TXRU        = 9,
// RADIO_STATE_TXIDLE      = 10,
// RADIO_STATE_TX          = 11,
// RADIO_STATE_TXDISABLE   = 12
// } radio_sm_state_t;

// radio_sm_state_t radio_sm_state = RADIO_SM_DISABLED_STATE;
//
// void iDebug_radio_sm()
// {
//   switch(radio_sm_state)
//   {
//     case RADIO_SM_DISABLED_STATE:   if(DEBUG_TSK_TXEN) {
//                                       radio_sm_state = RADIO_SM_TXRU_STATE;
//                                       RADIO_SM_DISABLED(0);
//                                       RADIO_SM_TXRU(1);
//                                     }
//                                     else if(DEBUG_TSK_RXEN) {
//                                       radio_sm_state = RADIO_SM_RXRU_STATE;
//                                       RADIO_SM_DISABLED(0);
//                                       RADIO_SM_RXRU(1);
//                                     }
//     break;
//
//     case RADIO_SM_TXRU_STATE:       if(!DEBUG_EVT_READY) {
//                                       radio_sm_state = RADIO_SM_TXIDLE_STATE;
//                                       RADIO_SM_TXRU(0);
//                                       RADIO_SM_TXIDLE(1);
//                                     }
//                                     else if(DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_TXDISABLE_STATE;
//                                       RADIO_SM_TXRU(0);
//                                       RADIO_SM_TXDISABLE(1);
//                                     }
//     break;
//
//     case RADIO_SM_TXIDLE_STATE:     if(DEBUG_TSK_START) {
//                                       radio_sm_state = RADIO_SM_TX_STATE;
//                                       RADIO_SM_TXIDLE(0);
//                                       RADIO_SM_TX(1);
//                                     }
//                                     else if(DEBUG_TSK_TXEN) {
//                                       radio_sm_state = RADIO_SM_TXRU_STATE;
//                                       RADIO_SM_TXIDLE(0);
//                                       RADIO_SM_TXRU(1);
//                                     }
//                                     else if(DEBUG_TSK_RXEN) {
//                                       radio_sm_state = RADIO_SM_RXRU_STATE;
//                                       RADIO_SM_TXIDLE(0);
//                                       RADIO_SM_RXRU(1);
//                                     }
//                                     else if(DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_TXDISABLE_STATE;
//                                       RADIO_SM_TXIDLE(0);
//                                       RADIO_SM_TXDISABLE(1);
//                                     }
//     break;
//
//     case RADIO_SM_TX_STATE:         if(DEBUG_TSK_STOP || !DEBUG_EVT_END) {
//                                       radio_sm_state = RADIO_SM_TXIDLE_STATE;
//                                       RADIO_SM_TX(0);
//                                       RADIO_SM_TXIDLE(1);
//                                     }
//                                     if(!DEBUG_EVT_ADDRESS || !DEBUG_EVT_PAYLOAD) {
//                                       radio_sm_state = RADIO_SM_TX_STATE;
//                                       RADIO_SM_TX(0);
//                                       RADIO_SM_TX(1);
//                                     }
//                                     else if(DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_TXDISABLE_STATE;
//                                       RADIO_SM_TX(0);
//                                       RADIO_SM_TXDISABLE(1);
//                                     }
//     break;
//
//     case RADIO_SM_TXDISABLE_STATE:  if(!DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_DISABLED_STATE;
//                                       RADIO_SM_TXDISABLE(0);
//                                       RADIO_SM_DISABLED(1);
//                                     }
//     break;
//
//     case RADIO_SM_RXRU_STATE:       if(!DEBUG_EVT_READY) {
//                                       radio_sm_state = RADIO_SM_RXIDLE_STATE;
//                                       RADIO_SM_RXRU(0);
//                                       RADIO_SM_RXIDLE(1);
//                                     }
//                                     else if(DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_RXDISABLE_STATE;
//                                       RADIO_SM_RXRU(0);
//                                       RADIO_SM_RXDISABLE(1);
//                                     }
//     break;
//
//     case RADIO_SM_RXIDLE_STATE:     if(DEBUG_TSK_START) {
//                                       radio_sm_state = RADIO_SM_RX_STATE;
//                                       RADIO_SM_RXIDLE(0);
//                                       RADIO_SM_RX(1);
//                                     }
//                                     else if(DEBUG_TSK_TXEN) {
//                                       radio_sm_state = RADIO_SM_TXRU_STATE;
//                                       RADIO_SM_RXIDLE(0);
//                                       RADIO_SM_TXRU(1);
//                                     }
//                                     else if(DEBUG_TSK_RXEN) {
//                                       radio_sm_state = RADIO_SM_RXRU_STATE;
//                                       RADIO_SM_RXIDLE(0);
//                                       RADIO_SM_RXRU(1);
//                                     }
//                                     else if(DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_RXDISABLE_STATE;
//                                       RADIO_SM_RXIDLE(0);
//                                       RADIO_SM_RXDISABLE(1);
//                                     }
//     break;
//
//     case RADIO_SM_RX_STATE:         if(DEBUG_TSK_STOP || !DEBUG_EVT_END) {
//                                       radio_sm_state = RADIO_SM_RXIDLE_STATE;
//                                       RADIO_SM_RX(0);
//                                       RADIO_SM_RXIDLE(1);
//                                     }
//                                     if(!DEBUG_EVT_ADDRESS || !DEBUG_EVT_PAYLOAD) {
//                                       radio_sm_state = RADIO_SM_RX_STATE;
//                                       RADIO_SM_RX(0);
//                                       RADIO_SM_RX(1);
//                                     }
//                                     else if(DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_TXDISABLE_STATE;
//                                       RADIO_SM_RX(0);
//                                       RADIO_SM_RXDISABLE(1);
//                                     }
//     break;
//
//     case RADIO_SM_RXDISABLE_STATE:  if(!DEBUG_TSK_DISABLE) {
//                                       radio_sm_state = RADIO_SM_DISABLED_STATE;
//                                       RADIO_SM_RXDISABLE(0);
//                                       RADIO_SM_DISABLED(1);
//                                     }
//     break;
//
//     default:
//     break;
//   }
//
//   DEBUG(0);
// }

void iDebug_init()
{
  NRF_P1->DIRSET = 0x0000000;
  NRF_P1->DIRSET = 0x0000000;
  DEBUG_PIN_INIT( DEBUG_PIN,
                  DEBUG_PORT);
  DEBUG_PIN_INIT( DEBUG_EXT_INT_LATENCY_PIN,
                  DEBUG_EXT_INT_LATENCY_PORT);
  DEBUG_PIN_INIT( DEBUG_SOFT_INT_LATENCY_PIN,
                  DEBUG_SOFT_INT_LATENCY_PORT);
  DEBUG_PIN_INIT( DEBUG_CONN_EVT_PIN,
                  DEBUG_CONN_EVT_PORT);
  DEBUG_PIN_INIT( RADIO_SM_DISABLED_PIN,
                  RADIO_SM_DISABLED_PORT);
  DEBUG_PIN_INIT( RADIO_SM_TXRU_PIN,
                  RADIO_SM_TXRU_PORT);
  DEBUG_PIN_INIT( RADIO_SM_TXIDLE_PIN,
                  RADIO_SM_TXIDLE_PORT);
  DEBUG_PIN_INIT( RADIO_SM_TX_PIN,
                  RADIO_SM_TX_PORT);
  DEBUG_PIN_INIT( RADIO_SM_TXDISABLE_PIN,
                  RADIO_SM_TXDISABLE_PORT);
  DEBUG_PIN_INIT( RADIO_SM_RXRU_PIN,
                  RADIO_SM_RXRU_PORT);
  DEBUG_PIN_INIT( RADIO_SM_RXIDLE_PIN,
                  RADIO_SM_RXIDLE_PORT);
  DEBUG_PIN_INIT( RADIO_SM_RX_PIN,
                  RADIO_SM_RX_PORT);
  DEBUG_PIN_INIT( RADIO_SM_RXDISABLE_PIN,
                  RADIO_SM_RXDISABLE_PORT);
  RADIO_SM_DISABLED(1);

  iPrint("[INIT] DEBUG initialized\n");
}
