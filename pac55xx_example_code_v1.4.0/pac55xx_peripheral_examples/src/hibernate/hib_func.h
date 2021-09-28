//=============================================================================
// Copyright (C) 2018-2019, Qorvo, Inc.
//
// THIS SOFTWARE IS SUBJECT TO A SOURCE CODE LICENSE AGREEMENT WHICH PROVIDES,
// AMONG OTHER THINGS:  (i) THAT IT CAN BE USED ONLY TO ADAPT THE LICENSEE'S
// APPLICATION TO PAC PROCESSORS SUPPLIED BY QORVO, INC.;
// (ii) THAT  IT IS PROVIDED "AS IS" WITHOUT WARRANTY;  (iii) THAT
// QORVO, INC. IS NOT LIABLE FOR ANY INDIRECT DAMAGES OR FOR DIRECT
// DAMAGES EXCEEDING US$1,500;  AND (iv) THAT IT CAN BE DISCLOSED TO AND USED
// ONLY BY CERTAIN AUTHORIZED PERSONS.
//
//=============================================================================

#ifndef HIB_FUNC_H
#define HIB_FUNC_H

#include "pac5xxx.h"
#include "pac5xxx_gpio.h"
#include "pac5xxx_driver_system.h"
#include "pac5xxx_driver_tile.h"
#include "pac5xxx_tile_power_manager.h"

#define HIB_NORMAL          (0 << 7)
#define HIB_SHUTDOWN_MODE   (1 << 7)

#define PBEN_EN             1
#define PBEN_DIS            0

#define TPBD_32ms			0
#define TPBD_1ms			1

#define SMEN_EN				1
#define SMEN_DIS			0

void hibernate_mode_config(void);

#endif
