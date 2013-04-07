
#include "compiler.h"
#include "sysclk.h"
#include "conf_board.h" /* To get on-board memories configurations */
#include "conf_access.h"
#include "datalog_tasks.h"


#include <sd_mmc.h>

void datalog_tasks_init(void)
{
	uint8_t slot = 0;
	sd_mmc_err_t err;
	sd_mmc_init();
	if (slot == sd_mmc_nb_slot()) {
			slot = 0;
	}
	// Wait for a card and ready
	do {
			err = sd_mmc_check(slot);
			if ((SD_MMC_ERR_NO_CARD != err)
							&& (SD_MMC_INIT_ONGOING != err)
							&& (SD_MMC_OK != err)) {
					while (SD_MMC_ERR_NO_CARD != sd_mmc_check(slot)) {
					}
			}
	} while (SD_MMC_OK != err);
}
