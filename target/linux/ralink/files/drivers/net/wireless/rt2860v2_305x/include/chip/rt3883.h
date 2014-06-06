/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************

    Module Name:
	rt3883.h
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */

#ifndef __RT3883_H__
#define __RT3883_H__

#ifndef RTMP_RBUS_SUPPORT
#error "For RT3883, you should define the compile flag -DRTMP_RBUS_SUPPORT"
#endif

#ifndef RTMP_MAC_PCI
#error "For RT3883, you should define the compile flag -DRTMP_MAC_PCI"
#endif

#include "chip/mac_pci.h"

#define RTMP_MAC_CSR_ADDR 	0xB0180000
#define RTMP_FLASH_BASE_ADDR	0xbc000000

extern REG_PAIR RT3883_RFRegTable[];
extern UCHAR RT3883_NUM_RF_REG_PARMS;

#ifdef DFS_SUPPORT
#define DFS_2_SUPPORT
#endif


#ifdef CARRIER_DETECTION_SUPPORT
#define CARRIER_2_SUPPORT
#define TONE_RADAR_DETECT_SUPPORT
#endif

//
// Device ID & Vendor ID, these values should match EEPROM value
//



#endif //__RT3883_H__ //

