#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

// Serial Communication---------------------------------------------------------
iSpi_id_t spi = SPI1;
iI2c_id_t i2c = I2C0;

// BLE--------------------------------------------------------------------------
#define ACC_UUID_SVC     0x0ACC
#define ACC_UUID_CHRC1   0x1ACC
#define ACC_UUID_CHRC2   0x2ACC
#define ACC_UUID_CHRC3   0x3ACC
#define ACC_UUID_CHRC4   0x4ACC
#define ACC_UUID_CHRC5   0x5ACC
#define ACC_UUID_BASE    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00

iBle_svc_t acc_svc;
size_t acc_nbr_chrcs = 5;
sample_t sample;
DEFINE_IBLE_SVC_CONFIG(acc_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID128(ACC_UUID_SVC, ACC_UUID_BASE)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC (  // AXIS X
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC1, ACC_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC1, ACC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &sample.x)
    ),
    DEFINE_IBLE_CHRC (  // AXIS Y
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC2, ACC_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC2, ACC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &sample.y)
    ),
    DEFINE_IBLE_CHRC (  // AXIS Z
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC3, ACC_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC3, ACC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &sample.z)
    ),
    DEFINE_IBLE_CHRC (  // CLICK
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC4, ACC_UUID_BASE), IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC4, ACC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, NULL)
    ),
    DEFINE_IBLE_CHRC (  // SLEEP
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC5, ACC_UUID_BASE), IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC5, ACC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, NULL)
    ),
  )
};

#define ADC_UUID_SVC     0x0ADC
#define ADC_UUID_CHRC1   0x1ADC
#define ADC_UUID_CHRC2   0x2ADC
#define ADC_UUID_BASE    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xE0, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00

iBle_svc_t adc_svc;
size_t adc_nbr_chrcs = 2;
uint32_t adc_measurement;
DEFINE_IBLE_SVC_CONFIG(adc_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID128(ADC_UUID_SVC, ADC_UUID_BASE)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC (  // A/D Converter measurements
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ADC_UUID_CHRC1, ADC_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ADC_UUID_CHRC1, ADC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &adc_measurement)
    ),
    DEFINE_IBLE_CHRC (  // SLEEP
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ADC_UUID_CHRC2, ADC_UUID_BASE), IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ADC_UUID_CHRC2, ADC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, NULL)
    ),
  )
};

static DEFINE_IBLE_ADV_DATA(advdata) =
{
  IBLE_ADV_ADD_DATA(IBLE_DATA_FLAGS, IBLE_FLAGS_GENERAL | IBLE_FLAGS_NO_BREDR),
  IBLE_ADV_ADD_DATA(IBLE_DATA_UUID16_ALL, 0xCC, 0x0A, 0xDC, 0x0A),
};

static DEFINE_IBLE_ADV_DATA(scanrsp) =
{
  IBLE_ADV_ADD_TEXT(IBLE_DATA_NAME_COMPLETE, IBLE_DEVICE_NAME),
};

// Interrupts-------------------------------------------------------------------
iGpio_t ext_irq;
IGPIO_HANDLER(on_ext_irq, pin)
{
  // TODO
  iPrint("External Interrupt\n");
}

iTimer_t soft_timer;
ITIMER_HANDLER(on_soft_timer)
{
  // TODO
  iPrint("Software Interrupt\n");
}

// Threads----------------------------------------------------------------------
extern iEventQueue_t acc_EventQueue;
extern iEventQueue_t adc_EventQueue;

DEFINE_ITHREAD(acc_thread, 4096, 1);
DEFINE_ITHREAD(adc_thread, 4096, 1);

ITHREAD_HANDLER(acc)
{
  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&acc_EventQueue)) { iThread_sleep(); }

    iEvent_t accEvent = iEventQueue_get(&acc_EventQueue);
    if(accEvent == ACC_EVENT_INT1)  // FIFO full
    {
      acc_getXYZ(&sample, 1);
      // iPrint("XYZ: 0x%04x, 0x%04x, 0x%04x\n", sample.x, sample.y, sample.z);
      iBle_svc_notify(&acc_svc, 1, (uint8_t*) &sample.x, sizeof(sample.x));
      iBle_svc_notify(&acc_svc, 2, (uint8_t*) &sample.y, sizeof(sample.y));
      iBle_svc_notify(&acc_svc, 3, (uint8_t*) &sample.z, sizeof(sample.z));
    }
    else if(accEvent == ACC_EVENT_INT2) // Click
    {
      bool click = true;
      iBle_svc_notify(&acc_svc, 4, (uint8_t*) &click, sizeof(click));
    }
    else if(accEvent == ACC_EVENT_SLEEP) // Sleep
    {
      bool isSleeping = acc_isSleeping();
      iBle_svc_notify(&acc_svc, 5, (uint8_t*) &isSleeping, sizeof(isSleeping));
    }
    else if(accEvent == ACC_EVENT_WAKEUP) // Wake-Up
    {
      bool isSleeping = acc_isSleeping();
      iBle_svc_notify(&acc_svc, 5, (uint8_t*) &isSleeping, sizeof(isSleeping));
    }
  }
}

ITHREAD_HANDLER(adc)
{
  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&adc_EventQueue)) { iThread_sleep(); }

    iEvent_t adcEvent = iEventQueue_get(&adc_EventQueue);
    if(adcEvent == ADC_EVENT_DATA)  // Data Ready
    {
      // Notify the central
      adc_getMeasurement(&adc_measurement);
      // iPrint("Measurement: %lu[uV]\n", adc_measurement);
      iBle_svc_notify(&adc_svc, 1, (uint8_t*) &adc_measurement, sizeof(adc_measurement));
    }
    else if(adcEvent == ADC_EVENT_SLEEP) // Sleep
    {
      bool isSleeping = adc_isSleeping();
      iBle_svc_notify(&acc_svc, 2, (uint8_t*) &isSleeping, sizeof(isSleeping));
    }
    else if(adcEvent == ADC_EVENT_WAKEUP) // Wake-Up
    {
      bool isSleeping = adc_isSleeping();
      iBle_svc_notify(&acc_svc, 2, (uint8_t*) &isSleeping, sizeof(isSleeping));
    }
  }
}

// -----------------------------------------------------------------------------
void bluetooth_init();
void extBoad_init();

int main()
{
  // Thread must be declare before the driver to avoid full filling the event queues
  iThread_run(&acc_thread, acc);
  iThread_run(&adc_thread, adc);

  bluetooth_init();
  extBoad_init();

  iGpio_interrupt_init(&ext_irq, INTERRUPT_PIN, IGPIO_RISING_EDGE, IGPIO_PULL_NORMAL, on_ext_irq);
  iGpio_enable_interrupt(&ext_irq);

  iTimer_start(&soft_timer, on_soft_timer, SOFT_INT_FREQ);

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

void bluetooth_init()
{
  iPrint("\nInitialize Bluetooth\n");
  iPrint("--------------------\n");

  iBle_init();
  iBle_svc_init(&acc_svc, acc_config, acc_nbr_chrcs);
  iBle_svc_init(&adc_svc, adc_config, adc_nbr_chrcs);
  iBle_adv_start(advdata, sizeof(advdata)/sizeof(iBle_advdata_t), scanrsp, sizeof(scanrsp)/sizeof(iBle_advdata_t));
}

void extBoad_init()
{
  iPrint("\nInitialize extBoard\n");
  iPrint("-------------------\n");

  iSpi_init(spi, SWG_SPI_FREQUENCY, SWG_SPI_MODE, SWG_SPI_BIT_ORDER);
  swg_init(EXT_INT_FREQ);

  iSpi_init(spi, ADC_SPI_FREQUENCY, ADC_SPI_MODE, ADC_SPI_BIT_ORDER);
  adc_init();

  iI2c_init(i2c, ACC_I2C_FREQEUNCY);
  acc_init();
}
