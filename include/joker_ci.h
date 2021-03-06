/* 
 * Access to Joker TV CI (Common Interface)
 * 
 * Conditional Access Module for scrambled streams (pay tv)
 * Based on EN 50221-1997 standard
 * 
 * https://jokersys.com
 * (c) Abylay Ospan, 2017
 * aospan@jokersys.com
 * GPLv2
 */

#include <stdio.h>
#include "joker_tv.h"

#ifndef _JOKER_CI
#define _JOKER_CI	1

#ifdef __cplusplus
extern "C" {
#endif

#define TUPLE_MAX_SIZE	128
#define JOKER_CI_IO	1
#define JOKER_CI_MEM	0
#define CAID_MAX_COUNT	128

// commands to read/write CAM module IO or MEM
#define JOKER_CI_CTRL_READ	1 << 0 // 0x01
#define JOKER_CI_CTRL_WRITE	1 << 1 // 0x02
#define JOKER_CI_CTRL_IO	1 << 2 // 0x04
#define JOKER_CI_CTRL_MEM	1 << 3 // 0x08
#define JOKER_CI_CTRL_BULK	1 << 4 // 0x10

// commands status
#define JOKER_CI_CTRL_ERR	1
#define JOKER_CI_CTRL_OK	2

/* from Linux kernel:
 * ./drivers/media/dvb-core/dvb_ca_en50221.c */
#define CTRLIF_DATA      0
/* command bits: R R R R RS SR SW HC */
#define CTRLIF_COMMAND   1
/* status bits: DA FR R R R R WE RE */
#define CTRLIF_STATUS    1
#define CTRLIF_SIZE_LOW  2
#define CTRLIF_SIZE_HIGH 3

#define CMDREG_HC        1      /* Host control */
#define CMDREG_SW        2      /* Size write */
#define CMDREG_SR        4      /* Size read */
#define CMDREG_RS        8      /* Reset interface */
#define CMDREG_FRIE   0x40      /* Enable FR interrupt */
#define CMDREG_DAIE   0x80      /* Enable DA interrupt */
#define IRQEN (CMDREG_DAIE)

#define STATUSREG_RE     1      /* read error */
#define STATUSREG_WE     2      /* write error */
#define STATUSREG_FR  0x40      /* module free */
#define STATUSREG_DA  0x80      /* data available */
#define STATUSREG_TXERR (STATUSREG_RE|STATUSREG_WE)     /* general transfer error */


struct ci_tuple_t {
	int type;
	int size;
	unsigned char data[TUPLE_MAX_SIZE];
};

struct joker_ci_t {
	/* enable CAM debug if not zero */
	int ci_verbose;

	/* CAM manufacturer info */
	uint16_t manfid;
	uint16_t devid;

	/* base address of CAM config */
	uint32_t config_base;

	/* value to write into Config Control register */
	uint8_t config_option;

	/* current offset when reading next tuple */
	int tuple_cur_offset;

	/* CAM module detected */
	int cam_detected;

	/* CAM module validated */
	int cam_validated;

	/* CAM module info string */
	unsigned char cam_infostring[TUPLE_MAX_SIZE];

	/* EN50221 stuff */
	uint8_t application_type;
	uint16_t application_manufacturer;
	uint16_t manufacturer_code;
	unsigned char menu_string[TUPLE_MAX_SIZE];
	uint16_t ca_ids[CAID_MAX_COUNT];
};

/* initialize CAM module
 * return 0 if success
 * other return values indicates error
 */
int joker_ci(struct joker_t * joker);

#ifdef __cplusplus
}
#endif

#endif /* end */
