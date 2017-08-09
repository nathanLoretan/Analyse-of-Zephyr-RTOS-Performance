#ifndef __NRF5X_INTERFACE_H__
#define __NRF5X_INTERFACE_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ble_uuid.h"

#include "SEGGER_RTT.h"

#include "fstorage.h"
#include "fds.h"
#include "peer_manager.h"
#include "ble_conn_state.h"

#include "nrf.h"
#include "boards.h"
#include "sensorsim.h"
#include "app_error.h"
#include "nordic_common.h"
#include "softdevice_handler.h"

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_delay.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_gpiote.h"
#include "nrf_ble_gatt.h"

#include "app_twi.h"
#include "app_timer.h"

#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"

#include "bsp.h"
#include "bsp_btn_ble.h"

#include "../../general_config.h"

//------------------------------------------------------------------//
// 														GENERAL		 														//
//------------------------------------------------------------------//
#define iPrint(...)					xxx

#define SLEEP()							xxx
#define iSleep()						xxx
#define iSleep_ms(_ms)			xxx

//------------------------------------------------------------------//
// 												 		ITIMER																//
//------------------------------------------------------------------//
typedef xxx		iTimer_t;
typedef xxx 	iTimer_handler_t;
#define TIMER_HANDLER(fn)   void fn(xxx)

void iTimer_start(iTimer_t* timer, iTimer_handler_t timer_handler, uint32_t period);
void iTimer_stop(timer)

//------------------------------------------------------------------//
// 														IGPIO			 														//
//------------------------------------------------------------------//
#define IGPIO_PORT						xxx

#define IGPIO_PIN_IN					xxx
#define IGPIO_PIN_OUT					xxx

#define IGPIO_RISING_EDGE			xxx
#define IGPIO_FALLING_EDGE		xxx
#define IGPIO_TOGGLE					xxx

#define IGPIO_PULL_NORMAL			xxx
#define IGPIO_PULL_UP 				xxx
#define IGPIO_PULL_DOWN 			xxx

typedef xxx			iGpio_pin_t;
typedef xxx 		iGpio_edge_t;
typedef xxx			iGpio_pull_t;
typedef xxx 		iGpio_dir_t;
typedef xxx			iGpio_device_t;
typedef xxx			iGpio_pin_name_t;
typedef xxx			Gpio_int_config_t;

typedef xxx 	iGpio_handler_t;
#define GPIO_HANDLER(fn, pin)   void fn(xxx)

typedef struct {
	iGpio_pin_t						iGpio_pin;
	iGpio_dir_t						iGpio_dir;
	iGpio_pin_name_t			iGpio_pin_name;
	iGpio_device_t 				iGpio_device;
	iGpio_int_config_t 		iGpio_int_config;
} iGpio_t;

void iGpio_interrupt_init(iGpio_t* gpio, iGpio_edge_t edge, iGpio_pull_t pull, iGpio_handler_t handler);
void iGpio_enable_interrupt(iGpio_t* gpio)
void iGpio_disable_interrupt(iGpio_t* gpio)

void iGpio_init(iGpio_t* gpio, iGpio_pull_t pull);
void iGpio_write(iGpio_t* gpio, uint32_t value)
void iGpio_read(iGpio_t* gpio, uint32_t value)

//-----------------------------------------------------------------//
// 															ISPI															   //
//-----------------------------------------------------------------//
#define ISPI_PIN_NOT_USED										xxx

#define ISPI_BIT_ORDER_MSB_FIRST						xxx
#define ISPI_BIT_ORDER_LSB_FIRST						xxx

#define ISPI_MODE_SCK_HIGH_CAPTURE_L_TO_H		xxx
#define ISPI_MODE_SCK_HIGH_CAPTURE_H_TO_L		xxx
#define ISPI_MODE_SCK_LOW_CAPTURE_L_TO_H		xxx
#define ISPI_MODE_SCK_LOW_CAPTURE_H_TO_L		xxx

#define ISPI_DEFAULT_IRQ_PRIORITY						xxx

typedef enum {
	SPI0 = 0,
	SPI1,
	SPI2
} iSpi_id_t;

typedef xxx 	iSpi_device_t;

typedef struct {
	iSpi_id_t					iSpi_id;
	iSpi_device_t 		iSpi_device;
} iSpi_t;

void iSpi_init(iSpi_t* spi);
void iSpi_transmit(iSpi_t* spi, uint8_t* tx_buf, size_t tx_buf_length, uint8_t* rx_buf, size_t rx_buf_length);
void iSPI_read(iSpi_t* spi, rx_buf, rx_buf_length)
void iSPI_write(iSpi_t* spi, tx_buf, tx_buf_length)

//------------------------------------------------------------------//
// 															II2C								 						   	//
//------------------------------------------------------------------//
#define II2C_FREQ_STANDARD				xxx
#define II2C_FREQ_FAST						xxx
#define II2C_FREQ_FAST_PLUS 			xxx
#define II2_FREQ_HIGH							xxx
#define II2_FREQ_ULTRA						xxx

#define II2_DEFAULT_IRQ_PRIORITY		xxx

#define MAX_PENDING_TRANSACTIONS    xxx

typedef enum {
	I2C0 = 0,
	I2C1
}	iI2c_id_t;

typedef xxx		iI2c_addr_reg_t;
typedef xxx		iI2c_addr_dev_t;
typedef xxx		iI2c_device_t;

typedef struct {
	iI2c_id_t 				iI2c_id;
	iI2c_device_t 		iI2c_device;
} iI2c_t;

void iI2c_init(iI2c_t* i2c);
void iI2c_read(iI2c_t* i2c, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* buf, size_t buf_length);
void iI2C_write(iI2c_t* i2c, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* buf, size_t buf_length);

//------------------------------------------------------------------//
// 											I	BlUETOOTH LOW ENERGY		 									//
//------------------------------------------------------------------//
// chrc = characteristic
// attr = attribute
// svc = service
#define IBLEP_DEVICE_NAME										xxx

#define IBLEP_FLAGS_LIMITED 									xxx
#define IBLEP_FLAGS_GENERAL									xxx
#define IBLEP_FLAGS_NO_BREDR									xxx

#define IBLEP_DATA_FLAGS                   	xxx
#define IBLEP_DATA_UUID16_SOME             	xxx
#define IBLEP_DATA_UUID16_ALL              	xxx
#define IBLEP_DATA_UUID32_SOME             	xxx
#define IBLEP_DATA_UUID32_ALL              	xxx
#define IBLEP_DATA_UUID128_SOME            	xxx
#define IBLEP_DATA_UUID128_ALL             	xxx
#define IBLEP_DATA_NAME_SHORTENED          	xxx
#define IBLEP_DATA_NAME_COMPLETE           	xxx
#define IBLEP_DATA_TX_POWER                	xxx
#define IBLEP_DATA_SOLICIT128              	xxx
#define IBLEP_DATA_SVC_DATA16              	xxx
#define IBLEP_DATA_SOLICIT16               	xxx
#define IBLEP_DATA_GAP_APPEARANCE          	xxx
#define IBLEP_DATA_SOLICIT32               	xxx
#define IBLEP_DATA_SVC_DATA32              	xxx
#define IBLEP_DATA_SVC_DATA128             	xxx
#define IBLEP_DATA_MANUFACTURER_DATA 				xxx

#define IBLEP_GAP_ADV_FAST_INT_MIN_1					xxx
#define IBLEP_GAP_ADV_FAST_INT_MAX_1					xxx
#define IBLEP_GAP_ADV_FAST_INT_MIN_2					xxx
#define IBLEP_GAP_ADV_FAST_INT_MAX_2					xxx
#define IBLEP_GAP_ADV_SLOW_INT_MIN						xxx
#define IBLEP_GAP_ADV_SLOW_INT_MAX						xxx

#define IBLEP_CHRC_PERM_BROADCAST 						xxx
#define IBLEP_CHRC_PERM_READ 								xxx
#define IBLEP_CHRC_PERM_WRITE_WITHOUT_RESP 	xxx
#define IBLEP_CHRC_PERM_WRITE 								xxx
#define IBLEP_CHRC_PERM_NOTIFY 							xxx
#define IBLEP_CHRC_PERM_INDICATE 						xxx
#define IBLEP_CHRC_PERM_AUTH 								xxx
#define IBLEP_CHRC_PERM_EXT_PROP 						xxx

#define IBLEP_GATT_PERM_NONE									xxx
#define IBLEP_GATT_PERM_READ									xxx
#define IBLEP_GATT_PERM_WRITE								xxx
#define IBLEP_GATT_PERM_READ_ENCRYPT					xxx
#define IBLEP_GATT_PERM_WRITE_ENCRYPT				xxx
#define	IBLEP_GATT_PERM_READ_AUTHEN					xxx
#define IBLEP_GATT_PERM_WRITE_AUTHEN					xxx
#define IBLEP_GATT_PERM_PREPARE_WRITE				xxx

#define IBLEP_ADV_NO_TIMEOUT									xxx

typedef enum {
	IBLEP_ADV_MODE_FAST,
	IBLEP_ADV_MODE_SLOW
} iBleP_adv_mode_t;

typedef xxx iBleP_adv_mode_t;
typedef xxx iBleP_uuid_t;
typedef xxx iBleP_adv_config_t;
typedef xxx iBleP_advdata_t;
typedef xxx iBleP_chrc_config_t;
typedef xxx iBleP_attr_config_t;
typedef xxx iBleP_chrc_t;
typedef xxx	iBleP_svc_config_t;
typedef xxx	iBleP_svc_t;
typedef xxx iBleP_chrc_handle_t;

#define IBLEP_ADV_ADD_DATA(_type, _data...)	 						xxx
#define IBLEP_ADV_ADD_TEXT(_type, _data...)							xxx
#define DEFINE_IBLEP_ADV_DATA(var)												xxx
#define DEFINE_IBLEP_UUID16(_uuid)												xxx
#define DEFINE_IBLEP_UUID128(_uuid, _base...)						xxx
#define DEFINE_IBLEP_SVC_CONFIG(_svc)										xxx
#define DEFINE_IBLEP_CHRCS(...)													xxx
#define DEFINE_IBLEP_CHRC(_chrc_cfg, _attr_cfg)					xxx
#define DEFINE_IBLEP_CHRC_NO_CCCD(_chrc_cfg, _attr_cfg)	xxx
#define IBLEP_SVC_UUID(_uuid) 														xxx
#define IBLEP_CHRC_CONFIG(_uuid, _perm)									xxx
#define IBLEP_ATTR_CONFIG(_uuid, _perm, _p_data)					xxx


int  iBleP_init();
void ble_sleep();
bool iBleP_isConnected();
int  iBleP_adv_start(iBleP_adv_config_t* adv_config, iBleP_advdata_t* advdata, size_t advdata_size, iBleP_advdata_t* scanrsp, size_t scanrsp_size);
int  iBleP_svc_init(iBleP_svc_t* svc, iBleP_svc_config_t* svc_config, size_t nbr_attrs, size_t nbr_chrcs);
int  iBleP_notify(iBleP_chrc_handle_t* chrc_handle, uint8_t* buf, size_t buf_length);
int  iBleP_indication(iBleP_chrc_handle_t* chrc_handle, uint8_t* buf, size_t buf_length);
iBleP_chrc_handle_t get_chrc_handle(iBleP_svc_t* svc, uint8_t chrc_nbr);

#endif	// __NRF5X_INTERFACE_H__
