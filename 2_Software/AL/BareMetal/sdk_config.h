#ifndef SDK_CONFIG_H
#define SDK_CONFIG_H

//==========================================================
// <h> GENERAL INFORMATION
//==========================================================

#define SPI1_SCK		       27
#define SPI1_MISO		       26
#define SPI1_MOSI		       2
#define SPI1_CS0			     11
#define SPI1_CS1			     12
#define SPI1_IRQ_PRIORITY  SPI_DEFAULT_CONFIG_IRQ_PRIORITY

#define I2C0_SCL           3
#define I2C0_SDA           4
#define I2C0_IRQ_PRIORITY TWI_DEFAULT_CONFIG_IRQ_PRIORITY

// <e> PIN MAPPING
//==========================================================
// <0=> 0 (P0.0)
// <1=> 1 (P0.1)
// <2=> 2 (P0.2)
// <3=> 3 (P0.3)
// <4=> 4 (P0.4)
// <5=> 5 (P0.5)
// <6=> 6 (P0.6)
// <7=> 7 (P0.7)
// <8=> 8 (P0.8)
// <9=> 9 (P0.9)
// <10=> 10 (P0.10)
// <11=> 11 (P0.11)
// <12=> 12 (P0.12)
// <13=> 13 (P0.13)
// <14=> 14 (P0.14)
// <15=> 15 (P0.15)
// <16=> 16 (P0.16)
// <17=> 17 (P0.17)
// <18=> 18 (P0.18)
// <19=> 19 (P0.19)
// <20=> 20 (P0.20)
// <21=> 21 (P0.21)
// <22=> 22 (P0.22)
// <23=> 23 (P0.23)
// <24=> 24 (P0.24)
// <25=> 25 (P0.25)
// <26=> 26 (P0.26)
// <27=> 27 (P0.27)
// <28=> 28 (P0.28)
// <29=> 29 (P0.29)
// <30=> 30 (P0.30)
// <31=> 31 (P0.31)
// <32=> 32 (P1.0)
// <33=> 33 (P1.1)
// <34=> 34 (P1.2)
// <35=> 35 (P1.3)
// <36=> 36 (P1.4)
// <37=> 37 (P1.5)
// <38=> 38 (P1.6)
// <39=> 39 (P1.7)
// <40=> 40 (P1.8)
// <41=> 41 (P1.9)
// <42=> 42 (P1.10)
// <43=> 43 (P1.11)
// <44=> 44 (P1.12)
// <45=> 45 (P1.13)
// <46=> 46 (P1.14)
// <47=> 47 (P1.15)
// <4294967295=> Not connected

// <e> INTERRUPT PRIORITY
//==========================================================
// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest)
// <1=> 1
// <2=> 2
// <3=> 3
// <4=> 4
// <5=> 5
// <6=> 6
// <7=> 7

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//==========================================================
// <h> SPI_CONFIGURATION
//==========================================================

// <e> SPI_ENABLED - nrf_drv_spi - SPI/SPIM peripheral driver
//==========================================================
#ifndef SPI_ENABLED
	#define SPI_ENABLED 1
#endif

// <o> SPI_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
#if  SPI_ENABLED

	#ifndef SPI_DEFAULT_CONFIG_IRQ_PRIORITY
		#define SPI_DEFAULT_CONFIG_IRQ_PRIORITY 7
	#endif

	// <e> SPI0_ENABLED - Enable SPI0 instance
	//==========================================================
	#ifndef SPI0_ENABLED
		#define SPI0_ENABLED 0
	#endif

	#if  SPI0_ENABLED

		// <q> SPI0_USE_EASY_DMA  - Use EasyDMA
		#ifndef SPI0_USE_EASY_DMA
			#define SPI0_USE_EASY_DMA 1
		#endif

		// <o> SPI0_DEFAULT_FREQUENCY  - SPI frequency
		// <33554432=> 		125 kHz
		// <67108864=> 		250 kHz
		// <134217728=> 	500 kHz
		// <268435456=> 	1 MHz
		// <536870912=> 	2 MHz
		// <1073741824=> 	4 MHz
		// <2147483648=> 	8 MHz
		#ifndef SPI0_DEFAULT_FREQUENCY
			#define SPI0_DEFAULT_FREQUENCY 1073741824
		#endif

	#endif //SPI0_ENABLED
	// </e>

	// <e> SPI1_ENABLED - Enable SPI1 instance
	//==========================================================
	#ifndef SPI1_ENABLED
		#define SPI1_ENABLED 1
	#endif

	#if  SPI1_ENABLED

		// <q> SPI1_USE_EASY_DMA  - Use EasyDMA
		#ifndef SPI1_USE_EASY_DMA
			#define SPI1_USE_EASY_DMA 1
		#endif

		// <o> SPI1_DEFAULT_FREQUENCY  - SPI frequency
		// <33554432=> 		125 kHz
		// <67108864=>		250 kHz
		// <134217728=> 	500 kHz
		// <268435456=> 	1 MHz
		// <536870912=> 	2 MHz
		// <1073741824=> 	4 MHz
		// <2147483648=> 	8 MHz
		#ifndef SPI1_DEFAULT_FREQUENCY
			#define SPI1_DEFAULT_FREQUENCY 1073741824
		#endif

	#endif //SPI1_ENABLED
	// </e>

	// <e> SPI2_ENABLED - Enable SPI2 instance
	//==========================================================
	#ifndef SPI2_ENABLED
		#define SPI2_ENABLED 0
	#endif

	#if  SPI2_ENABLED

		// <q> SPI2_USE_EASY_DMA  - Use EasyDMA
		#ifndef SPI2_USE_EASY_DMA
			#define SPI2_USE_EASY_DMA 1
		#endif

		// <q> SPI2_DEFAULT_FREQUENCY  - Use EasyDMA
		#ifndef SPI2_DEFAULT_FREQUENCY
			#define SPI2_DEFAULT_FREQUENCY 1
		#endif

	#endif //SPI2_ENABLED
	// </e>

	// <e> SPI_CONFIG_LOG_ENABLED - Enables logging in the module.
	//==========================================================
	#ifndef SPI_CONFIG_LOG_ENABLED
		#define SPI_CONFIG_LOG_ENABLED 0
	#endif

	#if  SPI_CONFIG_LOG_ENABLED

		// <o> SPI_CONFIG_LOG_LEVEL  - Default Severity level
		// <0=> Off
		// <1=> Error
		// <2=> Warning
		// <3=> Info
		// <4=> Debug
		#ifndef SPI_CONFIG_LOG_LEVEL
			#define SPI_CONFIG_LOG_LEVEL 3
		#endif

		// <o> SPI_CONFIG_INFO_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef SPI_CONFIG_INFO_COLOR
			#define SPI_CONFIG_INFO_COLOR 0
		#endif

		// <o> SPI_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef SPI_CONFIG_DEBUG_COLOR
			#define SPI_CONFIG_DEBUG_COLOR 0
		#endif

	#endif //SPI_CONFIG_LOG_ENABLED
	// </e>

#endif //SPI_ENABLED
// </e>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//==========================================================
// <h> TWI_CONFIGURATION
//==========================================================

// <e> TWI_ENABLED - nrf_drv_twi - TWI/TWIM peripheral driver
//==========================================================
#ifndef TWI_ENABLED
	#define TWI_ENABLED 1
#endif

// <q> APP_TWI_ENABLED  - app_twi - TWI transaction manager
#ifndef APP_TWI_ENABLED
	#define APP_TWI_ENABLED 0
#endif

#if  TWI_ENABLED

	// <o> TWI_DEFAULT_CONFIG_FREQUENCY  - Frequency
	// <26738688=> 	100k
	// <67108864=> 	250k
	// <104857600=> 400k
	#ifndef TWI_DEFAULT_CONFIG_FREQUENCY
		#define TWI_DEFAULT_CONFIG_FREQUENCY 26738688
	#endif

	// <q> TWI_DEFAULT_CONFIG_CLR_BUS_INIT  - Enables bus clearing procedure during init
	#ifndef TWI_DEFAULT_CONFIG_CLR_BUS_INIT
		#define TWI_DEFAULT_CONFIG_CLR_BUS_INIT 0
	#endif

	// <q> TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT  - Enables bus holding after uninit
	#ifndef TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT
		#define TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT 0
	#endif

	// <o> TWI_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
	// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
	// <0=> 0 (highest)
	// <1=> 1
	// <2=> 2
	// <3=> 3
	// <4=> 4
	// <5=> 5
	// <6=> 6
	// <7=> 7
	#ifndef TWI_DEFAULT_CONFIG_IRQ_PRIORITY
		#define TWI_DEFAULT_CONFIG_IRQ_PRIORITY 7
	#endif

	// <e> TWI0_ENABLED - Enable TWI0 instance
	//==========================================================
	#ifndef TWI0_ENABLED
		#define TWI0_ENABLED 1
	#endif

	#if  TWI0_ENABLED

		// <q> TWI0_USE_EASY_DMA  - Use EasyDMA (if present)
		#ifndef TWI0_USE_EASY_DMA
			#define TWI0_USE_EASY_DMA 0
		#endif

	#endif //TWI0_ENABLED
	// </e>

	// <e> TWI1_ENABLED - Enable TWI1 instance
	//==========================================================
	#ifndef TWI1_ENABLED
		#define TWI1_ENABLED 0
	#endif

	#if  TWI1_ENABLED

		// <q> TWI1_USE_EASY_DMA  - Use EasyDMA (if present)
		#ifndef TWI1_USE_EASY_DMA
			#define TWI1_USE_EASY_DMA 0
		#endif

	#endif //TWI1_ENABLED
	// </e>

	// <e> TWI_CONFIG_LOG_ENABLED - Enables logging in the module.
	//==========================================================
	#ifndef TWI_CONFIG_LOG_ENABLED
		#define TWI_CONFIG_LOG_ENABLED 0
	#endif

	#if  TWI_CONFIG_LOG_ENABLED

		// <o> TWI_CONFIG_LOG_LEVEL  - Default Severity level
		// <0=> Off
		// <1=> Error
		// <2=> Warning
		// <3=> Info
		// <4=> Debug
		#ifndef TWI_CONFIG_LOG_LEVEL
			#define TWI_CONFIG_LOG_LEVEL 3
		#endif

		// <o> TWI_CONFIG_INFO_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef TWI_CONFIG_INFO_COLOR
			#define TWI_CONFIG_INFO_COLOR 0
		#endif

		// <o> TWI_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef TWI_CONFIG_DEBUG_COLOR
			#define TWI_CONFIG_DEBUG_COLOR 0
		#endif

	#endif //TWI_CONFIG_LOG_ENABLED
	// </e>

#endif //TWI_ENABLED
// </e>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//==========================================================
// <h> GPIOTE_CONFIGURATION
//==========================================================

// <e> GPIOTE_ENABLED - nrf_drv_gpiote - GPIOTE peripheral driver
//==========================================================
#ifndef GPIOTE_ENABLED
	#define GPIOTE_ENABLED 1
#endif

#if  GPIOTE_ENABLED

	// <o> GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS - Number of lower power input pins
	#ifndef GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS
		#define GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS 2
	#endif

	// <o> GPIOTE_CONFIG_IRQ_PRIORITY  - Interrupt priority
	// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
	// <0=> 0 (highest)
	// <1=> 1
	// <2=> 2
	// <3=> 3
	// <4=> 4
	// <5=> 5
	// <6=> 6
	// <7=> 7
	#ifndef GPIOTE_CONFIG_IRQ_PRIORITY
		#define GPIOTE_CONFIG_IRQ_PRIORITY 7
	#endif

	// <e> GPIOTE_CONFIG_LOG_ENABLED - Enables logging in the module.
	//==========================================================
	#ifndef GPIOTE_CONFIG_LOG_ENABLED
		#define GPIOTE_CONFIG_LOG_ENABLED 0
	#endif

	#if  GPIOTE_CONFIG_LOG_ENABLED
		// <o> GPIOTE_CONFIG_LOG_LEVEL  - Default Severity level
		// <0=> Off
		// <1=> Error
		// <2=> Warning
		// <3=> Info
		// <4=> Debug
		#ifndef GPIOTE_CONFIG_LOG_LEVEL
			#define GPIOTE_CONFIG_LOG_LEVEL 3
		#endif

		// <o> GPIOTE_CONFIG_INFO_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef GPIOTE_CONFIG_INFO_COLOR
			#define GPIOTE_CONFIG_INFO_COLOR 0
		#endif

		// <o> GPIOTE_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef GPIOTE_CONFIG_DEBUG_COLOR
			#define GPIOTE_CONFIG_DEBUG_COLOR 0
		#endif

	#endif //GPIOTE_CONFIG_LOG_ENABLED
	// </e>

#endif //GPIOTE_ENABLED
// </e>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//==========================================================
// <h> BLE_CONFIGURATION
//==========================================================

// <e> BLE_ADVERTISING_ENABLED  - ble_advertising - Advertising module
//==========================================================


#ifndef BLE_ADVERTISING_ENABLED
	#define BLE_ADVERTISING_ENABLED 1
#endif

// <q> BLE_DTM_ENABLED  - ble_dtm - Module for testing RF/PHY using DTM commands
#ifndef BLE_DTM_ENABLED
	#define BLE_DTM_ENABLED 0
#endif

// <q> BLE_RACP_ENABLED  - ble_racp - Record Access Control Point library
#ifndef BLE_RACP_ENABLED
	#define BLE_RACP_ENABLED 0
#endif

// <e> NRF_BLE_GATT_ENABLED - nrf_ble_gatt - GATT module
//==========================================================
#ifndef NRF_BLE_GATT_ENABLED
	#define NRF_BLE_GATT_ENABLED 1
#endif

#if  NRF_BLE_GATT_ENABLED

	// <o> NRF_BLE_GATT_MAX_MTU_SIZE - Static maximum MTU size that is passed to the @ref sd_ble_enable function.
	#ifndef NRF_BLE_GATT_MAX_MTU_SIZE
		#define NRF_BLE_GATT_MAX_MTU_SIZE 23
	#endif

#endif //NRF_BLE_GATT_ENABLED
// </e>

// <q> NRF_BLE_QWR_ENABLED  - nrf_ble_qwr - Queued writes support module (prepare/execute write)
#ifndef NRF_BLE_QWR_ENABLED
	#define NRF_BLE_QWR_ENABLED 0
#endif

// <q> PEER_MANAGER_ENABLED  - peer_manager - Peer Manager
#ifndef PEER_MANAGER_ENABLED
	#define PEER_MANAGER_ENABLED 1
#endif

// <e> nRF_BLE_Services
//==========================================================
// <q> BLE_ANCS_C_ENABLED  - ble_ancs_c - Apple Notification Service Client
#ifndef BLE_ANCS_C_ENABLED
	#define BLE_ANCS_C_ENABLED 0
#endif

// <q> BLE_ANS_C_ENABLED  - ble_ans_c - Alert Notification Service Client
#ifndef BLE_ANS_C_ENABLED
	#define BLE_ANS_C_ENABLED 0
#endif

// <q> BLE_BAS_C_ENABLED  - ble_bas_c - Battery Service Client
#ifndef BLE_BAS_C_ENABLED
	#define BLE_BAS_C_ENABLED 0
#endif

// <q> BLE_BAS_ENABLED  - ble_bas - Battery Service
#ifndef BLE_BAS_ENABLED
	#define BLE_BAS_ENABLED 0
#endif

// <q> BLE_CSCS_ENABLED  - ble_cscs - Cycling Speed and Cadence Service
#ifndef BLE_CSCS_ENABLED
	#define BLE_CSCS_ENABLED 0
#endif

// <q> BLE_CTS_C_ENABLED  - ble_cts_c - Current Time Service Client
#ifndef BLE_CTS_C_ENABLED
	#define BLE_CTS_C_ENABLED 0
#endif

// <q> BLE_DIS_ENABLED  - ble_dis - Device Information Service
#ifndef BLE_DIS_ENABLED
	#define BLE_DIS_ENABLED 0
#endif

// <q> BLE_GLS_ENABLED  - ble_gls - Glucose Service
#ifndef BLE_GLS_ENABLED
	#define BLE_GLS_ENABLED 0
#endif

// <q> BLE_HIDS_ENABLED  - ble_hids - Human Interface Device Service
#ifndef BLE_HIDS_ENABLED
	#define BLE_HIDS_ENABLED 0
#endif

// <e> BLE_HRS_C_ENABLED - ble_hrs_c - Heart Rate Service Client
//==========================================================
#ifndef BLE_HRS_C_ENABLED
	#define BLE_HRS_C_ENABLED 0
#endif

#if  BLE_HRS_C_ENABLED

	// <o> BLE_HRS_C_RR_INTERVALS_MAX_CNT - Maximum number of RR_INTERVALS per notification to be decoded
	#ifndef BLE_HRS_C_RR_INTERVALS_MAX_CNT
		#define BLE_HRS_C_RR_INTERVALS_MAX_CNT 30
	#endif

#endif //BLE_HRS_C_ENABLED
// </e>

// <q> BLE_HRS_ENABLED  - ble_hrs - Heart Rate Service
#ifndef BLE_HRS_ENABLED
	#define BLE_HRS_ENABLED 0
#endif

// <q> BLE_HTS_ENABLED  - ble_hts - Health Thermometer Service
#ifndef BLE_HTS_ENABLED
	#define BLE_HTS_ENABLED 0
#endif

// <q> BLE_IAS_C_ENABLED  - ble_ias_c - Immediate Alert Service Client
#ifndef BLE_IAS_C_ENABLED
	#define BLE_IAS_C_ENABLED 0
#endif

// <q> BLE_IAS_ENABLED  - ble_ias - Immediate Alert Service
#ifndef BLE_IAS_ENABLED
	#define BLE_IAS_ENABLED 0
#endif

// <q> BLE_LBS_C_ENABLED  - ble_lbs_c - Nordic LED Button Service Client
#ifndef BLE_LBS_C_ENABLED
	#define BLE_LBS_C_ENABLED 0
#endif

// <q> BLE_LBS_ENABLED  - ble_lbs - LED Button Service
#ifndef BLE_LBS_ENABLED
	#define BLE_LBS_ENABLED 0
#endif

// <q> BLE_LLS_ENABLED  - ble_lls - Link Loss Service
#ifndef BLE_LLS_ENABLED
	#define BLE_LLS_ENABLED 0
#endif

// <q> BLE_NUS_C_ENABLED  - ble_nus_c - Nordic UART Central Service
#ifndef BLE_NUS_C_ENABLED
	#define BLE_NUS_C_ENABLED 0
#endif

// <q> BLE_NUS_ENABLED  - ble_nus - Nordic UART Service
#ifndef BLE_NUS_ENABLED
	#define BLE_NUS_ENABLED 0
#endif

// <q> BLE_RSCS_C_ENABLED  - ble_rscs_c - Running Speed and Cadence Client
#ifndef BLE_RSCS_C_ENABLED
	#define BLE_RSCS_C_ENABLED 0
#endif

// <q> BLE_RSCS_ENABLED  - ble_rscs - Running Speed and Cadence Service
#ifndef BLE_RSCS_ENABLED
	#define BLE_RSCS_ENABLED 0
#endif

// <q> BLE_TPS_ENABLED  - ble_tps - TX Power Service
#ifndef BLE_TPS_ENABLED
	#define BLE_TPS_ENABLED 0
#endif

// <e> FDS_ENABLED - fds - Flash data storage module
//==========================================================
#ifndef FDS_ENABLED
	#define FDS_ENABLED 1
#endif

#if  FDS_ENABLED

	// <o> FDS_OP_QUEUE_SIZE - Size of the internal queue.
	#ifndef FDS_OP_QUEUE_SIZE
		#define FDS_OP_QUEUE_SIZE 4
	#endif

	// <o> FDS_CHUNK_QUEUE_SIZE - Determines how many @ref fds_record_chunk_t structures can be buffered at any time.
	#ifndef FDS_CHUNK_QUEUE_SIZE
		#define FDS_CHUNK_QUEUE_SIZE 8
	#endif

	// <o> FDS_MAX_USERS - Maximum number of callbacks that can be registered.
	#ifndef FDS_MAX_USERS
		#define FDS_MAX_USERS 8
	#endif

	// <o> FDS_VIRTUAL_PAGES - Number of virtual flash pages to use.
	// <i> One of the virtual pages is reserved by the system for garbage collection.
	// <i> Therefore, the minimum is two virtual pages: one page to store data and
	// <i> one page to be used by the system for garbage collection. The total amount
	// <i> of flash memory that is used by FDS amounts to @ref FDS_VIRTUAL_PAGES
	// <i> @ref FDS_VIRTUAL_PAGE_SIZE * 4 bytes.
	#ifndef FDS_VIRTUAL_PAGES
		#define FDS_VIRTUAL_PAGES 3
	#endif

	// <o> FDS_VIRTUAL_PAGE_SIZE  - The size of a virtual page of flash memory, expressed in number of 4-byte words.
	// <i> By default, a virtual page is the same size as a physical page.
	// <i> The size of a virtual page must be a multiple of the size of a physical page.
	// <1024=> 1024
	// <2048=> 2048
	#ifndef FDS_VIRTUAL_PAGE_SIZE
		#define FDS_VIRTUAL_PAGE_SIZE 1024
	#endif

#endif //FDS_ENABLED
// </e>

// <e> FSTORAGE_ENABLED - fstorage - Flash storage module
//==========================================================
#ifndef FSTORAGE_ENABLED
	#define FSTORAGE_ENABLED 1
#endif

#if  FSTORAGE_ENABLED
	// <o> FS_QUEUE_SIZE - Configures the size of the internal queue.
	// <i> Increase this if there are many users, or if it is likely that many
	// <i> operation will be queued at once without waiting for the previous operations
	// <i> to complete. In general, increase the queue size if you frequently receive
	// <i> @ref FS_ERR_QUEUE_FULL errors when calling @ref fs_store or @ref fs_erase.
	#ifndef FS_QUEUE_SIZE
		#define FS_QUEUE_SIZE 4
	#endif

	// <o> FS_OP_MAX_RETRIES - Number attempts to execute an operation if the SoftDevice fails.
	// <i> Increase this value if events return the @ref FS_ERR_OPERATION_TIMEOUT
	// <i> error often. The SoftDevice may fail to schedule flash access due to high BLE activity.
	#ifndef FS_OP_MAX_RETRIES
		#define FS_OP_MAX_RETRIES 3
	#endif

	// <o> FS_MAX_WRITE_SIZE_WORDS - Maximum number of words to be written to flash in a single operation.
	// <i> Tweaking this value can increase the chances of the SoftDevice being
	// <i> able to fit flash operations in between radio activity. This value is bound by the
	// <i> maximum number of words which the SoftDevice can write to flash in a single call to
	// <i> @ref sd_flash_write, which is 256 words for nRF51 ICs and 1024 words for nRF52 ICs.
	#ifndef FS_MAX_WRITE_SIZE_WORDS
		#define FS_MAX_WRITE_SIZE_WORDS 1024
	#endif

#endif //FSTORAGE_ENABLED
// </e>

// <q> HARDFAULT_HANDLER_ENABLED  - hardfault_default - HardFault default handler for debugging and release
#ifndef HARDFAULT_HANDLER_ENABLED
	#define HARDFAULT_HANDLER_ENABLED 0
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//==========================================================
// <h> CLOCK_CONFIGURATION
//==========================================================

// <e> CLOCK_ENABLED - nrf_drv_clock - CLOCK peripheral driver
//==========================================================
#ifndef CLOCK_ENABLED
	#define CLOCK_ENABLED 1
#endif

#if  CLOCK_ENABLED

	// <o> CLOCK_CONFIG_XTAL_FREQ  - HF XTAL Frequency
	// <0=> Default (64 MHz)
	#ifndef CLOCK_CONFIG_XTAL_FREQ
		#define CLOCK_CONFIG_XTAL_FREQ 0
	#endif

	// <o> CLOCK_CONFIG_LF_SRC  - LF Clock Source
	// <0=> RC
	// <1=> XTAL
	// <2=> Synth
	#ifndef CLOCK_CONFIG_LF_SRC
		#define CLOCK_CONFIG_LF_SRC 1
	#endif

	// <o> CLOCK_CONFIG_IRQ_PRIORITY  - Interrupt priority
	// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
	// <0=> 0 (highest)
	// <1=> 1
	// <2=> 2
	// <3=> 3
	// <4=> 4
	// <5=> 5
	// <6=> 6
	// <7=> 7
	#ifndef CLOCK_CONFIG_IRQ_PRIORITY
		#define CLOCK_CONFIG_IRQ_PRIORITY 7
	#endif

	// <e> CLOCK_CONFIG_LOG_ENABLED - Enables logging in the module.
	//==========================================================
	#ifndef CLOCK_CONFIG_LOG_ENABLED
		#define CLOCK_CONFIG_LOG_ENABLED 0
	#endif

	#if  CLOCK_CONFIG_LOG_ENABLED

		// <o> CLOCK_CONFIG_LOG_LEVEL  - Default Severity level
		// <0=> Off
		// <1=> Error
		// <2=> Warning
		// <3=> Info
		// <4=> Debug
		#ifndef CLOCK_CONFIG_LOG_LEVEL
			#define CLOCK_CONFIG_LOG_LEVEL 3
		#endif

		// <o> CLOCK_CONFIG_INFO_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef CLOCK_CONFIG_INFO_COLOR
			#define CLOCK_CONFIG_INFO_COLOR 0
		#endif

		// <o> CLOCK_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef CLOCK_CONFIG_DEBUG_COLOR
			#define CLOCK_CONFIG_DEBUG_COLOR 0
		#endif

	#endif //CLOCK_CONFIG_LOG_ENABLED
	// </e>

#endif //CLOCK_ENABLED
// </e>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//==========================================================
// <h> TIMER CONFIGURATION
//==========================================================

// <e> APP_TIMER_ENABLED - app_timer - Application timer functionality
//==========================================================
#ifndef APP_TIMER_ENABLED
	#define APP_TIMER_ENABLED 1
#endif

#if  APP_TIMER_ENABLED

	// <o> APP_TIMER_CONFIG_RTC_FREQUENCY  - Configure RTC prescaler.
	// <0=> 32768 Hz
	// <1=> 16384 Hz
	// <3=> 8192 Hz
	// <7=> 4096 Hz
	// <15=> 2048 Hz
	// <31=> 1024 Hz
	#ifndef APP_TIMER_CONFIG_RTC_FREQUENCY
		#define APP_TIMER_CONFIG_RTC_FREQUENCY 0
	#endif

	// <o> APP_TIMER_CONFIG_IRQ_PRIORITY  - Interrupt priority
	// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
	// <0=> 0 (highest)
	// <1=> 1
	// <2=> 2
	// <3=> 3
	// <4=> 4
	// <5=> 5
	// <6=> 6
	// <7=> 7
	#ifndef APP_TIMER_CONFIG_IRQ_PRIORITY
		#define APP_TIMER_CONFIG_IRQ_PRIORITY 7
	#endif

	// <o> APP_TIMER_CONFIG_OP_QUEUE_SIZE - Capacity of timer requests queue.
	// <i> Size of the queue depends on how many timers are used
	// <i> in the system, how often timers are started and overall
	// <i> system latency. If queue size is too small app_timer calls
	// <i> will fail.
	#ifndef APP_TIMER_CONFIG_OP_QUEUE_SIZE
		#define APP_TIMER_CONFIG_OP_QUEUE_SIZE 100
	#endif

	// <q> APP_TIMER_CONFIG_USE_SCHEDULER  - Enable scheduling app_timer events to app_scheduler
	#ifndef APP_TIMER_CONFIG_USE_SCHEDULER
		#define APP_TIMER_CONFIG_USE_SCHEDULER 0
	#endif

	// <q> APP_TIMER_WITH_PROFILER  - Enable app_timer profiling
	#ifndef APP_TIMER_WITH_PROFILER
		#define APP_TIMER_WITH_PROFILER 0
	#endif

	// <q> APP_TIMER_KEEPS_RTC_ACTIVE  - Enable RTC always on
	// <i> If option is enabled RTC is kept running even if there is no active timers.
	// <i> This option can be used when app_timer is used for timestamping.
	#ifndef APP_TIMER_KEEPS_RTC_ACTIVE
		#define APP_TIMER_KEEPS_RTC_ACTIVE 0
	#endif

	// <o> APP_TIMER_CONFIG_SWI_NUMBER  - Configure SWI instance used.
	// <0=> 0
	// <1=> 1
	#ifndef APP_TIMER_CONFIG_SWI_NUMBER
		#define APP_TIMER_CONFIG_SWI_NUMBER 0
	#endif

#endif //APP_TIMER_ENABLED
// </e>

// <e> RTC_ENABLED - nrf_drv_rtc - RTC peripheral driver
//==========================================================
#ifndef RTC_ENABLED
	#define RTC_ENABLED 0
#endif

#if  RTC_ENABLED

	// <o> RTC_DEFAULT_CONFIG_FREQUENCY - Frequency  <16-32768>
	#ifndef RTC_DEFAULT_CONFIG_FREQUENCY
		#define RTC_DEFAULT_CONFIG_FREQUENCY 32768
	#endif

	// <q> RTC_DEFAULT_CONFIG_RELIABLE  - Ensures safe compare event triggering
	#ifndef RTC_DEFAULT_CONFIG_RELIABLE
		#define RTC_DEFAULT_CONFIG_RELIABLE 0
	#endif

	// <o> RTC_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
	// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
	// <0=> 0 (highest)
	// <1=> 1
	// <2=> 2
	// <3=> 3
	// <4=> 4
	// <5=> 5
	// <6=> 6
	// <7=> 7
	#ifndef RTC_DEFAULT_CONFIG_IRQ_PRIORITY
		#define RTC_DEFAULT_CONFIG_IRQ_PRIORITY 7
	#endif

	// <q> RTC0_ENABLED  - Enable RTC0 instance
	#ifndef RTC0_ENABLED
		#define RTC0_ENABLED 0
	#endif

	// <q> RTC1_ENABLED  - Enable RTC1 instance
	#ifndef RTC1_ENABLED
		#define RTC1_ENABLED 0
	#endif

	// <q> RTC2_ENABLED  - Enable RTC2 instance
	#ifndef RTC2_ENABLED
		#define RTC2_ENABLED 0
	#endif

	// <o> NRF_MAXIMUM_LATENCY_US - Maximum possible time[us] in highest priority interrupt
	#ifndef NRF_MAXIMUM_LATENCY_US
		#define NRF_MAXIMUM_LATENCY_US 2000
	#endif

	// <e> RTC_CONFIG_LOG_ENABLED - Enables logging in the module.
	//==========================================================
	#ifndef RTC_CONFIG_LOG_ENABLED
		#define RTC_CONFIG_LOG_ENABLED 0
	#endif
	#if  RTC_CONFIG_LOG_ENABLED

		// <o> RTC_CONFIG_LOG_LEVEL  - Default Severity level
		// <0=> Off
		// <1=> Error
		// <2=> Warning
		// <3=> Info
		// <4=> Debug
		#ifndef RTC_CONFIG_LOG_LEVEL
			#define RTC_CONFIG_LOG_LEVEL 3
		#endif

		// <o> RTC_CONFIG_INFO_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef RTC_CONFIG_INFO_COLOR
			#define RTC_CONFIG_INFO_COLOR 0
		#endif

		// <o> RTC_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.
		// <0=> Default
		// <1=> Black
		// <2=> Red
		// <3=> Green
		// <4=> Yellow
		// <5=> Blue
		// <6=> Magenta
		// <7=> Cyan
		// <8=> White
		#ifndef RTC_CONFIG_DEBUG_COLOR
			#define RTC_CONFIG_DEBUG_COLOR 0
		#endif

	#endif //RTC_CONFIG_LOG_ENABLED
	// </e>

#endif //RTC_ENABLED
// </e>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//==========================================================
// <e> APP_SCHEDULER_ENABLED - app_scheduler - Events scheduler
//==========================================================
#ifndef APP_SCHEDULER_ENABLED
	#define APP_SCHEDULER_ENABLED 1
#endif

#if  APP_SCHEDULER_ENABLED

	// <q> APP_SCHEDULER_WITH_PAUSE  - Enabling pause feature
	#ifndef APP_SCHEDULER_WITH_PAUSE
		#define APP_SCHEDULER_WITH_PAUSE 0
	#endif

	// <q> APP_SCHEDULER_WITH_PROFILER  - Enabling scheduler profiling
	#ifndef APP_SCHEDULER_WITH_PROFILER
		#define APP_SCHEDULER_WITH_PROFILER 0
	#endif

#endif //APP_SCHEDULER_ENABLED
// </e>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// <q> BUTTON_ENABLED  - app_button - buttons handling module


#ifndef BUTTON_ENABLED
#define BUTTON_ENABLED 1
#endif

// <q> NRF_STRERROR_ENABLED  - nrf_strerror - Library for converting error code to string.


#ifndef NRF_STRERROR_ENABLED
#define NRF_STRERROR_ENABLED 1
#endif

// </h>
//==========================================================

// <h> nRF_Log

//==========================================================
// <e> NRF_LOG_ENABLED - nrf_log - Logging
//==========================================================
#ifndef NRF_LOG_ENABLED
#define NRF_LOG_ENABLED 0
#endif
#if  NRF_LOG_ENABLED
// <e> NRF_LOG_USES_COLORS - If enabled then ANSI escape code for colors is prefixed to every string
//==========================================================
#ifndef NRF_LOG_USES_COLORS
#define NRF_LOG_USES_COLORS 0
#endif
#if  NRF_LOG_USES_COLORS
// <o> NRF_LOG_COLOR_DEFAULT  - ANSI escape code prefix.

// <0=> Default
// <1=> Black
// <2=> Red
// <3=> Green
// <4=> Yellow
// <5=> Blue
// <6=> Magenta
// <7=> Cyan
// <8=> White

#ifndef NRF_LOG_COLOR_DEFAULT
#define NRF_LOG_COLOR_DEFAULT 0
#endif

// <o> NRF_LOG_ERROR_COLOR  - ANSI escape code prefix.

// <0=> Default
// <1=> Black
// <2=> Red
// <3=> Green
// <4=> Yellow
// <5=> Blue
// <6=> Magenta
// <7=> Cyan
// <8=> White

#ifndef NRF_LOG_ERROR_COLOR
#define NRF_LOG_ERROR_COLOR 0
#endif

// <o> NRF_LOG_WARNING_COLOR  - ANSI escape code prefix.

// <0=> Default
// <1=> Black
// <2=> Red
// <3=> Green
// <4=> Yellow
// <5=> Blue
// <6=> Magenta
// <7=> Cyan
// <8=> White

#ifndef NRF_LOG_WARNING_COLOR
#define NRF_LOG_WARNING_COLOR 0
#endif

#endif //NRF_LOG_USES_COLORS
// </e>

// <o> NRF_LOG_DEFAULT_LEVEL  - Default Severity level

// <0=> Off
// <1=> Error
// <2=> Warning
// <3=> Info
// <4=> Debug

#ifndef NRF_LOG_DEFAULT_LEVEL
#define NRF_LOG_DEFAULT_LEVEL 3
#endif

// <e> NRF_LOG_DEFERRED - Enable deffered logger.

// <i> Log data is buffered and can be processed in idle.
//==========================================================
#ifndef NRF_LOG_DEFERRED
#define NRF_LOG_DEFERRED 1
#endif
#if  NRF_LOG_DEFERRED
// <o> NRF_LOG_DEFERRED_BUFSIZE - Size of the buffer for logs in words.
// <i> Must be power of 2

#ifndef NRF_LOG_DEFERRED_BUFSIZE
#define NRF_LOG_DEFERRED_BUFSIZE 256
#endif

#endif //NRF_LOG_DEFERRED
// </e>

// <q> NRF_LOG_USES_TIMESTAMP  - Enable timestamping


// <i> Function for getting the timestamp is provided by the user

#ifndef NRF_LOG_USES_TIMESTAMP
#define NRF_LOG_USES_TIMESTAMP 0
#endif

#endif //NRF_LOG_ENABLED
// </e>

// <h> nrf_log_backend - Logging sink

//==========================================================
// <o> NRF_LOG_BACKEND_MAX_STRING_LENGTH - Buffer for storing single output string
// <i> Logger backend RAM usage is determined by this value.

#ifndef NRF_LOG_BACKEND_MAX_STRING_LENGTH
#define NRF_LOG_BACKEND_MAX_STRING_LENGTH 256
#endif

// <o> NRF_LOG_TIMESTAMP_DIGITS - Number of digits for timestamp
// <i> If higher resolution timestamp source is used it might be needed to increase that

#ifndef NRF_LOG_TIMESTAMP_DIGITS
#define NRF_LOG_TIMESTAMP_DIGITS 8
#endif

// <e> NRF_LOG_BACKEND_SERIAL_USES_UART - If enabled data is printed over UART
//==========================================================
#ifndef NRF_LOG_BACKEND_SERIAL_USES_UART
#define NRF_LOG_BACKEND_SERIAL_USES_UART 1
#endif
#if  NRF_LOG_BACKEND_SERIAL_USES_UART
// <o> NRF_LOG_BACKEND_SERIAL_UART_BAUDRATE  - Default Baudrate

// <323584=> 1200 baud
// <643072=> 2400 baud
// <1290240=> 4800 baud
// <2576384=> 9600 baud
// <3862528=> 14400 baud
// <5152768=> 19200 baud
// <7716864=> 28800 baud
// <10289152=> 38400 baud
// <15400960=> 57600 baud
// <20615168=> 76800 baud
// <30801920=> 115200 baud
// <61865984=> 230400 baud
// <67108864=> 250000 baud
// <121634816=> 460800 baud
// <251658240=> 921600 baud
// <268435456=> 57600 baud

#ifndef NRF_LOG_BACKEND_SERIAL_UART_BAUDRATE
#define NRF_LOG_BACKEND_SERIAL_UART_BAUDRATE 30801920
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_TX_PIN - UART TX pin
#ifndef NRF_LOG_BACKEND_SERIAL_UART_TX_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_TX_PIN 6
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_RX_PIN - UART RX pin
#ifndef NRF_LOG_BACKEND_SERIAL_UART_RX_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_RX_PIN 8
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_RTS_PIN - UART RTS pin
#ifndef NRF_LOG_BACKEND_SERIAL_UART_RTS_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_RTS_PIN 5
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_CTS_PIN - UART CTS pin
#ifndef NRF_LOG_BACKEND_SERIAL_UART_CTS_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_CTS_PIN 7
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_FLOW_CONTROL  - Hardware Flow Control

// <0=> Disabled
// <1=> Enabled

#ifndef NRF_LOG_BACKEND_SERIAL_UART_FLOW_CONTROL
#define NRF_LOG_BACKEND_SERIAL_UART_FLOW_CONTROL 0
#endif

// <o> NRF_LOG_BACKEND_UART_INSTANCE  - UART instance used

// <0=> 0

#ifndef NRF_LOG_BACKEND_UART_INSTANCE
#define NRF_LOG_BACKEND_UART_INSTANCE 0
#endif

#endif //NRF_LOG_BACKEND_SERIAL_USES_UART
// </e>

// <e> NRF_LOG_BACKEND_SERIAL_USES_RTT - If enabled data is printed using RTT
//==========================================================
#ifndef NRF_LOG_BACKEND_SERIAL_USES_RTT
#define NRF_LOG_BACKEND_SERIAL_USES_RTT 1
#endif
#if  NRF_LOG_BACKEND_SERIAL_USES_RTT
// <o> NRF_LOG_BACKEND_RTT_OUTPUT_BUFFER_SIZE - RTT output buffer size.
// <i> Should be equal or bigger than \ref NRF_LOG_BACKEND_MAX_STRING_LENGTH.
// <i> This value is used in Segger RTT configuration to set the buffer size
// <i> if it is bigger than default RTT buffer size.

#ifndef NRF_LOG_BACKEND_RTT_OUTPUT_BUFFER_SIZE
#define NRF_LOG_BACKEND_RTT_OUTPUT_BUFFER_SIZE 512
#endif

#endif //NRF_LOG_BACKEND_SERIAL_USES_RTT
// </e>

// </h>
//==========================================================

// </h>
//==========================================================

// <h> nRF_Segger_RTT

//==========================================================
// <h> segger_rtt - SEGGER RTT

//==========================================================
// <o> SEGGER_RTT_CONFIG_BUFFER_SIZE_UP - Size of upstream buffer.
// <i> Note that either @ref NRF_LOG_BACKEND_RTT_OUTPUT_BUFFER_SIZE
// <i> or this value is actually used. It depends on which one is bigger.

#ifndef SEGGER_RTT_CONFIG_BUFFER_SIZE_UP
#define SEGGER_RTT_CONFIG_BUFFER_SIZE_UP 64
#endif

// <o> SEGGER_RTT_CONFIG_MAX_NUM_UP_BUFFERS - Size of upstream buffer.
#ifndef SEGGER_RTT_CONFIG_MAX_NUM_UP_BUFFERS
#define SEGGER_RTT_CONFIG_MAX_NUM_UP_BUFFERS 2
#endif

// <o> SEGGER_RTT_CONFIG_BUFFER_SIZE_DOWN - Size of upstream buffer.
#ifndef SEGGER_RTT_CONFIG_BUFFER_SIZE_DOWN
#define SEGGER_RTT_CONFIG_BUFFER_SIZE_DOWN 16
#endif

// <o> SEGGER_RTT_CONFIG_MAX_NUM_DOWN_BUFFERS - Size of upstream buffer.
#ifndef SEGGER_RTT_CONFIG_MAX_NUM_DOWN_BUFFERS
#define SEGGER_RTT_CONFIG_MAX_NUM_DOWN_BUFFERS 2
#endif

// <o> SEGGER_RTT_CONFIG_DEFAULT_MODE  - RTT behavior if the buffer is full.


// <i> The following modes are supported:
// <i> - SKIP  - Do not block, output nothing.
// <i> - TRIM  - Do not block, output as much as fits.
// <i> - BLOCK - Wait until there is space in the buffer.
// <0=> SKIP
// <1=> TRIM
// <2=> BLOCK_IF_FIFO_FULL

#ifndef SEGGER_RTT_CONFIG_DEFAULT_MODE
#define SEGGER_RTT_CONFIG_DEFAULT_MODE 0
#endif

// </h>
//==========================================================

// </h>
//==========================================================

// <<< end of configuration section >>>
#endif //SDK_CONFIG_H
