#include "iTimer.h"

static bool isAppTimerInit = false;

// Only used by iTimer and iBle
void iTimer_init()
{
	int error;

	if(!isAppTimerInit)
	{


		// Initialize clock
		nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
		SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

		// Initialize and enable APP timer
		error = app_timer_init();
		if(error) {
			iPrint("/!\\ APP Timer init failed: error %d\n", error);
			return ;
		}
		isAppTimerInit = true;
	}
}

void iTimer_start(iTimer_t* timer, iTimer_handler_t handler, uint32_t period)
{
	int error;
	iTimer_init();

	memset(&timer->timer_data, 0, sizeof(timer->timer_data));
	timer->id = &timer->timer_data;

	error = app_timer_create(&timer->id, APP_TIMER_MODE_REPEATED, handler);
	if(error) {
		iPrint("/!\\ Fail to create timer: error %d, %d, %d\n", error, NRF_ERROR_INVALID_PARAM, NRF_ERROR_INVALID_STATE);
		return ;
	}

  error =  app_timer_start(timer->id, APP_TIMER_TICKS(period), NULL);
	if(error) {
		iPrint("/!\\ Fail to start timer: error %d\n", error);
		return ;
	}
}
