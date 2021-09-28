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

#ifndef COMMON_H
#define COMMON_H

#ifndef PAC5XXX_OK
#define PAC5XXX_OK      0
#endif

#ifndef PAC5XXX_ERROR
#define PAC5XXX_ERROR   1
#endif

#define READ_UINT16(address)                      (*((uint16_t *) address))
#define READ_UINT32(address)                      (*((uint32_t *) address))

#endif // #ifndef COMMON_H
