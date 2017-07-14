#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

void bluetooth_init();
void extBoad_init();

void bluetooth_test();
void acc_test();
void adc_test();
void swg_test();
void event_test();

// The attribute value must be static
static uint32_t current_time;

static DEFINE_IBLE_ADV_DATA(advdata) =
{
  IBLE_ADV_ADD_DATA(IBLE_DATA_FLAGS, IBLE_FLAGS_GENERAL | IBLE_FLAGS_NO_BREDR),

  // Heart Rate(0x180D),  Current Time Service(0x1805), my Service(0xAAAA)
  IBLE_ADV_ADD_DATA(IBLE_DATA_UUID16_ALL, 0x0d, 0x18, 0x05, 0x18, 0xAA, 0xAA),
};

static DEFINE_IBLE_ADV_DATA(scanrsp) =
{
  IBLE_ADV_ADD_TEXT(IBLE_DATA_NAME_COMPLETE, IBLE_DEVICE_NAME),
};

iBle_svc_t cts_svc;
size_t cts_nbr_attrs = 4;
size_t cts_nbr_chrcs = 1;
DEFINE_IBLE_SVC_CONFIG(cts_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID16(BT_UUID_CTS)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC (
    	IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY | IBLE_CHRC_PERM_WRITE),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &current_time)
    ),
  )
};

iBle_svc_t hrs_svc;
size_t hrs_nbr_attrs = 8;
size_t hrs_nbr_chrcs = 3;
DEFINE_IBLE_SVC_CONFIG(hrs_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID16(BT_UUID_HRS)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC (
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_INDICATE | IBLE_CHRC_PERM_WRITE),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &current_time)
    ),
    DEFINE_IBLE_CHRC_NO_CCCD (
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_BODY_SENSOR), IBLE_CHRC_PERM_READ),
    	IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_BODY_SENSOR), IBLE_GATT_PERM_READ, NULL)
    ),
    DEFINE_IBLE_CHRC_NO_CCCD (
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_CONTROL_POINT), IBLE_CHRC_PERM_WRITE),
    	IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_CONTROL_POINT), IBLE_GATT_PERM_READ, NULL)
    )
  )
};

#define MY_UUID_SVC     0xAAAA
#define MY_UUID_CHRC    0xBBBB
#define MY_UUID_BASE    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E

iBle_svc_t my_svc;
size_t my_nbr_attrs = 2;
size_t my_nbr_chrcs = 1;
DEFINE_IBLE_SVC_CONFIG(my_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID128(MY_UUID_SVC, MY_UUID_BASE)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC_NO_CCCD (
    	IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(MY_UUID_CHRC, MY_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_WRITE),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(MY_UUID_CHRC, MY_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &current_time)
    ),
  )
};

iSpi_t spi;
iI2c_t i2c;

// -----------------------------------------------------------------------------
iEventQueue_t eventTh1;
iEventQueue_t eventTh2;
iEventQueue_t eventTh3;
iEventQueue_t eventTh4;

iTimer_t timer;

typedef enum  {
  EV_1 = 0,
  EV_2,
  EV_3,
  EV_4
} threadEvent_t;

ITHREAD_HANDLER(thread1)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh1)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh1);
    if(event == EV_1)
    iPrint("TH1 Event1 occured\n");
    else if(event == EV_2)
    iPrint("TH1 Event2 occured\n");
    else if(event == EV_3)
    iPrint("TH1 Event3 occured\n");
    else if(event == EV_4)
    iPrint("TH1 Event4 occured\n");
  }
}

ITHREAD_HANDLER(thread2)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh2)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh2);
    if(event == EV_1)
    iPrint("TH2 Event1 occured\n");
    else if(event == EV_2)
    iPrint("TH2 Event2 occured\n");
    else if(event == EV_3)
    iPrint("TH2 Event3 occured\n");
    else if(event == EV_4)
    iPrint("TH2 Event4 occured\n");
  }
}

ITHREAD_HANDLER(thread3)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh3)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh3);
    if(event == EV_1)
    iPrint("TH3 Event1 occured\n");
    else if(event == EV_2)
    iPrint("TH3 Event2 occured\n");
    else if(event == EV_3)
    iPrint("TH3 Event3 occured\n");
    else if(event == EV_4)
    iPrint("TH3 Event4 occured\n");
  }
}

ITHREAD_HANDLER(thread4)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh4)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh4);
    if(event == EV_1)
    iPrint("TH4 Event1 occured\n");
    else if(event == EV_2)
    iPrint("TH4 Event2 occured\n");
    else if(event == EV_3)
    iPrint("TH4 Event3 occured\n");
    else if(event == EV_4)
    iPrint("TH4 Event4 occured\n");
  }
}

ITIMER_HANDLER(timer_handler)
{
  static int cnt = 0;
  cnt = (cnt + 1) % 4;

  if(cnt == 1){
    iEventQueue_add(&eventTh1, EV_1);
  }
  else if(cnt == 2){
    iEventQueue_add(&eventTh2, EV_2);
  }
  else if(cnt == 3){
    iEventQueue_add(&eventTh3, EV_3);
  }
  else{
    iEventQueue_add(&eventTh4, EV_4);
  }
}

DEFINE_ITHREAD(th1, 4096, 0);
DEFINE_ITHREAD(th2, 4096, 0);
DEFINE_ITHREAD(th3, 4096, 0);
DEFINE_ITHREAD(th4, 4096, 0);

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iPrint("\tInitialization\n");
  iPrint("\t--------------\n");

  // bluetooth_init();
  extBoad_init();

  iPrint("\t--------------\n");

  // bluetooth_test();
  // acc_test();
  // adc_test();
  // swg_test();
  // event_test();

  iThread_init(&th1, thread1);
  iThread_init(&th2, thread2);
  iThread_init(&th3, thread3);
  iThread_init(&th4, thread4);

  iEventQueue_init(&eventTh1);
  iEventQueue_init(&eventTh2);
  iEventQueue_init(&eventTh3);
  iEventQueue_init(&eventTh4);

  iTimer_start(&timer, timer_handler, 1000);

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

iGpio_t ext_irq;
IGPIO_HANDLER(on_ext_irq, pin)
{
  iPrint("It's work\n");
}

void swg_test()
{
  iPrint("- Initialize SWG\n");
  // iGpio_interrupt_init(&ext_irq, INTERRUPT_PIN, IGPIO_RISING_EDGE, IGPIO_PULL_NORMAL, on_ext_irq);
  // iGpio_enable_interrupt(&ext_irq);

  while(1)
  {
    swg_init(EXT_INT_FREQ);
    // iSleep_ms(5000);
    // swg_sleep();
    // iSleep_ms(5000);
    // swg_wakeup();
  }
}

void extBoad_init()
{
  iPrint("- Initialize extBoard\n");

  // Configure SPI
  iSpi_init(&spi, SPI1);

  // Configure I2C
  // iI2c_init(&i2c, I2C0);
}

void bluetooth_init()
{
  iPrint("- Initialize Bluetooth\n");

  iBle_init();
  iBle_svc_init(&cts_svc, cts_config, cts_nbr_attrs, cts_nbr_chrcs);
  iBle_svc_init(&hrs_svc, hrs_config, hrs_nbr_attrs, hrs_nbr_chrcs);
  iBle_svc_init(&my_svc, my_config, my_nbr_attrs, my_nbr_chrcs);
  iBle_adv_start(advdata, sizeof(advdata)/sizeof(iBle_advdata_t), scanrsp, sizeof(scanrsp)/sizeof(iBle_advdata_t));
}

void bluetooth_test()
{
  while(1)
  {
    if(iBle_isConnected())
    {
      iSleep_ms(1000);
      current_time++;

      iBle_svc_notify(&cts_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
      iBle_svc_indication(&hrs_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
    }
  }
}

void acc_test()
{
  sample_t samples[ACC_SAMPLE_NBR_MAX];

  acc_init();

  while(1)
  {
    acc_getXYZ(samples, ACC_SAMPLE_NBR_MAX);
  }
}

void adc_test()
{
  uint32_t measurement;
  adc_init();

  while(1)
  {
    for(int i = 0; i < 100; i++)
    {
      adc_getMeasurement(&measurement);
    }

    adc_sleep();

    for(int i = 0; i < 100; i++)
    {
      adc_getMeasurement(&measurement);
    }

    adc_wakeup();
  }
}

iEventQueue_t eventQueue;
typedef enum  {
  EVENT_1 = 0,
  EVENT_2,
  EVENT_3,
  EVENT_4
} testEvent_t;

void event_test()
{
  iEventQueue_init(&eventQueue);

  while(1)
  {
    while(iEventQueue_isEmpty(&eventQueue)) { iSleep(); }

    iEvent_t event = iEventQueue_get(&eventQueue);

    if(event == EVENT_1)
    iPrint("Event1 occured\n");
    else if(event == EVENT_2)
    iPrint("Event2 occured\n");
    else if(event == EVENT_3)
    iPrint("Event3 occured\n");
    else if(event == EVENT_4)
    iPrint("Event4 occured\n");
  }
}
