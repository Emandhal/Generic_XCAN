/*!*****************************************************************************
 * @file    XCAN_core.h
 * @author  Fabien 'Emandhal' MAILLY
 * @version 1.0.0
 * @date    01/04/2023
 * @brief   Bosch X_CAN core IP registers
 * @details
 * The X_CAN Controller IP is a CAN-bus controller supporting CAN2.0A, CAN2.0B,
 *   CAN-FD, CAN-XL
 * Follow datasheet X_CAN user manual v3.50 (Nov 2022)
 ******************************************************************************/
/* @page License
 *
 * Copyright (c) 2020-2023 Fabien MAILLY
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#ifndef XCAN_CORE_H_INC
#define XCAN_CORE_H_INC
//=============================================================================

//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
//-----------------------------------------------------------------------------
#ifdef __cplusplus
  extern "C" {
#  define __XCAN_PACKED__
#  define XCAN_PACKITEM             __pragma(pack(push, 1))
#  define XCAN_UNPACKITEM           __pragma(pack(pop))
#  define XCAN_PACKENUM(name,type)  typedef enum name : type
#  define XCAN_UNPACKENUM(name)     name
#else
#  include "Conf_XCAN.h"
#  define __XCAN_PACKED__           __attribute__((packed))
#  define XCAN_PACKITEM
#  define XCAN_UNPACKITEM
#  define XCAN_PACKENUM(name,type)  typedef enum __MCAN_PACKED__
#  define XCAN_UNPACKENUM(name)     name
#endif
//-----------------------------------------------------------------------------

//! This macro is used to check the size of an object. If not, it will raise a "divide by 0" error at compile time
#define XCAN_CONTROL_ITEM_SIZE(item, size)  enum { item##_size_must_be_##size##_bytes = 1 / (int)(!!(sizeof(item) == size)) }

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN common definitions
//********************************************************************************************************************

//! Data Length Size for the CAN2.0 and CAN-FD message
typedef enum
{
  XCAN_DLC_0BYTE   = 0b0000, //!< The DLC is  0 data byte
  XCAN_DLC_1BYTE   = 0b0001, //!< The DLC is  1 data byte
  XCAN_DLC_2BYTE   = 0b0010, //!< The DLC is  2 data bytes
  XCAN_DLC_3BYTE   = 0b0011, //!< The DLC is  3 data bytes
  XCAN_DLC_4BYTE   = 0b0100, //!< The DLC is  4 data bytes
  XCAN_DLC_5BYTE   = 0b0101, //!< The DLC is  5 data bytes
  XCAN_DLC_6BYTE   = 0b0110, //!< The DLC is  6 data bytes
  XCAN_DLC_7BYTE   = 0b0111, //!< The DLC is  7 data bytes
  XCAN_DLC_8BYTE   = 0b1000, //!< The DLC is  8 data bytes
  XCAN_DLC_12BYTE  = 0b1001, //!< The DLC is 12 data bytes
  XCAN_DLC_16BYTE  = 0b1010, //!< The DLC is 16 data bytes
  XCAN_DLC_20BYTE  = 0b1011, //!< The DLC is 20 data bytes
  XCAN_DLC_24BYTE  = 0b1100, //!< The DLC is 24 data bytes
  XCAN_DLC_32BYTE  = 0b1101, //!< The DLC is 32 data bytes
  XCAN_DLC_48BYTE  = 0b1110, //!< The DLC is 48 data bytes
  XCAN_DLC_64BYTE  = 0b1111, //!< The DLC is 64 data bytes
  XCAN_DLC_COUNT,            // Keep last
  XCAN_PAYLOAD_MIN =  8,
  XCAN_PAYLOAD_MAX = 64,
} eXCAN_DataLength;

static const uint8_t XCAN20_DLC_TO_VALUE[XCAN_DLC_COUNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8,  8,  8,  8,  8,  8,  8,  8};
static const uint8_t XCANFD_DLC_TO_VALUE[XCAN_DLC_COUNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
#define XCAN_DLCToByte(dlc, isCANFD)  ( ((isCANFD) ? CANFD_DLC_TO_VALUE[(size_t)(dlc) & 0xF] : CAN20_DLC_TO_VALUE[(size_t)(dlc) & 0xF] )

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Filters Objects
//********************************************************************************************************************

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Tx Messages Objects
//********************************************************************************************************************

//! CAN Tx DMA info control 1 (DMA Info Ctrl 1)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TxDMAinfoCtrl1
{
  uint32_t TxDMAinfoCtrl1;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t STS  : 4; /*!<  0- 3 - [Managed by MH] Status: gives the status of the TX message transmitted.
                        *           The MH writes back only the Header Descriptor (HD bit set to 1) for status report.
                        *           The SW must always set it to 0
                        */
    uint32_t RC   : 5; /*!<  4- 8 - [Managed by SW] Rolling Counter: use to track the order of TX descriptor  fetched when a TX FIFO Queue or a TX Priority Queue slot is running.
                        *           TX FIFO Queue: When a TX FIFO Queue is started for the first time, its first TX descriptor must have the RC[4:0] set to 5’b00000.
                        *             This value must be incremented for every new TX descriptor up to 5’b11111 and then back to 5’b00000 even in case of wrap.
                        *           TX Priority Queue: This bit field must be set to 5’b00000 as the initial value for all TX Header descriptor defined in the different slots
                        */
    uint32_t      : 2; //!<  9-10
    uint32_t PQSN : 5; /*!< 11-15 - [Managed by SW] FQN[4:1] TX FIFO Queue: define the TX FIFO Queue number allocated to this TX descriptor.
                        *             Despite being set to 4bit, only the FQN[2:0] bit range is used
                        *           [Managed by SW] PQSN[4:0] TX Priority Queue: define the TX FIFO Queue slot number allocate to this descriptor
                        */
    uint32_t CRC  : 9; /*!< 16-24 - [Managed by SW] CRC: this CRC is computed by the SW for the current TX descriptor.
                        *           It must consider all elements assuming this bit field as set to 0. Any CRC error is triggering an interrupt to the system.
                        *           The CRC is not evaluated if the MH_SFTY_CTRL.TX_DESC_CRC_EN bit is set to 0
                        */
    uint32_t END  : 1; /*!< 25    - [Managed by SW] For the TX FIFO Queue: when set to 1 the TX FIFO Queue defined is ending, it means, it is set as inactive.
                        *             Once done, the TX FIFO Queue can be reprogrammed and started
                        *           For the TX Priority Queue: must be set to 0
                        */
    uint32_t PQ   : 1; //!< 26    - [Managed by SW] TX Priority Queue: '1' = TX descriptor belongs to the TX Priority Queue ; '0' = TX FIFO Queue
    uint32_t IRQ  : 1; //!< 27    - [Managed by SW] Interrupt: when set to 1 an interrupt is triggered to the system when the descriptor execution is complete meaning when the TX message has been sent to the CAN bus
    uint32_t NEXT : 1; //!< 28    - [Managed by SW] Set to 0
    uint32_t WRAP : 1; /*!< 29    - [Managed by SW] Wrap: When set to 1 the next message descriptor is the one declared at the initial start address of the TX FIFO Queue (First Descriptor).
                        *             This bit provides a way to the SW to keep the next TX message continuous in a memory buffer if less space is available at the end of a data container
                        */
    uint32_t HD   : 1; //!< 30    - [Managed by SW] Set to 1
    uint32_t VALID: 1; /*!< 31    - [Managed by SW/MH] Valid: The SW must set this bit to 1 to define a TX descriptor is valid for the MH. When the descriptor has been fully used,
                        *             the MH will clear this bit when writing the acknowledge data information back to this descriptor. This update occurs only when the HD bit is set to 1.
                        *           In case the descriptor is fetched when this bit is set to 0, an interrupt TX_FQ_IRQ is triggered to the system for the TX FIFO queue n having this descriptor
                        */
  };
} XCAN_TxDMAinfoCtrl1;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TxDMAinfoCtrl1, 4);

//! Tx messages status enumerator
typedef enum eXCAN_TxStatus
{
  XCAN_TX_STATUS_NONE                   = 0b0000, //!< None
  XCAN_TX_STATUS_MESSAGE_SENT_SUCCESS   = 0b0001, //!< Message sent successfully
  XCAN_TX_STATUS_MESSAGE_NOT_SENT       = 0b0010, //!< Message not sent after a number of trials
  XCAN_TX_STATUS_MESSAGE_SKIPPED        = 0b0011, //!< Message skipped due to HFI
  XCAN_TX_STATUS_MESSAGE_REJECTED       = 0b0100, //!< Message rejected by TX filter
  XCAN_TX_STATUS_MESSAGE_ACK_WITH_ERROR = 0b1111, //!< Message acknowledge data with parity error
} eXCAN_TxStatus;

#define XCAN_TxDMA1_STS_Pos                      0
#define XCAN_TxDMA1_STS_Mask                     (0xFu << XCAN_TxDMA1_STS_Pos)
#define XCAN_TxDMA1_STS_SET(value)               (((uint32_t)(value) << XCAN_TxDMA1_STS_Pos) & XCAN_TxDMA1_STS_Mask) //!< Set TX Message Status
#define XCAN_TxDMA1_STS_GET(value)               (eXCAN_TxStatus)(((uint32_t)(value) & XCAN_TxDMA1_STS_Mask) >> XCAN_TxDMA1_STS_Pos) //!< Get TX Message Status
#define XCAN_TxDMA1_RC_Pos                       4
#define XCAN_TxDMA1_RC_Mask                      (0x1Fu << XCAN_TxDMA1_RC_Pos)
#define XCAN_TxDMA1_RC_SET(value)                (((uint32_t)(value) << XCAN_TxDMA1_RC_Pos) & XCAN_TxDMA1_RC_Mask) //!< Set Rolling Counter
#define XCAN_TxDMA1_RC_GET(value)                (((uint32_t)(value) & XCAN_TxDMA1_RC_Mask) >> XCAN_TxDMA1_RC_Pos) //!< Get Rolling Counter
#define XCAN_TxDMA1_FQN_Pos                      12
#define XCAN_TxDMA1_FQN_Mask                     (0xFu << XCAN_TxDMA1_FQN_Pos)
#define XCAN_TxDMA1_FQN_SET(value)               (((uint32_t)(value) << XCAN_TxDMA1_FQN_Pos) & XCAN_TxDMA1_FQN_Mask) //!< Set TX FIFO Queue number allocated to this TX descriptor
#define XCAN_TxDMA1_FQN_GET(value)               (((uint32_t)(value) & XCAN_TxDMA1_FQN_Mask) >> XCAN_TxDMA1_FQN_Pos) //!< Get TX FIFO Queue number allocated to this TX descriptor
#define XCAN_TxDMA1_PQSN_Pos                     11
#define XCAN_TxDMA1_PQSN_Mask                    (0x1Fu << XCAN_TxDMA1_PQSN_Pos)
#define XCAN_TxDMA1_PQSN_SET(value)              (((uint32_t)(value) << XCAN_TxDMA1_PQSN_Pos) & XCAN_TxDMA1_PQSN_Mask) //!< Set Priority Queue Slot Number
#define XCAN_TxDMA1_PQSN_GET(value)              (((uint32_t)(value) & XCAN_TxDMA1_PQSN_Mask) >> XCAN_TxDMA1_PQSN_Pos) //!< Get Priority Queue Slot Number
#define XCAN_TxDMA1_CRC_Pos                      16
#define XCAN_TxDMA1_CRC_Mask                     (0x1FFu << XCAN_TxDMA1_CRC_Pos)
#define XCAN_TxDMA1_CRC_SET(value)               (((uint32_t)(value) << XCAN_TxDMA1_CRC_Pos) & XCAN_TxDMA1_CRC_Mask) //!< Set CRC
#define XCAN_TxDMA1_CRC_GET(value)               (((uint32_t)(value) & XCAN_TxDMA1_CRC_Mask) >> XCAN_TxDMA1_CRC_Pos) //!< Get CRC
#define XCAN_TxDMA1_END_TX_PRIORITY_QUEUE        (0x1u << 25) //!< End TX Priority Queue
#define XCAN_TxDMA1_END_TX_FIFO_QUEUE            (0x0u << 25) //!< End TX FIFO Queue defined is ending, it means, it is set as inactive
#define XCAN_TxDMA1_PQ_TX_PRIORITY_QUEUE         (0x1u << 26) //!< TX Priority Queue
#define XCAN_TxDMA1_PQ_TX_FIFO_QUEUE             (0x0u << 26) //!< TX FIFO Queue
#define XCAN_TxDMA1_IRQ_WHEN_SENT                (0x1u << 27) //!< Interrupt is triggered to the system when the descriptor execution is complete, meaning when the TX message has been sent to the CAN bus
#define XCAN_TxDMA1_IRQ_NO_IRQ                   (0x0u << 27) //!< No interrupt
#define XCAN_TxDMA1_WRAP_TO_FIRST_ELEMENT        (0x1u << 29) //!< TX FIFO Queue can be used as a circular buffer when the Last Descriptor defines a wrap to the First Descriptor
#define XCAN_TxDMA1_NO_WRAP                      (0x0u << 29) //!< TX Priority Queue do not use wrap
#define XCAN_TxDMA1_HD                           (0x1u << 30) //!< HD must be set to 1
#define XCAN_TxDMA1_VALID_SET_VALID_FOR_MH       (0x1u << 31) //!< Define a TX descriptor is valid for the MH
#define XCAN_TxDMA1_VALID_IS_ACKNOWLEDGE(value)  ( ((value) & XCAN_TxDMA1_VALID_SET_VALID_FOR_MH) > 0 ) //!< Is the MH clears this bit when writing the acknowledge data information back to this descriptor

//-----------------------------------------------------------------------------

//! CAN Tx DMA info control 2 (DMA Info Ctrl 2)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TxDMAinfoCtrl2
{
  uint32_t TxDMAinfoCtrl2;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t      :  2; //!<  0- 1
    uint32_t TDO  : 10; /*!<  2-11 - [Managed by SW] For the TX Priority Queue: must be set to 0
                         *           [Managed by SW] For the TX FIFO Queue: must be set to 1
                         */
    uint32_t      :  1; //!< 12
    uint32_t IN   :  3; /*!< 13-15 - [Managed by SW] Instance Number: define the X_CAN instance number using that descriptor.
                         *             This bit field is relevant if several X_CAN are running concurrently. It provides a way to detect descriptor fetch issue between instances.
                         *             The value defined must be equal to the one defined in the MH_CFG.INST_NUM bit field register
                         */
    uint32_t SIZE : 10; /*!< 16-25 - [Managed by SW] Define the buffer size in word (32bit) for the given TX descriptor to transmit to the PRT. As an example, a payload from 1 to 4 bytes requires SIZE to be set to 1.
                         *             As only 32bit read accesses are performed the buffer size containing the payload must be 32bit aligned.
                         *           When set to 0, there is no payload data attached to the TX descriptor (only valid for Classical CAN/CAN FD without payload or a Classical CAN remote frame).
                         *           For CAN XL no data is defined in TX descriptor. The MH replies only on the address pointer defined in element 7 to fetch payload data from S_MEM.
                         *           For CAN FD: SIZE > 1: The copy of the first data payload (aligned on 32bit) is required in element 6. The address pointer in element 7 is used to fetch the payload data from S_MEM.
                         *                       SIZE = 1: The copy of the first data payload (aligned on 32bit) is required in element 6. In case it is less than 4bytes, just pad with 0s. The address pointer in element 7 is not used.
                                                           Nevertheless, it is required to have it set to the address of the payload data in S_MEM.
                                                 SIZE=0: Elements 7 and 6 are not used
                         */
    uint32_t PLSRC:  1; /*!< 26    - [Managed by SW] Payload Source: This bit provides to the MH the information about the need to fetch payload data in the data container when executing only a TX Header Descriptor.
                         *           When set to 1: the TX descriptor is attached to a data container which would need to be accessed and the bit field SIZE[9:0] defines the number of TX data to send for this descriptor.
                         *             For CAN XL, as no payload data can be defined in TX descriptor, this bit is always set to 1 for CAN XL.
                         *             For CAN FD, this bit is set to 1 when the payload data is greater than 4bytes.
                         *           When set to 0: the payload data defined in the data container are not required. Therefore, the TX descriptor includes all data payload.
                         *             For the Classical CAN, all payload data are always included, this bit must always be set to 0.
                         *             In case of CAN FD, it would be set to 0 only when the payload data is less or equal to 4bytes.
                         *             Nevertheless, the bit field SIZE[9:0] still defines the number of payload data to send per TX descriptor
                         */
    uint32_t      :  5; //!< 27-31
  };
} XCAN_TxDMAinfoCtrl2;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TxDMAinfoCtrl2, 4);

#define XCAN_TxDMA2_TDO_VALUE                ((uint32_t)0x000) //!< TDO value for the TX Priority Queue: must be set to 0
#define XCAN_TxDMA2_TDO_Pos                  2
#define XCAN_TxDMA2_TDO_Mask                 (0x3FFu << XCAN_TxDMA2_TDO_Pos)
#define XCAN_TxDMA2_TDO_SET(value)           (((uint32_t)(value) << XCAN_TxDMA2_TDO_Pos) & XCAN_TxDMA2_TDO_Mask) //!< Set TDO value
#define XCAN_TxDMA2_TDO_GET(value)           (((uint32_t)(value) & XCAN_TxDMA2_TDO_Mask) >> XCAN_TxDMA2_TDO_Pos) //!< Get TDO value
#define XCAN_TxDMA2_NHDO_VALUE               ((uint32_t)0x3FF) //!< NHDO value for the TX FIFO Queue: must be set to 1
#define XCAN_TxDMA2_NHDO_Pos                 2
#define XCAN_TxDMA2_NHDO_Mask                (XCAN_TxDMA2_NHDO_VALUE << XCAN_TxDMA2_NHDO_Pos)
#define XCAN_TxDMA2_NHDO_SET(value)          (((uint32_t)(value) << XCAN_TxDMA2_NHDO_Pos) & XCAN_TxDMA2_NHDO_Mask) //!< Set NHDO value
#define XCAN_TxDMA2_NHDO_GET(value)          (((uint32_t)(value) & XCAN_TxDMA2_NHDO_Mask) >> XCAN_TxDMA2_NHDO_Pos) //!< Get NHDO value
#define XCAN_TxDMA2_IN_Pos                   13
#define XCAN_TxDMA2_IN_Mask                  (0x7u << XCAN_TxDMA2_IN_Pos)
#define XCAN_TxDMA2_IN_SET(value)            (((uint32_t)(value) << XCAN_TxDMA2_IN_Pos) & XCAN_TxDMA2_IN_Mask) //!< Set Instance Number
#define XCAN_TxDMA2_IN_GET(value)            (((uint32_t)(value) & XCAN_TxDMA2_IN_Mask) >> XCAN_TxDMA2_IN_Pos) //!< Get Instance Number
#define XCAN_TxDMA2_SIZE_Pos                 16
#define XCAN_TxDMA2_SIZE_Mask                (0x3FFu << XCAN_TxDMA2_SIZE_Pos)
#define XCAN_TxDMA2_SIZE_SET(value)          (((uint32_t)(value) << XCAN_TxDMA2_SIZE_Pos) & XCAN_TxDMA2_SIZE_Mask) //!< Set buffer size in word (32bit)
#define XCAN_TxDMA2_SIZE_GET(value)          (((uint32_t)(value) & XCAN_TxDMA2_SIZE_Mask) >> XCAN_TxDMA2_SIZE_Pos) //!< Get buffer size in word (32bit)
#define XCAN_TxDMA1_PLSRC_IN_DATA_CONTAINER  (0x1u << 26) //!< TX descriptor is attached to a data container (CAN-XL and CAN-FD > 4 bytes)
#define XCAN_TxDMA1_PLSRC_IN_TX_DESCRIPTOR   (0x0u << 26) //!< The TX descriptor includes all data payload (CAN2.0: TD0 and TD1, CAN-FD <= 4 bytes: TD0)

//-----------------------------------------------------------------------------

//! Tx messages descriptor overview enumerator
typedef enum eXCAN_TxStatus
{
  XCAN_CAN_TXDESC_TIC1,                        //!< CAN Tx DMA info control 1 (DMA Info Ctrl 1)
  XCAN_CAN_TXDESC_TIC2,                        //!< CAN Tx DMA info control 2 (DMA Info Ctrl 2)
  XCAN_CAN_TXDESC_TS0,                         //!< TimeStamp [31:0]
  XCAN_CAN_TXDESC_TS1,                         //!< TimeStamp [63:32]
  XCAN_CAN_TXDESC_T0,                          //!< TX Message Header Information 0
  XCAN_CAN_TXDESC_T1,                          //!< TX Message Header Information 1
  XCAN_CAN_TXDESC_T2,                          //!< TX Message Header Information 2 (CAN-XL)
  XCAN_CAN_TXDESC_TD0 = XCAN_CAN_TXDESC_T2,    //!< First TX Data Payload 0
  XCAN_CAN_TXDESC_TD1,                         //!< First TX Data Payload 1
  XCAN_CAN_TXDESC_TX_AP = XCAN_CAN_TXDESC_TD1, //!< TX Payload Data Address Pointer
  XCAN_CAN_TXDESC_COUNT,                       // KEEP LAST!
} eXCAN_TxDescriptor;

//! TX Queue Descriptor Overview (TX Queue, and TX FIFO)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_CAN_TxMessage
{
  uint32_t Word[XCAN_CAN_TXDESC_COUNT];
  uint8_t Bytes[XCAN_CAN_TXDESC_COUNT * sizeof(uint32_t)];
  struct
  {
    XCAN_TxDMAinfoCtrl1 TIC1; //!< CAN Tx DMA info control 1 (TIC1)
    XCAN_TxDMAinfoCtrl2 TIC2; //!< CAN Tx DMA info control 2 (TIC2)
    uint32_t TS0;             //!< Timestamp 0: LSB of the 64bits timestamp of the successfully sent TX message (only valid when HD bit is set to 1)
    uint32_t TS1;             //!< Timestamp 1: MSB of the 64bits timestamp of the successfully sent TX message (only valid when HD bit is set to 1)
    XCAN_TxMessageHeader0 T0; //!< CAN Transmit Message Header 0 (T0)
    XCAN_TxMessageHeader1 T1; //!< CAN Transmit Message Header 1 (T1)
    union
    {
      uint32_t TD0;           //!< Classical CAN and CAN FD: define the first payload of the TX message
      uint32_t T2;            //!< CAN-XL Acceptance Field
    };
    union
    {
      uint32_t TD1;           //!< Classical CAN with payload greater equal to 4byte: define the last payload data of the TX message for the Classical CAN (in case payload data is greater than 4bytes)
      uint32_t TX_AP;         /*!< CAN XL and CAN FD (with payload greater than 4bytes): Address pointer to fetch the TX message payload data for CAN FD and CAN XL frames.
                               *     For CAN FD frames with more than 4 bytes this bit field is, nevertheless, mandatory.
                               *     As the address pointer must be 32bit aligned the two LSB will not be considered and so must be set to 0 all time.
                               *     In case the TX_AP is not used it must be set to 0
                               */
    };
  };
} XCAN_CAN_TxMessage;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_CAN_TxMessage, 32);

//-----------------------------------------------------------------------------

#define MCAN_CAN_TX_MESSAGE_SIZE  ( sizeof(XCAN_CAN_TxMessage) )

//-----------------------------------------------------------------------------

//! CAN Transmit Message Header 1 (T0)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TxMessageHeader0
{
  uint32_t T0;
  uint8_t Bytes[sizeof(uint32_t)];
  struct                 // Classical CAN TX Header definition
  {
    uint32_t ExtID : 18; //!<  0-17 - Extended ID
    uint32_t BaseID: 11; //!< 18-28 - Base ID
    uint32_t XTD   :  1; //!< 29    - Extended Identifier
    uint32_t XLF   :  1; //!< 30    - XL Format
    uint32_t FDF   :  1; //!< 31    - FD Format
  } CAN20;
  struct                 // CAN FD TX Header definition
  {
    uint32_t ExtID : 18; //!<  0-17 - Extended ID
    uint32_t BaseID: 11; //!< 18-28 - Base ID
    uint32_t XTD   :  1; //!< 29    - Extended Identifier
    uint32_t XLF   :  1; //!< 30    - XL Format
    uint32_t FDF   :  1; //!< 31    - FD Format
  } CANFD;
  struct                 // CAN XL TX Header definition
  {
    uint32_t SDT   :  8; //!<  0- 7 - SDU Type
    uint32_t VCID  :  8; //!<  8-15 - Virtual CAN Network ID
    uint32_t SEC   :  1; //!< 16    - Simple Extended Content
    uint32_t RRS   :  1; //!< 17    - Remote Request Substitution
    uint32_t PrioID: 11; //!< 18-28 - Priority identifier
    uint32_t XTD   :  1; //!< 29    - Extended Identifier
    uint32_t XLF   :  1; //!< 30    - XL Format
    uint32_t FDF   :  1; //!< 31    - FD Format
  } CANXL;
} XCAN_TxMessageHeader0;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TxMessageHeader0, 4);

#define MCAN_T0_ID_Pos           0
#define MCAN_T0_ID_Mask          (0x1FFFFFFFu << MCAN_T0_ID_Pos)
#define MCAN_T0_ID_SET(value)    (((uint32_t)(value) << MCAN_T0_ID_Pos) & MCAN_T0_ID_Mask) //!< Set Message Identifier filter

#define MCAN_T0_EID_Pos          0
#define MCAN_T0_EID_Mask         (0x3FFFFu << MCAN_T0_EID_Pos)
#define MCAN_T0_EID_SET(value)   (((uint32_t)(value) << MCAN_T0_EID_Pos) & MCAN_T0_EID_Mask) //!< Set Extended Identifier filter
#define MCAN_T0_SID_Pos          18
#define MCAN_T0_SID_Mask         (0x7FFu << MCAN_T0_SID_Pos)
#define MCAN_T0_SID_SET(value)   (((uint32_t)(value) << MCAN_T0_SID_Pos) & MCAN_T0_SID_Mask) //!< Set Standard Identifier filter

#define XCAN_T0_XTD_EXTENDED_ID  (0x1u << 29) //!< 29-bit extended identifier
#define XCAN_T0_XTD_STANDARD_ID  (0x0u << 29) //!< 11-bit standard identifier
#define XCAN_T0_XTD              (0x1u << 29) //!< XL Format
#define XCAN_T0_XLF              (0x1u << 30) //!< XL Format
#define XCAN_T0_FDF              (0x1u << 31) //!< FD Format
#define XCAN_T0_CAN20_SET        ( 0u ) //!< Set classical CAN2.0 frame
#define XCAN_T0_CANFD_SET        ( XCAN_T0_FDF ) //!< Set CAN-FD frame
#define XCAN_T0_CANXL_SET        ( XCAN_T0_XLF | XCAN_T0_FDF) //!< Set CAN-XL frame

//-----------------------------------------------------------------------------

//! CAN Transmit Message Header 1 (T1)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TxMessageHeader1
{
  uint32_t T1;
  uint8_t Bytes[sizeof(uint32_t)];
  struct                 // Classical CAN TX Header definition
  {
    uint32_t    : 16; //!<  0-15
    uint32_t DLC:  4; //!< 16-19 - Data Length Code
    uint32_t    :  6; //!< 20-25
    uint32_t RTR:  1; //!< 26    - Remote Transmission Request
    uint32_t    :  3; //!< 27-29
    uint32_t FIR:  1; //!< 30    - Fault Injection Request
    uint32_t    :  1; //!< 31
  } CAN20;
  struct                 // CAN FD TX Header definition
  {
    uint32_t    : 16; //!<  0-15
    uint32_t DLC:  4; //!< 16-19 - Data Length Code
    uint32_t ESI:  1; //!< 20    - Error State Indicator
    uint32_t    :  4; //!< 21-24
    uint32_t BRS:  1; //!< 25    - Bit Rate Switch
    uint32_t    :  1; //!< 26    - Set to 0
    uint32_t    :  3; //!< 27-29
    uint32_t FIR:  1; //!< 30    - Fault Injection Request
    uint32_t    :  1; //!< 31
  } CANFD;
  struct                 // CAN XL TX Header definition
  {
    uint32_t      : 16; //!<  0-15
    uint32_t DLCXL: 11; //!< 16-26 - Data Length Code with CAN XL encoding
    uint32_t      :  3; //!< 27-29
    uint32_t FIR  :  1; //!< 30    - Fault Injection Request
    uint32_t      :  1; //!< 31
  } CANXL;
} XCAN_TxMessageHeader1;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TxMessageHeader1, 4);

#define MCAN_T1_CANXL_DLC_Pos         16
#define MCAN_T1_CANXL_DLC_Mask        (0x7FFu << MCAN_T1_CANXL_DLC_Pos)
#define MCAN_T1_CANXL_DLC_SET(value)  (((uint32_t)(value) << MCAN_T1_CANXL_DLC_Pos) & MCAN_T1_CANXL_DLC_Mask) //!< Set Data Length Code with CAN XL encoding
#define MCAN_T1_DLC_Pos               16
#define MCAN_T1_DLC_Mask              (0xFu << MCAN_T1_DLC_Pos)
#define MCAN_T1_DLC_SET(value)        (((uint32_t)(value) << MCAN_T1_DLC_Pos) & MCAN_T1_DLC_Mask) //!< Set Data Length Code for CAN2.0 and CAN-FD
#define XCAN_T1_ESI                   (0x1u << 20) //!< Error State Indicator
#define XCAN_T1_BRS                   (0x1u << 25) //!< Bit Rate Switch
#define XCAN_T1_RTR                   (0x1u << 26) //!< Remote Transmission Request
#define XCAN_T1_FIR                   (0x1u << 30) //!< Fault Injection Request

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Rx Messages Objects
//********************************************************************************************************************

//! CAN Rx DMA info control 1 (DMA Info Ctrl 1)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RxDMAinfoCtrl1
{
  uint32_t RxDMAinfoCtrl1;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t STS  : 4; /*!<  0- 3 - [Managed by MH] Status: gives the status of the RX message received.
                        *           This bit field is written back by the MH when the descriptor has been completed. This bit field must be set to 0 by SW.
                        */
    uint32_t RC   : 5; /*!<  4- 8 - [Managed by SW] Rolling Counter: use to track the order of RX descriptor fetched when a RX FIFO Queue is running.
                        *           When a RX FIFO Queue is started for the first time, its first RX descriptor must have the RC[4:0] set to 5’b00000.
                        *           This value must be incremented for every new RX descriptor up to 5’b11111 and then back to 5’b00000.
                        *           Even if a wrap occurs at to the end of the RX FIFO Queue, there must be continuous. Therefore, the First RX descriptor may have a RC[4:0] different from 5’b00000.
                        *           To always have RC[4:0] = 5’b00000 for the First RX descriptor, the RX FIFO Queue size must be a multiple of 32 RX descriptor size
                        */
    uint32_t IN   : 3; /*!<  9-11 - [Managed by SW] Instance Number: define the X_CAN instance number using that descriptor.
                        *           This bit field is relevant if several X_CAN are running concurrently. It provides a way to detect descriptor fetch issue between instances.
                        *           The value defined must be equal to the one defined in the MH_CFG.INST_NUM bit field register
                        */
    uint32_t FQN  : 4; //!< 12-15 - [Managed by SW] RX FIFO Queue number: define the RX FIFO Queue number allocated to this RX descriptor
    uint32_t CRC  : 9; /*!< 16-24 - [Managed by SW] CRC: this CRC is computed by the SW for the current RX descriptor. It must consider all elements assuming this bit field as set to 0.
                        *           Any CRC error is triggering an interrupt to the system. The CRC is not evaluated if the MH_STS.RX_DESC_CRC_EN bit is set to 0
                        */
    uint32_t      : 2; //!< 25-26
    uint32_t IRQ  : 1; /*!< 27    - [Managed by SW] Interrupt: when set to 1, an interrupt is triggered to the system when the descriptor execution is complete and a correctly received RX message was written to it.
                        *           This interrupt can provide to the SW, a synchronization point to monitor the RX FIFO Queue execution
                        */
    uint32_t NEXT : 1; /*!< 28    - [Managed by MH] Next: Set to 1 by the MH to indicate in the RX Header descriptor that more than one descriptor is used for the RX message.
                        *           This information is only mentioned in the Header Descriptor, the RX Trailing Descriptors are not modified.
                        *           This allows the SW to acknowledge only the RX Header Descriptor for any RX messages.
                        *           In Continuous Mode NEXT is always set to 0 as only one RX descriptor is used per RX message
                        */
    uint32_t      : 1; //!< 29
    uint32_t HD   : 1; /*!< 30    - [Managed by MH] Message header: when set to 1 the RX descriptor is defined as containing the header of the RX message.
                        *           Any other RX descriptor, if several descriptors are used for the same RX message, will contain only payload data.
                        *           In Continuous Mode HD is always set to 1 as only one RX descriptor is used per RX message
                        */
    uint32_t VALID: 1; /*!< 31    - [Managed by SW/MH] Valid: The SW must set this bit to 0 to define a RX descriptor is pointing to a valid data container.
                        *           As soon as the RX descriptor is executed the MH will set this bit to 1 to indicate to the SW valid data written to the S_MEM.
                        *           In case the RX descriptor is fetched with this bit set to 1 and interrupt RX_FQ_IRQ is triggered to the system for the RX FIFO Queue having this non valid descriptor.
                        *           The SW must clear this bit only when all the RX message data attached have been read
                        */
  };
} XCAN_RxDMAinfoCtrl1;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RxDMAinfoCtrl1, 4);

//! Rx messages status enumerator
typedef enum eXCAN_RxStatus
{
  XCAN_RX_STATUS_NONE                    = 0b0000, //!< None
  XCAN_RX_STATUS_MESSAGE_RECEIVE_SUCCESS = 0b0001, //!< Message received successfully
  XCAN_RX_STATUS_MESSAGE_NOT_FILTERED    = 0b0010, //!< Message received but not filtered
  XCAN_RX_STATUS_MESSAGE_ACK_WITH_ERROR  = 0b1111, //!< Message acknowledge data with parity error
} eXCAN_RxStatus;

#define XCAN_RxDMA1_STS_Pos                         0
#define XCAN_RxDMA1_STS_Mask                        (0xFu << XCAN_RxDMA1_STS_Pos)
#define XCAN_RxDMA1_STS_SET(value)                  (((uint32_t)(value) << XCAN_RxDMA1_STS_Pos) & XCAN_RxDMA1_STS_Mask) //!< Set RX Message Status
#define XCAN_RxDMA1_STS_GET(value)                  (eXCAN_RxStatus)(((uint32_t)(value) & XCAN_RxDMA1_STS_Mask) >> XCAN_RxDMA1_STS_Pos) //!< Get RX Message Status
#define XCAN_RxDMA1_RC_Pos                          4
#define XCAN_RxDMA1_RC_Mask                         (0x1Fu << XCAN_RxDMA1_RC_Pos)
#define XCAN_RxDMA1_RC_SET(value)                   (((uint32_t)(value) << XCAN_RxDMA1_RC_Pos) & XCAN_RxDMA1_RC_Mask) //!< Set Rolling Counter
#define XCAN_RxDMA1_RC_GET(value)                   (((uint32_t)(value) & XCAN_RxDMA1_RC_Mask) >> XCAN_RxDMA1_RC_Pos) //!< Get Rolling Counter
#define XCAN_RxDMA1_FQN_Pos                         12
#define XCAN_RxDMA1_FQN_Mask                        (0xFu << XCAN_RxDMA1_FQN_Pos)
#define XCAN_RxDMA1_FQN_SET(value)                  (((uint32_t)(value) << XCAN_RxDMA1_FQN_Pos) & XCAN_RxDMA1_FQN_Mask) //!< Set RX FIFO Queue number allocated to this RX descriptor
#define XCAN_RxDMA1_FQN_GET(value)                  (((uint32_t)(value) & XCAN_RxDMA1_FQN_Mask) >> XCAN_RxDMA1_FQN_Pos) //!< Get RX FIFO Queue number allocated to this RX descriptor
#define XCAN_RxDMA1_CRC_Pos                         16
#define XCAN_RxDMA1_CRC_Mask                        (0x1FFu << XCAN_RxDMA1_CRC_Pos)
#define XCAN_RxDMA1_CRC_SET(value)                  (((uint32_t)(value) << XCAN_RxDMA1_CRC_Pos) & XCAN_RxDMA1_CRC_Mask) //!< Set CRC
#define XCAN_RxDMA1_CRC_GET(value)                  (((uint32_t)(value) & XCAN_RxDMA1_CRC_Mask) >> XCAN_RxDMA1_CRC_Pos) //!< Get CRC
#define XCAN_RxDMA1_IRQ_WHEN_SENT                   (0x1u << 27) //!< Interrupt is triggered to the system when the descriptor execution is complete and a correctly received RX message was written to it
#define XCAN_RxDMA1_IRQ_NO_IRQ                      (0x0u << 27) //!< No interrupt
#define XCAN_RxDMA1_NEXT_HAVE_NEXT_DESCRIPTOR       (0x1u << 28) //!< Indicate in the RX Header descriptor that more than one descriptor is used for the RX message
#define XCAN_RxDMA1_NO_NEXT_DESCRIPTOR              (0x0u << 28) //!< No next descriptor
#define XCAN_RxDMA1_HD                              (0x1u << 30) //!< HD must be set to 1
#define XCAN_RxDMA1_VALID_SET_VALID_FOR_MH          (0x1u << 31) //!< Define a RX descriptor is valid for the MH
#define XCAN_RxDMA1_VALID_DATA_IS_AVAILABLE(value)  ( ((value) & XCAN_RxDMA1_VALID_SET_VALID_FOR_MH) > 0 ) //!< Is valid data written to the S_MEM?

//-----------------------------------------------------------------------------

//! Rx messages descriptor overview enumerator
typedef enum eXCAN_RxStatus
{
  XCAN_CAN_RXDESC_RIC1,  //!< CAN Rx DMA info control 1 (DMA Info Ctrl 1)
  XCAN_CAN_RXDESC_RX_AP, //!< RX Payload Data Address Pointer
  XCAN_CAN_RXDESC_TS0,   //!< TimeStamp [31:0]
  XCAN_CAN_RXDESC_TS1,   //!< TimeStamp [63:32]
  XCAN_CAN_RXDESC_COUNT, // KEEP LAST!
} eXCAN_RxDescriptor;

//! RX FIFO Queue Descriptor Overview (RX FIFO)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_CAN_RxMessage
{
  uint32_t Word[XCAN_CAN_RXDESC_COUNT];
  uint8_t Bytes[XCAN_CAN_RXDESC_COUNT * sizeof(uint32_t)];
  struct
  {
    XCAN_TxDMAinfoCtrl1 RIC1; //!< CAN Rx DMA info control 1 (RIC1)
    uint32_t RX_AP;           /*!< Normal Mode: the SW defines the address of the RX data container to write RX data.
                               *   Continuous Mode: The SW must set this bit field to 0 as default value.
                               *     The MH writes this field with the address pointer to find the RX message attached to the RX descriptor.
                               *     Only the RX Header Descriptor is having this bit field updated, with the RX message address in the data container.
                               *   This address must be 32bit aligned, the two LSB bits are assumed to be always 0
                               */
    uint32_t TS0;             //!< Timestamp 0: LSB of the 64bits timestamp of the successfully received RX message (only valid when HD bit is set to 1)
    uint32_t TS1;             //!< Timestamp 1: MSB of the 64bits timestamp of the successfully received RX message (only valid when HD bit is set to 1)
  };
} XCAN_CAN_RxMessage;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_CAN_RxMessage, 16);

//-----------------------------------------------------------------------------

#define MCAN_CAN_RX_MESSAGE_SIZE  ( sizeof(XCAN_CAN_RxMessage) )

//-----------------------------------------------------------------------------

//! CAN Receive Message Header 1 (R0)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RxMessageHeader0
{
  uint32_t R0;
  uint8_t Bytes[sizeof(uint32_t)];
  struct                 // Classical CAN RX Header definition
  {
    uint32_t ExtID : 18; //!<  0-17 - Extended ID
    uint32_t BaseID: 11; //!< 18-28 - Base ID
    uint32_t XTD   :  1; //!< 29    - Extended Identifier
    uint32_t XLF   :  1; //!< 30    - XL Format
    uint32_t FDF   :  1; //!< 31    - FD Format
  } CAN20;
  struct                 // CAN FD RX Header definition
  {
    uint32_t ExtID : 18; //!<  0-17 - Extended ID
    uint32_t BaseID: 11; //!< 18-28 - Base ID
    uint32_t XTD   :  1; //!< 29    - Extended Identifier
    uint32_t XLF   :  1; //!< 30    - XL Format
    uint32_t FDF   :  1; //!< 31    - FD Format
  } CANFD;
  struct                 // CAN XL RX Header definition
  {
    uint32_t SDT   :  8; //!<  0- 7 - SDU Type
    uint32_t VCID  :  8; //!<  8-15 - Virtual CAN Network ID
    uint32_t SEC   :  1; //!< 16    - Simple Extended Content
    uint32_t RRS   :  1; //!< 17    - Remote Request Substitution
    uint32_t PrioID: 11; //!< 18-28 - Priority identifier
    uint32_t       :  1; //!< 29
    uint32_t XLF   :  1; //!< 30    - XL Format
    uint32_t FDF   :  1; //!< 31    - FD Format
  } CANXL;
} XCAN_RxMessageHeader0;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RxMessageHeader0, 4);

#define MCAN_R0_ID_Pos           0
#define MCAN_R0_ID_Mask          (0x1FFFFFFFu << MCAN_R0_ID_Pos)
#define MCAN_R0_ID_GET(value)    (((uint32_t)(value) & MCAN_R0_ID_Mask) >> MCAN_R0_ID_Pos) //!< Get Message Identifier filter

#define MCAN_R0_EID_Pos          0
#define MCAN_R0_EID_Mask         (0x3FFFFu << MCAN_R0_EID_Pos)
#define MCAN_R0_EID_GET(value)   (((uint32_t)(value) & MCAN_R0_EID_Mask) >> MCAN_R0_EID_Pos) //!< Get Extended Identifier filter
#define MCAN_R0_SID_Pos          18
#define MCAN_R0_SID_Mask         (0x7FFu << MCAN_R0_SID_Pos)
#define MCAN_R0_SID_GET(value)   (((uint32_t)(value) & MCAN_R0_SID_Mask) >> MCAN_R0_SID_Pos) //!< Get Standard Identifier filter

#define XCAN_T0_XTD_EXTENDED_ID  (0x1u << 29) //!< 29-bit extended identifier
#define XCAN_T0_XTD_STANDARD_ID  (0x0u << 29) //!< 11-bit standard identifier
#define XCAN_T0_XTD              (0x1u << 29) //!< Extended Identifier
#define XCAN_T0_XLF              (0x1u << 30) //!< XL Format
#define XCAN_T0_FDF              (0x1u << 31) //!< FD Format
#define XCAN_T0_IS_CAN20(value)  ( ((value) & (XCAN_T0_XLF | XCAN_T0_FDF)) == 0) //!< Is a classical CAN2.0 frame
#define XCAN_T0_IS_CANFD(value)  ( ((value) & (XCAN_T0_XLF | XCAN_T0_FDF)) == XCAN_T0_FDF) //!< Is a CAN-FD frame
#define XCAN_T0_IS_CANXL(value)  ( ((value) & (XCAN_T0_XLF | XCAN_T0_FDF | XCAN_T0_XTD)) == (XCAN_T0_XLF | XCAN_T0_FDF)) //!< Is a CAN-XL frame

//-----------------------------------------------------------------------------

//! CAN Receive Message Header 1 (R1)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RxMessageHeader1
{
  uint32_t T1;
  uint8_t Bytes[sizeof(uint32_t)];
  struct                 // Classical CAN RX Header definition
  {
    uint32_t FIDX: 8; //!<  0- 7 - Filter index: provide the information of the filter index which has been triggered
    uint32_t FM  : 1; //!<  8    - Filter Match: When set to 1 one of the filter elements (defined by FIDX[7:0]) has detected a match
    uint32_t BLK : 1; //!<  9    - Black List: When set to 1, the RX message filtered belongs to a blacklist
    uint32_t FAB : 1; //!< 10    - Filter Aborted: when set to 1, the RX filtering process was ending before completing with no match
    uint32_t     : 5; //!< 11-15
    uint32_t DLC : 4; //!< 16-19 - Data Length Code
    uint32_t     : 6; //!< 20-25
    uint32_t RTR : 1; //!< 26    - Remote Transmission Request
    uint32_t     : 5; //!< 27-31
  } CAN20;
  struct                 // CAN FD RX Header definition
  {
    uint32_t FIDX: 8; //!<  0- 7 - Filter index: provide the information of the filter index which has been triggered
    uint32_t FM  : 1; //!<  8    - Filter Match: When set to 1 one of the filter elements (defined by FIDX[7:0]) has detected a match
    uint32_t BLK : 1; //!<  9    - Black List: When set to 1, the RX message filtered belongs to a blacklist
    uint32_t FAB : 1; //!< 10    - Filter Aborted: when set to 1, the RX filtering process was ending before completing with no match
    uint32_t     : 5; //!< 11-15
    uint32_t DLC : 4; //!< 16-19 - Data Length Code
    uint32_t ESI : 1; //!< 20    - Error State Indicator
    uint32_t     : 4; //!< 21-24
    uint32_t BRS : 1; //!< 25    - Bit Rate Switch
    uint32_t     : 6; //!< 26-31
  } CANFD;
  struct                 // CAN XL RX Header definition
  {
    uint32_t FIDX :  8; //!<  0- 7 - Filter index: provide the information of the filter index which has been triggered
    uint32_t FM   :  1; //!<  8    - Filter Match: When set to 1 one of the filter elements (defined by FIDX[7:0]) has detected a match
    uint32_t BLK  :  1; //!<  9    - Black List: When set to 1, the RX message filtered belongs to a blacklist
    uint32_t FAB  :  1; //!< 10    - Filter Aborted: when set to 1, the RX filtering process was ending before completing with no match
    uint32_t      :  5; //!< 11-15
    uint32_t DLCXL: 11; //!< 16-26 - Data Length Code with CAN XL encoding
    uint32_t      :  5; //!< 27-31
  } CANXL;
} XCAN_RxMessageHeader1;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RxMessageHeader1, 4);

#define MCAN_R1_FIDX_Pos              0
#define MCAN_R1_FIDX_Mask             (0xFFu << MCAN_R1_FIDX_Pos)
#define MCAN_R1_FIDX_SET(value)       (((uint32_t)(value) & MCAN_R1_FIDX_Mask) >> MCAN_R1_FIDX_Pos) //!< Get Filter index
#define XCAN_R1_FM                    (0x1u << 08) //!< Filter Match
#define XCAN_R1_BLK                   (0x1u << 09) //!< Black List
#define XCAN_R1_FAB                   (0x1u << 10) //!< Filter Aborted
#define MCAN_R1_CANXL_DLC_Pos         16
#define MCAN_R1_CANXL_DLC_Mask        (0x7FFu << MCAN_R1_CANXL_DLC_Pos)
#define MCAN_R1_CANXL_DLC_SET(value)  (((uint32_t)(value) & MCAN_R1_CANXL_DLC_Mask) >> MCAN_R1_CANXL_DLC_Pos) //!< Get Data Length Code with CAN XL encoding
#define MCAN_R1_DLC_Pos               16
#define MCAN_R1_DLC_Mask              (0xFu << MCAN_R1_DLC_Pos)
#define MCAN_R1_DLC_SET(value)        (((uint32_t)(value) & MCAN_R1_DLC_Mask) >> MCAN_R1_DLC_Pos) //!< Get Data Length Code for CAN2.0 and CAN-FD
#define XCAN_R1_ESI                   (0x1u << 20) //!< Error State Indicator
#define XCAN_R1_BRS                   (0x1u << 25) //!< Bit Rate Switch
#define XCAN_R1_RTR                   (0x1u << 26) //!< Remote Transmission Request

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Core Register list
//********************************************************************************************************************

#define RegXCAN_COUNT  ( 704 )
#define RegXCAN_SIZE   ( RegXCAN_COUNT * sizeof(uint32_t) )

//! XCAN Core registers list
typedef enum
{
  //--- XCAN Message Handler Registers ---
  RegXCAN_MsgHandlerRegisters    = 0x000u, //!< (Offset: 0x000) Message Handler Registers
  // General Registers
  RegXCAN_GeneralRegisters       = 0x000u, //!< (Offset: 0x000) General Registers
  RegXCAN_VERSION                = 0x000u, //!< (Offset: 0x000) Release Identification Register
  RegXCAN_MH_CTRL                = 0x004u, //!< (Offset: 0x004) Message Handler Control register
  RegXCAN_MH_CFG                 = 0x008u, //!< (Offset: 0x008) Message Handler Configuration register
  RegXCAN_MH_STS                 = 0x00Cu, //!< (Offset: 0x00C) Message Handler Status register
  RegXCAN_MH_SFTY_CFG            = 0x010u, //!< (Offset: 0x010) Message Handler Safety Configuration register
  RegXCAN_MH_SFTY_CTRL           = 0x014u, //!< (Offset: 0x014) Message Handler Safety Control register
  RegXCAN_RX_FILTER_MEM_ADD      = 0x018u, //!< (Offset: 0x018) RX Filter Base Address register
  RegXCAN_TX_DESC_MEM_ADD        = 0x01Cu, //!< (Offset: 0x01C) TX Descriptor Base Address register
  RegXCAN_AXI_ADD_EXT            = 0x020u, //!< (Offset: 0x020) AXI address extension register
  RegXCAN_AXI_PARAMS             = 0x024u, //!< (Offset: 0x024) AXI parameter register
  RegXCAN_MH_LOCK                = 0x028u, //!< (Offset: 0x028) Message Handler Lock register
                                           //   (Offset: 0x02C..0x100) Reserved
  // TX FIFO Queues Registers
  RegXCAN_TxFIFOQueuesRegisters  = 0x100u, //!< (Offset: 0x100) TX FIFO Queues Registers
  RegXCAN_TX_DESC_ADD_PT         = 0x100u, //!< (Offset: 0x100) TX descriptor current address pointer register
  RegXCAN_TX_STATISTICS          = 0x104u, //!< (Offset: 0x104) Unsuccessful and Successful message counter registers
  RegXCAN_TX_FQ_STS0             = 0x108u, //!< (Offset: 0x108) TX FIFO Queue Status register
  RegXCAN_TX_FQ_STS1             = 0x10Cu, //!< (Offset: 0x10C) TX FIFO Queue Status register
  RegXCAN_TX_FQ_CTRL0            = 0x110u, //!< (Offset: 0x110) TX FIFO Queue Control register 0
  RegXCAN_TX_FQ_CTRL1            = 0x114u, //!< (Offset: 0x114) TX FIFO Queue Control register 1
  RegXCAN_TX_FQ_CTRL2            = 0x118u, //!< (Offset: 0x118) TX FIFO Queue Control register 2
                                           //   (Offset: 0x11C) Reserved
  RegXCAN_TX_FQ_ADD_PT0          = 0x120u, //!< (Offset: 0x120) TX FIFO Queue 0 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD0       = 0x124u, //!< (Offset: 0x124) TX FIFO Queue 0 Start Address register
  RegXCAN_TX_FQ_SIZE0            = 0x128u, //!< (Offset: 0x128) TX FIFO Queue 0 Size register
                                           //   (Offset: 0x12C) Reserved
  RegXCAN_TX_FQ_ADD_PT1          = 0x130u, //!< (Offset: 0x130) TX FIFO Queue 1 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD1       = 0x134u, //!< (Offset: 0x134) TX FIFO Queue 1 Start Address register
  RegXCAN_TX_FQ_SIZE1            = 0x138u, //!< (Offset: 0x138) TX FIFO Queue 1 Size register
                                           //   (Offset: 0x13C) Reserved
  RegXCAN_TX_FQ_ADD_PT2          = 0x140u, //!< (Offset: 0x140) TX FIFO Queue 2 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD2       = 0x144u, //!< (Offset: 0x144) TX FIFO Queue 2 Start Address register
  RegXCAN_TX_FQ_SIZE2            = 0x148u, //!< (Offset: 0x148) TX FIFO Queue 2 Size register
                                           //   (Offset: 0x14C) Reserved
  RegXCAN_TX_FQ_ADD_PT3          = 0x150u, //!< (Offset: 0x150) TX FIFO Queue 3 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD3       = 0x154u, //!< (Offset: 0x154) TX FIFO Queue 3 Start Address register
  RegXCAN_TX_FQ_SIZE3            = 0x158u, //!< (Offset: 0x158) TX FIFO Queue 3 Size register
                                           //   (Offset: 0x15C) Reserved
  RegXCAN_TX_FQ_ADD_PT4          = 0x160u, //!< (Offset: 0x160) TX FIFO Queue 4 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD4       = 0x164u, //!< (Offset: 0x164) TX FIFO Queue 4 Start Address register
  RegXCAN_TX_FQ_SIZE4            = 0x168u, //!< (Offset: 0x168) TX FIFO Queue 4 Size register
                                           //   (Offset: 0x16C) Reserved
  RegXCAN_TX_FQ_ADD_PT5          = 0x170u, //!< (Offset: 0x170) TX FIFO Queue 5 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD5       = 0x174u, //!< (Offset: 0x174) TX FIFO Queue 5 Start Address register
  RegXCAN_TX_FQ_SIZE5            = 0x178u, //!< (Offset: 0x178) TX FIFO Queue 5 Size register
                                           //   (Offset: 0x17C) Reserved
  RegXCAN_TX_FQ_ADD_PT6          = 0x180u, //!< (Offset: 0x180) TX FIFO Queue 6 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD6       = 0x184u, //!< (Offset: 0x184) TX FIFO Queue 6 Start Address register
  RegXCAN_TX_FQ_SIZE6            = 0x188u, //!< (Offset: 0x188) TX FIFO Queue 6 Size register
                                           //   (Offset: 0x18C) Reserved
  RegXCAN_TX_FQ_ADD_PT7          = 0x190u, //!< (Offset: 0x190) TX FIFO Queue 7 Current Address Pointer register
  RegXCAN_TX_FQ_START_ADD7       = 0x194u, //!< (Offset: 0x194) TX FIFO Queue 7 Start Address register
  RegXCAN_TX_FQ_SIZE7            = 0x198u, //!< (Offset: 0x198) TX FIFO Queue 7 Size register
                                           //   (Offset: 0x19C..0x2FC) Reserved
  // TX Priority Queues Registers
  RegXCAN_TxPrioQueuesRegisters  = 0x300u, //!< (Offset: 0x300) TX Priority Queues Registers
  RegXCAN_TX_PQ_STS0             = 0x300u, //!< (Offset: 0x300) TX Priority Queue Status register 0
  RegXCAN_TX_PQ_STS1             = 0x304u, //!< (Offset: 0x304) TX Priority Queue Status register 1
                                           //   (Offset: 0x308) Reserved
  RegXCAN_TX_PQ_CTRL0            = 0x30Cu, //!< (Offset: 0x30C) TX Priority Queue Control register 0
  RegXCAN_TX_PQ_CTRL1            = 0x310u, //!< (Offset: 0x310) TX Priority Queue Control register 1
  RegXCAN_TX_PQ_CTRL2            = 0x314u, //!< (Offset: 0x314) TX Priority Queue Control register 2
  RegXCAN_TX_PQ_START_ADD        = 0x318u, //!< (Offset: 0x318) TX Priority Queue Start Address
                                           //   (Offset: 0x32C..0x3FC) Reserved
  // RX FIFO Queues Registers
  RegXCAN_RxFIFOQueuesRegisters  = 0x400u, //!< (Offset: 0x400) RX FIFO Queues Registers
  RegXCAN_RX_DESC_ADD_PT         = 0x400u, //!< (Offset: 0x400) RX descriptor Current Address Pointer
  RegXCAN_RX_STATISTICS          = 0x404u, //!< (Offset: 0x404) Unsuccessful and Successful Message Received Counter
  RegXCAN_RX_FQ_STS0             = 0x408u, //!< (Offset: 0x408) RX FIFO Queue Status register 0
  RegXCAN_RX_FQ_STS1             = 0x40Cu, //!< (Offset: 0x40C) RX FIFO Queue Status register 1
  RegXCAN_RX_FQ_STS2             = 0x410u, //!< (Offset: 0x410) RX FIFO Queue Status register 2
  RegXCAN_RX_FQ_CTRL0            = 0x414u, //!< (Offset: 0x414) RX FIFO Queue Control register 0
  RegXCAN_RX_FQ_CTRL1            = 0x418u, //!< (Offset: 0x418) RX FIFO Queue Control register 1
  RegXCAN_RX_FQ_CTRL2            = 0x41Cu, //!< (Offset: 0x41C) RX FIFO Queue Control register 2
  RegXCAN_RX_FQ_ADD_PT0          = 0x420u, //!< (Offset: 0x420) RX FIFO Queue 0 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD0       = 0x424u, //!< (Offset: 0x424) RX FIFO Queue 0 link list Start Address
  RegXCAN_RX_FQ_SIZE0            = 0x428u, //!< (Offset: 0x428) RX FIFO Queue 0 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD0    = 0x42Cu, //!< (Offset: 0x42C) RX FIFO Queue 0 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT0       = 0x430u, //!< (Offset: 0x430) RX FIFO Queue 0 Read Address Pointer
                                           //   (Offset: 0x434) Reserved
  RegXCAN_RX_FQ_ADD_PT1          = 0x438u, //!< (Offset: 0x438) RX FIFO Queue 1 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD1       = 0x43Cu, //!< (Offset: 0x43C) RX FIFO Queue 1 link list Start Address
  RegXCAN_RX_FQ_SIZE1            = 0x440u, //!< (Offset: 0x440) RX FIFO Queue 1 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD1    = 0x444u, //!< (Offset: 0x444) RX FIFO Queue 1 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT1       = 0x448u, //!< (Offset: 0x448) RX FIFO Queue 1 Read Address Pointer
                                           //   (Offset: 0x44C) Reserved
  RegXCAN_RX_FQ_ADD_PT2          = 0x450u, //!< (Offset: 0x450) RX FIFO Queue 2 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD2       = 0x454u, //!< (Offset: 0x454) RX FIFO Queue 2 link list Start Address
  RegXCAN_RX_FQ_SIZE2            = 0x458u, //!< (Offset: 0x458) RX FIFO Queue 2 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD2    = 0x45Cu, //!< (Offset: 0x45C) RX FIFO Queue 2 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT2       = 0x460u, //!< (Offset: 0x460) RX FIFO Queue 2 Read Address Pointer
                                           //   (Offset: 0x464) Reserved
  RegXCAN_RX_FQ_ADD_PT3          = 0x468u, //!< (Offset: 0x468) RX FIFO Queue 3 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD3       = 0x46Cu, //!< (Offset: 0x46C) RX FIFO Queue 3 link list Start Address
  RegXCAN_RX_FQ_SIZE3            = 0x470u, //!< (Offset: 0x470) RX FIFO Queue 3 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD3    = 0x474u, //!< (Offset: 0x474) RX FIFO Queue 3 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT3       = 0x478u, //!< (Offset: 0x478) RX FIFO Queue 3 Read Address Pointer
                                           //   (Offset: 0x47C) Reserved
  RegXCAN_RX_FQ_ADD_PT4          = 0x480u, //!< (Offset: 0x480) RX FIFO Queue 4 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD4       = 0x484u, //!< (Offset: 0x484) RX FIFO Queue 4 link list Start Address
  RegXCAN_RX_FQ_SIZE4            = 0x488u, //!< (Offset: 0x488) RX FIFO Queue 4 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD4    = 0x48Cu, //!< (Offset: 0x48C) RX FIFO Queue 4 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT4       = 0x490u, //!< (Offset: 0x490) RX FIFO Queue 4 Read Address Pointer
                                           //   (Offset: 0x494) Reserved
  RegXCAN_RX_FQ_ADD_PT5          = 0x498u, //!< (Offset: 0x498) RX FIFO Queue 5 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD5       = 0x49Cu, //!< (Offset: 0x49C) RX FIFO Queue 5 link list Start Address
  RegXCAN_RX_FQ_SIZE5            = 0x4A0u, //!< (Offset: 0x4A0) RX FIFO Queue 5 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD5    = 0x4A4u, //!< (Offset: 0x4A4) RX FIFO Queue 5 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT5       = 0x4A8u, //!< (Offset: 0x4A8) RX FIFO Queue 5 Read Address Pointer
                                           //   (Offset: 0x4AC) Reserved
  RegXCAN_RX_FQ_ADD_PT6          = 0x4B0u, //!< (Offset: 0x4B0) RX FIFO Queue 6 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD6       = 0x4B4u, //!< (Offset: 0x4B4) RX FIFO Queue 6 link list Start Address
  RegXCAN_RX_FQ_SIZE6            = 0x4B8u, //!< (Offset: 0x4B8) RX FIFO Queue 6 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD6    = 0x4BCu, //!< (Offset: 0x4BC) RX FIFO Queue 6 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT6       = 0x4C0u, //!< (Offset: 0x4C0) RX FIFO Queue 6 Read Address Pointer
                                           //   (Offset: 0x4C4) Reserved
  RegXCAN_RX_FQ_ADD_PT7          = 0x4C8u, //!< (Offset: 0x4C8) RX FIFO Queue 7 Current Address Pointer
  RegXCAN_RX_FQ_START_ADD7       = 0x4CCu, //!< (Offset: 0x4CC) RX FIFO Queue 7 link list Start Address
  RegXCAN_RX_FQ_SIZE7            = 0x4D0u, //!< (Offset: 0x4D0) RX FIFO Queue 7 link list and data container Size
  RegXCAN_RX_FQ_DC_START_ADD7    = 0x4D4u, //!< (Offset: 0x4D4) RX FIFO Queue 7 Data Container Start Address
  RegXCAN_RX_FQ_RD_ADD_PT7       = 0x4D8u, //!< (Offset: 0x4D8) RX FIFO Queue 7 Read Address Pointer
                                           //   (Offset: 0x4DC..0x5FC) Reserved
  // TX/RX Filter Registers
  RegXCAN_TxRxFiltersRegisters   = 0x600u, //!< (Offset: 0x600) TX/RX Filter Registers
  RegXCAN_TX_FILTER_CTRL0        = 0x600u, //!< (Offset: 0x600) TX Filter Control register 0
  RegXCAN_TX_FILTER_CTRL1        = 0x604u, //!< (Offset: 0x604) TX Filter Control register 1
  RegXCAN_TX_FILTER_REFVAL0      = 0x608u, //!< (Offset: 0x608) TX Filter Reference Value register 0
  RegXCAN_TX_FILTER_REFVAL1      = 0x60Cu, //!< (Offset: 0x60C) TX Filter Reference Value register 1
  RegXCAN_TX_FILTER_REFVAL2      = 0x610u, //!< (Offset: 0x610) TX Filter Reference Value register 2
  RegXCAN_TX_FILTER_REFVAL3      = 0x614u, //!< (Offset: 0x614) TX Filter Reference Value register 3
                                           //   (Offset: 0x61C..0x67C) Reserved
  // RX Filter Control Registers
  RegXCAN_RxFiltCtrlRegisters    = 0x680u, //!< (Offset: 0x680) RX Filter Control Registers
  RegXCAN_RX_FILTER_CTRL         = 0x680u, //!< (Offset: 0x680) RX Filter Control register
                                           //   (Offset: 0x684..0x6FC) Reserved
  // Interrupt Registers
  RegXCAN_InterruptRegisters     = 0x700u, //!< (Offset: 0x700) Interrupt Registers
  RegXCAN_TX_FQ_INT_STS          = 0x700u, //!< (Offset: 0x700) TX FIFO Queue Interrupt Status register
  RegXCAN_RX_FQ_INT_STS          = 0x704u, //!< (Offset: 0x704) RX FIFO Queue Interrupt Status register
  RegXCAN_TX_PQ_INT_STS0         = 0x708u, //!< (Offset: 0x708) TX Priority Queue Interrupt Status register 0
  RegXCAN_TX_PQ_INT_STS1         = 0x70Cu, //!< (Offset: 0x70C) TX Priority Queue Interrupt Status register 1
  RegXCAN_STATS_INT_STS          = 0x710u, //!< (Offset: 0x710) Statistics Interrupt Status register
  RegXCAN_ERR_INT_STS            = 0x714u, //!< (Offset: 0x714) Error Interrupt Status register
  RegXCAN_SFTY_INT_STS           = 0x718u, //!< (Offset: 0x718) Safety Interrupt Status register
  RegXCAN_AXI_ERR_INFO           = 0x71Cu, //!< (Offset: 0x71C) AXI Error Information
  RegXCAN_DESC_ERR_INFO0         = 0x720u, //!< (Offset: 0x720) Descriptor Error Information 0
  RegXCAN_DESC_ERR_INFO1         = 0x724u, //!< (Offset: 0x724) Descriptor Error Information 1
  RegXCAN_TX_FILTER_ERR_INFO     = 0x728u, //!< (Offset: 0x728) TX Filter Error Information
                                           //   (Offset: 0x72C..0x7FC) Reserved
  // Misc Registers
  RegXCAN_TX_FQ_INT_STS          = 0x800u, //!< (Offset: 0x800) Integration/Debug control and status Registers
  RegXCAN_DEBUG_TEST_CTRL        = 0x800u, //!< (Offset: 0x800) Debug Control register
  RegXCAN_INT_TEST0              = 0x804u, //!< (Offset: 0x804) Interrupt Test register 0
  RegXCAN_INT_TEST1              = 0x808u, //!< (Offset: 0x808) Interrupt Test register 1
                                           //   (Offset: 0x80C) Reserved
  RegXCAN_TX_SCAN_FC             = 0x810u, //!< (Offset: 0x810) TX-SCAN first candidates register
  RegXCAN_TX_SCAN_BC             = 0x814u, //!< (Offset: 0x814) TX-SCAN best candidates register
  RegXCAN_TX_FQ_DESC_VALID       = 0x818u, //!< (Offset: 0x818) Valid TX FIFO Queue descriptors in local memory
  RegXCAN_TX_PQ_DESC_VALID       = 0x81Cu, //!< (Offset: 0x81C) Valid TX Priority Queue descriptors in local memory
                                           //   (Offset: 0x820..0x87C) Reserved
  RegXCAN_CRC_CTRL               = 0x880u, //!< (Offset: 0x880) CRC Control register
  RegXCAN_CRC_REG                = 0x884u, //!< (Offset: 0x884) CRC register
                                           //   (Offset: 0x02C..0x100) Reserved

  //--- XCAN Protocol Controller Registers ---
  RegXCAN_ProtocolCtrlRegisters  = 0x900u, //!< (Offset: 0x900) Protocol Controller Registers
  // Status information of the PRT
  RegXCAN_ENDN                   = 0x900u, //!< (Offset: 0x900) Endianness Test Register
  RegXCAN_PREL                   = 0x904u, //!< (Offset: 0x904) PRT Release Identification Register
  RegXCAN_STAT                   = 0x908u, //!< (Offset: 0x908) PRT Status Register
                                           //   (Offset: 0x90C..0x91C) Reserved
  // Event information of the PRT
  RegXCAN_EVNT                   = 0x920u, //!< (Offset: 0x920) Event Status Flags Register
                                           //   (Offset: 0x924..0x93C) Reserved
  // Control of the PRT during runtime
  RegXCAN_LOCK                   = 0x940u, //!< (Offset: 0x940) Unlock Sequence Register
  RegXCAN_CTRL                   = 0x944u, //!< (Offset: 0x944) Control Register
  RegXCAN_FIMC                   = 0x948u, //!< (Offset: 0x948) Fault Injection Module Control Register
  RegXCAN_TEST                   = 0x94Cu, //!< (Offset: 0x94C) Hardware Test functions register
                                           //   (Offset: 0x950..0x95C) Reserved
  // Configuration of the PRT before runtime
  RegXCAN_MODE                   = 0x960u, //!< (Offset: 0x960) Operating Mode Register
  RegXCAN_NBTP                   = 0x964u, //!< (Offset: 0x964) Arbitration Phase Nominal Bit Timing Register
  RegXCAN_DBTP                   = 0x968u, //!< (Offset: 0x968) CAN FD Data Phase Bit Timing Register
  RegXCAN_XBTP                   = 0x96Cu, //!< (Offset: 0x96C) CAN XL Data Phase Bit Timing Register
  RegXCAN_PCFG                   = 0x970u, //!< (Offset: 0x970) PWME Configuration Register
                                           //   (Offset: 0x974..0x97C) Reserved

  //--- XCAN Interrupt Controller Registers ---
  RegXCAN_InterruptCtrlRegisters = 0xA00u, //!< (Offset: 0xA00) Interrupt Controller Registers
  // MH and PRT capture event registers (Base Address: 0x00, Address Range: 0x10)
  RegXCAN_FUNC_RAW               = 0xA00u, //!< (Offset: 0xA00) Functional raw event status register
  RegXCAN_ERR_RAW                = 0xA04u, //!< (Offset: 0xA04) Error raw event status register
  RegXCAN_SAFETY_RAW             = 0xA08u, //!< (Offset: 0xA08) Safety raw event status register
                                           //   (Offset: 0xA0C) Reserved
  // IRC control register (Base Address: 0x10, Address Range: 0x20)
  RegXCAN_FUNC_CLR               = 0xA10u, //!< (Offset: 0xA10) Functional raw event clear register
  RegXCAN_ERR_CLR                = 0xA14u, //!< (Offset: 0xA14) Error raw event clear register
  RegXCAN_SAFETY_CLR             = 0xA18u, //!< (Offset: 0xA18) Safety raw event clear register
                                           //   (Offset: 0xA1C) Reserved
  RegXCAN_FUNC_ENA               = 0xA20u, //!< (Offset: 0xA20) Functional raw event enable register
  RegXCAN_ERR_ENA                = 0xA24u, //!< (Offset: 0xA24) Error raw event enable register
  RegXCAN_SAFETY_ENA             = 0xA28u, //!< (Offset: 0xA28) Safety raw event enable register
                                           //   (Offset: 0xA2C) Reserved
  // Hardware configuration of the IRC (Base Address: 0x30, Address Range: 0x10)
  RegXCAN_CAPTURING_MODE         = 0xA30u, //!< (Offset: 0xA30) IRC configuration register
                                           //   (Offset: 0xA34..0xA3C) Reserved
  // Auxiliary (Base Address: 0x40, Address Range: 0xB0)
  RegXCAN_HDP                    = 0xA40u, //!< (Offset: 0xA40) Hardware Debug Port control register
                                           //   (Offset: 0xA44..0xAFC) Reserved
} eXCAN_Registers;





//********************************************************************************************************************
// XCAN Message Handler Registers
//********************************************************************************************************************

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Protocol Controller Registers
//********************************************************************************************************************

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Interrupt Controller Registers
//********************************************************************************************************************

/*! XCAN Functional raw event clear register (Write, Offset: 0xA10, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register FUNC_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_FC_Register
{
  uint32_t FUNC_CLR;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_TX_FQ0_IRQ   : 1; /*!<  0    - MH interrupt of the TX FIFO Queue 0: Clear bit by writing 1
                                   *           This interrupt is triggered when an invalid TX descriptor is fetched from this TX FIFO Queue, a TX message from that FIFO Queue is sent (if set in TX descriptor),
                                   *           or a TX message of that TX FIFO Queue is skipped, see description of TX_FQ_IRQ[7:0] in MH section
                                   */
    uint32_t MH_TX_FQ1_IRQ   : 1; //!<  1    - MH interrupt of the TX FIFO Queue 1: Clear bit by writing 1
    uint32_t MH_TX_FQ2_IRQ   : 1; //!<  2    - MH interrupt of the TX FIFO Queue 2: Clear bit by writing 1
    uint32_t MH_TX_FQ3_IRQ   : 1; //!<  3    - MH interrupt of the TX FIFO Queue 3: Clear bit by writing 1
    uint32_t MH_TX_FQ4_IRQ   : 1; //!<  4    - MH interrupt of the TX FIFO Queue 4: Clear bit by writing 1
    uint32_t MH_TX_FQ5_IRQ   : 1; //!<  5    - MH interrupt of the TX FIFO Queue 5: Clear bit by writing 1
    uint32_t MH_TX_FQ6_IRQ   : 1; //!<  6    - MH interrupt of the TX FIFO Queue 6: Clear bit by writing 1
    uint32_t MH_TX_FQ7_IRQ   : 1; //!<  7    - MH interrupt of the TX FIFO Queue 7: Clear bit by writing 1
    uint32_t MH_RX_FQ0_IRQ   : 1; /*!<  8    - MH interrupt of the RX FIFO Queue 0: Clear bit by writing 1
                                   *           This interrupt is triggered when an invalid RX descriptor is fetched from this RX FIFO Queue,
                                   *           or an RX message is received (if set in RX descriptor) in this RX FIFO Queue, see description of RX_FQ_IRQ[7:0] in MH section
                                   */
    uint32_t MH_RX_FQ1_IRQ   : 1; //!<  9    - MH interrupt of the RX FIFO Queue 1: Clear bit by writing 1
    uint32_t MH_RX_FQ2_IRQ   : 1; //!< 10    - MH interrupt of the RX FIFO Queue 2: Clear bit by writing 1
    uint32_t MH_RX_FQ3_IRQ   : 1; //!< 11    - MH interrupt of the RX FIFO Queue 3: Clear bit by writing 1
    uint32_t MH_RX_FQ4_IRQ   : 1; //!< 12    - MH interrupt of the RX FIFO Queue 4: Clear bit by writing 1
    uint32_t MH_RX_FQ5_IRQ   : 1; //!< 13    - MH interrupt of the RX FIFO Queue 5: Clear bit by writing 1
    uint32_t MH_RX_FQ6_IRQ   : 1; //!< 14    - MH interrupt of the RX FIFO Queue 6: Clear bit by writing 1
    uint32_t MH_RX_FQ7_IRQ   : 1; //!< 15    - MH interrupt of the RX FIFO Queue 7: Clear bit by writing 1
    uint32_t MH_TX_PQ_IRQ    : 1; /*!< 16    - Interrupt of TX Priority Queue: Clear bit by writing 1
                                   *           Any TX message sent from the TX Priority Queue can be configured to trigger this interrupt.
                                   *           The SW would then need to look at the MH register TX_PQ_INT_STS to identify which slot has generated the interrupt and for which reason
                                   */
    uint32_t MH_STOP_IRQ     : 1; //!< 17    - The interrupt is triggered when the PRT is stopped. The MH finishes its task and switches to idle mode: Clear bit by writing 1
    uint32_t MH_RX_FILTER_IRQ: 1; //!< 18    - In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful: Clear bit by writing 1
    uint32_t MH_TX_FILTER_IRQ: 1; //!< 19    - The interrupt is triggered when the TX filter is enabled, and a TX message is rejected: Clear bit by writing 1
    uint32_t MH_TX_ABORT_IRQ : 1; //!< 20    - This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT: Clear bit by writing 1
    uint32_t MH_RX_ABORT_IRQ : 1; //!< 21    - This interrupt line is triggered when the MH needs to abort a RX message being received from PRT: Clear bit by writing 1
    uint32_t MH_STATS_IRQ    : 1; //!< 22    - One of the RX/TX counters have reached the threshold: Clear bit by writing 1
    uint32_t                 : 1; //!< 23
    uint32_t PRT_E_ACTIVE    : 1; //!< 24    - PRT switched from Error-Passive to Error-Active state: Clear bit by writing 1
    uint32_t PRT_BUS_ON      : 1; //!< 25    - PRT started CAN communication, after start or end of BusOff: Clear bit by writing 1
    uint32_t PRT_TX_EVT      : 1; //!< 26    - PRT transmitted a valid CAN message: Clear bit by writing 1
    uint32_t PRT_RX_EVT      : 1; //!< 27    - PRT received a valid CAN message: Clear bit by writing 1
    uint32_t                 : 4; //!< 28-31
  } Bits;
} XCAN_IC_FC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_FC_Register, 4);

#define XCAN_IC_FC_MH_TX_FQ0_IRQ_CLEAR     (1u <<  0) //!< Enable MH interrupt of the TX FIFO Queue 0 interrupt
#define XCAN_IC_FC_MH_TX_FQ1_IRQ_CLEAR     (1u <<  1) //!< Enable MH interrupt of the TX FIFO Queue 1 interrupt
#define XCAN_IC_FC_MH_TX_FQ2_IRQ_CLEAR     (1u <<  2) //!< Enable MH interrupt of the TX FIFO Queue 2 interrupt
#define XCAN_IC_FC_MH_TX_FQ3_IRQ_CLEAR     (1u <<  3) //!< Enable MH interrupt of the TX FIFO Queue 3 interrupt
#define XCAN_IC_FC_MH_TX_FQ4_IRQ_CLEAR     (1u <<  4) //!< Enable MH interrupt of the TX FIFO Queue 4 interrupt
#define XCAN_IC_FC_MH_TX_FQ5_IRQ_CLEAR     (1u <<  5) //!< Enable MH interrupt of the TX FIFO Queue 5 interrupt
#define XCAN_IC_FC_MH_TX_FQ6_IRQ_CLEAR     (1u <<  6) //!< Enable MH interrupt of the TX FIFO Queue 6 interrupt
#define XCAN_IC_FC_MH_TX_FQ7_IRQ_CLEAR     (1u <<  7) //!< Enable MH interrupt of the TX FIFO Queue 7 interrupt
#define XCAN_IC_FC_MH_RX_FQ0_IRQ_CLEAR     (1u <<  8) //!< Enable MH interrupt of the RX FIFO Queue 0 interrupt
#define XCAN_IC_FC_MH_RX_FQ1_IRQ_CLEAR     (1u <<  9) //!< Enable MH interrupt of the RX FIFO Queue 1 interrupt
#define XCAN_IC_FC_MH_RX_FQ2_IRQ_CLEAR     (1u << 10) //!< Enable MH interrupt of the RX FIFO Queue 2 interrupt
#define XCAN_IC_FC_MH_RX_FQ3_IRQ_CLEAR     (1u << 11) //!< Enable MH interrupt of the RX FIFO Queue 3 interrupt
#define XCAN_IC_FC_MH_RX_FQ4_IRQ_CLEAR     (1u << 12) //!< Enable MH interrupt of the RX FIFO Queue 4 interrupt
#define XCAN_IC_FC_MH_RX_FQ5_IRQ_CLEAR     (1u << 13) //!< Enable MH interrupt of the RX FIFO Queue 5 interrupt
#define XCAN_IC_FC_MH_RX_FQ6_IRQ_CLEAR     (1u << 14) //!< Enable MH interrupt of the RX FIFO Queue 6 interrupt
#define XCAN_IC_FC_MH_RX_FQ7_IRQ_CLEAR     (1u << 15) //!< Enable MH interrupt of the RX FIFO Queue 7 interrupt
#define XCAN_IC_FC_MH_TX_PQ_IRQ_CLEAR      (1u << 16) //!< Enable Interrupt of TX Priority Queue interrupt
#define XCAN_IC_FC_MH_STOP_IRQ_CLEAR       (1u << 17) //!< Enable The interrupt is triggered when the PRT is stopped interrupt
#define XCAN_IC_FC_MH_RX_FILTER_IRQ_CLEAR  (1u << 18) //!< Enable In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful interrupt
#define XCAN_IC_FC_MH_TX_FILTER_IRQ_CLEAR  (1u << 19) //!< Enable The interrupt is triggered when the TX filter is enabled, and a TX message is rejected interrupt
#define XCAN_IC_FC_MH_TX_ABORT_IRQ_CLEAR   (1u << 20) //!< Enable This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT interrupt
#define XCAN_IC_FC_MH_RX_ABORT_IRQ_CLEAR   (1u << 21) //!< Enable This interrupt line is triggered when the MH needs to abort a RX message being received from PRT interrupt
#define XCAN_IC_FC_MH_STATS_IRQ_CLEAR      (1u << 22) //!< Enable One of the RX/TX counters have reached the threshold interrupt

#define XCAN_IC_FC_PRT_E_ACTIVE_CLEAR      (1u << 24) //!< Enable PRT switched from Error-Passive to Error-Active state interrupt
#define XCAN_IC_FC_PRT_BUS_ON_CLEAR        (1u << 25) //!< Enable PRT started CAN communication, after start or end of BusOff interrupt
#define XCAN_IC_FC_PRT_TX_EVT_CLEAR        (1u << 26) //!< Enable PRT transmitted a valid CAN message interrupt
#define XCAN_IC_FC_PRT_RX_EVT_CLEAR        (1u << 27) //!< Enable PRT received a valid CAN message interrupt

//-----------------------------------------------------------------------------

/*! XCAN Error raw event clear register (Write, Offset: 0xA14, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register ERR_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_EC_Register
{
  uint32_t ERR_CLR;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_RX_FILTER_ERR: 1; //!<  0    - MH RX filtering has not finished in time, i.e. current RX filtering has not been completed before next incoming RX message requires RX filtering: Clear bit by writing 1
    uint32_t MH_MEM_SFTY_ERR : 1; /*!<  1    - MH detected error in L_MEM error interrupt: Clear bit by writing 1
                                   *           This interrupt is triggered when either the MEM_SFTY_CE or MEM_SFTY_UE input signal is active.
                                   *           The Message Handler provides the information, which signal was active, see flags MH:SFTY_INT_STS.MEM_SFTY_CE and MH:SFTY_INT_STS.MEM_SFTY_UE
                                   */
    uint32_t MH_REG_CRC_ERR  : 1; //!<  2    - MH detected CRC error at the register bank. See also description of REG_CRC_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DESC_ERR     : 1; /*!<  3    - CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt: Clear bit by writing 1
                                   *           A status flag can define if it is on TX or RX path, see SFTY_INT_STS register
                                   */
    uint32_t MH_AP_PARITY_ERR: 1; /*!<  4    - MH detected parity error at address pointers, used to manage the MH Queues (RX/TX FIFO Queues and TX Priority Queues) error interrupt: Clear bit by writing 1
                                   *           See also description of AP_PARITY_ERR in MH section
                                   */
    uint32_t MH_DP_PARITY_ERR: 1; /*!<  5    - MH detected parity error at RX message data error interrupt: Clear bit by writing 1
                                   *           (received from PRT and written to AXI system bus) respective parity error detected at TX message data (read from AXI system bus and transferred to PRT).
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX message or TX message was affected
                                   */
    uint32_t MH_DP_SEQ_ERR   : 1; /*!<  6    - MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt: Clear bit by writing 1
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX or TX interface was affected
                                   */
    uint32_t MH_DP_DO_ERR    : 1; //!<  7    - MH detected a data overflow at RX buffer, see description of DP_DO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DP_TO_ERR    : 1; //!<  8    - MH detected timeout at TX_MSG interface located between MH and PRT, see description of DP_TO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DMA_TO_ERR   : 1; //!<  9    - MH detected timeout at DMA_AXI interface, see description of DMA_TO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DMA_CH_ERR   : 1; //!< 10    - MH detected routing error, i.e. data received or sent are not properly routed to or from DMA channel interfaces, see description of DMA_CH_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_RD_RESP_ERR  : 1; //!< 11    - MH detected a bus error caused by a read access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_WR_RESP_ERR  : 1; //!< 12    - MH detected a bus error caused by a write access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_MEM_TO_ERR   : 1; //!< 13    - MH detected timeout at local memory interface MEM_AXI, see description of MEM_TO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t                 : 2; //!< 14-15
    uint32_t PRT_ABORTED     : 1; //!< 16    - PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT: Clear bit by writing 1
    uint32_t PRT_USOS        : 1; //!< 17    - PRT detected unexpected Start of Sequence during TX_MSG sequence: Clear bit by writing 1
    uint32_t PRT_TX_DU       : 1; //!< 18    - PRT detected underrun condition at TX_MSG sequence: Clear bit by writing 1
    uint32_t PRT_RX_DO       : 1; //!< 19    - PRT detected overflow condition at RX_MSG sequence: Clear bit by writing 1
    uint32_t PRT_IFF_RQ      : 1; //!< 20    - PRT detected invalid Frame Format at TX_MSG: Clear bit by writing 1
    uint32_t PRT_BUS_ERR     : 1; //!< 21    - PRT detected error on the CAN Bus: Clear bit by writing 1
    uint32_t PRT_E_PASSIVE   : 1; //!< 22    - PRT switched from Error-Active to Error-Passive state: Clear bit by writing 1
    uint32_t PRT_BUS_OFF     : 1; //!< 23    - PRT entered Bus_Off state: Clear bit by writing 1
    uint32_t                 : 4; //!< 24-27
    uint32_t TOP_MUX_TO_ERR  : 1; //!< 28    - Timeout at top-level multiplexer error interrupt: Clear bit by writing 1
    uint32_t                 : 3; //!< 29-31
  } Bits;
} XCAN_IC_EC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_EC_Register, 4);

#define XCAN_IC_EC_MH_RX_FILTER_ERR_CLEAR  (1u <<  0) //!< Clear MH RX filtering has not finished in time error interrupt
#define XCAN_IC_EC_MH_MEM_SFTY_ERR_CLEAR   (1u <<  1) //!< Clear MH detected error in L_MEM error interrupt
#define XCAN_IC_EC_MH_REG_CRC_ERR_CLEAR    (1u <<  2) //!< Clear MH detected CRC error at the register bank error interrupt
#define XCAN_IC_EC_MH_DESC_ERR_CLEAR       (1u <<  3) //!< Clear CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_EC_MH_AP_PARITY_ERR_CLEAR  (1u <<  4) //!< Clear MH detected parity error at address pointers error interrupt
#define XCAN_IC_EC_MH_DP_PARITY_ERR_CLEAR  (1u <<  5) //!< Clear MH detected parity error at RX message data error interrupt
#define XCAN_IC_EC_MH_DP_SEQ_ERR_CLEAR     (1u <<  6) //!< Clear MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_EC_MH_DP_DO_ERR_CLEAR      (1u <<  7) //!< Clear MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_EC_MH_DP_TO_ERR_CLEAR      (1u <<  8) //!< Clear MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_EC_MH_DMA_TO_ERR_CLEAR     (1u <<  9) //!< Clear MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_EC_MH_DMA_CH_ERR_CLEAR     (1u << 10) //!< Clear MH detected routing error interrupt
#define XCAN_IC_EC_MH_RD_RESP_ERR_CLEAR    (1u << 11) //!< Clear MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EC_MH_WR_RESP_ERR_CLEAR    (1u << 12) //!< Clear MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EC_MH_MEM_TO_ERR_CLEAR     (1u << 13) //!< Clear MH detected timeout at local memory interface MEM_AXI error interrupt

#define XCAN_IC_EC_PRT_ABORTED_CLEAR       (1u << 16) //!< Clear PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT interrupt
#define XCAN_IC_EC_PRT_USOS_CLEAR          (1u << 17) //!< Clear PRT detected unexpected Start of Sequence during TX_MSG sequence interrupt
#define XCAN_IC_EC_PRT_TX_DU_CLEAR         (1u << 18) //!< Clear PRT detected underrun condition at TX_MSG sequence interrupt
#define XCAN_IC_EC_PRT_RX_DO_CLEAR         (1u << 19) //!< Clear PRT detected overflow condition at RX_MSG sequence interrupt
#define XCAN_IC_EC_PRT_IFF_RQ_CLEAR        (1u << 20) //!< Clear PRT detected invalid Frame Format at TX_MSG interrupt
#define XCAN_IC_EC_PRT_BUS_ERR_CLEAR       (1u << 21) //!< Clear PRT detected error on the CAN Bus interrupt
#define XCAN_IC_EC_PRT_E_PASSIVE_CLEAR     (1u << 22) //!< Clear PRT detected error on the CAN Bus interrupt
#define XCAN_IC_EC_PRT_BUS_OFF_CLEAR       (1u << 23) //!< Clear PRT entered Bus_Off state interrupt

#define XCAN_IC_EC_TOP_MUX_TO_ERR_CLEAR    (1u << 28) //!< Clear Timeout at top-level multiplexer error interrupt

//-----------------------------------------------------------------------------

/*! XCAN Safety raw event clear register (Write, Offset: 0xA18, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register SAFETY_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_SC_Register
{
  uint32_t SAFETY_CLR;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_RX_FILTER_ERR: 1; //!<  0    - MH RX filtering has not finished in time, i.e. current RX filtering has not been completed before next incoming RX message requires RX filtering: Clear bit by writing 1
    uint32_t MH_MEM_SFTY_ERR : 1; /*!<  1    - MH detected error in L_MEM error interrupt: Clear bit by writing 1
                                   *           This interrupt is triggered when either the MEM_SFTY_CE or MEM_SFTY_UE input signal is active.
                                   *           The Message Handler provides the information, which signal was active, see flags MH:SFTY_INT_STS.MEM_SFTY_CE and MH:SFTY_INT_STS.MEM_SFTY_UE
                                   */
    uint32_t MH_REG_CRC_ERR  : 1; //!<  2    - MH detected CRC error at the register bank. See also description of REG_CRC_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DESC_ERR     : 1; /*!<  3    - CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt: Clear bit by writing 1
                                   *           A status flag can define if it is on TX or RX path, see SFTY_INT_STS register
                                   */
    uint32_t MH_AP_PARITY_ERR: 1; /*!<  4    - MH detected parity error at address pointers, used to manage the MH Queues (RX/TX FIFO Queues and TX Priority Queues) error interrupt: Clear bit by writing 1
                                   *           See also description of AP_PARITY_ERR in MH section
                                   */
    uint32_t MH_DP_PARITY_ERR: 1; /*!<  5    - MH detected parity error at RX message data error interrupt: Clear bit by writing 1
                                   *           (received from PRT and written to AXI system bus) respective parity error detected at TX message data (read from AXI system bus and transferred to PRT).
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX message or TX message was affected
                                   */
    uint32_t MH_DP_SEQ_ERR   : 1; /*!<  6    - MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt: Clear bit by writing 1
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX or TX interface was affected
                                   */
    uint32_t MH_DP_DO_ERR    : 1; //!<  7    - MH detected a data overflow at RX buffer, see description of DP_DO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DP_TO_ERR    : 1; //!<  8    - MH detected timeout at TX_MSG interface located between MH and PRT, see description of DP_TO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DMA_TO_ERR   : 1; //!<  9    - MH detected timeout at DMA_AXI interface, see description of DMA_TO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_DMA_CH_ERR   : 1; //!< 10    - MH detected routing error, i.e. data received or sent are not properly routed to or from DMA channel interfaces, see description of DMA_CH_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_RD_RESP_ERR  : 1; //!< 11    - MH detected a bus error caused by a read access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_WR_RESP_ERR  : 1; //!< 12    - MH detected a bus error caused by a write access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t MH_MEM_TO_ERR   : 1; //!< 13    - MH detected timeout at local memory interface MEM_AXI, see description of MEM_TO_ERR in MH section error interrupt: Clear bit by writing 1
    uint32_t                 : 2; //!< 14-15
    uint32_t PRT_ABORTED     : 1; //!< 16    - PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT: Clear bit by writing 1
    uint32_t PRT_USOS        : 1; //!< 17    - PRT detected unexpected Start of Sequence during TX_MSG sequence: Clear bit by writing 1
    uint32_t PRT_TX_DU       : 1; //!< 18    - PRT detected underrun condition at TX_MSG sequence: Clear bit by writing 1
    uint32_t PRT_RX_DO       : 1; //!< 19    - PRT detected overflow condition at RX_MSG sequence: Clear bit by writing 1
    uint32_t PRT_IFF_RQ      : 1; //!< 20    - PRT detected invalid Frame Format at TX_MSG: Clear bit by writing 1
    uint32_t PRT_BUS_ERR     : 1; //!< 21    - PRT detected error on the CAN Bus: Clear bit by writing 1
    uint32_t PRT_E_PASSIVE   : 1; //!< 22    - PRT switched from Error-Active to Error-Passive state: Clear bit by writing 1
    uint32_t PRT_BUS_OFF     : 1; //!< 23    - PRT entered Bus_Off state: Clear bit by writing 1
    uint32_t                 : 4; //!< 24-27
    uint32_t TOP_MUX_TO_ERR  : 1; //!< 28    - Timeout at top-level multiplexer error interrupt: Clear bit by writing 1
    uint32_t                 : 3; //!< 29-31
  } Bits;
} XCAN_IC_SC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_SC_Register, 4);

#define XCAN_IC_SC_MH_RX_FILTER_ERR_CLEAR  (1u <<  0) //!< Clear MH RX filtering has not finished in time error interrupt
#define XCAN_IC_SC_MH_MEM_SFTY_ERR_CLEAR   (1u <<  1) //!< Clear MH detected error in L_MEM error interrupt
#define XCAN_IC_SC_MH_REG_CRC_ERR_CLEAR    (1u <<  2) //!< Clear MH detected CRC error at the register bank error interrupt
#define XCAN_IC_SC_MH_DESC_ERR_CLEAR       (1u <<  3) //!< Clear CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_SC_MH_AP_PARITY_ERR_CLEAR  (1u <<  4) //!< Clear MH detected parity error at address pointers error interrupt
#define XCAN_IC_SC_MH_DP_PARITY_ERR_CLEAR  (1u <<  5) //!< Clear MH detected parity error at RX message data error interrupt
#define XCAN_IC_SC_MH_DP_SEQ_ERR_CLEAR     (1u <<  6) //!< Clear MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_SC_MH_DP_DO_ERR_CLEAR      (1u <<  7) //!< Clear MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_SC_MH_DP_TO_ERR_CLEAR      (1u <<  8) //!< Clear MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_SC_MH_DMA_TO_ERR_CLEAR     (1u <<  9) //!< Clear MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_SC_MH_DMA_CH_ERR_CLEAR     (1u << 10) //!< Clear MH detected routing error interrupt
#define XCAN_IC_SC_MH_RD_RESP_ERR_CLEAR    (1u << 11) //!< Clear MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_SC_MH_WR_RESP_ERR_CLEAR    (1u << 12) //!< Clear MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_SC_MH_MEM_TO_ERR_CLEAR     (1u << 13) //!< Clear MH detected timeout at local memory interface MEM_AXI error interrupt

#define XCAN_IC_SC_PRT_ABORTED_CLEAR       (1u << 16) //!< Clear PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT interrupt
#define XCAN_IC_SC_PRT_USOS_CLEAR          (1u << 17) //!< Clear PRT detected unexpected Start of Sequence during TX_MSG sequence interrupt
#define XCAN_IC_SC_PRT_TX_DU_CLEAR         (1u << 18) //!< Clear PRT detected underrun condition at TX_MSG sequence interrupt
#define XCAN_IC_SC_PRT_RX_DO_CLEAR         (1u << 19) //!< Clear PRT detected overflow condition at RX_MSG sequence interrupt
#define XCAN_IC_SC_PRT_IFF_RQ_CLEAR        (1u << 20) //!< Clear PRT detected invalid Frame Format at TX_MSG interrupt
#define XCAN_IC_SC_PRT_BUS_ERR_CLEAR       (1u << 21) //!< Clear PRT detected error on the CAN Bus interrupt
#define XCAN_IC_SC_PRT_E_PASSIVE_CLEAR     (1u << 22) //!< Clear PRT detected error on the CAN Bus interrupt
#define XCAN_IC_SC_PRT_BUS_OFF_CLEAR       (1u << 23) //!< Clear PRT entered Bus_Off state interrupt

#define XCAN_IC_SC_TOP_MUX_TO_ERR_CLEAR    (1u << 28) //!< Clear Timeout at top-level multiplexer error interrupt

//-----------------------------------------------------------------------------

/*! XCAN Functional raw event enable register (Read/Write, Offset: 0xA20, Initial value: 0x00000000)
 * Any bit in the FUNC_ENA register enables the corresponding bit in the FUNC_RAW to trigger the interrupt line FUNC_INT.
 * The interrupt line gets active high, when at least one RAW/ENA pair is 1, e.g. FUNC_RAW.MH_TX_FQ_IRQ = FUNC_ENA.MH_TX_FQ_IRQ = 1
 */
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_FE_Register
{
  uint32_t FUNC_ENA;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_TX_FQ0_IRQ   : 1; /*!<  0    - MH interrupt of the TX FIFO Queue 0: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           This interrupt is triggered when an invalid TX descriptor is fetched from this TX FIFO Queue, a TX message from that FIFO Queue is sent (if set in TX descriptor),
                                   *           or a TX message of that TX FIFO Queue is skipped, see description of TX_FQ_IRQ[7:0] in MH section
                                   */
    uint32_t MH_TX_FQ1_IRQ   : 1; //!<  1    - MH interrupt of the TX FIFO Queue 1: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_FQ2_IRQ   : 1; //!<  2    - MH interrupt of the TX FIFO Queue 2: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_FQ3_IRQ   : 1; //!<  3    - MH interrupt of the TX FIFO Queue 3: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_FQ4_IRQ   : 1; //!<  4    - MH interrupt of the TX FIFO Queue 4: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_FQ5_IRQ   : 1; //!<  5    - MH interrupt of the TX FIFO Queue 5: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_FQ6_IRQ   : 1; //!<  6    - MH interrupt of the TX FIFO Queue 6: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_FQ7_IRQ   : 1; //!<  7    - MH interrupt of the TX FIFO Queue 7: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FQ0_IRQ   : 1; /*!<  8    - MH interrupt of the RX FIFO Queue 0: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           This interrupt is triggered when an invalid RX descriptor is fetched from this RX FIFO Queue,
                                   *           or an RX message is received (if set in RX descriptor) in this RX FIFO Queue, see description of RX_FQ_IRQ[7:0] in MH section
                                   */
    uint32_t MH_RX_FQ1_IRQ   : 1; //!<  9    - MH interrupt of the RX FIFO Queue 1: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FQ2_IRQ   : 1; //!< 10    - MH interrupt of the RX FIFO Queue 2: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FQ3_IRQ   : 1; //!< 11    - MH interrupt of the RX FIFO Queue 3: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FQ4_IRQ   : 1; //!< 12    - MH interrupt of the RX FIFO Queue 4: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FQ5_IRQ   : 1; //!< 13    - MH interrupt of the RX FIFO Queue 5: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FQ6_IRQ   : 1; //!< 14    - MH interrupt of the RX FIFO Queue 6: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FQ7_IRQ   : 1; //!< 15    - MH interrupt of the RX FIFO Queue 7: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_PQ_IRQ    : 1; /*!< 16    - Interrupt of TX Priority Queue: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           Any TX message sent from the TX Priority Queue can be configured to trigger this interrupt.
                                   *           The SW would then need to look at the MH register TX_PQ_INT_STS to identify which slot has generated the interrupt and for which reason
                                   */
    uint32_t MH_STOP_IRQ     : 1; //!< 17    - The interrupt is triggered when the PRT is stopped. The MH finishes its task and switches to idle mode: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_FILTER_IRQ: 1; //!< 18    - In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_FILTER_IRQ: 1; //!< 19    - The interrupt is triggered when the TX filter is enabled, and a TX message is rejected: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_TX_ABORT_IRQ : 1; //!< 20    - This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RX_ABORT_IRQ : 1; //!< 21    - This interrupt line is triggered when the MH needs to abort a RX message being received from PRT: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_STATS_IRQ    : 1; //!< 22    - One of the RX/TX counters have reached the threshold: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 1; //!< 23
    uint32_t PRT_E_ACTIVE    : 1; //!< 24    - PRT switched from Error-Passive to Error-Active state: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_BUS_ON      : 1; //!< 25    - PRT started CAN communication, after start or end of BusOff: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_TX_EVT      : 1; //!< 26    - PRT transmitted a valid CAN message: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_RX_EVT      : 1; //!< 27    - PRT received a valid CAN message: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 4; //!< 28-31
  } Bits;
} XCAN_IC_FE_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_FE_Register, 4);

#define XCAN_IC_FE_MH_TX_FQ0_IRQ_EN      (1u <<  0) //!< Enable MH interrupt of the TX FIFO Queue 0 interrupt
#define XCAN_IC_FE_MH_TX_FQ0_IRQ_DIS     (0u <<  0) //!< Disable MH interrupt of the TX FIFO Queue 0 interrupt
#define XCAN_IC_FE_MH_TX_FQ1_IRQ_EN      (1u <<  1) //!< Enable MH interrupt of the TX FIFO Queue 1 interrupt
#define XCAN_IC_FE_MH_TX_FQ1_IRQ_DIS     (0u <<  1) //!< Disable MH interrupt of the TX FIFO Queue 1 interrupt
#define XCAN_IC_FE_MH_TX_FQ2_IRQ_EN      (1u <<  2) //!< Enable MH interrupt of the TX FIFO Queue 2 interrupt
#define XCAN_IC_FE_MH_TX_FQ2_IRQ_DIS     (0u <<  2) //!< Disable MH interrupt of the TX FIFO Queue 2 interrupt
#define XCAN_IC_FE_MH_TX_FQ3_IRQ_EN      (1u <<  3) //!< Enable MH interrupt of the TX FIFO Queue 3 interrupt
#define XCAN_IC_FE_MH_TX_FQ3_IRQ_DIS     (0u <<  3) //!< Disable MH interrupt of the TX FIFO Queue 3 interrupt
#define XCAN_IC_FE_MH_TX_FQ4_IRQ_EN      (1u <<  4) //!< Enable MH interrupt of the TX FIFO Queue 4 interrupt
#define XCAN_IC_FE_MH_TX_FQ4_IRQ_DIS     (0u <<  4) //!< Disable MH interrupt of the TX FIFO Queue 4 interrupt
#define XCAN_IC_FE_MH_TX_FQ5_IRQ_EN      (1u <<  5) //!< Enable MH interrupt of the TX FIFO Queue 5 interrupt
#define XCAN_IC_FE_MH_TX_FQ5_IRQ_DIS     (0u <<  5) //!< Disable MH interrupt of the TX FIFO Queue 5 interrupt
#define XCAN_IC_FE_MH_TX_FQ6_IRQ_EN      (1u <<  6) //!< Enable MH interrupt of the TX FIFO Queue 6 interrupt
#define XCAN_IC_FE_MH_TX_FQ6_IRQ_DIS     (0u <<  6) //!< Disable MH interrupt of the TX FIFO Queue 6 interrupt
#define XCAN_IC_FE_MH_TX_FQ7_IRQ_EN      (1u <<  7) //!< Enable MH interrupt of the TX FIFO Queue 7 interrupt
#define XCAN_IC_FE_MH_TX_FQ7_IRQ_DIS     (0u <<  7) //!< Disable MH interrupt of the TX FIFO Queue 7 interrupt
#define XCAN_IC_FE_MH_RX_FQ0_IRQ_EN      (1u <<  8) //!< Enable MH interrupt of the RX FIFO Queue 0 interrupt
#define XCAN_IC_FE_MH_RX_FQ0_IRQ_DIS     (0u <<  8) //!< Disable MH interrupt of the RX FIFO Queue 0 interrupt
#define XCAN_IC_FE_MH_RX_FQ1_IRQ_EN      (1u <<  9) //!< Enable MH interrupt of the RX FIFO Queue 1 interrupt
#define XCAN_IC_FE_MH_RX_FQ1_IRQ_DIS     (0u <<  9) //!< Disable MH interrupt of the RX FIFO Queue 1 interrupt
#define XCAN_IC_FE_MH_RX_FQ2_IRQ_EN      (1u << 10) //!< Enable MH interrupt of the RX FIFO Queue 2 interrupt
#define XCAN_IC_FE_MH_RX_FQ2_IRQ_DIS     (0u << 10) //!< Disable MH interrupt of the RX FIFO Queue 2 interrupt
#define XCAN_IC_FE_MH_RX_FQ3_IRQ_EN      (1u << 11) //!< Enable MH interrupt of the RX FIFO Queue 3 interrupt
#define XCAN_IC_FE_MH_RX_FQ3_IRQ_DIS     (0u << 11) //!< Disable MH interrupt of the RX FIFO Queue 3 interrupt
#define XCAN_IC_FE_MH_RX_FQ4_IRQ_EN      (1u << 12) //!< Enable MH interrupt of the RX FIFO Queue 4 interrupt
#define XCAN_IC_FE_MH_RX_FQ4_IRQ_DIS     (0u << 12) //!< Disable MH interrupt of the RX FIFO Queue 4 interrupt
#define XCAN_IC_FE_MH_RX_FQ5_IRQ_EN      (1u << 13) //!< Enable MH interrupt of the RX FIFO Queue 5 interrupt
#define XCAN_IC_FE_MH_RX_FQ5_IRQ_DIS     (0u << 13) //!< Disable MH interrupt of the RX FIFO Queue 5 interrupt
#define XCAN_IC_FE_MH_RX_FQ6_IRQ_EN      (1u << 14) //!< Enable MH interrupt of the RX FIFO Queue 6 interrupt
#define XCAN_IC_FE_MH_RX_FQ6_IRQ_DIS     (0u << 14) //!< Disable MH interrupt of the RX FIFO Queue 6 interrupt
#define XCAN_IC_FE_MH_RX_FQ7_IRQ_EN      (1u << 15) //!< Enable MH interrupt of the RX FIFO Queue 7 interrupt
#define XCAN_IC_FE_MH_RX_FQ7_IRQ_DIS     (0u << 15) //!< Disable MH interrupt of the RX FIFO Queue 7 interrupt
#define XCAN_IC_FE_MH_TX_PQ_IRQ_EN       (1u << 16) //!< Enable Interrupt of TX Priority Queue interrupt
#define XCAN_IC_FE_MH_TX_PQ_IRQ_DIS      (0u << 16) //!< Disable Interrupt of TX Priority Queue interrupt
#define XCAN_IC_FE_MH_STOP_IRQ_EN        (1u << 17) //!< Enable The interrupt is triggered when the PRT is stopped interrupt
#define XCAN_IC_FE_MH_STOP_IRQ_DIS       (0u << 17) //!< Disable The interrupt is triggered when the PRT is stopped interrupt
#define XCAN_IC_FE_MH_RX_FILTER_IRQ_EN   (1u << 18) //!< Enable In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful interrupt
#define XCAN_IC_FE_MH_RX_FILTER_IRQ_DIS  (0u << 18) //!< Disable In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful interrupt
#define XCAN_IC_FE_MH_TX_FILTER_IRQ_EN   (1u << 19) //!< Enable The interrupt is triggered when the TX filter is enabled, and a TX message is rejected interrupt
#define XCAN_IC_FE_MH_TX_FILTER_IRQ_DIS  (0u << 19) //!< Disable The interrupt is triggered when the TX filter is enabled, and a TX message is rejected interrupt
#define XCAN_IC_FE_MH_TX_ABORT_IRQ_EN    (1u << 20) //!< Enable This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT interrupt
#define XCAN_IC_FE_MH_TX_ABORT_IRQ_DIS   (0u << 20) //!< Disable This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT interrupt
#define XCAN_IC_FE_MH_RX_ABORT_IRQ_EN    (1u << 21) //!< Enable This interrupt line is triggered when the MH needs to abort a RX message being received from PRT interrupt
#define XCAN_IC_FE_MH_RX_ABORT_IRQ_DIS   (0u << 21) //!< Disable This interrupt line is triggered when the MH needs to abort a RX message being received from PRT interrupt
#define XCAN_IC_FE_MH_STATS_IRQ_EN       (1u << 22) //!< Enable One of the RX/TX counters have reached the threshold interrupt
#define XCAN_IC_FE_MH_STATS_IRQ_DIS      (0u << 22) //!< Disable One of the RX/TX counters have reached the threshold interrupt

#define XCAN_IC_FE_PRT_E_ACTIVE_EN       (1u << 24) //!< Enable PRT switched from Error-Passive to Error-Active state interrupt
#define XCAN_IC_FE_PRT_E_ACTIVE_DIS      (0u << 24) //!< Disable PRT switched from Error-Passive to Error-Active state interrupt
#define XCAN_IC_FE_PRT_BUS_ON_EN         (1u << 25) //!< Enable PRT started CAN communication, after start or end of BusOff interrupt
#define XCAN_IC_FE_PRT_BUS_ON_DIS        (0u << 25) //!< Disable PRT started CAN communication, after start or end of BusOff interrupt
#define XCAN_IC_FE_PRT_TX_EVT_EN         (1u << 26) //!< Enable PRT transmitted a valid CAN message interrupt
#define XCAN_IC_FE_PRT_TX_EVT_DIS        (0u << 26) //!< Disable PRT transmitted a valid CAN message interrupt
#define XCAN_IC_FE_PRT_RX_EVT_EN         (1u << 27) //!< Enable PRT received a valid CAN message interrupt
#define XCAN_IC_FE_PRT_RX_EVT_DIS        (0u << 27) //!< Disable PRT received a valid CAN message interrupt

//-----------------------------------------------------------------------------

/*! XCAN Error raw event enable register (Read/Write, Offset: 0xA24, Initial value: 0x00000000)
 * Any bit in the ERR_ENA register enables the corresponding bit in the ERR_RAW to trigger the interrupt line ERR_INT.
 * The interrupt line gets active high, when at least one RAW/ENA pair is 1, e.g. ERR_RAW.MH_TX_FQ_IRQ = ERR_ENA.MH_TX_FQ_IRQ = 1
 */
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_EE_Register
{
  uint32_t ERR_ENA;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_RX_FILTER_ERR: 1; //!<  0    - MH RX filtering has not finished in time, i.e. current RX filtering has not been completed before next incoming RX message requires RX filtering: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_MEM_SFTY_ERR : 1; /*!<  1    - MH detected error in L_MEM error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           This interrupt is triggered when either the MEM_SFTY_CE or MEM_SFTY_UE input signal is active.
                                   *           The Message Handler provides the information, which signal was active, see flags MH:SFTY_INT_STS.MEM_SFTY_CE and MH:SFTY_INT_STS.MEM_SFTY_UE
                                   */
    uint32_t MH_REG_CRC_ERR  : 1; //!<  2    - MH detected CRC error at the register bank. See also description of REG_CRC_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DESC_ERR     : 1; /*!<  3    - CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           A status flag can define if it is on TX or RX path, see SFTY_INT_STS register
                                   */
    uint32_t MH_AP_PARITY_ERR: 1; /*!<  4    - MH detected parity error at address pointers, used to manage the MH Queues (RX/TX FIFO Queues and TX Priority Queues) error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           See also description of AP_PARITY_ERR in MH section
                                   */
    uint32_t MH_DP_PARITY_ERR: 1; /*!<  5    - MH detected parity error at RX message data error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           (received from PRT and written to AXI system bus) respective parity error detected at TX message data (read from AXI system bus and transferred to PRT).
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX message or TX message was affected
                                   */
    uint32_t MH_DP_SEQ_ERR   : 1; /*!<  6    - MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX or TX interface was affected
                                   */
    uint32_t MH_DP_DO_ERR    : 1; //!<  7    - MH detected a data overflow at RX buffer, see description of DP_DO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DP_TO_ERR    : 1; //!<  8    - MH detected timeout at TX_MSG interface located between MH and PRT, see description of DP_TO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DMA_TO_ERR   : 1; //!<  9    - MH detected timeout at DMA_AXI interface, see description of DMA_TO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DMA_CH_ERR   : 1; //!< 10    - MH detected routing error, i.e. data received or sent are not properly routed to or from DMA channel interfaces, see description of DMA_CH_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RD_RESP_ERR  : 1; //!< 11    - MH detected a bus error caused by a read access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_WR_RESP_ERR  : 1; //!< 12    - MH detected a bus error caused by a write access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_MEM_TO_ERR   : 1; //!< 13    - MH detected timeout at local memory interface MEM_AXI, see description of MEM_TO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 2; //!< 14-15
    uint32_t PRT_ABORTED     : 1; //!< 16    - PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_USOS        : 1; //!< 17    - PRT detected unexpected Start of Sequence during TX_MSG sequence: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_TX_DU       : 1; //!< 18    - PRT detected underrun condition at TX_MSG sequence: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_RX_DO       : 1; //!< 19    - PRT detected overflow condition at RX_MSG sequence: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_IFF_RQ      : 1; //!< 20    - PRT detected invalid Frame Format at TX_MSG: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_BUS_ERR     : 1; //!< 21    - PRT detected error on the CAN Bus: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_E_PASSIVE   : 1; //!< 22    - PRT switched from Error-Active to Error-Passive state: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_BUS_OFF     : 1; //!< 23    - PRT entered Bus_Off state: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 4; //!< 24-27
    uint32_t TOP_MUX_TO_ERR  : 1; //!< 28    - Timeout at top-level multiplexer error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 3; //!< 29-31
  } Bits;
} XCAN_IC_EE_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_EE_Register, 4);

#define XCAN_IC_EE_MH_RX_FILTER_ERR_EN   (1u <<  0) //!< Enable MH RX filtering has not finished in time error interrupt
#define XCAN_IC_EE_MH_RX_FILTER_ERR_DIS  (0u <<  0) //!< Disable MH RX filtering has not finished in time error interrupt
#define XCAN_IC_EE_MH_MEM_SFTY_ERR_EN    (1u <<  1) //!< Enable MH detected error in L_MEM error interrupt
#define XCAN_IC_EE_MH_MEM_SFTY_ERR_DIS   (0u <<  1) //!< Disable MH detected error in L_MEM error interrupt
#define XCAN_IC_EE_MH_REG_CRC_ERR_EN     (1u <<  2) //!< Enable MH detected CRC error at the register bank error interrupt
#define XCAN_IC_EE_MH_REG_CRC_ERR_DIS    (0u <<  2) //!< Disable MH detected CRC error at the register bank error interrupt
#define XCAN_IC_EE_MH_DESC_ERR_EN        (1u <<  3) //!< Enable CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_EE_MH_DESC_ERR_DIS       (0u <<  3) //!< Disable CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_EE_MH_AP_PARITY_ERR_EN   (1u <<  4) //!< Enable MH detected parity error at address pointers error interrupt
#define XCAN_IC_EE_MH_AP_PARITY_ERR_DIS  (0u <<  4) //!< Disable MH detected parity error at address pointers error interrupt
#define XCAN_IC_EE_MH_DP_PARITY_ERR_EN   (1u <<  5) //!< Enable MH detected parity error at RX message data error interrupt
#define XCAN_IC_EE_MH_DP_PARITY_ERR_DIS  (0u <<  5) //!< Disable MH detected parity error at RX message data error interrupt
#define XCAN_IC_EE_MH_DP_SEQ_ERR_EN      (1u <<  6) //!< Enable MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_EE_MH_DP_SEQ_ERR_DIS     (0u <<  6) //!< Disable MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_EE_MH_DP_DO_ERR_EN       (1u <<  7) //!< Enable MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_EE_MH_DP_DO_ERR_DIS      (0u <<  7) //!< Disable MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_EE_MH_DP_TO_ERR_EN       (1u <<  8) //!< Enable MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_EE_MH_DP_TO_ERR_DIS      (0u <<  8) //!< Disable MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_EE_MH_DMA_TO_ERR_EN      (1u <<  9) //!< Enable MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_EE_MH_DMA_TO_ERR_DIS     (0u <<  9) //!< Disable MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_EE_MH_DMA_CH_ERR_EN      (1u << 10) //!< Enable MH detected routing error interrupt
#define XCAN_IC_EE_MH_DMA_CH_ERR_DIS     (0u << 10) //!< Disable MH detected routing error interrupt
#define XCAN_IC_EE_MH_RD_RESP_ERR_EN     (1u << 11) //!< Enable MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_MH_RD_RESP_ERR_DIS    (0u << 11) //!< Disable MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_MH_WR_RESP_ERR_EN     (1u << 12) //!< Enable MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_MH_WR_RESP_ERR_DIS    (0u << 12) //!< Disable MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_MH_MEM_TO_ERR_EN      (1u << 13) //!< Enable MH detected timeout at local memory interface MEM_AXI error interrupt
#define XCAN_IC_EE_MH_MEM_TO_ERR_DIS     (0u << 13) //!< Disable MH detected timeout at local memory interface MEM_AXI error interrupt

#define XCAN_IC_EE_PRT_ABORTED_EN        (1u << 16) //!< Enable PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT interrupt
#define XCAN_IC_EE_PRT_ABORTED_DIS       (0u << 16) //!< Disable PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT interrupt
#define XCAN_IC_EE_PRT_USOS_EN           (1u << 17) //!< Enable PRT detected unexpected Start of Sequence during TX_MSG sequence interrupt
#define XCAN_IC_EE_PRT_USOS_DIS          (0u << 17) //!< Disable PRT detected unexpected Start of Sequence during TX_MSG sequence interrupt
#define XCAN_IC_EE_PRT_TX_DU_EN          (1u << 18) //!< Enable PRT detected underrun condition at TX_MSG sequence interrupt
#define XCAN_IC_EE_PRT_TX_DU_DIS         (0u << 18) //!< Disable PRT detected underrun condition at TX_MSG sequence interrupt
#define XCAN_IC_EE_PRT_RX_DO_EN          (1u << 19) //!< Enable PRT detected overflow condition at RX_MSG sequence interrupt
#define XCAN_IC_EE_PRT_RX_DO_DIS         (0u << 19) //!< Disable PRT detected overflow condition at RX_MSG sequence interrupt
#define XCAN_IC_EE_PRT_IFF_RQ_EN         (1u << 20) //!< Enable PRT detected invalid Frame Format at TX_MSG interrupt
#define XCAN_IC_EE_PRT_IFF_RQ_DIS        (0u << 20) //!< Disable PRT detected invalid Frame Format at TX_MSG interrupt
#define XCAN_IC_EE_PRT_BUS_ERR_EN        (1u << 21) //!< Enable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_EE_PRT_BUS_ERR_DIS       (0u << 21) //!< Disable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_EE_PRT_E_PASSIVE_EN      (1u << 22) //!< Enable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_EE_PRT_E_PASSIVE_DIS     (0u << 22) //!< Disable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_EE_PRT_BUS_OFF_EN        (1u << 23) //!< Enable PRT entered Bus_Off state interrupt
#define XCAN_IC_EE_PRT_BUS_OFF_DIS       (0u << 23) //!< Disable PRT entered Bus_Off state interrupt

#define XCAN_IC_EE_TOP_MUX_TO_ERR_EN     (1u << 28) //!< Enable Timeout at top-level multiplexer error interrupt
#define XCAN_IC_EE_TOP_MUX_TO_ERR_DIS    (0u << 28) //!< Disable Timeout at top-level multiplexer error interrupt

//-----------------------------------------------------------------------------

/*! XCAN Safety raw event enable register (Read/Write, Offset: 0xA28, Initial value: 0x00000000)
 * Any bit in the SAFETY_ENA register enables the corresponding bit in the SAFETY_RAW to trigger the interrupt line SAFETY_INT.
 * The interrupt line gets active high, when at least one RAW/ENA pair is 1, e.g. SAFETY_RAW.MH_TX_FQ_IRQ = SAFETY_ENA.MH_TX_FQ_IRQ = 1
 */
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_SA_Register
{
  uint32_t SAFETY_ENA;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_RX_FILTER_ERR: 1; //!<  0    - MH RX filtering has not finished in time, i.e. current RX filtering has not been completed before next incoming RX message requires RX filtering: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_MEM_SFTY_ERR : 1; /*!<  1    - MH detected error in L_MEM error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           This interrupt is triggered when either the MEM_SFTY_CE or MEM_SFTY_UE input signal is active.
                                   *           The Message Handler provides the information, which signal was active, see flags MH:SFTY_INT_STS.MEM_SFTY_CE and MH:SFTY_INT_STS.MEM_SFTY_UE
                                   */
    uint32_t MH_REG_CRC_ERR  : 1; //!<  2    - MH detected CRC error at the register bank. See also description of REG_CRC_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DESC_ERR     : 1; /*!<  3    - CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           A status flag can define if it is on TX or RX path, see SFTY_INT_STS register
                                   */
    uint32_t MH_AP_PARITY_ERR: 1; /*!<  4    - MH detected parity error at address pointers, used to manage the MH Queues (RX/TX FIFO Queues and TX Priority Queues) error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           See also description of AP_PARITY_ERR in MH section
                                   */
    uint32_t MH_DP_PARITY_ERR: 1; /*!<  5    - MH detected parity error at RX message data error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           (received from PRT and written to AXI system bus) respective parity error detected at TX message data (read from AXI system bus and transferred to PRT).
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX message or TX message was affected
                                   */
    uint32_t MH_DP_SEQ_ERR   : 1; /*!<  6    - MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX or TX interface was affected
                                   */
    uint32_t MH_DP_DO_ERR    : 1; //!<  7    - MH detected a data overflow at RX buffer, see description of DP_DO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DP_TO_ERR    : 1; //!<  8    - MH detected timeout at TX_MSG interface located between MH and PRT, see description of DP_TO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DMA_TO_ERR   : 1; //!<  9    - MH detected timeout at DMA_AXI interface, see description of DMA_TO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_DMA_CH_ERR   : 1; //!< 10    - MH detected routing error, i.e. data received or sent are not properly routed to or from DMA channel interfaces, see description of DMA_CH_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_RD_RESP_ERR  : 1; //!< 11    - MH detected a bus error caused by a read access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_WR_RESP_ERR  : 1; //!< 12    - MH detected a bus error caused by a write access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t MH_MEM_TO_ERR   : 1; //!< 13    - MH detected timeout at local memory interface MEM_AXI, see description of MEM_TO_ERR in MH section error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 2; //!< 14-15
    uint32_t PRT_ABORTED     : 1; //!< 16    - PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_USOS        : 1; //!< 17    - PRT detected unexpected Start of Sequence during TX_MSG sequence: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_TX_DU       : 1; //!< 18    - PRT detected underrun condition at TX_MSG sequence: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_RX_DO       : 1; //!< 19    - PRT detected overflow condition at RX_MSG sequence: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_IFF_RQ      : 1; //!< 20    - PRT detected invalid Frame Format at TX_MSG: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_BUS_ERR     : 1; //!< 21    - PRT detected error on the CAN Bus: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_E_PASSIVE   : 1; //!< 22    - PRT switched from Error-Active to Error-Passive state: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t PRT_BUS_OFF     : 1; //!< 23    - PRT entered Bus_Off state: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 4; //!< 24-27
    uint32_t TOP_MUX_TO_ERR  : 1; //!< 28    - Timeout at top-level multiplexer error interrupt: '1' = Enable the interrupt ; '0' = Disable the interrupt
    uint32_t                 : 3; //!< 29-31
  } Bits;
} XCAN_IC_SA_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_SA_Register, 4);

#define XCAN_IC_SA_MH_RX_FILTER_ERR_EN   (1u <<  0) //!< Enable MH RX filtering has not finished in time error interrupt
#define XCAN_IC_SA_MH_RX_FILTER_ERR_DIS  (0u <<  0) //!< Disable MH RX filtering has not finished in time error interrupt
#define XCAN_IC_SA_MH_MEM_SFTY_ERR_EN    (1u <<  1) //!< Enable MH detected error in L_MEM error interrupt
#define XCAN_IC_SA_MH_MEM_SFTY_ERR_DIS   (0u <<  1) //!< Disable MH detected error in L_MEM error interrupt
#define XCAN_IC_SA_MH_REG_CRC_ERR_EN     (1u <<  2) //!< Enable MH detected CRC error at the register bank error interrupt
#define XCAN_IC_SA_MH_REG_CRC_ERR_DIS    (0u <<  2) //!< Disable MH detected CRC error at the register bank error interrupt
#define XCAN_IC_SA_MH_DESC_ERR_EN        (1u <<  3) //!< Enable CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_SA_MH_DESC_ERR_DIS       (0u <<  3) //!< Disable CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_SA_MH_AP_PARITY_ERR_EN   (1u <<  4) //!< Enable MH detected parity error at address pointers error interrupt
#define XCAN_IC_SA_MH_AP_PARITY_ERR_DIS  (0u <<  4) //!< Disable MH detected parity error at address pointers error interrupt
#define XCAN_IC_SA_MH_DP_PARITY_ERR_EN   (1u <<  5) //!< Enable MH detected parity error at RX message data error interrupt
#define XCAN_IC_SA_MH_DP_PARITY_ERR_DIS  (0u <<  5) //!< Disable MH detected parity error at RX message data error interrupt
#define XCAN_IC_SA_MH_DP_SEQ_ERR_EN      (1u <<  6) //!< Enable MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_SA_MH_DP_SEQ_ERR_DIS     (0u <<  6) //!< Disable MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_SA_MH_DP_DO_ERR_EN       (1u <<  7) //!< Enable MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_SA_MH_DP_DO_ERR_DIS      (0u <<  7) //!< Disable MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_SA_MH_DP_TO_ERR_EN       (1u <<  8) //!< Enable MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_SA_MH_DP_TO_ERR_DIS      (0u <<  8) //!< Disable MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_SA_MH_DMA_TO_ERR_EN      (1u <<  9) //!< Enable MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_SA_MH_DMA_TO_ERR_DIS     (0u <<  9) //!< Disable MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_SA_MH_DMA_CH_ERR_EN      (1u << 10) //!< Enable MH detected routing error interrupt
#define XCAN_IC_SA_MH_DMA_CH_ERR_DIS     (0u << 10) //!< Disable MH detected routing error interrupt
#define XCAN_IC_SA_MH_RD_RESP_ERR_EN     (1u << 11) //!< Enable MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_SA_MH_RD_RESP_ERR_DIS    (0u << 11) //!< Disable MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_SA_MH_WR_RESP_ERR_EN     (1u << 12) //!< Enable MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_SA_MH_WR_RESP_ERR_DIS    (0u << 12) //!< Disable MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_SA_MH_MEM_TO_ERR_EN      (1u << 13) //!< Enable MH detected timeout at local memory interface MEM_AXI error interrupt
#define XCAN_IC_SA_MH_MEM_TO_ERR_DIS     (0u << 13) //!< Disable MH detected timeout at local memory interface MEM_AXI error interrupt

#define XCAN_IC_SA_PRT_ABORTED_EN        (1u << 16) //!< Enable PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT interrupt
#define XCAN_IC_SA_PRT_ABORTED_DIS       (0u << 16) //!< Disable PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT interrupt
#define XCAN_IC_SA_PRT_USOS_EN           (1u << 17) //!< Enable PRT detected unexpected Start of Sequence during TX_MSG sequence interrupt
#define XCAN_IC_SA_PRT_USOS_DIS          (0u << 17) //!< Disable PRT detected unexpected Start of Sequence during TX_MSG sequence interrupt
#define XCAN_IC_SA_PRT_TX_DU_EN          (1u << 18) //!< Enable PRT detected underrun condition at TX_MSG sequence interrupt
#define XCAN_IC_SA_PRT_TX_DU_DIS         (0u << 18) //!< Disable PRT detected underrun condition at TX_MSG sequence interrupt
#define XCAN_IC_SA_PRT_RX_DO_EN          (1u << 19) //!< Enable PRT detected overflow condition at RX_MSG sequence interrupt
#define XCAN_IC_SA_PRT_RX_DO_DIS         (0u << 19) //!< Disable PRT detected overflow condition at RX_MSG sequence interrupt
#define XCAN_IC_SA_PRT_IFF_RQ_EN         (1u << 20) //!< Enable PRT detected invalid Frame Format at TX_MSG interrupt
#define XCAN_IC_SA_PRT_IFF_RQ_DIS        (0u << 20) //!< Disable PRT detected invalid Frame Format at TX_MSG interrupt
#define XCAN_IC_SA_PRT_BUS_ERR_EN        (1u << 21) //!< Enable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_SA_PRT_BUS_ERR_DIS       (0u << 21) //!< Disable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_SA_PRT_E_PASSIVE_EN      (1u << 22) //!< Enable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_SA_PRT_E_PASSIVE_DIS     (0u << 22) //!< Disable PRT detected error on the CAN Bus interrupt
#define XCAN_IC_SA_PRT_BUS_OFF_EN        (1u << 23) //!< Enable PRT entered Bus_Off state interrupt
#define XCAN_IC_SA_PRT_BUS_OFF_DIS       (0u << 23) //!< Disable PRT entered Bus_Off state interrupt

#define XCAN_IC_SA_TOP_MUX_TO_ERR_EN     (1u << 28) //!< Enable Timeout at top-level multiplexer error interrupt
#define XCAN_IC_SA_TOP_MUX_TO_ERR_DIS    (0u << 28) //!< Disable Timeout at top-level multiplexer error interrupt

//-----------------------------------------------------------------------------

/*! XCAN IRC configuration register (Read, Offset: 0xA30, Initial value: 0x00000007)
 * This register shows the hardware configuration of the IRC concerning the capturing mode of the event inputs.
 * The IP internal events signals coming from the MH and the PRT require an 'edge sensitive' capturing.
 * That is why the value of this register is 0x7 and cannot be changed
 */
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_CM_Register
{
  uint32_t CM;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t FUNC  :  1; //!< 0    - Capturing mode of FUNC_RAW register: '1' = Edge sensitive ; '0' = Level sensitive
    uint32_t ERR   :  1; //!< 1    - Capturing mode of ERR RAW register: '1' = Edge sensitive ; '0' = Level sensitive
    uint32_t SAFETY:  1; //!< 2    - Capturing mode of SAFETY RAW register: '1' = Edge sensitive ; '0' = Level sensitive
    uint32_t       : 29; //!< 3-31
  } Bits;
} XCAN_IC_CM_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_CM_Register, 4);

#define XCAN_IC_CM_FUNC_EDGE_SENSITIVE     (1u << 0) //!< Capturing mode of FUNC_RAW is Edge sensitive
#define XCAN_IC_CM_FUNC_LEVEL_SENSITIVE    (0u << 0) //!< Capturing mode of FUNC_RAW is Level sensitive
#define XCAN_IC_CM_ERR_EDGE_SENSITIVE      (1u << 0) //!< Capturing mode of ERR RAW register is Edge sensitive
#define XCAN_IC_CM_ERR_LEVEL_SENSITIVE     (0u << 0) //!< Capturing mode of ERR RAW register is Level sensitive
#define XCAN_IC_CM_SAFETY_EDGE_SENSITIVE   (1u << 0) //!< Capturing mode of SAFETY RAW is Edge sensitive
#define XCAN_IC_CM_SAFETY_LEVEL_SENSITIVE  (0u << 0) //!< Capturing mode of SAFETY RAW is Level sensitive

//-----------------------------------------------------------------------------

//! XCAN Hardware Debug Port control register (Read/Write, Offset: 0xA40, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __MCAN_PACKED__ XCAN_IC_HDP_Register
{
  uint32_t HDP;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t HDP_SEL:  1; //!< 0    - Select the driver of the Hardware Debug Port: '1' = Protocol Controller ; '0' = Message Handler
    uint32_t        : 31; //!< 2-31
  } Bits;
} XCAN_IC_HDP_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_HDP_Register, 4);

#define XCAN_IC_HDP_PROTOCOL_CONTROLLER  (1u << 0) //!< Select the Protocol Controller for the Hardware Debug Port
#define XCAN_IC_HDP_MESSAGE_HANDLER      (0u << 0) //!< Select the Message Handler for the Hardware Debug Port

//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif /* XCAN_CORE_H_INC */