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
#  define XCAN_PACKENUM(name,type)  typedef enum __XCAN_PACKED__
#  define XCAN_UNPACKENUM(name)     name
#endif
//-----------------------------------------------------------------------------

//! This macro is used to check the size of an object. If not, it will raise a "divide by 0" error at compile time
#define XCAN_CONTROL_ITEM_SIZE(item, size)  enum { item##_size_must_be_##size##_bytes = 1 / (int)(!!(sizeof(item) == size)) }

//-----------------------------------------------------------------------------

//! 8-bit DCB to decimal without error checking
#define XCAN_DCB8_TO_DECIMAL(dcb)  ( (uint8_t)(dcb) - (6u * ((uint8_t)(dcb) >> 4u)) )

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

#define XCAN_CAN_TX_MESSAGE_SIZE  ( sizeof(XCAN_CAN_TxMessage) )

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

#define XCAN_T0_ID_Pos           0
#define XCAN_T0_ID_Mask          (0x1FFFFFFFu << XCAN_T0_ID_Pos)
#define XCAN_T0_ID_SET(value)    (((uint32_t)(value) << XCAN_T0_ID_Pos) & XCAN_T0_ID_Mask) //!< Set Message Identifier filter

#define XCAN_T0_EID_Pos          0
#define XCAN_T0_EID_Mask         (0x3FFFFu << XCAN_T0_EID_Pos)
#define XCAN_T0_EID_SET(value)   (((uint32_t)(value) << XCAN_T0_EID_Pos) & XCAN_T0_EID_Mask) //!< Set Extended Identifier filter
#define XCAN_T0_SID_Pos          18
#define XCAN_T0_SID_Mask         (0x7FFu << XCAN_T0_SID_Pos)
#define XCAN_T0_SID_SET(value)   (((uint32_t)(value) << XCAN_T0_SID_Pos) & XCAN_T0_SID_Mask) //!< Set Standard Identifier filter

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

#define XCAN_T1_CANXL_DLC_Pos         16
#define XCAN_T1_CANXL_DLC_Mask        (0x7FFu << XCAN_T1_CANXL_DLC_Pos)
#define XCAN_T1_CANXL_DLC_SET(value)  (((uint32_t)(value) << XCAN_T1_CANXL_DLC_Pos) & XCAN_T1_CANXL_DLC_Mask) //!< Set Data Length Code with CAN XL encoding
#define XCAN_T1_DLC_Pos               16
#define XCAN_T1_DLC_Mask              (0xFu << XCAN_T1_DLC_Pos)
#define XCAN_T1_DLC_SET(value)        (((uint32_t)(value) << XCAN_T1_DLC_Pos) & XCAN_T1_DLC_Mask) //!< Set Data Length Code for CAN2.0 and CAN-FD
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

#define XCAN_CAN_RX_MESSAGE_SIZE  ( sizeof(XCAN_CAN_RxMessage) )

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

#define XCAN_R0_ID_Pos           0
#define XCAN_R0_ID_Mask          (0x1FFFFFFFu << XCAN_R0_ID_Pos)
#define XCAN_R0_ID_GET(value)    (((uint32_t)(value) & XCAN_R0_ID_Mask) >> XCAN_R0_ID_Pos) //!< Get Message Identifier filter

#define XCAN_R0_EID_Pos          0
#define XCAN_R0_EID_Mask         (0x3FFFFu << XCAN_R0_EID_Pos)
#define XCAN_R0_EID_GET(value)   (((uint32_t)(value) & XCAN_R0_EID_Mask) >> XCAN_R0_EID_Pos) //!< Get Extended Identifier filter
#define XCAN_R0_SID_Pos          18
#define XCAN_R0_SID_Mask         (0x7FFu << XCAN_R0_SID_Pos)
#define XCAN_R0_SID_GET(value)   (((uint32_t)(value) & XCAN_R0_SID_Mask) >> XCAN_R0_SID_Pos) //!< Get Standard Identifier filter

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

#define XCAN_R1_FIDX_Pos              0
#define XCAN_R1_FIDX_Mask             (0xFFu << XCAN_R1_FIDX_Pos)
#define XCAN_R1_FIDX_SET(value)       (((uint32_t)(value) & XCAN_R1_FIDX_Mask) >> XCAN_R1_FIDX_Pos) //!< Get Filter index
#define XCAN_R1_FM                    (0x1u << 08) //!< Filter Match
#define XCAN_R1_BLK                   (0x1u << 09) //!< Black List
#define XCAN_R1_FAB                   (0x1u << 10) //!< Filter Aborted
#define XCAN_R1_CANXL_DLC_Pos         16
#define XCAN_R1_CANXL_DLC_Mask        (0x7FFu << XCAN_R1_CANXL_DLC_Pos)
#define XCAN_R1_CANXL_DLC_SET(value)  (((uint32_t)(value) & XCAN_R1_CANXL_DLC_Mask) >> XCAN_R1_CANXL_DLC_Pos) //!< Get Data Length Code with CAN XL encoding
#define XCAN_R1_DLC_Pos               16
#define XCAN_R1_DLC_Mask              (0xFu << XCAN_R1_DLC_Pos)
#define XCAN_R1_DLC_SET(value)        (((uint32_t)(value) & XCAN_R1_DLC_Mask) >> XCAN_R1_DLC_Pos) //!< Get Data Length Code for CAN2.0 and CAN-FD
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
  // Status information of the PRT (Base Address: 0x00, Address Range: 0x20)
  RegXCAN_ENDN                   = 0x900u, //!< (Offset: 0x900) Endianness Test Register
  RegXCAN_PREL                   = 0x904u, //!< (Offset: 0x904) PRT Release Identification Register
  RegXCAN_STAT                   = 0x908u, //!< (Offset: 0x908) PRT Status Register
                                           //   (Offset: 0x90C..0x91C) Reserved
  // Event information of the PRT (Base Address: 0x20, Address Range: 0x20)
  RegXCAN_EVNT                   = 0x920u, //!< (Offset: 0x920) Event Status Flags Register
                                           //   (Offset: 0x924..0x93C) Reserved
  // Control of the PRT during runtime (Base Address: 0x40, Address Range: 0x20)
  RegXCAN_LOCK                   = 0x940u, //!< (Offset: 0x940) Unlock Sequence Register
  RegXCAN_CTRL                   = 0x944u, //!< (Offset: 0x944) Control Register
  RegXCAN_FIMC                   = 0x948u, //!< (Offset: 0x948) Fault Injection Module Control Register
  RegXCAN_TEST                   = 0x94Cu, //!< (Offset: 0x94C) Hardware Test functions register
                                           //   (Offset: 0x950..0x95C) Reserved
  // Configuration of the PRT before runtime (Base Address: 0x60, Address Range: 0x20)
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

/*! XCAN RX FIFO Queue n Current Address Pointers.
 * RX FIFO Queue 0 Current Address Pointer (Read-Only, Offset: 0x420, Initial value: 0x00000000).
 * RX FIFO Queue 1 Current Address Pointer (Read-Only, Offset: 0x438, Initial value: 0x00000000).
 * RX FIFO Queue 2 Current Address Pointer (Read-Only, Offset: 0x450, Initial value: 0x00000000).
 * RX FIFO Queue 3 Current Address Pointer (Read-Only, Offset: 0x468, Initial value: 0x00000000).
 * RX FIFO Queue 4 Current Address Pointer (Read-Only, Offset: 0x480, Initial value: 0x00000000).
 * RX FIFO Queue 5 Current Address Pointer (Read-Only, Offset: 0x498, Initial value: 0x00000000).
 * RX FIFO Queue 6 Current Address Pointer (Read-Only, Offset: 0x4B0, Initial value: 0x00000000).
 * RX FIFO Queue 7 Current Address Pointer (Read-Only, Offset: 0x4C8, Initial value: 0x00000000).
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RX_FQ_ADD_PT_Register
{
  uint32_t RX_FQ_ADD_PT;           /*!< Provide the current RX Header Descriptor address pointer for the RX FIFO Queue 0 in the system memory.
                                    *   To follow RX descriptor over time, refer to the RX_DESC_ADD_PT register. This address value is always word aligned (32bit)
                                    */
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_RX_FQ_ADD_PT_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RX_FQ_ADD_PT_Register, 4);

#define XCAN_RX_FQ_ADD_PT_Pos         0
#define XCAN_RX_FQ_ADD_PT_Mask        (0xFFFFFFFCu << XCAN_RX_FQ_RD_ADD_PT_Pos)
#define XCAN_RX_FQ_ADD_PT_GET(value)  ((uint32_t)(value) & XCAN_RX_FQ_RD_ADD_PT_Mask) >> XCAN_RX_FQ_RD_ADD_PT_Pos) //!< Get the current RX Header Descriptor address pointer for the RX FIFO Queue 0 in the system memory



/*! XCAN RX FIFO Queue n link list Start Addresses.
 * RX FIFO Queue 0 link list Start Address (Read-Only, Offset: 0x424, Initial value: 0x00000000).
 * RX FIFO Queue 1 link list Start Address (Read-Only, Offset: 0x43C, Initial value: 0x00000000).
 * RX FIFO Queue 2 link list Start Address (Read-Only, Offset: 0x454, Initial value: 0x00000000).
 * RX FIFO Queue 3 link list Start Address (Read-Only, Offset: 0x46C, Initial value: 0x00000000).
 * RX FIFO Queue 4 link list Start Address (Read-Only, Offset: 0x484, Initial value: 0x00000000).
 * RX FIFO Queue 5 link list Start Address (Read-Only, Offset: 0x49C, Initial value: 0x00000000).
 * RX FIFO Queue 6 link list Start Address (Read-Only, Offset: 0x4B4, Initial value: 0x00000000).
 * RX FIFO Queue 7 link list Start Address (Read-Only, Offset: 0x4CC, Initial value: 0x00000000).
 * This register is only accessible in write mode if the RX FIFO Queue n is not busy, see BUSY flag in RX_FQ_STSn register.
 * This register is protected by a register bank CRC defined in CRC_REG register
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RX_FQ_START_ADD_Register
{
  uint32_t RX_FQ_START_ADD;        /*!< Define the start address of the RX FIFO Queue link list descriptor in system memory.
                                    *   The VAL[1:0] bits are always assumed to be 0b00 whatever the value written. This address value must always be word aligned (32bit).
                                    *   This register is only accessible in write mode if the RX FIFO Queue n is not busy, see BUSY flag in RX_FQ_STSn register
                                    */
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_RX_FQ_START_ADD_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RX_FQ_START_ADD_Register, 4);

#define XCAN_RX_FQ_ADD_PT_Pos         0
#define XCAN_RX_FQ_ADD_PT_Mask        (0xFFFFFFFCu << XCAN_RX_FQ_RD_ADD_PT_Pos)
#define XCAN_RX_FQ_ADD_PT_GET(value)  ((uint32_t)(value) & XCAN_RX_FQ_RD_ADD_PT_Mask) >> XCAN_RX_FQ_RD_ADD_PT_Pos) //!< Get the current RX Header Descriptor address pointer for the RX FIFO Queue 0 in the system memory



/*! XCAN RX FIFO Queue 0 link list and data container Sizes.
 * RX FIFO Queue 0 link list and data container Size (Read/Write, Offset: 0x428, Initial value: 0x00000000).
 * RX FIFO Queue 1 link list and data container Size (Read/Write, Offset: 0x440, Initial value: 0x00000000).
 * RX FIFO Queue 2 link list and data container Size (Read/Write, Offset: 0x458, Initial value: 0x00000000).
 * RX FIFO Queue 3 link list and data container Size (Read/Write, Offset: 0x470, Initial value: 0x00000000).
 * RX FIFO Queue 4 link list and data container Size (Read/Write, Offset: 0x488, Initial value: 0x00000000).
 * RX FIFO Queue 5 link list and data container Size (Read/Write, Offset: 0x4A0, Initial value: 0x00000000).
 * RX FIFO Queue 6 link list and data container Size (Read/Write, Offset: 0x4B8, Initial value: 0x00000000).
 * RX FIFO Queue 7 link list and data container Size (Read/Write, Offset: 0x4D0, Initial value: 0x00000000).
 * This register is only accessible in write mode if the RX FIFO Queue n is not busy, see BUSY flag in RX_FQ_STSn register.
 * This register is protected by a register bank CRC defined in CRC_REG register
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RX_FQ_SIZE_Register
{
  uint32_t RX_FQ_SIZE;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MAX_DESC: 10; /*!<  0- 9 - Define the maximum number of descriptors in the RX FIFO Queue link list. It is important to note that MAX_DESC = 0 does not prevent the RX FIFO Queue to be enabled and started.
                            *           An active and running RX FIFO Queue with MAX_DESC = 0 is not allowed and will result in a DESC_ERR interrupt if no RX descriptor is defined.
                            *           The size to be allocated to the link list must be equal to MAX_DESC * 16bytes for MAX_DESC >= 1.
                            *           This register is only accessible in write mode if the RX FIFO Queue 0 is not busy, see BUSY flag in RX_FQ_STS0 register
                            */
    uint32_t         :  8; //!< 10-15
    uint32_t DC_SIZE : 12; /*!< 16-27 - In Normal mode only the DC_SIZE[6:0] is used to define the maximum size of an RX data container for the RX FIFO Queue.
                            *           The data container size is DC_SIZE[6:0] * 32bytes and one is attached to every RX descriptor.
                            *           In continuous mode, it defines the size of the single data container used to write all RX messages. The overall data container size is DC_SIZE[11:0] * 32bytes for MAX_DESC > = 1.
                            *           When set to 0, the RX FIFO Queue can be enabled but not started. This register is only accessible in write mode if the RX FIFO Queue 0 is not busy, see BUSY flag in RX_FQ_STS0 register
                            */
    uint32_t         :  4; //!< 28-31
  } Bits;
} XCAN_RX_FQ_SIZE_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RX_FQ_SIZE_Register, 4);

#define XCAN_RX_FQ_SIZE_MAX_DESC_Pos         0
#define XCAN_RX_FQ_SIZE_MAX_DESC_Mask        (0x3Fu << XCAN_RX_FQ_SIZE_MAX_DESC_Pos)
#define XCAN_RX_FQ_SIZE_MAX_DESC_GET(value)  ((uint32_t)(value) & XCAN_RX_FQ_SIZE_MAX_DESC_Mask) >> XCAN_RX_FQ_SIZE_MAX_DESC_Pos) //!< Get the maximum number of descriptors in the RX FIFO Queue link list
#define XCAN_RX_FQ_SIZE_MAX_DESC_SET(value)  ((uint32_t)(value) << XCAN_RX_FQ_SIZE_MAX_DESC_Pos) & XCAN_RX_FQ_SIZE_MAX_DESC_Mask) //!< Set the maximum number of descriptors in the RX FIFO Queue link list
#define XCAN_RX_FQ_SIZE_DC_SIZE_Pos          16
#define XCAN_RX_FQ_SIZE_DC_SIZE_Mask         (0xFFFu << XCAN_RX_FQ_SIZE_DC_SIZE_Pos)
#define XCAN_RX_FQ_SIZE_DC_SIZE_GET(value)   ((uint32_t)(value) & XCAN_RX_FQ_SIZE_DC_SIZE_Mask) >> XCAN_RX_FQ_SIZE_DC_SIZE_Pos) //!< Get the maximum size of an RX data container for the RX FIFO Queue
#define XCAN_RX_FQ_SIZE_DC_SIZE_SET(value)   ((uint32_t)(value) << XCAN_RX_FQ_SIZE_DC_SIZE_Pos) & XCAN_RX_FQ_SIZE_DC_SIZE_Mask) //!< Set the maximum size of an RX data container for the RX FIFO Queue



/*! XCAN RX FIFO Queue n Data Container Start Addresses.
 * RX FIFO Queue 0 Data Container Start Address (Read-Only, Offset: 0x42C, Initial value: 0x00000000).
 * RX FIFO Queue 1 Data Container Start Address (Read-Only, Offset: 0x444, Initial value: 0x00000000).
 * RX FIFO Queue 2 Data Container Start Address (Read-Only, Offset: 0x45C, Initial value: 0x00000000).
 * RX FIFO Queue 3 Data Container Start Address (Read-Only, Offset: 0x474, Initial value: 0x00000000).
 * RX FIFO Queue 4 Data Container Start Address (Read-Only, Offset: 0x48C, Initial value: 0x00000000).
 * RX FIFO Queue 5 Data Container Start Address (Read-Only, Offset: 0x4A4, Initial value: 0x00000000).
 * RX FIFO Queue 6 Data Container Start Address (Read-Only, Offset: 0x4BC, Initial value: 0x00000000).
 * RX FIFO Queue 7 Data Container Start Address (Read-Only, Offset: 0x4D4, Initial value: 0x00000000).
 * This register is accessible in write mode if the RX FIFO Queue n is not busy, see BUSY flag in RX_FQ_STSn register.
 * This register is protected by a register bank CRC defined in CRC_REG register. This register is used only in Continuous Mode
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RX_FQ_DC_START_ADD_Register
{
  uint32_t RX_FQ_DC_START_ADD;     /*!< Define the Data Container Start Address in system memory. This bit field is relevant only when the MH is configured in Continuous Mode.
                                    *   The VAL[1:0] bits are always assumed to be 0b00 whatever the value written. This address value must always be word aligned (32bit).
                                    *   This register is only accessible in write mode if the RX FIFO Queue n is not busy, see BUSY flag in RX_FQ_STSn register
                                    */
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_RX_FQ_DC_START_ADD_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RX_FQ_DC_START_ADD_Register, 4);

#define XCAN_RX_FQ_DC_START_ADD_Pos         0
#define XCAN_RX_FQ_DC_START_ADD_Mask        (0xFFFFFFFCu << XCAN_RX_FQ_DC_START_ADD_Pos)
#define XCAN_RX_FQ_DC_START_ADD_GET(value)  ((uint32_t)(value) & XCAN_RX_FQ_DC_START_ADD_Mask) >> XCAN_RX_FQ_DC_START_ADD_Pos) //!< Get 
#define XCAN_RX_FQ_DC_START_ADD_SET(value)  ((uint32_t)(value) << XCAN_RX_FQ_DC_START_ADD_Pos) & XCAN_RX_FQ_DC_START_ADD_Mask) //!< Set 



/*! XCAN RX FIFO Queue n Read Address Pointers.
 * RX FIFO Queue 0 Read Address Pointers (Read/Write, Offset: 0x430, Initial value: 0x00000000).
 * RX FIFO Queue 1 Read Address Pointers (Read/Write, Offset: 0x448, Initial value: 0x00000000).
 * RX FIFO Queue 2 Read Address Pointers (Read/Write, Offset: 0x460, Initial value: 0x00000000).
 * RX FIFO Queue 3 Read Address Pointers (Read/Write, Offset: 0x478, Initial value: 0x00000000).
 * RX FIFO Queue 4 Read Address Pointers (Read/Write, Offset: 0x490, Initial value: 0x00000000).
 * RX FIFO Queue 5 Read Address Pointers (Read/Write, Offset: 0x4A8, Initial value: 0x00000000).
 * RX FIFO Queue 6 Read Address Pointers (Read/Write, Offset: 0x4C0, Initial value: 0x00000000).
 * RX FIFO Queue 7 Read Address Pointers (Read/Write, Offset: 0x4D8, Initial value: 0x00000000).
 * This register is used only in Continuous Mode
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RX_FQ_RD_ADD_PT_Register
{
  uint32_t RX_FQ_RD_ADD_PT;        /*!< The SW uses this register to indicate the Data Read Address of the RX message being read to the MH.
                                    *   This address must point to the last word of the RX message considered in the data container. This bit field is relevant only when the MH is configured in Continuous mode.
                                    *   The MH uses this information to ensure enough memory space is available to write the next message.
                                    *   For an initial start, it is mandatory to set VAL[1:0] to 0b11, to avoid RX_FQ_RD_ADD_PTn register to be equal to the RX_FQ_START_ADDRn register.
                                    *   Excepted for the initial value, the address value must always be word aligned (32bit), VAL[1:0] must be set to 0b00
                                    */
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_RX_FQ_RD_ADD_PT_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RX_FQ_RD_ADD_PT_Register, 4);

#define XCAN_RX_FQ_RD_ADD_PT_Pos         0
#define XCAN_RX_FQ_RD_ADD_PT_Mask        (0xFFFFFFFCu << XCAN_RX_FQ_RD_ADD_PT_Pos)
#define XCAN_RX_FQ_RD_ADD_PT_GET(value)  ((uint32_t)(value) & XCAN_RX_FQ_RD_ADD_PT_Mask) >> XCAN_RX_FQ_RD_ADD_PT_Pos) //!< Get the Data Read Address of the RX message being read to the MH
#define XCAN_RX_FQ_RD_ADD_PT_SET(value)  ((uint32_t)(value) << XCAN_RX_FQ_RD_ADD_PT_Pos) & XCAN_RX_FQ_RD_ADD_PT_Mask) //!< Set the Data Read Address of the RX message being read to the MH

//-----------------------------------------------------------------------------

/*! XCAN TX Filter Control register 0 (Read/Write, Offset: 0x600, Initial value: 0x00000000)
 * This register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register.
 * The register is accessible in write access in privileged mode only. This register is protected by a register bank CRC defined in CRC_REG register
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_FILTER_CTRL0_Register
{
  uint32_t TX_FILTER_CTRL0;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t COMB  :  8; /*!<  0-15 - When COMB[n] =1 the comparison attached to the reference values (REF_VAL0 and REF_VAL1) or (REF_VAL2 and REF_VAL3) are required to accept a TX message.
                          *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                          */
    uint32_t MASK  :  8; /*!< 16-31 - When MASK[n] =1 the reference values REF_VAL0/1 or REF_VAL2/3 are combined to define a value (REF_VAL0 or REF_VAL2) and a mask (REF_VAL1 or REF_VAL3).
                          *           Otherwise, the comparison uses the REF_VAL0/1/2/3 bit field as reference value only.
                          *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                          */
    uint32_t MODE  :  1; //!< 16    - When set to 1 accept on match, otherwise reject on match. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
    uint32_t CAN_FD:  1; //!< 17    - When set to 1 reject CAN-FD messages, otherwise accept them. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
    uint32_t CC_CAN:  1; //!< 18    - When set to 1 reject Classic CAN messages, otherwise accept them. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
    uint32_t EN    :  1; //!< 19    - When set to 1, enable the TX filter for all TX message to be sent. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
    uint32_t IRQ_EN:  1; /*!< 20    - When set to 1, enable the interrupt tx_filter_irq to be triggered. The interrupt is triggered when a message is rejected.
                          *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                          */
    uint32_t       : 11; //!< 21-31
  } Bits;
} XCAN_TX_FILTER_CTRL0_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_FILTER_CTRL0_Register, 4);

#define XCAN_TX_FILTER_CTRL0_COMB_Pos           0
#define XCAN_TX_FILTER_CTRL0_COMB_Mask          (0xFFu << XCAN_TX_FILTER_CTRL0_COMB_Pos)
#define XCAN_TX_FILTER_CTRL0_COMB_GET(value)    ((uint32_t)(value) & XCAN_TX_FILTER_CTRL0_COMB_Mask) >> XCAN_TX_FILTER_CTRL0_COMB_Pos) //!< Get the comparison attached to the reference values
#define XCAN_TX_FILTER_CTRL0_COMB_SET(value)    ((uint32_t)(value) << XCAN_TX_FILTER_CTRL0_COMB_Pos) & XCAN_TX_FILTER_CTRL0_COMB_Mask) //!< Set the comparison attached to the reference values
#define XCAN_TX_FILTER_CTRL0_MASK_Pos           8
#define XCAN_TX_FILTER_CTRL0_MASK_Mask          (0xFFu << XCAN_TX_FILTER_CTRL0_MASK_Pos)
#define XCAN_TX_FILTER_CTRL0_MASK_GET(value)    ((uint32_t)(value) & XCAN_TX_FILTER_CTRL0_MASK_Mask) >> XCAN_TX_FILTER_CTRL0_MASK_Pos) //!< Get the reference values REF_VAL0/1 or REF_VAL2/3 are combined to define a value and a mask
#define XCAN_TX_FILTER_CTRL0_MASK_SET(value)    ((uint32_t)(value) << XCAN_TX_FILTER_CTRL0_MASK_Pos) & XCAN_TX_FILTER_CTRL0_MASK_Mask) //!< Set the reference values REF_VAL0/1 or REF_VAL2/3 are combined to define a value and a mask
#define XCAN_RX_FILTER_CTRL0_ACCEPT_ON_MATCH    (1u <<  16) //!< Accept on match
#define XCAN_RX_FILTER_CTRL0_REJECT_CANFD_MSG   (1u <<  17) //!< Reject CAN-FD messages
#define XCAN_RX_FILTER_CTRL0_REJECT_CAN20_MSG   (1u <<  18) //!< Reject Classic CAN messages
#define XCAN_RX_FILTER_CTRL0_ENABLE_TX_FILTER   (1u <<  19) //!< Enable the TX filter for all TX message to be sent
#define XCAN_RX_FILTER_CTRL0_ENABLE_FILTER_INT  (1u <<  20) //!< Enable the interrupt tx_filter_irq to be triggered

//-----------------------------------------------------------------------------

/*! XCAN TX Filter Control register 1 (Read/Write, Offset: 0x604, Initial value: 0x00000000)
 * This register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register.
 * The register is accessible in write access in privileged mode only. This register is protected by a register bank CRC defined in CRC_REG register
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_FILTER_CTRL1_Register
{
  uint32_t TX_FILTER_CTRL1;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t VALID: 16; /*!<  0-15 - When VALID[n] = 1 the reference value defined for the TX filter n is valid.
                         *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register.
                         *           The valid reference value used is defined as follow:
                         *           VALID[n] is assigned to TX_FILTER_REFVAL0.REF_VAL{n} (n € {0, 1, 2, 3})
                         *           VALID[n+4] is assigned to TX_FILTER_REFVAL1.REF_VAL{n} (n € {0, 1, 2, 3})
                         *           VALID[n+8] is assigned to TX_FILTER_REFVAL2.REF_VAL{n} (n € {0, 1, 2, 3})
                         *           VALID[n+12] is assigned to TX_FILTER_REFVAL3.REF_VAL{n} (n € {0, 1, 2, 3})
                         */
    uint32_t FIELD: 16; /*!< 16-31 - When FIELD[n] = 1 the TX filter element n is considering SDT, otherwise VCID, to compare with the reference value defined in TX_FILTER_REFVAL0/1/2/3.
                         *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register.
                         *           The reference value to be set is defined as follow:
                         *           FIELD[n] is assigned to TX_FILTER_REFVAL0.REF_VAL{n} (n € {0, 1, 2, 3})
                         *           FIELD[n+4] is assigned to TX_FILTER_REFVAL1.REF_VAL{n} (n € {0, 1, 2, 3})
                         *           FIELD[n+8] is assigned to TX_FILTER_REFVAL2.REF_VAL{n} (n € {0, 1, 2, 3})
                         *           FIELD[n+12] is assigned to TX_FILTER_REFVAL3.REF_VAL{n} (n € {0, 1, 2, 3})
                         */
  } Bits;
} XCAN_TX_FILTER_CTRL1_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_FILTER_CTRL1_Register, 4);

#define XCAN_TX_FILTER_CTRL1_VALID_Pos         0
#define XCAN_TX_FILTER_CTRL1_VALID_Mask        (0xFFFFu << XCAN_TX_FILTER_CTRL1_VALID_Pos)
#define XCAN_TX_FILTER_CTRL1_VALID_GET(value)  ((uint32_t)(value) & XCAN_TX_FILTER_CTRL1_VALID_Mask) >> XCAN_TX_FILTER_CTRL1_VALID_Pos) //!< Get the reference value defined for the TX filter n is valid
#define XCAN_TX_FILTER_CTRL1_VALID_SET(value)  ((uint32_t)(value) << XCAN_TX_FILTER_CTRL1_VALID_Pos) & XCAN_TX_FILTER_CTRL1_VALID_Mask) //!< Set the reference value defined for the TX filter n is valid
#define XCAN_TX_FILTER_CTRL1_FIELD_Pos         16
#define XCAN_TX_FILTER_CTRL1_FIELD_Mask        (0xFFFFu << XCAN_TX_FILTER_CTRL1_FIELD_Pos)
#define XCAN_TX_FILTER_CTRL1_FIELD_GET(value)  ((uint32_t)(value) & XCAN_TX_FILTER_CTRL1_FIELD_Mask) >> XCAN_TX_FILTER_CTRL1_FIELD_Pos) //!< Get the TX filter element n is considering SDT
#define XCAN_TX_FILTER_CTRL1_FIELD_SET(value)  ((uint32_t)(value) << XCAN_TX_FILTER_CTRL1_FIELD_Pos) & XCAN_TX_FILTER_CTRL1_FIELD_Mask) //!< Set the TX filter element n is considering SDT

//-----------------------------------------------------------------------------

/*! XCAN TX Filter Reference Value registers.
 * TX Filter Reference Value register 0 (Read/Write, Offset: 0x608, Initial value: 0x00000000).
 * TX Filter Reference Value register 1 (Read/Write, Offset: 0x60C, Initial value: 0x00000000).
 * TX Filter Reference Value register 2 (Read/Write, Offset: 0x610, Initial value: 0x00000000).
 * TX Filter Reference Value register 3 (Read/Write, Offset: 0x614, Initial value: 0x00000000).
 * Theses registers are only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register.
 * Theses registers are accessible in write access in privileged mode only. Theses registers are protected by a register bank CRC defined in CRC_REG register
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_FILTER_REFVAL_Register
{
  uint32_t TX_FILTER_REFVAL;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t REF_VAL0: 8; //!<  0- 7 - Define the reference value 0. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
    uint32_t REF_VAL1: 8; //!<  8-15 - Define the reference value 1. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
    uint32_t REF_VAL2: 8; //!< 16-23 - Define the reference value 2. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
    uint32_t REF_VAL3: 8; //!< 54-31 - Define the reference value 3. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
  } Bits;
} XCAN_TX_FILTER_REFVAL_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_FILTER_REFVAL_Register, 4);

#define XCAN_TX_FILTER_REFVAL0_Pos         0
#define XCAN_TX_FILTER_REFVAL0_Mask        (0xFFu << XCAN_TX_FILTER_REFVAL0_Pos)
#define XCAN_TX_FILTER_REFVAL0_GET(value)  ((uint32_t)(value) & XCAN_TX_FILTER_REFVAL0_Mask) >> XCAN_TX_FILTER_REFVAL0_Pos) //!< Get the reference value 0
#define XCAN_TX_FILTER_REFVAL0_SET(value)  ((uint32_t)(value) << XCAN_TX_FILTER_REFVAL0_Pos) & XCAN_TX_FILTER_REFVAL0_Mask) //!< Set the reference value 0
#define XCAN_TX_FILTER_REFVAL1_Pos         8
#define XCAN_TX_FILTER_REFVAL1_Mask        (0xFFu << XCAN_TX_FILTER_REFVAL1_Pos)
#define XCAN_TX_FILTER_REFVAL1_GET(value)  ((uint32_t)(value) & XCAN_TX_FILTER_REFVAL1_Mask) >> XCAN_TX_FILTER_REFVAL1_Pos) //!< Get the reference value 1
#define XCAN_TX_FILTER_REFVAL1_SET(value)  ((uint32_t)(value) << XCAN_TX_FILTER_REFVAL1_Pos) & XCAN_TX_FILTER_REFVAL1_Mask) //!< Set the reference value 1
#define XCAN_TX_FILTER_REFVAL2_Pos         16
#define XCAN_TX_FILTER_REFVAL2_Mask        (0xFFu << XCAN_TX_FILTER_REFVAL2_Pos)
#define XCAN_TX_FILTER_REFVAL2_GET(value)  ((uint32_t)(value) & XCAN_TX_FILTER_REFVAL2_Mask) >> XCAN_TX_FILTER_REFVAL2_Pos) //!< Get the reference value 2
#define XCAN_TX_FILTER_REFVAL2_SET(value)  ((uint32_t)(value) << XCAN_TX_FILTER_REFVAL2_Pos) & XCAN_TX_FILTER_REFVAL2_Mask) //!< Set the reference value 2
#define XCAN_TX_FILTER_REFVAL3_Pos         24
#define XCAN_TX_FILTER_REFVAL3_Mask        (0xFFu << XCAN_TX_FILTER_REFVAL3_Pos)
#define XCAN_TX_FILTER_REFVAL3_GET(value)  ((uint32_t)(value) & XCAN_TX_FILTER_REFVAL3_Mask) >> XCAN_TX_FILTER_REFVAL3_Pos) //!< Get the reference value 3
#define XCAN_TX_FILTER_REFVAL3_SET(value)  ((uint32_t)(value) << XCAN_TX_FILTER_REFVAL3_Pos) & XCAN_TX_FILTER_REFVAL3_Mask) //!< Set the reference value 3

//-----------------------------------------------------------------------------

/*! XCAN RX Filter Control register (Read/Write, Offset: 0x680, Initial value: 0x00000000)
 * This register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register.
 * The register is accessible in write access in privileged mode only. This register is protected by a register bank CRC defined in CRC_REG register
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RX_FILTER_CTRL_Register
{
  uint32_t RX_FILTER_CTRL;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t NB_FE    :  8; /*!<  0- 7 - Define the number of RX filter elements that are defined in the local memory.
                             *           When set to 0, all RX messages are accepted and stored in the RX FIFO Queue number defined by ANMF_FQ[3:0].
                             *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                             */
    uint32_t THRESHOLD:  5; /*!<  8-12 - THRESHOLD defines the latest point in time to wait for the result of the RX filtering process. Once this limit is reached, the MH starts fetching an RX descriptor from S_MEM.
                             *           THRESHOLD value is only used when greater than 0 and ANFF bit set to 1. See chapter "RX Filter" for an explanation how to configure THRESHOLD.
                             *           When the RX filtering is not providing the result before the threshold of the RX DMA FIFO is reached, the RX message is sent to the default RX FIFO Queue mentioned in the ANMF_FQ[2:0] (only enabled when ANFF set to 1).
                             *           When the level is over the threshold and the RX filtering result is already known, no action is taken.
                             *           Threshold is given in number of word of 32bit. This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                             */
    uint32_t          :  3; //!< 13-15
    uint32_t ANMF_FQ  :  3; /*!< 16-18 - Define the default RX FIFO Queue number (from 0 to 7) when non matching frames are accepted (ANMF = 1) AND/OR when the threshold mechanism is active (ANFF = 1).
                             *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                             */
    uint32_t          :  1; //!< 19
    uint32_t ANMF     :  1; /*!< 20    - When set to 1, non matching frames are accepted, otherwise rejected.
                             *           It is mandatory to have the default RX FIFO Queue defined in the ANMF_FQ[2:0] bit field, enabled and started (see RX_FQ_CTRL2 and RX_FQ_CTRL0 registers).
                             *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                             */
    uint32_t ANFF     :  1; /*!< 21    - When set to 1, frames not filtered in time and over the DMA RX FIFO level defined in THRESHOLD[4:0], are routed to the default RX FIFO Queue (defined by the ANMF_FQ[2:0] bit field).
                             *           It is mandatory to have the default RX FIFO Queue defined in the ANMF_FQ[2:0] bit field, enabled and started (see RX_FQ_CTRL2 and RX_FQ_CTRL0 registers).
                             *           This bit field register is only accessible in write mode if the MH is not busy, see BUSY flag in MH_STS register
                             */
    uint32_t          : 10; //!< 22-31
  } Bits;
} XCAN_RX_FILTER_CTRL_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RX_FILTER_CTRL_Register, 4);

#define XCAN_RX_FILTER_CTRL_NB_FE_Pos                   0
#define XCAN_RX_FILTER_CTRL_NB_FE_Mask                  (0xFFu << XCAN_RX_FILTER_CTRL_NB_FE_Pos)
#define XCAN_RX_FILTER_CTRL_NB_FE_GET(value)            ((uint32_t)(value) & XCAN_RX_FILTER_CTRL_NB_FE_Mask) >> XCAN_RX_FILTER_CTRL_NB_FE_Pos) //!< Get the number of RX filter elements that are defined in the local memory
#define XCAN_RX_FILTER_CTRL_NB_FE_SET(value)            ((uint32_t)(value) << XCAN_RX_FILTER_CTRL_NB_FE_Pos) & XCAN_RX_FILTER_CTRL_NB_FE_Mask) //!< Set the number of RX filter elements that are defined in the local memory
#define XCAN_RX_FILTER_CTRL_THRESHOLD_Pos               8
#define XCAN_RX_FILTER_CTRL_THRESHOLD_Mask              (0x1Fu << XCAN_RX_FILTER_CTRL_THRESHOLD_Pos)
#define XCAN_RX_FILTER_CTRL_THRESHOLD_GET(value)        ((uint32_t)(value) & XCAN_RX_FILTER_CTRL_THRESHOLD_Mask) >> XCAN_RX_FILTER_CTRL_THRESHOLD_Pos) //!< Get the latest point in time to wait for the result of the RX filtering process
#define XCAN_RX_FILTER_CTRL_THRESHOLD_SET(value)        ((uint32_t)(value) << XCAN_RX_FILTER_CTRL_THRESHOLD_Pos) & XCAN_RX_FILTER_CTRL_THRESHOLD_Mask) //!< Set the latest point in time to wait for the result of the RX filtering process
#define XCAN_RX_FILTER_CTRL_ANMF_FQ_Pos                 16
#define XCAN_RX_FILTER_CTRL_ANMF_FQ_Mask                (0x7u << XCAN_RX_FILTER_CTRL_ANMF_FQ_Pos)
#define XCAN_RX_FILTER_CTRL_ANMF_FQ_GET(value)          ((uint32_t)(value) & XCAN_RX_FILTER_CTRL_ANMF_FQ_Mask) >> XCAN_RX_FILTER_CTRL_ANMF_FQ_Pos) //!< Get the default RX FIFO Queue number
#define XCAN_RX_FILTER_CTRL_ANMF_FQ_SET(value)          ((uint32_t)(value) << XCAN_RX_FILTER_CTRL_ANMF_FQ_Pos) & XCAN_RX_FILTER_CTRL_ANMF_FQ_Mask) //!< Set the default RX FIFO Queue number
#define XCAN_RX_FILTER_CTRL_ACCEPT_NON_MATCHING_FRAMES  (1u <<  20) //!< Non matching frames are accepted
#define XCAN_RX_FILTER_CTRL_ROUTE_NOT_FILTERED_IN_TIME  (1u <<  21) //!< Frames not filtered in time and over the DMA RX FIFO level defined in THRESHOLD

//-----------------------------------------------------------------------------

//! XCAN TX FIFO Queue Interrupt Status register (Read/Write, Offset: 0x700, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_FQ_INT_STS_Register
{
  uint32_t TX_FQ_INT_STS;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t SENT   : 8; //!<  0- 7 - When SENT[n] = 1, a TX message was sent from the TX FIFO Queue n and writing a 1 clears the bit
    uint32_t        : 8; //!<  8-15
    uint32_t UNVALID: 8; //!< 16-23 - When TX FIFO Queue n loads a TX descriptor with VALID = 0, the bit UNVALID[n] will be set. Writing 1 to UNVALID[n] clears the bit
    uint32_t        : 8; //!< 24-31
  } Bits;
} XCAN_TX_FQ_INT_STS_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_FQ_INT_STS_Register, 4);

#define XCAN_TX_FQ_INT_STS_RECEIVED_Pos         0
#define XCAN_TX_FQ_INT_STS_RECEIVED_Mask        (0xFFu << XCAN_TX_FQ_INT_STS_RECEIVED_Pos)
#define XCAN_TX_FQ_INT_STS_RECEIVED_GET(value)  ((uint32_t)(value) & XCAN_TX_FQ_INT_STS_RECEIVED_Mask) >> XCAN_TX_FQ_INT_STS_RECEIVED_Pos) //!< Get TX message was sent from the TX FIFO Queue n
#define XCAN_TX_FQ_INT_STS_RECEIVED_SET(value)  ((uint32_t)(value) << XCAN_TX_FQ_INT_STS_RECEIVED_Pos) & XCAN_TX_FQ_INT_STS_RECEIVED_Mask) //!< Set TX message was sent from the TX FIFO Queue n
#define XCAN_TX_FQ_INT_STS_UNVALID_Pos          16
#define XCAN_TX_FQ_INT_STS_UNVALID_Mask         (0xFFu << XCAN_TX_FQ_INT_STS_UNVALID_Pos)
#define XCAN_TX_FQ_INT_STS_UNVALID_GET(value)   ((uint32_t)(value) & XCAN_TX_FQ_INT_STS_UNVALID_Mask) >> XCAN_TX_FQ_INT_STS_UNVALID_Pos) //!< Get TX FIFO Queue n loads TX descriptor
#define XCAN_TX_FQ_INT_STS_UNVALID_SET(value)   ((uint32_t)(value) << XCAN_TX_FQ_INT_STS_UNVALID_Pos) & XCAN_TX_FQ_INT_STS_UNVALID_Mask) //!< Set TX FIFO Queue n loads TX descriptor

//-----------------------------------------------------------------------------

//! XCAN RX FIFO Queue Interrupt Status register (Read/Write, Offset: 0x704, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_RX_FQ_INT_STS_Register
{
  uint32_t RX_FQ_INT_STS;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t RECEIVED: 8; //!<  0- 7 - When RECEIVED[n] = 1, an RX message was received in the RX FIFO Queue n, writing a 1 clears the bit
    uint32_t         : 8; //!<  8-15
    uint32_t UNVALID : 8; //!< 16-23 - When RX FIFO Queue n loads an RX descriptor with VALID=0, the bit UNVALID[n] will be set. Writing 1 to UNVALID[n] clears the bit
    uint32_t         : 8; //!< 24-31
  } Bits;
} XCAN_RX_FQ_INT_STS_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_RX_FQ_INT_STS_Register, 4);

#define XCAN_RX_FQ_INT_STS_RECEIVED_Pos         0
#define XCAN_RX_FQ_INT_STS_RECEIVED_Mask        (0xFFu << XCAN_RX_FQ_INT_STS_RECEIVED_Pos)
#define XCAN_RX_FQ_INT_STS_RECEIVED_GET(value)  ((uint32_t)(value) & XCAN_RX_FQ_INT_STS_RECEIVED_Mask) >> XCAN_RX_FQ_INT_STS_RECEIVED_Pos) //!< Get RX message was received in the RX FIFO Queue n
#define XCAN_RX_FQ_INT_STS_RECEIVED_SET(value)  ((uint32_t)(value) << XCAN_RX_FQ_INT_STS_RECEIVED_Pos) & XCAN_RX_FQ_INT_STS_RECEIVED_Mask) //!< Set RX message was received in the RX FIFO Queue n
#define XCAN_RX_FQ_INT_STS_UNVALID_Pos          16
#define XCAN_RX_FQ_INT_STS_UNVALID_Mask         (0xFFu << XCAN_RX_FQ_INT_STS_UNVALID_Pos)
#define XCAN_RX_FQ_INT_STS_UNVALID_GET(value)   ((uint32_t)(value) & XCAN_RX_FQ_INT_STS_UNVALID_Mask) >> XCAN_RX_FQ_INT_STS_UNVALID_Pos) //!< Get RX FIFO Queue n loads RX descriptor
#define XCAN_RX_FQ_INT_STS_UNVALID_SET(value)   ((uint32_t)(value) << XCAN_RX_FQ_INT_STS_UNVALID_Pos) & XCAN_RX_FQ_INT_STS_UNVALID_Mask) //!< Set RX FIFO Queue n loads RX descriptor

//-----------------------------------------------------------------------------

//! XCAN TX Priority Queue Interrupt Status register 0 register (Read/Write, Offset: 0x708, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_PQ_INT_STS0_Register
{
  uint32_t TX_PQ_INT_STS0;         //!< When SENT[n] = 1 TX message was sent from the TX Priority Queue slot n, writing a 1 clears the bit
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_TX_PQ_INT_STS0_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_PQ_INT_STS0_Register, 4);

#define XCAN_TX_PQ_INT_STS0_Pos         0
#define XCAN_TX_PQ_INT_STS0_Mask        (0xFFFFFFFFu << XCAN_TX_PQ_INT_STS0_Pos)
#define XCAN_TX_PQ_INT_STS0_GET(value)  ((uint32_t)(value) & XCAN_TX_PQ_INT_STS0_Mask) >> XCAN_TX_PQ_INT_STS0_Pos) //!< Get TX Priority Queue slot n message sent
#define XCAN_TX_PQ_INT_STS0_SET(value)  ((uint32_t)(value) << XCAN_TX_PQ_INT_STS0_Pos) & XCAN_TX_PQ_INT_STS0_Mask) //!< Set TX Priority Queue slot n message sent

//-----------------------------------------------------------------------------

//! XCAN TX Priority Queue Interrupt Status register 1 (Read/Write, Offset: 0x70C, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_PQ_INT_STS1_Register
{
  uint32_t TX_PQ_INT_STS1;         /*!< When UNVALID[n] = 1, an invalid RX descriptor is detected while running the TX Priority Queue slot n.
                                    *   Writing a 1 clears the bit. When set to 1, the TX Priority Queue slot n is on hold, waiting for the SW to react.
                                    *   As the TX message is fully defined in system memory before starting the relevant slot, there should not be any invalid TX descriptor interrupts
                                    */
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_TX_PQ_INT_STS1_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_PQ_INT_STS1_Register, 4);

#define XCAN_TX_PQ_INT_STS1_Pos         0
#define XCAN_TX_PQ_INT_STS1_Mask        (0xFFFFFFFFu << XCAN_TX_PQ_INT_STS1_Pos)
#define XCAN_TX_PQ_INT_STS1_GET(value)  ((uint32_t)(value) & XCAN_TX_PQ_INT_STS1_Mask) >> XCAN_TX_PQ_INT_STS1_Pos) //!< Get an invalid RX descriptor slot n is detected while running the TX Priority Queue
#define XCAN_TX_PQ_INT_STS1_SET(value)  ((uint32_t)(value) << XCAN_TX_PQ_INT_STS1_Pos) & XCAN_TX_PQ_INT_STS1_Mask) //!< Set an invalid RX descriptor slot n is detected while running the TX Priority Queue

//-----------------------------------------------------------------------------

//! XCAN Statistics Interrupt Status register (Read/Write, Offset: 0x710, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_STATS_INT_STS_Register
{
  uint32_t STATS_INT_STS;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t TX_SUCC  :  1; //!< 0    - Counter of TX message transmitted successfully has wrapped, writing a 1 clears the bit
    uint32_t TX_UNSUCC:  1; //!< 1    - Counter of TX message transmitted unsuccessfully has wrapped, writing a 1 clears the bit
    uint32_t RX_SUCC  :  1; //!< 2    - Counter of RX message received successfully has wrapped, writing a 1 clears the bit
    uint32_t RX_UNSUCC:  1; //!< 3    - Counter of RX message received unsuccessfully has wrapped, writing a 1 clears the bit
    uint32_t          : 28; //!< 4-31
  } Bits;
} XCAN_STATS_INT_STS_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_STATS_INT_STS_Register, 4);

#define XCAN_STATS_INT_STS_TX_SUCC    (1u <<  0) //!< Counter of TX message transmitted successfully has wrapped
#define XCAN_STATS_INT_STS_TX_UNSUCC  (1u <<  1) //!< Counter of TX message transmitted unsuccessfully has wrapped
#define XCAN_STATS_INT_STS_RX_SUCC    (1u <<  2) //!< Counter of RX message received successfully has wrapped
#define XCAN_STATS_INT_STS_RX_UNSUCC  (1u <<  3) //!< Counter of RX message received unsuccessfully has wrapped

#define XCAN_STAT_INT_STATUS_FLAGS  ( XCAN_STATS_INT_STS_TX_SUCC | XCAN_STATS_INT_STS_TX_UNSUCC | \
                                      XCAN_STATS_INT_STS_RX_SUCC | XCAN_STATS_INT_STS_RX_UNSUCC )   //!< All stats events status flags

//! Stats Interrupt Events, can be OR'ed.
typedef enum
{
  XCAN_STATS_INT_NO_EVENT                            = 0x00000000,                   //!< No stats interrupt events
  XCAN_STATS_INT_TX_MSG_SUCCESS_COUNTER_WRAP_EVENT   = XCAN_STATS_INT_STS_TX_SUCC,   //!< Counter of TX message transmitted successfully has wrapped
  XCAN_STATS_INT_TX_MSG_UNSUCCESS_COUNTER_WRAP_EVENT = XCAN_STATS_INT_STS_TX_UNSUCC, //!< Counter of TX message transmitted unsuccessfully has wrapped
  XCAN_STATS_INT_RX_MSG_SUCCESS_COUNTER_WRAP_EVENT   = XCAN_STATS_INT_STS_RX_SUCC,   //!< Counter of RX message received successfully has wrapped
  XCAN_STATS_INT_RX_MSG_UNSUCCESS_COUNTER_WRAP_EVENT = XCAN_STATS_INT_STS_RX_UNSUCC, //!< Counter of RX message received unsuccessfully has wrapped

  XCAN_STATS_INT_ENABLE_ALL_EVENTS                   = XCAN_STAT_INT_STATUS_FLAGS,   //!< Enable all safety events
  XCAN_STATS_INT_EVENTS_STATUS_FLAGS_MASK            = XCAN_STAT_INT_STATUS_FLAGS,   //!< Stats events flags mask
} eXCAN_StatsInterruptEvents;

typedef eXCAN_StatsInterruptEvents setXCAN_StatsInterruptEvents; //! Set of Stats Interrupt Events (can be OR'ed)

//-----------------------------------------------------------------------------

//! XCAN Error Interrupt Status register (Read/Write, Offset: 0x714, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_ERR_INT_STS_Register
{
  uint32_t ERR_INT_STS;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t DP_TX_ACK_DO_ERR :  1; //!< 0    - When set to 1, a TX acknowledge data overflow is detected, writing a 1 clears the bit
    uint32_t DP_RX_FIFO_DO_ERR:  1; //!< 1    - When set to 1, a RX DMA FIFO overflow issue is detected, writing a 1 clears the bit
    uint32_t DP_RX_ACK_DO_ERR :  1; //!< 2    - When set to 1, a RX acknowledge data overflow is detected, writing a 1 clears the bit
    uint32_t DP_TX_SEQ_ERR    :  1; //!< 3    - When set to 1, a TX sequence issue is detected, writing a 1 clears the bit
    uint32_t DP_RX_SEQ_ERR    :  1; //!< 4    - When set to 1, a RX sequence issue is detected, writing a 1 clears the bit
    uint32_t                  : 27; //!< 5-31
  } Bits;
} XCAN_ERR_INT_STS_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_ERR_INT_STS_Register, 4);

#define XCAN_ERR_INT_STS_DP_TX_ACK_DO_ERR   (1u <<  0) //!< A TX acknowledge data overflow is detected
#define XCAN_ERR_INT_STS_DP_RX_FIFO_DO_ERR  (1u <<  1) //!< A RX DMA FIFO overflow issue is detected
#define XCAN_ERR_INT_STS_DP_RX_ACK_DO_ERR   (1u <<  2) //!< A RX acknowledge data overflow is detected
#define XCAN_ERR_INT_STS_DP_TX_SEQ_ERR      (1u <<  3) //!< A TX sequence issue is detected
#define XCAN_ERR_INT_STS_DP_RX_SEQ_ERR      (1u <<  4) //!< A RX sequence issue is detected

#define XCAN_ERR_INT_STATUS_FLAGS  ( XCAN_ERR_INT_STS_DP_TX_ACK_DO_ERR | XCAN_ERR_INT_STS_DP_RX_FIFO_DO_ERR | XCAN_ERR_INT_STS_DP_RX_ACK_DO_ERR | \
                                     XCAN_ERR_INT_STS_DP_TX_SEQ_ERR    | XCAN_ERR_INT_STS_DP_RX_SEQ_ERR )                                           //!< All error events status flags

//! Error interrupt Events, can be OR'ed.
typedef enum
{
  XCAN_ERROR_INT_NO_EVENT                   = 0x00000000,                         //!< No error interrupt events
  XCAN_ERROR_INT_TX_ACK_OVERFLOW_EVENT      = XCAN_ERR_INT_STS_DP_TX_ACK_DO_ERR,  //!< A TX acknowledge data overflow is detected
  XCAN_ERROR_INT_RX_DMA_FIFO_OVERFLOW_EVENT = XCAN_ERR_INT_STS_DP_RX_FIFO_DO_ERR, //!< A RX DMA FIFO overflow issue is detected
  XCAN_ERROR_INT_RX_ACK_OVERFLOW_EVENT      = XCAN_ERR_INT_STS_DP_RX_ACK_DO_ERR,  //!< A RX acknowledge data overflow is detected
  XCAN_ERROR_INT_TX_SEQUENCE_ISSUE_EVENT    = XCAN_ERR_INT_STS_DP_TX_SEQ_ERR,     //!< A TX sequence issue is detected
  XCAN_ERROR_INT_RX_SEQUENCE_ISSUE_EVENT    = XCAN_ERR_INT_STS_DP_RX_SEQ_ERR,     //!< A RX sequence issue is detected

  XCAN_ERROR_INT_ENABLE_ALL_EVENTS          = XCAN_ERR_INT_STATUS_FLAGS,          //!< Enable all safety events
  XCAN_ERROR_INT_EVENTS_STATUS_FLAGS_MASK   = XCAN_ERR_INT_STATUS_FLAGS,          //!< Safety events flags mask
} eXCAN_ErrorInterruptEvents;

typedef eXCAN_ErrorInterruptEvents setXCAN_ErrorInterruptEvents; //! Set of Error Interrupt Events (can be OR'ed)

//-----------------------------------------------------------------------------

//! XCAN Safety Interrupt Status register (Read/Write, Offset: 0x718, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_SFTY_INT_STS_Register
{
  uint32_t SFTY_INT_STS;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t DMA_AXI_RW_R_TO_ERR:  1; //!<  0    - When set to 1, an AXI write access timeout issue is detected on DMA interface, writing a 1 clears the bit
    uint32_t DMA_AXI_RD_TO_ERR  :  1; //!<  1    - When set to 1, an AXI read access timeout issue is detected on DMA interface, writing a 1 clears the bit
    uint32_t DP_PRT_TX_TO_ERR   :  1; //!<  2    - When set to 1, a TX_MSG timeout issue is detected, writing a 1 clears the bit
    uint32_t DP_PRT_RX_TO_ERR   :  1; //!<  3    - When set to 1, an RX_MSG timeout issue is detected, writing a 1 clears the bit
    uint32_t MEM_AXI_RW_R_TO_ERR:  1; //!<  4    - When set to 1, an AXI write access timeout issue is detected on local memory interface, writing a 1 clears the bit
    uint32_t MEM_AXI_RD_TO_ERR  :  1; //!<  5    - When set to 1, an AXI read access timeout issue is detected on local memory interface, writing a 1 clears the bit
    uint32_t DP_TX_PARITY_ERR   :  1; //!<  6    - When set to 1, a TX data parity error is detected on datapath, writing a 1 clears the bit
    uint32_t DP_RX_PARITY_ERR   :  1; //!<  7    - When set to 1, an RX data parity error is detected on datapath, writing a 1 clears the bit
    uint32_t AP_TX_PARITY_ERR   :  1; //!<  8    - When set to 1, a TX address pointer parity issue is detected, writing a 1 clears the bit
    uint32_t AP_RX_PARITY_ERR   :  1; //!<  9    - When set to 1, an RX address pointer parity issue is detected, writing a 1 clears the bit
    uint32_t TX_DESC_REQ_ERR    :  1; //!< 10    - When set to 1, a TX descriptor fetched does not match the one expected, writing a 1 clears the bit
    uint32_t TX_DESC_CRC_ERR    :  1; //!< 11    - When set to 1, a TX descriptor has a wrong CRC, writing a 1 clears the bit
    uint32_t RX_DESC_REQ_ERR    :  1; //!< 12    - When set to 1, an RX descriptor fetched does not match the one expected, writing a 1 clears the bit
    uint32_t RX_DESC_CRC_ERR    :  1; //!< 13    - When set to 1, an RX descriptor has a wrong CRC, writing a 1 clears the bit
    uint32_t MEM_SFTY_UE        :  1; //!< 14    - When set to 1, an uncorrectable error is detected on the local memory interface
    uint32_t MEM_SFTY_CE        :  1; //!< 15    - When set to 1, a correctable error is detected on the local memory interface
    uint32_t ACK_TX_PARITY_ERR  :  1; //!< 16    - When set to 1, an acknowledge data parity issue is detected on the TX path
    uint32_t ACK_RX_PARITY_ERR  :  1; //!< 17    - When set to 1, an acknowledge data parity issue is detected on the RX path
    uint32_t                    : 14; //!< 18-31
  } Bits;
} XCAN_SFTY_INT_STS_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_SFTY_INT_STS_Register, 4);

#define XCAN_SFTY_INT_STS_DMA_AXI_RW_R_TO_ERR  (1u <<  0) //!< An AXI write access timeout issue is detected on DMA interface
#define XCAN_SFTY_INT_STS_DMA_AXI_RD_TO_ERR    (1u <<  1) //!< An AXI read access timeout issue is detected on DMA interface
#define XCAN_SFTY_INT_STS_DP_PRT_TX_TO_ERR     (1u <<  2) //!< A TX_MSG timeout issue is detected
#define XCAN_SFTY_INT_STS_DP_PRT_RX_TO_ERR     (1u <<  3) //!< A RX_MSG timeout issue is detected
#define XCAN_SFTY_INT_STS_MEM_AXI_RW_R_TO_ERR  (1u <<  4) //!< An AXI write access timeout issue is detected on local memory interface
#define XCAN_SFTY_INT_STS_MEM_AXI_RD_TO_ERR    (1u <<  5) //!< An AXI read access timeout issue is detected on local memory interface
#define XCAN_SFTY_INT_STS_DP_TX_PARITY_ERR     (1u <<  6) //!< A TX data parity error is detected on datapath
#define XCAN_SFTY_INT_STS_DP_RX_PARITY_ERR     (1u <<  7) //!< A RX data parity error is detected on datapath
#define XCAN_SFTY_INT_STS_AP_TX_PARITY_ERR     (1u <<  8) //!< A TX address pointer parity issue is detected
#define XCAN_SFTY_INT_STS_AP_RX_PARITY_ERR     (1u <<  9) //!< A RX address pointer parity issue is detected
#define XCAN_SFTY_INT_STS_TX_DESC_REQ_ERR      (1u << 10) //!< A TX descriptor fetched does not match the one expected
#define XCAN_SFTY_INT_STS_TX_DESC_CRC_ERR      (1u << 11) //!< A TX descriptor has a wrong CRC
#define XCAN_SFTY_INT_STS_RX_DESC_REQ_ERR      (1u << 12) //!< A RX descriptor fetched does not match the one expected
#define XCAN_SFTY_INT_STS_RX_DESC_CRC_ERR      (1u << 13) //!< A RX descriptor has a wrong CRC
#define XCAN_SFTY_INT_STS_MEM_SFTY_UE          (1u << 14) //!< An uncorrectable error is detected on the local memory interface
#define XCAN_SFTY_INT_STS_MEM_SFTY_CE          (1u << 15) //!< A correctable error is detected on the local memory interface
#define XCAN_SFTY_INT_STS_ACK_TX_PARITY_ERR    (1u << 16) //!< An acknowledge data parity issue is detected on the TX path
#define XCAN_SFTY_INT_STS_ACK_RX_PARITY_ERR    (1u << 17) //!< An acknowledge data parity issue is detected on the RX path

#define XCAN_SAFTY_INT_STATUS_FLAGS  ( XCAN_SFTY_INT_STS_DMA_AXI_RW_R_TO_ERR | XCAN_SFTY_INT_STS_DMA_AXI_RD_TO_ERR | XCAN_SFTY_INT_STS_DP_PRT_TX_TO_ERR | XCAN_SFTY_INT_STS_DP_PRT_RX_TO_ERR | \
                                       XCAN_SFTY_INT_STS_MEM_AXI_RW_R_TO_ERR | XCAN_SFTY_INT_STS_MEM_AXI_RD_TO_ERR |                                                                           \
                                       XCAN_SFTY_INT_STS_DP_TX_PARITY_ERR    | XCAN_SFTY_INT_STS_DP_RX_PARITY_ERR  | XCAN_SFTY_INT_STS_AP_TX_PARITY_ERR | XCAN_SFTY_INT_STS_AP_RX_PARITY_ERR | \
                                       XCAN_SFTY_INT_STS_TX_DESC_REQ_ERR     | XCAN_SFTY_INT_STS_TX_DESC_CRC_ERR   | XCAN_SFTY_INT_STS_RX_DESC_REQ_ERR  | XCAN_SFTY_INT_STS_RX_DESC_CRC_ERR  | \
                                       XCAN_SFTY_INT_STS_MEM_SFTY_UE         | XCAN_SFTY_INT_STS_MEM_SFTY_CE       |                                                                           \
                                       XCAN_SFTY_INT_STS_ACK_TX_PARITY_ERR   | XCAN_SFTY_INT_STS_ACK_RX_PARITY_ERR )                                                                             //!< All safety events status flags

//! Safety Interrupt Events, can be OR'ed.
typedef enum
{
  XCAN_SAFETY_INT_NO_EVENT                              = 0x00000000,                            //!< No safety interrupt events
  XCAN_SAFETY_INT_AXI_WRITE_ACCESS_TIMEOUT_ON_DMA_EVENT = XCAN_SFTY_INT_STS_DMA_AXI_RW_R_TO_ERR, //!< An AXI write access timeout issue is detected on DMA interface
  XCAN_SAFETY_INT_AXI_READ_ACCESS_TIMEOUT_ON_DMA_EVENT  = XCAN_SFTY_INT_STS_DMA_AXI_RD_TO_ERR,   //!< An AXI read access timeout issue is detected on DMA interface
  XCAN_SAFETY_INT_TX_MESSAGE_TIMEOUT_EVENT              = XCAN_SFTY_INT_STS_DP_PRT_TX_TO_ERR,    //!< A TX_MSG timeout issue is detected
  XCAN_SAFETY_INT_RX_MESSAGE_TIMEOUT_EVENT              = XCAN_SFTY_INT_STS_DP_PRT_RX_TO_ERR,    //!< A RX_MSG timeout issue is detected
  XCAN_SAFETY_INT_AXI_WRITE_ACCESS_TIMEOUT_LMEM_EVENT   = XCAN_SFTY_INT_STS_MEM_AXI_RW_R_TO_ERR, //!< An AXI write access timeout issue is detected on local memory interface
  XCAN_SAFETY_INT_AXI_READ_ACCESS_TIMEOUT_LMEM_EVENT    = XCAN_SFTY_INT_STS_MEM_AXI_RD_TO_ERR,   //!< An AXI read access timeout issue is detected on local memory interface
  XCAN_SAFETY_INT_TX_DATA_PARITY_ERROR_EVENT            = XCAN_SFTY_INT_STS_DP_TX_PARITY_ERR,    //!< A TX data parity error is detected on datapath
  XCAN_SAFETY_INT_RX_DATA_PARITY_ERROR_EVENT            = XCAN_SFTY_INT_STS_DP_RX_PARITY_ERR,    //!< A RX data parity error is detected on datapath
  XCAN_SAFETY_INT_TX_ADDRESS_POINTER_PARITY_ERROR_EVENT = XCAN_SFTY_INT_STS_AP_TX_PARITY_ERR,    //!< A TX address pointer parity issue is detected
  XCAN_SAFETY_INT_RX_ADDRESS_POINTER_PARITY_ERROR_EVENT = XCAN_SFTY_INT_STS_AP_RX_PARITY_ERR,    //!< A RX address pointer parity issue is detected
  XCAN_SAFETY_INT_TX_DESC_FETCH_NO_MATCH_EVENT          = XCAN_SFTY_INT_STS_TX_DESC_REQ_ERR,     //!< A TX descriptor fetched does not match the one expected
  XCAN_SAFETY_INT_TX_DESC_WRONG_CRC_EVENT               = XCAN_SFTY_INT_STS_TX_DESC_CRC_ERR,     //!< A TX descriptor has a wrong CRC
  XCAN_SAFETY_INT_RX_DESC_FETCH_NO_MATCH_EVENT          = XCAN_SFTY_INT_STS_RX_DESC_REQ_ERR,     //!< A RX descriptor fetched does not match the one expected
  XCAN_SAFETY_INT_RX_DESC_WRONG_CRC_EVENT               = XCAN_SFTY_INT_STS_RX_DESC_CRC_ERR,     //!< A RX descriptor has a wrong CRC
  XCAN_SAFETY_INT_UNCORRECTABLE_ERROR_ON_LMEM_EVENT     = XCAN_SFTY_INT_STS_MEM_SFTY_UE,         //!< An uncorrectable error is detected on the local memory interface
  XCAN_SAFETY_INT_CORRECTABLE_ERROR_ON_LMEM_EVENT       = XCAN_SFTY_INT_STS_MEM_SFTY_CE,         //!< A correctable error is detected on the local memory interface
  XCAN_SAFETY_INT_ACK_DATA_PERITY_ISSUE_TX_PATH_EVENT   = XCAN_SFTY_INT_STS_ACK_TX_PARITY_ERR,   //!< An acknowledge data parity issue is detected on the TX path
  XCAN_SAFETY_INT_ACK_DATA_PERITY_ISSUE_RX_PATH_EVENT   = XCAN_SFTY_INT_STS_ACK_RX_PARITY_ERR,   //!< An acknowledge data parity issue is detected on the RX path

  XCAN_SAFETY_INT_ENABLE_ALL_EVENTS                     = XCAN_SAFTY_INT_STATUS_FLAGS,           //!< Enable all safety events
  XCAN_SAFETY_INT_EVENTS_STATUS_FLAGS_MASK              = XCAN_SAFTY_INT_STATUS_FLAGS,           //!< Safety events flags mask
} eXCAN_SafetyInterruptEvents;

typedef eXCAN_SafetyInterruptEvents setXCAN_SafetyInterruptEvents; //! Set of Safety Interrupt Events (can be OR'ed)

//-----------------------------------------------------------------------------

//! XCAN DMA Error Information register (Read-Only, Offset: 0x71C, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_AXI_ERR_INFO_Register
{
  uint32_t AXI_ERR_INFO;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t DMA_ID  :  2; //!< 0- 1 - On DMA_AXI interface. Define the AXI ID used when a write or read error response is detected. According to the value, the DMA channel can be identified and so it is possible to define what's the effect of such issue
    uint32_t DMA_RESP:  2; //!< 2- 3 - On DMA_AXI interface. When set to 0b10, the AXI response is SLVERR. When set to 0b11, the response is DECERR. By default, set to 0b00 (OKAY)
    uint32_t MEM_ID  :  2; //!< 4- 5 - On MEM_AXI interface. Define the AXI ID used when a write or read error response is detected. According to the value, the DMA channel can be identified and so it is possible to define what's the effect of such issue
    uint32_t MEM_RESP:  2; //!< 6- 7 - On MEM_AXI interface. When set to 0b10, the AXI response is SLVERR. When set to 0b11, the response is DECERR. By default, set to 0b00 (OKAY)
    uint32_t         : 24; //!< 8-31
  } Bits;
} XCAN_AXI_ERR_INFO_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_AXI_ERR_INFO_Register, 4);

//! AXI error enumerator
typedef enum
{
  XCAN_AXI_OKAY   = 0b00, //!< AXI response is OKAY
  XCAN_AXI_RES    = 0b01, //!< AXI response is reserved
  XCAN_AXI_SLVERR = 0b10, //!< AXI response is SLVERR
  XCAN_AXI_DECERR = 0b11, //!< AXI response is DECERR
} eXCAN_AXIerror;

#define XCAN_AXI_ERR_INFO_DMA_ID_Pos           0
#define XCAN_AXI_ERR_INFO_DMA_ID_Mask          (0x3u << XCAN_DESC_ERR_INFO1_FQN_PQSN_Pos)
#define XCAN_AXI_ERR_INFO_DMA_ID_GET(value)    ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_FQN_PQSN_Mask) >> XCAN_DESC_ERR_INFO1_FQN_PQSN_Pos) //!< Get the AXI ID used when a write or read error response is detected
#define XCAN_AXI_ERR_INFO_DMA_RESP_Pos         2
#define XCAN_AXI_ERR_INFO_DMA_RESP_Mask        (0x3u << XCAN_DESC_ERR_INFO1_IN_Pos)
#define XCAN_AXI_ERR_INFO_DMA_RESP_GET(value)  ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_IN_Mask) >> XCAN_DESC_ERR_INFO1_IN_Pos) //!< Get DMA_AXI error
#define XCAN_AXI_ERR_INFO_MEM_ID_Pos           4
#define XCAN_AXI_ERR_INFO_MEM_ID_Mask          (0x3u << XCAN_DESC_ERR_INFO1_RC_Pos)
#define XCAN_AXI_ERR_INFO_MEM_ID_GET(value)    ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_RC_Mask) >> XCAN_DESC_ERR_INFO1_RC_Pos) //!< Get the AXI ID used when a write or read error response is detected
#define XCAN_AXI_ERR_INFO_MEM_RESP_Pos         6
#define XCAN_AXI_ERR_INFO_MEM_RESP_Mask        (0x3u << XCAN_DESC_ERR_INFO1_CRC_Pos)
#define XCAN_AXI_ERR_INFO_MEM_RESP_GET(value)  ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_CRC_Mask) >> XCAN_DESC_ERR_INFO1_CRC_Pos) //!< Get MEM_AXI error

//-----------------------------------------------------------------------------

/*! XCAN Descriptor Error Information 0 register (Read-Only, Offset: 0x720, Initial value: 0x00000000)
 * If the DESC_ERR_INFO0.ADD[31:16] = 0 and DESC_ERR_INFO1.CRC[8:0], DESC_ERR_INFO1.RX_TX and DESC_ERR_INFO1.RC[4:0] are all equal to 0, the faulty descriptor is a TX descriptor fetched from L_MEM
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_DESC_ERR_INFO0_Register
{
  uint32_t ADD;                    //!< Descriptor address being used when the error is detected
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_DESC_ERR_INFO0_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_DESC_ERR_INFO0_Register, 4);

#define XCAN_DESC_ERR_INFO0_ADD_Pos         0
#define XCAN_DESC_ERR_INFO0_ADD_Mask        (0xFFFFFFFFu << XCAN_DESC_ERR_INFO0_ADD_Pos)
#define XCAN_DESC_ERR_INFO0_ADD_GET(value)  ((uint32_t)(value) & XCAN_DESC_ERR_INFO0_ADD_Mask) >> XCAN_DESC_ERR_INFO0_ADD_Pos) //!< Get Descriptor address being used when the error is detected

//-----------------------------------------------------------------------------

/*! XCAN Descriptor Error Information 1 register (Read-Only, Offset: 0x724, Initial value: 0x00000000)
 * When the DESC_ERR_INFO1.CRC[8:0], DESC_ERR_INFO1.RX_TX and DESC_ERR_INFO1.RC[4:0] are all equal to 0, the faulty descriptor is a TX descriptor fetched from L_MEM only if the DESC_ERR_INFO0.ADD[31:16] = 0
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_DESC_ERR_INFO1_Register
{
  uint32_t DESC_ERR_INFO1;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t FQN_PQSN: 5; //!<  0- 4 - Provide the information regarding the RX/TX FIFO Queue number or the TX Priority Queue slot having an issue
    uint32_t IN      : 3; //!<  5- 7 - Provide the instance number defined in RX or TX descriptor logged in
    uint32_t PQ      : 1; //!<  8    - Identify which TX queue is impacted, either the TX Priority Queue (PQ set to 1) or the TX FIFO Queues
    uint32_t RC      : 5; //!<  9-13 - Provide the information regarding the Rolling Counter defined in RX or TX descriptor impacted
    uint32_t         : 1; //!< 14
    uint32_t RX_TX   : 1; //!< 15    - RX descriptor has an issue (RX_TX set to 1), otherwise the same for a TX descriptor
    uint32_t CRC     : 9; //!< 16-24 - CRC value defined in the RX or TX descriptor logged in
    uint32_t         : 7; //!< 25-31
  } Bits;
} XCAN_DESC_ERR_INFO1_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_DESC_ERR_INFO1_Register, 4);

#define XCAN_DESC_ERR_INFO1_FQN_PQSN_Pos                0
#define XCAN_DESC_ERR_INFO1_FQN_PQSN_Mask               (0x1Fu << XCAN_DESC_ERR_INFO1_FQN_PQSN_Pos)
#define XCAN_DESC_ERR_INFO1_FQN_PQSN_GET(value)         ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_FQN_PQSN_Mask) >> XCAN_DESC_ERR_INFO1_FQN_PQSN_Pos) //!< Get the information regarding the RX/TX FIFO Queue number or the TX Priority Queue slot having an issue
#define XCAN_DESC_ERR_INFO1_IN_Pos                      5
#define XCAN_DESC_ERR_INFO1_IN_Mask                     (0x7u << XCAN_DESC_ERR_INFO1_IN_Pos)
#define XCAN_DESC_ERR_INFO1_IN_GET(value)               ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_IN_Mask) >> XCAN_DESC_ERR_INFO1_IN_Pos) //!< Get the instance number defined in RX or TX descriptor logged in
#define XCAN_DESC_ERR_INFO1_PQ_TX_PRIORITY_QUEUE        (1u << 8) //!< TX Priority queue is impacted
#define XCAN_DESC_ERR_INFO1_PQ_TX_FIFO_QUEUE            (0u << 8) //!< TX FIFO queue is impacted
#define XCAN_DESC_ERR_INFO1_RC_Pos                      9
#define XCAN_DESC_ERR_INFO1_RC_Mask                     (0x1Fu << XCAN_DESC_ERR_INFO1_RC_Pos)
#define XCAN_DESC_ERR_INFO1_RC_GET(value)               ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_RC_Mask) >> XCAN_DESC_ERR_INFO1_RC_Pos) //!< Get the information regarding the Rolling Counter defined in RX or TX descriptor impacted
#define XCAN_DESC_ERR_INFO1_RX_DESCRIPTOR_HAS_AN_ISSUE  (1u << 15) //!< RX descriptor has an issue
#define XCAN_DESC_ERR_INFO1_TX_DESCRIPTOR_HAS_AN_ISSUE  (0u << 15) //!< TX descriptor has an issue
#define XCAN_DESC_ERR_INFO1_CRC_Pos                     16
#define XCAN_DESC_ERR_INFO1_CRC_Mask                    (0x1FFu << XCAN_DESC_ERR_INFO1_CRC_Pos)
#define XCAN_DESC_ERR_INFO1_CRC_GET(value)              ((uint32_t)(value) & XCAN_DESC_ERR_INFO1_CRC_Mask) >> XCAN_DESC_ERR_INFO1_CRC_Pos) //!< Get CRC value defined in the RX or TX descriptor logged in

//-----------------------------------------------------------------------------

//! XCAN TX Filter Error Information register (Read-Only, Offset: 0x728, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_FILTER_ERR_INFO_Register
{
  uint32_t TX_FILTER_ERR_INFO;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t FQ     :  1; //!< 0    - When set to 1, one of the TX FIFO Queues has triggered the TX_FILTER_ERR interrupt, otherwise it is a TX Priority Queue slot
    uint32_t FQN_PQS:  5; //!< 1- 5 - Provide the information of the TX FIFO Queue number or TX Priority Queue slot number which has triggered the TX_FILTER_ERR interrupt
    uint32_t        : 26; //!< 6-31
  } Bits;
} XCAN_TX_FILTER_ERR_INFO_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_FILTER_ERR_INFO_Register, 4);

#define XCAN_TX_FILTER_ERR_INFO_FIFO_QUEUE_TRIGGERED      (1u << 0) //!< One of the TX FIFO Queues has triggered the TX_FILTER_ERR interrupt
#define XCAN_TX_FILTER_ERR_INFO_PRIORITY_QUEUE_TRIGGERED  (0u << 0) //!< One of the TX Priority Queues slot has triggered the TX_FILTER_ERR interrupt
#define XCAN_TX_FILTER_ERR_INFO_FQN_PQS_Pos               1
#define XCAN_TX_FILTER_ERR_INFO_FQN_PQS_Mask              (0x1Fu << XCAN_TX_FILTER_ERR_INFO_FQN_PQS_Pos)
#define XCAN_TX_FILTER_ERR_INFO_FQN_PQS_GET(value)        ((uint32_t)(value) & XCAN_TX_FILTER_ERR_INFO_FQN_PQS_Mask) >> XCAN_TX_FILTER_ERR_INFO_FQN_PQS_Pos) //!< Get the information of the TX FIFO Queue number or TX Priority Queue slot number

//-----------------------------------------------------------------------------

/*! XCAN Debug Control register (Read/Write, Offset: 0x800, Initial value: 0x00000000)
 * This register is only accessible in write mode if the Test Mode Key sequence has been performed prior to write. The register is accessible in write access in privileged mode only.
 * This register is protected by a register bank CRC defined in CRC_REG register
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_DEBUG_TEST_CTRL_Register
{
  uint32_t DEBUG_TEST_CTRL;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t TEST_IRQ_EN: 1; /*!<  0    - When writing 1, enable the control of the interrupt lines using the INT_TEST0 and INT_TEST1 registers.
                              *           This bit field register is only accessible in write mode if the Test Mode Key sequence has been performed prior to write
                              */
    uint32_t HDP_EN     : 1; /*!<  1    - When writing 1, enable the hardware debug port to monitor MH internal signals.
                              *           This bit field register is only accessible in write mode if the Test Mode Key sequence has been performed prior to write
                              */
    uint32_t            : 6; //!<  2- 7
    uint32_t HDP_SEL    : 3; /*!<  8-10 - Define the set of signals to be monitored on the HDP[15:0] bus signal interface.
                              *           This bit field register is only accessible in write mode if the Test Mode Key sequence has been performed prior to write
                              */
    uint32_t            : 21; //!< 11-31
  } Bits;
} XCAN_DEBUG_TEST_CTRL_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_DEBUG_TEST_CTRL_Register, 4);

#define XCAN_DEBUG_TEST_CTRL_TEST_IRQ_EN         (1u << 0) //!< The enable the control of the interrupt lines
#define XCAN_DEBUG_TEST_CTRL_TEST_IRQ_DIS        (0u << 0) //!< The disable the control of the interrupt lines
#define XCAN_DEBUG_TEST_CTRL_HDP_EN              (1u << 2) //!< The enable the hardware debug port to monitor MH internal signals
#define XCAN_DEBUG_TEST_CTRL_HDP_DIS             (0u << 2) //!< The disable the hardware debug port to monitor MH internal signals
#define XCAN_DEBUG_TEST_CTRL_HDP_SEL_Pos         8
#define XCAN_DEBUG_TEST_CTRL_HDP_SEL_Mask        (0x7u << XCAN_DEBUG_TEST_CTRL_HDP_SEL_Pos)
#define XCAN_DEBUG_TEST_CTRL_HDP_SEL_GET(value)  ((uint32_t)(value) & XCAN_DEBUG_TEST_CTRL_HDP_SEL_Mask) >> XCAN_DEBUG_TEST_CTRL_HDP_SEL_Pos) //!< Get the set of signals to be monitored on the HDP[15:0] bus signal interface
#define XCAN_DEBUG_TEST_CTRL_HDP_SEL_SET(value)  ((uint32_t)(value) << XCAN_DEBUG_TEST_CTRL_HDP_SEL_Pos) & XCAN_DEBUG_TEST_CTRL_HDP_SEL_Mask) //!< Set the set of signals to be monitored on the HDP[15:0] bus signal interface

//-----------------------------------------------------------------------------

/*! XCAN TX-SCAN first candidates register (Read/Write, Offset: 0x804, Initial value: 0x00000000)
 * This register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_INT_TEST0_Register
{
  uint32_t INT_TEST0;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t TX_FQ_IRQ: 8; /*!<  0- 7 - When writing 1 to TX_FQ_IRQ[n], triggers the interrupt line tx_fq_irq[n], those bits are auto-cleared.
                            *           This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
                            */
    uint32_t          : 8; //!<  8-15
    uint32_t RX_FQ_IRQ: 8; /*!< 16-23 - When writing 1 to RX_FQ_IRQ[n], triggers the interrupt line rx_fq_irq[n], those bits are auto-cleared.
                            *           This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
                            */
    uint32_t          : 8; //!< 24-31
  } Bits;
} XCAN_INT_TEST0_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_INT_TEST0_Register, 4);

#define XCAN_TX_SCAN_FC_FQ_PQ0_TX_PRIORITY_QUEUE  (1u << 0) //!< The first candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ0_TX_FIFO_QUEUE      (0u << 0) //!< The first candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN0_PQSN_Pos           1
#define XCAN_TX_SCAN_FQN_PQSN0_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN0_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN0_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN0_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN0_PQSN_Pos) //!< Get the first candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FQN_PQSN0_PQSN_SET(value)    ((uint32_t)(value) << XCAN_TX_SCAN_FQN_PQSN0_PQSN_Pos) & XCAN_TX_SCAN_FQN_PQSN0_PQSN_Mask) //!< Set the first candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FC_FQ_PQ1_TX_PRIORITY_QUEUE  (1u << 8) //!< The second candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ1_TX_FIFO_QUEUE      (0u << 8) //!< The second candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN1_PQSN_Pos           9
#define XCAN_TX_SCAN_FQN_PQSN1_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN1_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN1_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN1_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN1_PQSN_Pos) //!< Get the second candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FQN_PQSN1_PQSN_SET(value)    ((uint32_t)(value) << XCAN_TX_SCAN_FQN_PQSN1_PQSN_Pos) & XCAN_TX_SCAN_FQN_PQSN1_PQSN_Mask) //!< Set the second candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FC_FQ_PQ2_TX_PRIORITY_QUEUE  (1u << 16) //!< The third candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ2_TX_FIFO_QUEUE      (0u << 16) //!< The third candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN2_PQSN_Pos           17
#define XCAN_TX_SCAN_FQN_PQSN2_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN2_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN2_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN2_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN2_PQSN_Pos) //!< Get the third candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FQN_PQSN2_PQSN_SET(value)    ((uint32_t)(value) << XCAN_TX_SCAN_FQN_PQSN2_PQSN_Pos) & XCAN_TX_SCAN_FQN_PQSN2_PQSN_Mask) //!< Set the third candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FC_FQ_PQ3_TX_PRIORITY_QUEUE  (1u << 24) //!< The fourth candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ3_TX_FIFO_QUEUE      (0u << 24) //!< The fourth candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN3_PQSN_Pos           25
#define XCAN_TX_SCAN_FQN_PQSN3_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN3_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN3_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN3_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN3_PQSN_Pos) //!< Get the fourth candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FQN_PQSN3_PQSN_SET(value)    ((uint32_t)(value) << XCAN_TX_SCAN_FQN_PQSN3_PQSN_Pos) & XCAN_TX_SCAN_FQN_PQSN3_PQSN_Mask) //!< Set the fourth candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M

//-----------------------------------------------------------------------------

/*! XCAN Interrupt Test register 1 (Read/Write, Offset: 0x808, Initial value: 0x00000000)
 * This register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_INT_TEST1_Register
{
  uint32_t INT_TEST1;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t RESP0_ERR     :  1; //!<  0    - Writing a 1 to the bit field triggers the interrupt line 0, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t RESP1_ERR     :  1; //!<  1    - Writing a 1 to the bit field triggers the interrupt line 1, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t DMA_TO_ERR    :  1; //!<  2    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t DMA_CH_ERR    :  1; //!<  3    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t DP_TO_ERR     :  1; //!<  4    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t DP_DO_ERR     :  1; //!<  5    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t DP_SEQ_ERR    :  1; //!<  6    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t DP_PARITY_ERR :  1; //!<  7    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t AP_PARITY_ERR :  1; //!<  8    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t DESC_ERR      :  1; //!<  9    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t REG_CRC_ERR   :  1; //!< 10    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t MEM_SFTY_ERR  :  1; //!< 11    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t MEM_TO_ERR    :  1; //!< 12    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t RX_FILTER_ERR :  1; //!< 13    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t RX_ABORT_IRQ  :  1; //!< 14    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t TX_ABORT_IRQ  :  1; //!< 15    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t TX_FILTER_IRQ :  1; //!< 16    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t RX_FILTER_IRQ :  1; //!< 17    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t STOP_IRQ      :  1; //!< 18    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t STATS_IRQ     :  1; //!< 19    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t TX_PQ_IRQ     :  1; //!< 20    - Writing a 1 to the bit field triggers the related interrupt line, this bit is auto-cleared. This bit field register is only accessible in write mode if the TEST_IRQ_EN bit in DEBUG_TEST_CTRL is set
    uint32_t               : 11; //!< 21-31
  } Bits;
} XCAN_INT_TEST1_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_INT_TEST1_Register, 4);

#define XCAN_INT_TEST1_TRIGG_RESP0_ERR      (1u <<  0) //!< Triggers the interrupt line 0
#define XCAN_INT_TEST1_TRIGG_RESP1_ERR      (1u <<  1) //!< Triggers the interrupt line 1
#define XCAN_INT_TEST1_TRIGG_DMA_TO_ERR     (1u <<  2) //!< Triggers the DMA_TO_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_DMA_CH_ERR     (1u <<  3) //!< Triggers the DMA_CH_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_DP_TO_ERR      (1u <<  4) //!< Triggers the DP_TO_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_DP_DO_ERR      (1u <<  5) //!< Triggers the DP_DO_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_DP_SEQ_ERR     (1u <<  6) //!< Triggers the DP_SEQ_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_DP_PARITY_ERR  (1u <<  7) //!< Triggers the DP_PARITY_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_AP_PARITY_ERR  (1u <<  8) //!< Triggers the AP_PARITY_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_DESC_ERR       (1u <<  9) //!< Triggers the DESC_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_REG_CRC_ERR    (1u << 10) //!< Triggers the REG_CRC_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_MEM_SFTY_ERR   (1u << 11) //!< Triggers the MEM_SFTY_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_MEM_TO_ERR     (1u << 12) //!< Triggers the MEM_TO_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_RX_FILTER_ERR  (1u << 13) //!< Triggers the RX_FILTER_ERR related interrupt line
#define XCAN_INT_TEST1_TRIGG_RX_ABORT_IRQ   (1u << 14) //!< Triggers the RX_ABORT_IRQ related interrupt line
#define XCAN_INT_TEST1_TRIGG_TX_ABORT_IRQ   (1u << 15) //!< Triggers the TX_ABORT_IRQ related interrupt line
#define XCAN_INT_TEST1_TRIGG_TX_FILTER_IRQ  (1u << 16) //!< Triggers the TX_FILTER_IRQ related interrupt line
#define XCAN_INT_TEST1_TRIGG_RX_FILTER_IRQ  (1u << 17) //!< Triggers the RX_FILTER_IRQ related interrupt line
#define XCAN_INT_TEST1_TRIGG_STOP_IRQ       (1u << 18) //!< Triggers the STOP_IRQ related interrupt line
#define XCAN_INT_TEST1_TRIGG_STATS_IRQ      (1u << 19) //!< Triggers the STATS_IRQ related interrupt line
#define XCAN_INT_TEST1_TRIGG_TX_PQ_IRQ      (1u << 20) //!< Triggers the TX_PQ_IRQ related interrupt line

//-----------------------------------------------------------------------------

/*! XCAN TX-SCAN first candidates register (Read-only, Offset: 0x810, Initial value: 0x00000000)
 * This register gives the 4 best candidates evaluated by the TX-Scan. This register gives the first and second highest priority TX descriptor after a TX-Scan.
 * It provides also the third and fourth candidates during a TXScan, considering the first and second candidates as already defined by a previous TX-Scan run
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_SCAN_FC_Register
{
  uint32_t TX_SCAN_FC;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t FQ_PQ0   : 1; //!<  0    - The first candidate evaluated by TX-Scan is either a TX Priority Queue (when set to 1) or a TX FIFO Queue (when set to 0). This bit field is identical to TX_SCAN_BC.FH_PQ bit register
    uint32_t FQN_PQSN0: 5; /*!<  1- 5 - The first candidate is coming from either the TX FIFO Queue number N (defined by FQN in TX descriptor) or the TX Priority Queue Slot number M (defined by the PQSN in TX descriptor).
                            *           The meaning of this bit field depends on the PQ0. This bit field is identical to TX_SCAN_BC.FH_FQN_PQSN bit register
                            */
    uint32_t          : 2; //!<  6- 7
    uint32_t FQ_PQ1   : 1; //!<  8    - The second candidate evaluated by TX-Scan is either a TX Priority Queue (when set to 1) or a TX FIFO Queue (when set to 0). This bit field is identical to TX_SCAN_BC.SH_PQ bit register
    uint32_t FQN_PQSN1: 5; /*!<  9-13 - The second candidate is coming from either the TX FIFO Queue number N (defined by FQN in TX descriptor) or the TX Priority Queue Slot number M (defined by the PQSN in TX descriptor).
                            *           The meaning of this bit field depends on the PQ0. This bit field is identical to the TX_SCAN_BC.SH_FQN_PQSN bit register
                            */
    uint32_t          : 2; //!< 14-15
    uint32_t FQ_PQ2   : 1; //!< 16    - The third candidate evaluated by TX-Scan is either a TX Priority Queue (when set to 1) or a TX FIFO Queue (when set to 0)
    uint32_t FQN_PQSN2: 5; /*!< 17-21 - The third candidate is coming from either the TX FIFO Queue number N (defined by FQN in TX descriptor) or the TX Priority Queue Slot number M (defined by the PQSN in TX descriptor).
                            *           The meaning of this bit field depends on the PQ2
                            */
    uint32_t          : 2; //!< 22-23
    uint32_t FQ_PQ3   : 1; //!< 24    - The fourth candidate evaluated by TX-Scan is either a TX Priority Queue (when set to 1) or a TX FIFO Queue (when set to 0)
    uint32_t FQN_PQSN3: 5; /*!< 25-29 - The fourth candidate is coming from either the TX FIFO Queue number N (defined by FQN in TX descriptor) or the TX Priority Queue Slot number M (defined by the PQSN in TX descriptor).
                            *           The meaning of this bit field depends on the PQ3
                            */
    uint32_t          : 2; //!< 30-31
  } Bits;
} XCAN_TX_SCAN_FC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_SCAN_FC_Register, 4);

#define XCAN_TX_SCAN_FC_FQ_PQ0_TX_PRIORITY_QUEUE  (1u << 0) //!< The first candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ0_TX_FIFO_QUEUE      (0u << 0) //!< The first candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN0_PQSN_Pos           1
#define XCAN_TX_SCAN_FQN_PQSN0_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN0_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN0_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN0_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN0_PQSN_Pos) //!< Get the first candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FC_FQ_PQ1_TX_PRIORITY_QUEUE  (1u << 8) //!< The second candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ1_TX_FIFO_QUEUE      (0u << 8) //!< The second candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN1_PQSN_Pos           9
#define XCAN_TX_SCAN_FQN_PQSN1_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN1_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN1_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN1_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN1_PQSN_Pos) //!< Get the second candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FC_FQ_PQ2_TX_PRIORITY_QUEUE  (1u << 16) //!< The third candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ2_TX_FIFO_QUEUE      (0u << 16) //!< The third candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN2_PQSN_Pos           17
#define XCAN_TX_SCAN_FQN_PQSN2_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN2_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN2_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN2_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN2_PQSN_Pos) //!< Get the third candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M
#define XCAN_TX_SCAN_FC_FQ_PQ3_TX_PRIORITY_QUEUE  (1u << 24) //!< The fourth candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_FC_FQ_PQ3_TX_FIFO_QUEUE      (0u << 24) //!< The fourth candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_FQN_PQSN3_PQSN_Pos           25
#define XCAN_TX_SCAN_FQN_PQSN3_PQSN_Mask          (0x1Fu << XCAN_TX_SCAN_FQN_PQSN3_PQSN_Pos)
#define XCAN_TX_SCAN_FQN_PQSN3_PQSN_GET(value)    ((uint32_t)(value) & XCAN_TX_SCAN_FQN_PQSN3_PQSN_Mask) >> XCAN_TX_SCAN_FQN_PQSN3_PQSN_Pos) //!< Get the fourth candidate is coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M

//-----------------------------------------------------------------------------

/*! XCAN TX-SCAN best candidates register (Read-only, Offset: 0x814, Initial value: 0x00000000)
 * This register gives the first and second highest priority TX descriptor after a TX-Scan
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_SCAN_BC_Register
{
  uint32_t TX_SCAN_BC;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t FH_PQ      :  1; //!<  0    - First highest priority candidate evaluated by TX-Scan. It is either a TX Priority Queue (when set to 1) or a TX FIFO Queue (when set to 0)
    uint32_t FH_FQN_PQSN:  5; /*!<  1- 5 - First highest priority candidate coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M.
                               *           The meaning of this bit field depends on the FH_PQ
                               */
    uint32_t FH_OFFSET  : 10; /*!<  6-15 - First highest priority candidate offset in multiple of 32bytes (TX descriptor size). This register is relevant only for the TX FIFO Queue.
                               *           It provides the index of the TX descriptor in the TX FIFO Queue which is in use on the CAN bus. When FH_PQ = 1 it is set to 0
                               */
    uint32_t SH_PQ      :  1; //!< 16    - Second highest priority candidate evaluated by TX-Scan. It is either a TX Priority Queue (when set to 1) or a TX FIFO Queue (when set to 0)
    uint32_t SH_FQN_PQSN:  5; /*!< 17-21 - Second highest priority candidate coming from either the TX FIFO Queue number N or the TX Priority Queue Slot number M.
                               *           The meaning of this bit field depends on the SH_PQ
                               */
    uint32_t SH_OFFSET  : 10; /*!< 22-31 - Second highest priority candidate offset in multiple of 32bytes (TX descriptor size). This register is relevant only for the TX FIFO Queue.
                               *           It provides the index of the TX descriptor in the TX FIFO Queue which is about to be sent on the CAN bus. When SH_PQ = 1 it is set to 0
                               */
  } Bits;
} XCAN_TX_SCAN_BC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_SCAN_BC_Register, 4);

#define XCAN_TX_SCAN_BC_FH_PQ_TX_PRIORITY_QUEUE  (1u << 0) //!< First highest priority candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_BC_FH_PQ_TX_FIFO_QUEUE      (0u << 0) //!< First highest priority candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_BC_FH_FQN_PQSN_Pos          1
#define XCAN_TX_SCAN_BC_FH_FQN_PQSN_Mask         (0x1Fu << XCAN_TX_SCAN_BC_FH_FQN_PQSN_Pos)
#define XCAN_TX_SCAN_BC_FH_FQN_PQSN_GET(value)   (((uint32_t)(value) & XCAN_TX_SCAN_BC_FH_FQN_PQSN_Mask) >> XCAN_TX_SCAN_BC_FH_FQN_PQSN_Pos) //!< Get First highest priority candidate coming from either the TX FIFO Queue number N
#define XCAN_TX_SCAN_BC_FH_OFFSET_Pos            6
#define XCAN_TX_SCAN_BC_FH_OFFSET_Mask           (0x3FFu << XCAN_TX_SCAN_BC_FH_OFFSET_Pos)
#define XCAN_TX_SCAN_BC_FH_OFFSET_GET(value)     (((uint32_t)(value) & XCAN_TX_SCAN_BC_FH_OFFSET_Mask) >> XCAN_TX_SCAN_BC_FH_OFFSET_Pos) //!< Get First highest priority candidate offset in multiple of 32bytes
#define XCAN_TX_SCAN_BC_SH_PQ_TX_PRIORITY_QUEUE  (1u << 16) //!< Second highest priority candidate evaluated by TX-Scan is a TX Priority Queue
#define XCAN_TX_SCAN_BC_SH_PQ_TX_FIFO_QUEUE      (1u << 16) //!< Second highest priority candidate evaluated by TX-Scan is a TX FIFO Queue
#define XCAN_TX_SCAN_BC_SH_FQN_PQSN_Pos          17
#define XCAN_TX_SCAN_BC_SH_FQN_PQSN_Mask         (0x1Fu << XCAN_TX_SCAN_BC_SH_FQN_PQSN_Pos)
#define XCAN_TX_SCAN_BC_SH_FQN_PQSN_GET(value)   (((uint32_t)(value) & XCAN_TX_SCAN_BC_SH_FQN_PQSN_Mask) >> XCAN_TX_SCAN_BC_SH_FQN_PQSN_Pos) //!< Get Second highest priority candidate coming from either the TX FIFO Queue number N
#define XCAN_TX_SCAN_BC_SH_OFFSET_Pos            22
#define XCAN_TX_SCAN_BC_SH_OFFSET_Mask           (0x3FFu << XCAN_TX_SCAN_BC_SH_OFFSET_Pos)
#define XCAN_TX_SCAN_BC_SH_OFFSET_GET(value)     (((uint32_t)(value) & XCAN_TX_SCAN_BC_SH_OFFSET_Mask) >> XCAN_TX_SCAN_BC_SH_OFFSET_Pos) //!< Get Second highest priority candidate offset in multiple of 32bytes

//-----------------------------------------------------------------------------

//! XCAN Valid TX FIFO Queue descriptors in local memory register (Read-only, Offset: 0x818, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_FQ_DESC_VALID_Register
{
  uint32_t PREL;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t DESC_CN_VALID: 8; //!<  0- 7 - When DESC_CN_VALID[n] = 1, the current/next TX descriptor for the TX FIFO Queue n is available in L_MEM
    uint32_t              : 8; //!<  8-15
    uint32_t DESC_NC_VALID: 8; //!< 16-23 - When DESC_NC_VALID[n] = 1, the next/current TX descriptor for the TX FIFO Queue is available in L_MEM
    uint32_t              : 8; //!< 24-31
  } Bits;
} XCAN_TX_FQ_DESC_VALID_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_FQ_DESC_VALID_Register, 4);

#define XCAN_TX_FQ_DESC_CN_VALID_Pos         0
#define XCAN_TX_FQ_DESC_CN_VALID_Mask        (0xFFu << XCAN_TX_FQ_DESC_CN_VALID_Pos)
#define XCAN_TX_FQ_DESC_CN_VALID_GET(value)  (((uint32_t)(value) & XCAN_TX_FQ_DESC_CN_VALID_Mask) >> XCAN_TX_FQ_DESC_CN_VALID_Pos) //!< Get TX descriptor for the TX FIFO Queue n is available
#define XCAN_TX_FQ_DESC_NC_VALID_Pos         16
#define XCAN_TX_FQ_DESC_NC_VALID_Mask        (0xFFu << XCAN_TX_FQ_DESC_NC_VALID_Pos)
#define XCAN_TX_FQ_DESC_NC_VALID_GET(value)  (((uint32_t)(value) & XCAN_TX_FQ_DESC_NC_VALID_Mask) >> XCAN_TX_FQ_DESC_NC_VALID_Pos) //!< Get TX descriptor for the TX FIFO Queue is available

//-----------------------------------------------------------------------------

//! XCAN Valid TX Priority Queue descriptors in local memory register (Read-Only, Offset: 0x81C, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_TX_PQ_DESC_VALID_Register
{
  uint32_t DESC_VALID;             //!< When DESC_VALID[n] = 1, the TX descriptor assigned to the slot n in local memory is valid
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_TX_PQ_DESC_VALID_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_TX_PQ_DESC_VALID_Register, 4);

#define XCAN_TX_PQ_DESC_VALID_Pos         0
#define XCAN_TX_PQ_DESC_VALID_Mask        (0xFFFFFFFFu << XCAN_TX_PQ_DESC_VALID_Pos)
#define XCAN_TX_PQ_DESC_VALID_GET(value)  (((uint32_t)(value) & XCAN_TX_PQ_DESC_VALID_Mask) >> XCAN_TX_PQ_DESC_VALID_Pos) //!< Get TX descriptor valid value

//-----------------------------------------------------------------------------

//! XCAN CRC Control register (Write-Only, Offset: 0x880, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_CRC_CTRL_Register
{
  uint32_t CRC_CTRL;                //!< CRC value of all the registers protected by CRC. Once done, a write to the START bit in the CRC_CTRL register must be done
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t START:  1; //!< 0    - Writing a 1 to this bit triggers the HW CRC check of registers. This action can be done any time for a sanity check
    uint32_t      : 31; //!< 1-31
  } Bits;
} XCAN_CRC_CTRL_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_CRC_CTRL_Register, 4);

#define XCAN_CRC_CTRL_ENABLE        (1ul << 0) //!< CRC Error flag
#define XCAN_CRC_CTRL_DISABLE_Mask  (~XCAN_CRC_CTRL_ENABLE) //!< CRC Error flag

//-----------------------------------------------------------------------------

//! XCAN CRC register (Read/Write, Offset: 0x884, Initial value: 0x00000000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_CRC_REG_Register
{
  uint32_t CRC_REG;                //!< CRC value of all the registers protected by CRC. Once done, a write to the START bit in the CRC_CTRL register must be done
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_CRC_REG_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_CRC_REG_Register, 4);

#define XCAN_CRC_REG_VAL_Pos         0
#define XCAN_CRC_REG_VAL_Mask        (0xFFFFFFFFu << XCAN_CRC_REG_VAL_Pos)
#define XCAN_CRC_REG_VAL_GET(value)  (((uint32_t)(value) & XCAN_CRC_REG_VAL_Mask) >> XCAN_CRC_REG_VAL_Pos) //!< Get CRC register value
#define XCAN_CRC_REG_VAL_SET(value)  (((uint32_t)(value) << XCAN_CRC_REG_VAL_Pos) & XCAN_CRC_REG_VAL_Mask) //!< Set CRC register value

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Protocol Controller Registers
//********************************************************************************************************************

/*! XCAN PRT Release Identification Register (Read-only, Offset: 0x900, Initial value: 0x87654321)
 * The purpose of this register is to identify the beginning of the PRT address map in a memory dump and to check the proper endianness data byte mapping when the data word is routed through different busses
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_ENDN_Register
{
  uint32_t ENDN;
  uint8_t Bytes[sizeof(uint32_t)];
} XCAN_PC_ENDN_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_ENDN_Register, 4);

#define XCAN_PC_ENDN_ENDIANNESS_TEST_VALUE         ( 0x87654321ul ) //!< The endianness test value is 0x87654321
#define XCAN_PC_ENDN_IS_CORRECT_ENDIANNESS(value)  ( (value) == XCAN_PC_ENDN_ENDIANNESS_TEST_VALUE ) //!< Is the XCAN endianness correct (useful in case of an external device that communicates in big-endian for example)

//-----------------------------------------------------------------------------

//! XCAN PRT Release Identification Register (Read-only, Offset: 0x904, Initial value: 0x05400000)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_PREL_Register
{
  uint32_t PREL;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t DAY    : 8; /*!<  0- 7 - Define the day of the release using a binary coded decimal representation (1 being the first day of the month and so forth).
                          *           This reset value is defined by the generic parameter DESIGN_TIME_STAMP_G[7:0].
                          *           If the generic parameter DESIGN_TIME_STAMP_G is not set, the default value is the one defined here
                          */
    uint32_t MON    : 8; /*!<  8-15 - Define the month of the release using a binary coded decimal representation (1 being January and so forth).
                          *           This reset value is defined by the generic parameter DESIGN_TIME_STAMP_G[15:8].
                          *           If the generic parameter DESIGN_TIME_STAMP_G is not set, the default value is the one defined here
                          */
    uint32_t YEAR   : 4; /*!< 16-19 - Define the year of the release using a binary coded decimal representation (0 being 2020 and so forth…).
                          *           This reset value is defined by the generic parameter DESIGN_TIME_STAMP_G[19:16].
                          *           If the generic parameter DESIGN_TIME_STAMP_G is not set, the default value is the one defined here
                          */
    uint32_t SUBSTEP: 4; //!< 20-23 - Sub-Step value according to Step
    uint32_t STEP   : 4; //!< 24-27 - Step value according to Release
    uint32_t REL    : 4; //!< 28-31 - Release value, used to identify the main release of the XCAN_PRT
  } Bits;
} XCAN_PC_PREL_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_PREL_Register, 4);

#define XCAN_PC_PREL_DAY_Pos             0
#define XCAN_PC_PREL_DAY_Mask            (0xFFu << XCAN_PC_PREL_DAY_Pos)
#define XCAN_PC_PREL_DAY_GET(value)      XCAN_DCB8_TO_DECIMAL(((uint32_t)(value) & XCAN_PC_PREL_DAY_Mask) >> XCAN_PC_PREL_DAY_Pos) //!< Get Timestamp Day
#define XCAN_PC_PREL_MON_Pos             8
#define XCAN_PC_PREL_MON_Mask            (0xFFu << XCAN_PC_PREL_MON_Pos)
#define XCAN_PC_PREL_MON_GET(value)      XCAN_DCB8_TO_DECIMAL(((uint32_t)(value) & XCAN_PC_PREL_MON_Mask) >> XCAN_PC_PREL_MON_Pos) //!< Get Timestamp Month
#define XCAN_PC_PREL_YEAR_Pos            16
#define XCAN_PC_PREL_YEAR_Mask           (0xFu << XCAN_PC_PREL_YEAR_Pos)
#define XCAN_PC_PREL_YEAR_GET(value)     XCAN_DCB8_TO_DECIMAL(((uint32_t)(value) & XCAN_PC_PREL_YEAR_Mask) >> XCAN_PC_PREL_YEAR_Pos) //!< Get Timestamp Year
#define XCAN_PC_PREL_SUBSTEP_Pos         20
#define XCAN_PC_PREL_SUBSTEP_Mask        (0xFu << XCAN_PC_PREL_SUBSTEP_Pos)
#define XCAN_PC_PREL_SUBSTEP_GET(value)  XCAN_DCB8_TO_DECIMAL(((uint32_t)(value) & XCAN_PC_PREL_SUBSTEP_Mask) >> XCAN_PC_PREL_SUBSTEP_Pos) //!< Get Sub-step of Core Release
#define XCAN_PC_PREL_STEP_Pos            24
#define XCAN_PC_PREL_STEP_Mask           (0xFu << XCAN_PC_PREL_STEP_Pos)
#define XCAN_PC_PREL_STEP_GET(value)     XCAN_DCB8_TO_DECIMAL(((uint32_t)(value) & XCAN_PC_PREL_STEP_Mask) >> XCAN_PC_PREL_STEP_Pos) //!< Get Step of Core Release
#define XCAN_PC_PREL_REL_Pos             28
#define XCAN_PC_PREL_REL_Mask            (0xFu << XCAN_PC_PREL_REL_Pos)
#define XCAN_PC_PREL_REL_GET(value)      XCAN_DCB8_TO_DECIMAL(((uint32_t)(value) & XCAN_PC_PREL_REL_Mask) >> XCAN_PC_PREL_REL_Pos) //!< Get Core Release

//-----------------------------------------------------------------------------

//! XCAN PRT Status Register (Read-only, Offset: 0x908, Initial value: 0x00000010)
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_STAT_Register
{
  uint32_t STAT;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t ACT :  2; //!<  0- 1 - The current activity of this node
    uint32_t INT :  1; //!<  2    - This node is integrating into CAN bus traffic
    uint32_t STP :  1; //!<  3    - Waiting for end of actual message after STOP command, see Starting and Stopping The Module chapter
    uint32_t CLKA:  1; //!<  4    - The actual value of the CLOCK_ACTIVE input signal, see Starting and Stopping the Module chapter. As the clock must be active when a reset is performed, the default value should be 1
    uint32_t FIMA:  1; //!<  5    - Fault Injection Module Activated
    uint32_t EP  :  1; //!<  6    - This node is in Error-Passive state. When both error counters drop below 127, or when the Bus-Off recovery sequence is finished, the EP bit is cleared
    uint32_t BO  :  1; /*!<  7    - This node is in Bus-Off state. This flag is set on an error condition that would have caused an increment of the Transmit Error Counter to a value beyond its 8 bit range.
                        *           When the PRT enters Bus-Off state, BO is set to 1 and CAN protocol operation is stopped. When the Bus-Off recovery sequence is finished, BO is cleared
                        */
    uint32_t TDCV:  8; /*!<  8-15 - Transmitter Delay Compensation’s delay value. A software reset clears the TDV bit field to 0x00.
                        *           This register shows the sum of the measured delay plus the configured offset, giving the position of the secondary sample point. It is updated for each frame transmission that includes a data phase
                        */
    uint32_t REC :  7; /*!< 16-22 - The CAN protocol’s Receive Error Counter. A software reset does not change the value in this register.
                        *           When the Bus-Off recovery sequence is finished, the error counter REC is cleared. The REC is a 7-bit-counter, together with the Error-Passive flag EP.
                        *           When the increment REC+1 or REC+8 would result in a value > 127 (carry-flag), the REC is kept unchanged, but EP is set.
                        *           When EP is set but REC is below 127 and further errors are detected with an REC+1 condition, the REC will be incremented until it reaches 127.
                        *           At the reception of a valid message, the REC-1 decrements the actual value of the REC by one AND clears the Error-Passive flag EP
                        */
    uint32_t RP  :  1; //!< 23    - The Passive flag of the CAN protocol’s Receive Error Counter. This flag is set on an error condition that would have caused an increment of the Receive Error Counter to a value beyond its 7 bit range
    uint32_t TEC :  8; /*!< 24-31 - The CAN protocol’s Transmit Error Counter. A software reset does not change the value in this register.
                        *           When the Bus-Off recovery sequence is finished, the error counter TEC is cleared. When the increment TEC+8 would result in a value > 255 (carry-flag), the TEC is kept unchanged, but BO is set.
                        *           The Transmit Error Counter is decremented by one each time a CAN message has been successfully transmitted, but it is not decremented below the value 0
                        */
  } Bits;
} XCAN_PC_STAT_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_STAT_Register, 4);

/*! Current node activity enumerator
 * When the CAN protocol operation is stopped, ACT changes to 0b00 and INT changes to 0.
 * When the CAN protocol operation is started, INT is set to 1, but ACT remains at 0b00 until the CAN protocol’s bus idle detection condition is met, then it changes to 0b01 and INT changes to 0.
 * When the CAN protocol operation is started while BO is set, the PRT remains in integrating state (INT=1 and ACT=0b00) until the Bus-Off recovery sequence is finished, then it changes to 0b01 and INT changes to 0.
 * When PRT detects a protocol exception event (see [1], chapter 10.9.5), ACT changes to 0b00 and INT changes to 1 until the CAN protocol’s bus idle detection condition is met, then ACT changes to 0b01 and INT changes to 0.
 * ACT changes from 0b01 to 0b10 when the PRT has received a Start-of-Frame from the CAN bus.
 * ACT changes from 0b01 to 0b11 when the PRT has sent a Start-of-Frame to the CAN bus.
 * ACT changes from 0b11 to 0b10 when the PRT loses arbitration during a transmission.
 * ACT changes from 0b10 to 0b01 or from 0b11 to 0b01when the PRT detects the second bit of intermission (see [1], chapter 10.4.6.2) to be recessive
 */
typedef enum
{
  XCAN_NODE_INACTIVE_STATE = 0b00, //!< The current activity of this node is inactive
  XCAN_NODE_IDLE           = 0b01, //!< The current activity of this node is idle
  XCAN_NODE_RECEIVER       = 0b10, //!< The current activity of this node is receiver
  XCAN_NODE_TRANSMITTER    = 0b11, //!< The current activity of this node is transmitter
} eXCAN_CurrentNodeActivity;

#define XCAN_PC_STAT_ACT_Pos          0
#define XCAN_PC_STAT_ACT_Mask         (0x3u << XCAN_PC_STAT_ACT_Pos)
#define XCAN_PC_STAT_ACT_GET(value)   (((uint32_t)(value) & XCAN_PC_STAT_ACT_Mask) >> XCAN_PC_STAT_ACT_Pos) //!< Get The current activity of this node
#define XCAN_PC_STAT_INT              (1u <<  2) //!< This node is integrating into CAN bus traffic
#define XCAN_PC_STAT_STP              (1u <<  3) //!< Waiting for end of actual message after STOP command
#define XCAN_PC_STAT_CLKA_HIGH        (1u <<  4) //!< The actual value of the CLOCK_ACTIVE input signal is high
#define XCAN_PC_STAT_CLKA_LOW         (0u <<  4) //!< The actual value of the CLOCK_ACTIVE input signal is low
#define XCAN_PC_STAT_FIMA             (1u <<  5) //!< Fault Injection Module Activated
#define XCAN_PC_STAT_EP               (1u <<  6) //!< This node is in Error-Passive state
#define XCAN_PC_STAT_BO               (1u <<  7) //!< This node is in Bus-Off state
#define XCAN_PC_STAT_TDCV_Pos         8
#define XCAN_PC_STAT_TDCV_Mask        (0xFFu << XCAN_PC_STAT_TDCV_Pos)
#define XCAN_PC_STAT_TDCV_GET(value)  (((uint32_t)(value) & XCAN_PC_STAT_TDCV_Mask) >> XCAN_PC_STAT_TDCV_Pos) //!< Get transmitter Delay Compensation’s delay value
#define XCAN_PC_STAT_REC_Pos          16
#define XCAN_PC_STAT_REC_Mask         (0x7Fu << XCAN_PC_STAT_REC_Pos)
#define XCAN_PC_STAT_REC_GET(value)   (((uint32_t)(value) & XCAN_PC_STAT_REC_Mask) >> XCAN_PC_STAT_REC_Pos) //!< Get the CAN protocol’s Receive Error Counter
#define XCAN_PC_STAT_RP               (1u << 23) //!< The Passive flag of the CAN protocol’s Receive Error Counter
#define XCAN_PC_STAT_TEC_Pos          24
#define XCAN_PC_STAT_TEC_Mask         (0xFFu << XCAN_PC_STAT_TEC_Pos)
#define XCAN_PC_STAT_TEC_GET(value)   (((uint32_t)(value) & XCAN_PC_STAT_TEC_Mask) >> XCAN_PC_STAT_TEC_Pos) //!< Get the CAN protocol’s Transmit Error Counter

//! CAN bus status enumerator
typedef enum
{
  XCAN_NODE_INTEGRATING       = XCAN_PC_STAT_INT,  //!< CRC Error flag
  XCAN_WAIT_END_AFTER_STOP    = XCAN_PC_STAT_STP,  //!< Bit0 Error flag
  XCAN_FAULT_INJECTION_ENABLE = XCAN_PC_STAT_FIMA, //!< Acknowledge Error flag
  XCAN_IS_ERROR_PASSIVE_STATE = XCAN_PC_STAT_EP,   //!< Form Error or the condition of CAN error counting rule flag
  XCAN_IS_BUS_OFF_STATE       = XCAN_PC_STAT_BO,   //!< Stuff Error flag
  XCAN_PASSIVE_FLAG           = XCAN_PC_STAT_RP,   //!< Overflow condition in RX_MSG sequence detected flag
  XCAN_CAN_STATUS             = 0x008000EC,        //!< CAN status mask
} eXCAN_CANbusStatus;

typedef eXCAN_CANbusStatus setXCAN_CANbusStatus; //! Set of CAN status (can be OR'ed)

//-----------------------------------------------------------------------------

/*! XCAN Event Status Flags Register (Read/Write, Offset: 0x920, Initial value: 0x00000000)
 * The EVNT Register contains event status flags. The flags are set by the PRT when specific events occur. A software reset clears all flags.
 * A host write access to this register, writing a 1 to a specific flag, clears that flag. When a host write access occurs concurrently with a set condition for a flag, the flag is set
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_EVNT_Register
{
  uint32_t EVNT;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t CRE:  1; //!<  0    - CRC Error
    uint32_t B0E:  1; /*!<  1    - Bit0 Error: The PRT wanted to send a dominant bit (logical value 0), but the monitored CAN bus value was recessive. During Bus-Off recovery,
                       *           B0E is also set each time a sequence of 11 recessive bits has been monitored, enabling the CPU to readily check whether the CAN bus is stuck at
                       *           dominant or continuously disturbed, and to monitor the proceeding of the Bus-Off recovery sequence
                       */
    uint32_t B1E:  1; /*!<  2    - Bit1 Error: During the transmission of a message (with the exception of the arbitration field), the PRT wanted to send a recessive bit (logical value 1),
                       *           but the monitored CAN bus value was dominant
                       */
    uint32_t AKE:  1; //!<  3    - Acknowledge Error
    uint32_t FRE:  1; //!<  4    - Form Error or the condition of CAN error counting rule
    uint32_t STE:  1; //!<  5    - Stuff Error
    uint32_t DO :  1; //!<  6    - Overflow condition in RX_MSG sequence detected
    uint32_t RXF:  1; //!<  7    - Frame received
    uint32_t TXF:  1; //!<  8    - Frame transmitted
    uint32_t PXE:  1; //!<  9    - Protocol Exception Event occurred
    uint32_t DU :  1; //!< 10    - Underrun condition in TX_MSG sequence detected
    uint32_t USO:  1; //!< 11    - Unexpected Start of Sequence during TX_MSG sequence detected
    uint32_t IFR:  1; //!< 12    - Invalid Frame Format requested in TX_MSG
    uint32_t ABO:  1; //!< 13    - TX_MSG sequence stopped by TX_MSG_WUSER code ABORT
    uint32_t    : 18; //!< 14-31
  } Bits;
} XCAN_PC_EVNT_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_EVNT_Register, 4);

#define XCAN_PC_EVNT_CRE  (1u <<  0) //!< CRC Error flag
#define XCAN_PC_EVNT_B0E  (1u <<  1) //!< Bit0 Error flag
#define XCAN_PC_EVNT_B1E  (1u <<  2) //!< Bit1 Error flag
#define XCAN_PC_EVNT_AKE  (1u <<  3) //!< Acknowledge Error flag
#define XCAN_PC_EVNT_FRE  (1u <<  4) //!< Form Error or the condition of CAN error counting rule flag
#define XCAN_PC_EVNT_STE  (1u <<  5) //!< Stuff Error flag
#define XCAN_PC_EVNT_DO   (1u <<  6) //!< Overflow condition in RX_MSG sequence detected flag
#define XCAN_PC_EVNT_RXF  (1u <<  7) //!< Frame received flag
#define XCAN_PC_EVNT_TXF  (1u <<  8) //!< Frame transmitted flag
#define XCAN_PC_EVNT_PXE  (1u <<  9) //!< Protocol Exception Event occurred flag
#define XCAN_PC_EVNT_DU   (1u << 10) //!< Underrun condition in TX_MSG sequence detected flag
#define XCAN_PC_EVNT_USO  (1u << 11) //!< Unexpected Start of Sequence during TX_MSG sequence detected flag
#define XCAN_PC_EVNT_IFR  (1u << 12) //!< Invalid Frame Format requested in TX_MSG flag
#define XCAN_PC_EVNT_ABO  (1u << 13) //!< TX_MSG sequence stopped by TX_MSG_WUSER code ABORT flag

//! Event Status Flags enumerator
typedef enum
{
  XCAN_CRC_EVENT               = XCAN_PC_EVNT_CRE, //!< CRC Error flag
  XCAN_BIT0_ERROR              = XCAN_PC_EVNT_B0E, //!< Bit0 Error flag
  XCAN_BIT1_ERROR              = XCAN_PC_EVNT_B1E, //!< Bit1 Error flag
  XCAN_ACK_ERROR               = XCAN_PC_EVNT_AKE, //!< Acknowledge Error flag
  XCAN_FORM_ERROR              = XCAN_PC_EVNT_FRE, //!< Form Error or the condition of CAN error counting rule flag
  XCAN_STUFF_ERROR             = XCAN_PC_EVNT_STE, //!< Stuff Error flag
  XCAN_OVERFLOW                = XCAN_PC_EVNT_DO,  //!< Overflow condition in RX_MSG sequence detected flag
  XCAN_FRAME_RECEIVED          = XCAN_PC_EVNT_RXF, //!< Frame received flag
  XCAN_FRAME_TRANSMIT          = XCAN_PC_EVNT_TXF, //!< Frame transmitted flag
  XCAN_PROTOCOL_EXCEPTION      = XCAN_PC_EVNT_PXE, //!< Protocol Exception Event occurred flag
  XCAN_UNDERRUN_CONDITION      = XCAN_PC_EVNT_DU,  //!< Underrun condition in TX_MSG sequence detected flag
  XCAN_UNEXPECTED_START        = XCAN_PC_EVNT_USO, //!< Unexpected Start of Sequence during TX_MSG sequence detected flag
  XCAN_INVALID_FRAME_FORMAT    = XCAN_PC_EVNT_IFR, //!< Invalid Frame Format requested in TX_MSG flag
  XCAN_TX_MSG_SEQUENCE_STOPPED = XCAN_PC_EVNT_ABO, //!< TX_MSG sequence stopped by TX_MSG_WUSER code ABORT flag
  XCAN_EVENT_STATUS_FLAGS_Mask = 0x00003FFF,       //!< Event Status Flags mask
} eXCAN_EventStatusFlags;

typedef eXCAN_EventStatusFlags setXCAN_EventStatusFlags; //! Set of Event Status Flags (can be OR'ed)

//-----------------------------------------------------------------------------

/*! XCAN Unlock Sequence Register (Write-only, Offset: 0x940, Initial value: 0x00000000)
 * Writing a sequence of specific data words enables the activation of control commands in the registers CTRL and FIMC. Reading this register always gives the value 0x00000000
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_LOCK_Register
{
  uint32_t LOCK;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t ULK: 16; //!<  0-15 - Unlock Key
    uint32_t TMK: 16; //!< 16-31 - Test Mode Key
  } Bits;
} XCAN_IC_LOCK_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_LOCK_Register, 4);

#define XCAN_IC_ULK_UNLOCK_KEY1       0x1234 //!< Unlock first key for CTRL and FIMC registers
#define XCAN_IC_ULK_UNLOCK_KEY2       0x4321 //!< Unlock second key for CTRL and FIMC registers
#define XCAN_IC_LOCK_ULK_Pos          0
#define XCAN_IC_LOCK_ULK_Mask         (0xFFFFu << XCAN_IC_LOCK_ULK_Pos)
#define XCAN_IC_LOCK_ULK_SET(value)   (((uint32_t)(value) << XCAN_IC_LOCK_ULK_Pos) & XCAN_IC_LOCK_ULK_Mask) //!< Set Unlock Key
#define XCAN_IC_TMK_TEST_UNLOCK_KEY1  0x6789 //!< Unlock first key for TEST registers
#define XCAN_IC_TMK_TEST_UNLOCK_KEY2  0x9876 //!< Unlock second key for TEST registers
#define XCAN_IC_LOCK_TMK_Pos          16
#define XCAN_IC_LOCK_TMK_Mask         (0xFFFFu << XCAN_IC_LOCK_TMK_Pos)
#define XCAN_IC_LOCK_TMK_SET(value)   (((uint32_t)(value) << XCAN_IC_LOCK_TMK_Pos) & XCAN_IC_LOCK_TMK_Mask) //!< Set Test Mode Key

//-----------------------------------------------------------------------------

/*! XCAN Control Register (Write-only, Offset: 0x944, Initial value: 0x00000000)
 * Writing to this register controls the CAN protocol operation. Reading this register gives the value 0x00000000.
 * When writing to this register, only one of the four bits TEST, SRES , STRT, or STOP may be written to 1, otherwise the write access takes no effect.
 * The bit IMMD may be written to 1 together with the bit STOP, but not together with one of the other bits
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_CTRL_Register
{
  uint32_t CTRL;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t STOP:  1; /*!<  0    - Stop CAN protocol operation. The Unlock Key must be used prior to write to this bit field.
                        *           When not set together with bit IMMD the PRT waits for an ongoing CAN message to finish before stopping CAN protocol operation
                        */
    uint32_t IMMD:  1; /*!<  1    - Stop CAN protocol operation immediately. The Unlock Key must be used prior to write to this bit.
                        *           This bit is only effective when being set together with the bit STOP
                        */
    uint32_t     :  2; //!<  2- 3
    uint32_t STRT:  1; //!<  4    - Start CAN protocol operation
    uint32_t     :  3; //!<  5- 7
    uint32_t SRES:  1; /*!<  8    - Software Reset. When the CAN protocol operation is stopped, the software reset of all state machines of the PRT (excluding the errorcounters and the error-states) is triggered by writing 1 to CTRL.SRES.
                        *           No unlocking sequence is required. A software reset will not be executed while the CAN protocol operation is started
                        */
    uint32_t     :  3; //!<  9-11
    uint32_t TEST:  1; //!< 12    - Enable Test Mode. The Test Mode Key must be used prior to write to this bit field
    uint32_t     : 19; //!< 13-31
  } Bits;
} XCAN_IC_CTRL_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_CTRL_Register, 4);

#define XCAN_PC_CTRL_STOP_CAN_OPERATION       (0x1u <<  0) //!< Stop CAN protocol operation
#define XCAN_PC_CTRL_STOP_CAN_OPERATION_ASAP  (0x3u <<  0) //!< Stop CAN protocol operation immediately
#define XCAN_PC_CTRL_START_CAN_OPERATION      (0x1u <<  4) //!< Start CAN protocol operation
#define XCAN_PC_CTRL_SOFTWARE_RESET           (0x1u <<  8) //!< Software Reset
#define XCAN_PC_CTRL_TEST_MODE_ENABLE         (0x1u << 12) //!< Enable Test Mode

//-----------------------------------------------------------------------------

/*! XCAN Fault Injection Module Control Register (Read/Write, Offset: 0x948, Initial value: 0x00000000)
 * Writing the fault injection position number requires the application of the test mode key sequence before writing to FIMC. This register must be accessed in privileged mode when supported
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_FIMC_Register
{
  uint32_t FIMC;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t FIP: 15; /*!<  0-14 - Fault Injection Position. Writing to FIMC while MODE.FIME is set activates the Fault Injection Module FIM (see Safety Measures chapter).
                       *           While the FIM is activated, the value of FIMC.FIP is protected from further write accesses until the FIM is de-activated again
                       */
    uint32_t    : 17; //!< 15-31
  } Bits;
} XCAN_IC_FIMC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_FIMC_Register, 4);

#define XCAN_IC_FIMC_FIP_Pos         0
#define XCAN_IC_FIMC_FIP_Mask        (0x7Fu << XCAN_IC_FIMC_FIP_Pos)
#define XCAN_IC_FIMC_FIP_SET(value)  (((uint32_t)(value) << XCAN_IC_FIMC_FIP_Pos) & XCAN_IC_FIMC_FIP_Mask) //!< Set Fault Injection Position
#define XCAN_IC_FIMC_FIP_GET(value)  (((uint32_t)(value) & XCAN_IC_FIMC_FIP_Mask) >> XCAN_IC_FIMC_FIP_Pos) //!< Get Fault Injection Position

//-----------------------------------------------------------------------------

/*! XCAN Hardware Test Functions Register (Read/Write, Offset: 0x94C, Initial value: 0x00000008)
 * This register is writable after the hardware test mode functions are enabled by writing the test mode key sequence to LOCK and CTRL registers.
 * While the hardware test mode functions are not enabled, this register is read-only. This register must be accessed in privileged mode when supported.
 * The hardware test mode functions are disabled and cleared by the software reset of the PRT
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_TEST_Register
{
  uint32_t TEST;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t LBCK     : 1; //!<  0    - Enable the Message Loop-Back mode, see chapter Trace and Debug
    uint32_t          : 2; //!<  1- 2
    uint32_t RXD      : 1; //!<  3    - Bit value seen at CAN_RX. The CAN_RX input (output signal of the transceiver) is always readable through this bit
    uint32_t TXC      : 2; //!<  4- 5 - Control the bit value driven at CAN_TX
    uint32_t          : 9; //!<  6-14
    uint32_t HWT      : 1; //!< 15    - This status flag HWT shows whether the hardware test mode functions are enabled, set to 1 means enable
    uint32_t ABORTED  : 1; //!< 16    - Writing a 1 to the bit field triggers the ABORTED interrupt line, this bit is auto-cleared
    uint32_t USOS     : 1; //!< 17    - Writing a 1 to the bit field triggers the USOS interrupt line, this bit is auto-cleared
    uint32_t TX_DU    : 1; //!< 18    - Writing a 1 to the bit field triggers the TX_DU interrupt line, this bit is auto-cleared
    uint32_t RX_DO    : 1; //!< 19    - Writing a 1 to the bit field triggers the RX_DO interrupt line, this bit is auto-cleared
    uint32_t IFF_RQ   : 1; //!< 20    - Writing a 1 to the bit field triggers the IFF_RQ interrupt line, this bit is auto-cleared
    uint32_t TX_EVT   : 1; //!< 21    - Writing a 1 to the bit field triggers the TX_EVT interrupt line, this bit is auto-cleared
    uint32_t RX_EVT   : 1; //!< 22    - Writing a 1 to the bit field triggers the RX_EVT interrupt line, this bit is auto-cleared
    uint32_t BUS_ERR  : 1; //!< 23    - Writing a 1 to the bit field triggers the BUS_ERR interrupt line, this bit is auto-cleared
    uint32_t E_ACTIVE : 1; //!< 24    - Writing a 1 to the bit field triggers the E_ACTIVE interrupt line, this bit is auto-cleared
    uint32_t E_PASSIVE: 1; //!< 25    - Writing a 1 to the bit field triggers the E_PASSIVE interrupt line, this bit is auto-cleared
    uint32_t BUS_ON   : 1; //!< 26    - Writing a 1 to the bit field triggers the BUS_ON interrupt line, this bit is auto-cleared
    uint32_t BUS_OFF  : 1; //!< 27    - Writing a 1 to the bit field triggers the BUS_OFF interrupt line, this bit is auto-cleared
    uint32_t          : 4; //!< 28-31
  } Bits;
} XCAN_PC_TEST_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_TEST_Register, 4);

#define XCAN_PC_TEST_LOOPBACK_MODE_EN   (0x1u << 0) //!< Loop Back mode is Enabled
#define XCAN_PC_TEST_LOOPBACK_MODE_DIS  (0x0u << 0) //!< Loop Back mode is Disabled

//! Receive Pin status enumerator
typedef enum
{
  XCAN_RX_DOMINANT  = 0b0, //!< Dominant ('0') level at pin CANRX
  XCAN_RX_RECESSIVE = 0b1, //!< Recessive ('1') level at pin CANRX
} eXCAN_TestRxPin;

#define XCAN_PC_TEST_RX_Pos          3
#define XCAN_PC_TEST_RX_Mask         (0x1u << XCAN_PC_TEST_RX_Pos)
#define XCAN_PC_TEST_RX_GET(value)   (((uint32_t)(value) & XCAN_PC_TEST_RX_Mask) >> XCAN_PC_TEST_RX_Pos) //!< Get Receive Pin status
#define XCAN_PC_TEST_RX_RECESSIVE    (0x1u << 7) //!< The CAN bus is recessive (CANRX = '1')
#define XCAN_PC_TEST_RX_DOMINANT     (0x0u << 7) //!< The CAN bus is dominant (CANRX = '0')

//! Control of Transmit Pin enumerator
typedef enum
{
  XCAN_TX_NORMAL           = 0b00, //!< Normal function of CAN TX
  XCAN_TX_NORMAL_IGNORE_RX = 0b01, //!< Normal function of CAN TX. CAN RX is ignored (for message look back mode)
  XCAN_TX_DOMINANT         = 0b10, //!< Dominant ('0') level at pin CANTX
  XCAN_TX_RECESSIVE        = 0b11, //!< Recessive ('1') level at pin CANTX
} eXCAN_TestTxPin;

#define XCAN_PC_TEST_TXC_Pos         4
#define XCAN_PC_TEST_TXC_Mask        (0x3u << XCAN_PC_TEST_TXC_Pos)
#define XCAN_PC_TEST_TXC_SET(value)  (((uint32_t)(value) << XCAN_PC_TEST_TXC_Pos) & XCAN_PC_TEST_TXC_Mask) //!< Set Control of Transmit Pin
#define XCAN_PC_TEST_TXC_GET(value)  (((uint32_t)(value) & XCAN_PC_TEST_TXC_Mask) >> XCAN_PC_TEST_TXC_Pos) //!< Get Control of Transmit Pin
#define XCAN_PC_TEST_MODE_EN         (1u << 15) //!< Test mode enabled
#define XCAN_PC_TEST_MODE_DIS        (0u << 15) //!< Test mode disabled

#define XCAN_PC_TEST_TRIGGER_ABORTED_INT_LINE    (1u << 16) //!< Trigger ABORTED interrupt line
#define XCAN_PC_TEST_TRIGGER_USOS_INT_LINE       (1u << 17) //!< Trigger USOS interrupt line
#define XCAN_PC_TEST_TRIGGER_TX_DU_INT_LINE      (1u << 18) //!< Trigger TX_DU interrupt line
#define XCAN_PC_TEST_TRIGGER_RX_DO_INT_LINE      (1u << 19) //!< Trigger RX_DO interrupt line
#define XCAN_PC_TEST_TRIGGER_IFF_RQ_INT_LINE     (1u << 20) //!< Trigger IFF_RQ interrupt line
#define XCAN_PC_TEST_TRIGGER_TX_EVT_INT_LINE     (1u << 21) //!< Trigger TX_EVT interrupt line
#define XCAN_PC_TEST_TRIGGER_RX_EVT_INT_LINE     (1u << 22) //!< Trigger RX_EVT interrupt line
#define XCAN_PC_TEST_TRIGGER_BUS_ERR_INT_LINE    (1u << 23) //!< Trigger BUS_ERR interrupt line
#define XCAN_PC_TEST_TRIGGER_E_ACTIVE_INT_LINE   (1u << 24) //!< Trigger E_ACTIVE interrupt line
#define XCAN_PC_TEST_TRIGGER_E_PASSIVE_INT_LINE  (1u << 25) //!< Trigger E_PASSIVE interrupt line
#define XCAN_PC_TEST_TRIGGER_BUS_ON_INT_LINE     (1u << 26) //!< Trigger BUS_ON interrupt line
#define XCAN_PC_TEST_TRIGGER_BUS_OFF_INT_LINE    (1u << 27) //!< Trigger BUS_OFF interrupt line

//! Test interrupt lines enumerator
typedef enum
{
  XCAN_TRIGGER_ABORTED_INT_LINE   = XCAN_PC_TEST_TRIGGER_ABORTED_INT_LINE,   //!< Trigger ABORTED interrupt line
  XCAN_TRIGGER_USOS_INT_LINE      = XCAN_PC_TEST_TRIGGER_USOS_INT_LINE,      //!< Trigger USOS interrupt line
  XCAN_TRIGGER_TX_DU_INT_LINE     = XCAN_PC_TEST_TRIGGER_TX_DU_INT_LINE,     //!< Trigger TX_DU interrupt line
  XCAN_TRIGGER_RX_DO_INT_LINE     = XCAN_PC_TEST_TRIGGER_RX_DO_INT_LINE,     //!< Trigger RX_DO interrupt line
  XCAN_TRIGGER_IFF_RQ_INT_LINE    = XCAN_PC_TEST_TRIGGER_IFF_RQ_INT_LINE,    //!< Trigger IFF_RQ interrupt line
  XCAN_TRIGGER_TX_EVT_INT_LINE    = XCAN_PC_TEST_TRIGGER_TX_EVT_INT_LINE,    //!< Trigger TX_EVT interrupt line
  XCAN_TRIGGER_RX_EVT_INT_LINE    = XCAN_PC_TEST_TRIGGER_RX_EVT_INT_LINE,    //!< Trigger RX_EVT interrupt line
  XCAN_TRIGGER_BUS_ERR_INT_LINE   = XCAN_PC_TEST_TRIGGER_BUS_ERR_INT_LINE,   //!< Trigger BUS_ERR interrupt line
  XCAN_TRIGGER_E_ACTIVE_INT_LINE  = XCAN_PC_TEST_TRIGGER_E_ACTIVE_INT_LINE,  //!< Trigger E_ACTIVE interrupt line
  XCAN_TRIGGER_E_PASSIVE_INT_LINE = XCAN_PC_TEST_TRIGGER_E_PASSIVE_INT_LINE, //!< Trigger E_PASSIVE interrupt line
  XCAN_TRIGGER_BUS_ON_INT_LINE    = XCAN_PC_TEST_TRIGGER_BUS_ON_INT_LINE,    //!< Trigger BUS_ON interrupt line
  XCAN_TRIGGER_BUS_OFF_INT_LINE   = XCAN_PC_TEST_TRIGGER_BUS_OFF_INT_LINE,   //!< Trigger BUS_OFF interrupt line
  XCAN_TRIGGER_INT_LINE_Mask      = 0x0FFF0000,                              //!< Trigger interrupt line mask
} eXCAN_TestInterruptLines;

typedef eXCAN_TestInterruptLines setXCAN_TestInterruptLines; //! Set of Test interrupt lines (can be OR'ed)

//-----------------------------------------------------------------------------

/*! XCAN Operating Mode Register (Read/Write, Offset: 0x960, Initial value: 0x00000000)
 * Configuration register that is writable while the CAN communication is stopped and that is read-only after the CAN communication is started. This register defines separate operating mode options.
 * The four configuration bits FDOE, XLOE, EFDI, and XLTR, are interrelated according to table Frame Formats defined in Operating Mode chapter
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_MODE_Register
{
  uint32_t MODE;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t FDOE:  1; /*!<  0    - FD Frame Format enabled. When set to 1, node is FD enabled according to ISO11898-1:2015.
                        *           When set to 0, node is FD tolerant according to ISO11898-1:2015 (only Classical CAN frames used).
                        *           This bit cannot be set to 1 when the static input ONLY_CC is set
                        */
    uint32_t XLOE:  1; /*!<  1    - XL Frame Format enabled. When set to 0, node behaves according to ISO11898-1:2015, no arbitration during FDF bit.
                        *           When set to 1, node behaves according to CiA610-1, arbitration during FDF bit and XLF bit.
                        *           This bit cannot be set to 1 when one of the static inputs ONLY_CC or ONLY_CC_FD is set. Setting XLOE without setting FDOE is an invalid configuration
                        */
    uint32_t TDCE:  1; //!<  2    - Transmitter Delay Compensation Enabled
    uint32_t PXHD:  1; //!<  3    - Protocol Exception Handling Disabled
    uint32_t EFBI:  1; //!<  4    - Edge Filtering during Bus Integration. If this bit is set, the PRT requires two consecutive dominant to to detect an edge causing the reset of the bit counter for the detection of the idle condition
    uint32_t TXP :  1; //!<  5    - Transmit Pause. If this bit is set, the PRT pauses for two CAN bit times before starting the next transmission after itself has successfully transmitted a frame
    uint32_t MON :  1; //!<  6    - Monitoring Mode Enabled
    uint32_t RSTR:  1; //!<  7    - Restricted Mode Enabled
    uint32_t SFS :  1; //!<  8    - Time stamp position: Start of Frame Stamping: '1' = Timestamps captured at the start of a frame ; '0' = Timestamps captured at the end of a frame
    uint32_t XLTR:  1; //!<  9    - XL Transceiver Connected
    uint32_t EFDI:  1; /*!< 10    - Error Flag Disable, 1 means Error Signalling is disabled and the error counters REC and TEC are not incremented.
                        *           When this bit is set, only CAN XL frames are transmitted and received dominant FDF or XLF bits are treated as form errors
                        */
    uint32_t FIME:  1; //!< 11    - Fault Injection Module Enable
    uint32_t     : 20; //!< 12-31
  } Bits;
} XCAN_PC_MODE_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_MODE_Register, 4);

#define XCAN_IC_MODE_CAN_FD_MODE_EN              (1u <<  0) //!< CAN FD operation enabled
#define XCAN_IC_MODE_CAN_FD_MODE_DIS             (0u <<  0) //!< CAN FD operation disabled
#define XCAN_IC_MODE_CAN_XL_MODE_EN              (1u <<  1) //!< CAN XL operation enabled
#define XCAN_IC_MODE_CAN_XL_MODE_DIS             (0u <<  1) //!< CAN XL operation disabled
#define XCAN_IC_MODE_TX_DELAY_COMP_EN            (1u <<  2) //!< Transmitter Delay Compensation enabled
#define XCAN_IC_MODE_TX_DELAY_COMP_DIS           (0u <<  2) //!< Transmitter Delay Compensation disabled
#define XCAN_IC_MODE_PROTOCOL_EXCEPTION_DIS      (1u <<  3) //!< Protocol exception handling disabled
#define XCAN_IC_MODE_PROTOCOL_EXCEPTION_EN       (0u <<  3) //!< Protocol exception handling enabled
#define XCAN_IC_MODE_EDGE_FILTERING_EN           (1u <<  4) //!< Edge filtering is enabled. Two consecutive dominant tq required to detect an edge for hard synchronization
#define XCAN_IC_MODE_EDGE_FILTERING_DIS          (0u <<  4) //!< Edge filtering is disabled
#define XCAN_IC_MODE_TRANSMIT_PAUSE_EN           (1u <<  5) //!< Transmit pause enabled
#define XCAN_IC_MODE_TRANSMIT_PAUSE_DIS          (0u <<  5) //!< Transmit pause disabled
#define XCAN_IC_MODE_BUS_MONITOR_EN              (1u <<  6) //!< Bus Monitoring mode is enabled
#define XCAN_IC_MODE_BUS_MONITOR_DIS             (0u <<  6) //!< Bus Monitoring mode is disabled
#define XCAN_IC_MODE_RESTRICTED_OPERATION_EN     (1u <<  7) //!< Restricted Operation mode enabled
#define XCAN_IC_MODE_RESTRICTED_OPERATION_DIS    (0u <<  7) //!< Restricted Operation mode disabled
#define XCAN_IC_MODE_TS_CAPTURE_START_FRAME      (1u <<  8) //!< Timestamps captured at the start of a frame
#define XCAN_IC_MODE_TS_CAPTURE_END_FRAME        (0u <<  8) //!< Timestamps captured at the end of a frame
#define XCAN_IC_MODE_XL_TRANSCEIVER_CONNECTED    (1u <<  9) //!< XL Transceiver Connected
#define XCAN_IC_MODE_ERROR_FLAG_DISABLE          (1u << 10) //!< Error Flag disabled
#define XCAN_IC_MODE_ERROR_FLAG_ENABLE           (0u << 10) //!< Error Flag enabled
#define XCAN_IC_MODE_FAULT_INJECTION_MODULE_EN   (1u << 11) //!< Fault Injection Module enabled
#define XCAN_IC_MODE_FAULT_INJECTION_MODULE_DIS  (0u << 11) //!< Fault Injection Module disabled

//-----------------------------------------------------------------------------

/*! XCAN Arbitration Phase Nominal Bit Timing Register (Read/Write, Offset: 0x964, Initial value: 0x00000000)
 * Configuration register that is writable while the CAN communication is stopped and that is read-only after the CAN communication is started.
 * This register defines the Nominal Bit Timing as defined in ISO 11898-1:2015 CAN data link layer and physical signaling
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_NBTP_Register
{
  uint32_t NBTP;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t NSJW  : 7; /*!<  0- 6 - Nominal SJW. Valid values for the Nominal Synchronization Jump Width NSJW are 0x00-0x7F.
                         *           The actual interpretation of this value is that the Nominal Synchronization Jump Width is (NSJW + 1) TQ long
                         */
    uint32_t       : 1; //!<  7
    uint32_t NTSEG2: 7; /*!<  8-14 - Nominal Phase_Seg2. Valid values for NTSEG2 are 0x01-0x7F. This value defines the length of Phase_Seg2(N).
                         *           The actual interpretation of this value is that the phase buffer segment 2 is (NTSEG2 + 1) TQ long
                         */
    uint32_t       : 1; //!< 15
    uint32_t NTSEG1: 9; /*!< 16-24 - Nominal Prop_Seg and Phase_Seg1. Valid values for NTSEG1 are 0x01- 0x1FF. This value defines the sum of Prop_Seg(N) and Phase_Seg1(N).
                         *           The actual interpretation of this value is that these segments together are (NTSEG1 + 1) TQ long
                         */
    uint32_t BRP   : 5; /*!< 25-29 - Bit Rate Prescaler. Valid values for the Bit Rate Prescaler BRP are 0x00-0x1F.
                         *           This value defines the length of the Time Quantum TQ for all three bit time configurations.
                         *           The actual interpretation of this value is that the TQ is (BRP + 1) CLK periods long
                         */
    uint32_t       : 2; //!< 30-31
  } Bits;
} XCAN_PC_NBTP_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_NBTP_Register, 4);

#define XCAN_PC_NBTP_NSJW_Pos           0
#define XCAN_PC_NBTP_NSJW_Mask          (0x7Fu << XCAN_PC_NBTP_NSJW_Pos)
#define XCAN_PC_NBTP_NSJW_SET(value)    (((uint32_t)(value) << XCAN_PC_NBTP_NSJW_Pos) & XCAN_PC_NBTP_NSJW_Mask) //!< Set Nominal SJW
#define XCAN_PC_NBTP_NSJW_GET(value)    (((uint32_t)(value) & XCAN_PC_NBTP_NSJW_Mask) >> XCAN_PC_NBTP_NSJW_Pos) //!< Get Nominal SJW
#define XCAN_PC_NBTP_NTSEG2_Pos         8
#define XCAN_PC_NBTP_NTSEG2_Mask        (0x7Fu << XCAN_PC_NBTP_NTSEG2_Pos)
#define XCAN_PC_NBTP_NTSEG2_SET(value)  (((uint32_t)(value) << XCAN_PC_NBTP_NTSEG2_Pos) & XCAN_PC_NBTP_NTSEG2_Mask) //!< Set Nominal Phase_Seg2
#define XCAN_PC_NBTP_NTSEG2_GET(value)  (((uint32_t)(value) & XCAN_PC_NBTP_NTSEG2_Mask) >> XCAN_PC_NBTP_NTSEG2_Pos) //!< Get Nominal Phase_Seg2
#define XCAN_PC_NBTP_NTSEG1_Pos         16
#define XCAN_PC_NBTP_NTSEG1_Mask        (0x1FFu << XCAN_PC_NBTP_NTSEG1_Pos)
#define XCAN_PC_NBTP_NTSEG1_SET(value)  (((uint32_t)(value) << XCAN_PC_NBTP_NTSEG1_Pos) & XCAN_PC_NBTP_NTSEG1_Mask) //!< Set Nominal Prop_Seg and Phase_Seg1
#define XCAN_PC_NBTP_NTSEG1_GET(value)  (((uint32_t)(value) & XCAN_PC_NBTP_NTSEG1_Mask) >> XCAN_PC_NBTP_NTSEG1_Pos) //!< Get Nominal Prop_Seg and Phase_Seg1
#define XCAN_PC_NBTP_BRP_Pos            25
#define XCAN_PC_NBTP_BRP_Mask           (0x1Fu << XCAN_PC_NBTP_BRP_Pos)
#define XCAN_PC_NBTP_BRP_SET(value)     (((uint32_t)(value) << XCAN_PC_NBTP_BRP_Pos) & XCAN_PC_NBTP_BRP_Mask) //!< Set Bit Rate Prescaler
#define XCAN_PC_NBTP_BRP_GET(value)     (((uint32_t)(value) & XCAN_PC_NBTP_BRP_Mask) >> XCAN_PC_NBTP_BRP_Pos) //!< Get Bit Rate Prescaler

//-----------------------------------------------------------------------------

/*! XCAN CAN FD Data Phase Bit Timing Register (Read/Write, Offset: 0x968, Initial value: 0x00000000)
 * Configuration register that is writable while the CAN communication is stopped and that is read-only after the CAN communication is started.
 * This register defines the FD Data Phase Bit Timing as defined in ISO 11898-1:2015 CAN data link layer and physical signaling
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_DBTP_Register
{
  uint32_t DBTP;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t DSJW  : 7; /*!<  0- 6 - FD data phase SJW. Valid values for the FD data phase Synchronization Jump Width DSJW are 0x00-0x7F.
                         *           The actual interpretation of this value is that the FD data phase Synchronization Jump Width is (DSJW + 1) TQ long
                         */
    uint32_t       : 1; //!<  7
    uint32_t DTSEG2: 7; /*!<  8-14 - FD data phase Phase_Seg2. Valid values for DTSEG2 are 0x01-0x7F. This value defines the length of Phase_Seg2(D).
                         *           The actual interpretation of this value is that the phase buffer segment 2 is (DTSEG2 + 1) TQ long
                         */
    uint32_t       : 1; //!< 15
    uint32_t DTSEG1: 8; /*!< 16-23 - FD data phase Prop_Seg and Phase_Seg1. Valid values for DTSEG1 are 0x00-0xFF. This value defines the sum of Prop_Seg(D) and Phase_Seg1(D).
                         *           The actual interpretation of this value is that these segments together are (DTSEG1 + 1) TQ long
                         */
    uint32_t DTDCO : 8; /*!< 24-31 - Transmitter Delay Compensation Offset for FD frames. Valid values for the FD Transmitter Delay Compensation Offset DTDCO is 0x00-0xFF.
                         *           This configuration defines the distance between the measured delay from CAN_TX to CAN_RX and the secondary sample point SSP, measured in CLK periods.
                         *           This value is used when transmitting a CAN FD frame
                         */
  } Bits;
} XCAN_PC_DBTP_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_DBTP_Register, 4);

#define XCAN_PC_DBTP_DSJW_Pos           0
#define XCAN_PC_DBTP_DSJW_Mask          (0x7Fu << XCAN_PC_DBTP_DSJW_Pos)
#define XCAN_PC_DBTP_DSJW_SET(value)    (((uint32_t)(value) << XCAN_PC_DBTP_DSJW_Pos) & XCAN_PC_DBTP_DSJW_Mask) //!< Set FD data phase SJW
#define XCAN_PC_DBTP_DSJW_GET(value)    (((uint32_t)(value) & XCAN_PC_DBTP_DSJW_Mask) >> XCAN_PC_DBTP_DSJW_Pos) //!< Get FD data phase SJW
#define XCAN_PC_DBTP_DTSEG2_Pos         8
#define XCAN_PC_DBTP_DTSEG2_Mask        (0x7Fu << XCAN_PC_DBTP_DTSEG2_Pos)
#define XCAN_PC_DBTP_DTSEG2_SET(value)  (((uint32_t)(value) << XCAN_PC_DBTP_DTSEG2_Pos) & XCAN_PC_DBTP_DTSEG2_Mask) //!< Set FD data phase Phase_Seg2
#define XCAN_PC_DBTP_DTSEG2_GET(value)  (((uint32_t)(value) & XCAN_PC_DBTP_DTSEG2_Mask) >> XCAN_PC_DBTP_DTSEG2_Pos) //!< Get FD data phase Phase_Seg2
#define XCAN_PC_DBTP_DTSEG1_Pos         16
#define XCAN_PC_DBTP_DTSEG1_Mask        (0xFFu << XCAN_PC_DBTP_DTSEG1_Pos)
#define XCAN_PC_DBTP_DTSEG1_SET(value)  (((uint32_t)(value) << XCAN_PC_DBTP_DTSEG1_Pos) & XCAN_PC_DBTP_DTSEG1_Mask) //!< Set FD data phase Prop_Seg and Phase_Seg1
#define XCAN_PC_DBTP_DTSEG1_GET(value)  (((uint32_t)(value) & XCAN_PC_DBTP_DTSEG1_Mask) >> XCAN_PC_DBTP_DTSEG1_Pos) //!< Get FD data phase Prop_Seg and Phase_Seg1
#define XCAN_PC_DBTP_DTDCO_Pos          24
#define XCAN_PC_DBTP_DTDCO_Mask         (0xFFu << XCAN_PC_DBTP_DTDCO_Pos)
#define XCAN_PC_DBTP_DTDCO_SET(value)   (((uint32_t)(value) << XCAN_PC_DBTP_DTDCO_Pos) & XCAN_PC_DBTP_DTDCO_Mask) //!< Set Transmitter Delay Compensation Offset for FD frames
#define XCAN_PC_DBTP_DTDCO_GET(value)   (((uint32_t)(value) & XCAN_PC_DBTP_DTDCO_Mask) >> XCAN_PC_DBTP_DTDCO_Pos) //!< Get Transmitter Delay Compensation Offset for FD frames

//-----------------------------------------------------------------------------

/*! XCAN CAN XL Data Phase Bit Timing Register (Read/Write, Offset: 0x96C, Initial value: 0x00000000)
 * Configuration register that is writable while the CAN communication is stopped and is read-only after the CAN communication is started.
 * This register defines the XL Data Phase Bit Timing as defined in CiA 610-1 CAN XL Protocol Specification, V1.0.0, 2021-11-30
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_XBTP_Register
{
  uint32_t XBTP;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t XSJW  : 7; /*!<  0- 6 - XL data phase SJW. Valid values for the XL data phase Synchronization Jump Width XSJW are 0x00-0x7F.
                         *           The actual interpretation of this value is that the XL data phase Synchronization Jump Width is (XSJW + 1) TQ long
                         */
    uint32_t       : 1; //!<  7
    uint32_t XTSEG2: 7; /*!<  8-14 - XL data phase Phase_Seg2. Valid values for XTSEG2 are 0x01-0x7F. This value defines the length of Phase_Seg2(X).
                         *           The actual interpretation of this value is that the phase buffer segment 2 is (XTSEG2 + 1) TQ long
                         */
    uint32_t       : 1; //!< 15
    uint32_t XTSEG1: 8; /*!< 16-23 - XL data phase Prop_Seg and Phase_Seg1. Valid values for XTSEG1 are 0x00-0xFF. This value defines the sum of Prop_Seg(X) and Phase_Seg1(X).
                         *           The actual interpretation of this value is that these segments together are (XTSEG1 + 1) TQ long
                         */
    uint32_t XTDCO : 8; /*!< 24-31 - Transmitter Delay Compensation Offset for XL frames. Valid values for the XL Transmitter Delay Compensation Offset XTDCO is 0x00-0xFF.
                         *           This configuration defines the distance between the measured delay from CAN_TX to CAN_RX and the secondary sample point SSP, measured in CLK periods.
                         *           This value is used when transmitting a CAN XL frame
                         */
  } Bits;
} XCAN_PC_XBTP_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_XBTP_Register, 4);

#define XCAN_PC_XBTP_XSJW_Pos           0
#define XCAN_PC_XBTP_XSJW_Mask          (0x7Fu << XCAN_PC_XBTP_XSJW_Pos)
#define XCAN_PC_XBTP_XSJW_SET(value)    (((uint32_t)(value) << XCAN_PC_XBTP_XSJW_Pos) & XCAN_PC_XBTP_XSJW_Mask) //!< Set XL data phase SJW
#define XCAN_PC_XBTP_XSJW_GET(value)    (((uint32_t)(value) & XCAN_PC_XBTP_XSJW_Mask) >> XCAN_PC_XBTP_XSJW_Pos) //!< Get XL data phase SJW
#define XCAN_PC_XBTP_XTSEG2_Pos         8
#define XCAN_PC_XBTP_XTSEG2_Mask        (0x7Fu << XCAN_PC_XBTP_XTSEG2_Pos)
#define XCAN_PC_XBTP_XTSEG2_SET(value)  (((uint32_t)(value) << XCAN_PC_XBTP_XTSEG2_Pos) & XCAN_PC_XBTP_XTSEG2_Mask) //!< Set XL data phase Phase_Seg2
#define XCAN_PC_XBTP_XTSEG2_GET(value)  (((uint32_t)(value) & XCAN_PC_XBTP_XTSEG2_Mask) >> XCAN_PC_XBTP_XTSEG2_Pos) //!< Get XL data phase Phase_Seg2
#define XCAN_PC_XBTP_XTSEG1_Pos         16
#define XCAN_PC_XBTP_XTSEG1_Mask        (0xFFu << XCAN_PC_XBTP_XTSEG1_Pos)
#define XCAN_PC_XBTP_XTSEG1_SET(value)  (((uint32_t)(value) << XCAN_PC_XBTP_XTSEG1_Pos) & XCAN_PC_XBTP_XTSEG1_Mask) //!< Set XL data phase Prop_Seg and Phase_Seg1
#define XCAN_PC_XBTP_XTSEG1_GET(value)  (((uint32_t)(value) & XCAN_PC_XBTP_XTSEG1_Mask) >> XCAN_PC_XBTP_XTSEG1_Pos) //!< Get XL data phase Prop_Seg and Phase_Seg1
#define XCAN_PC_XBTP_XTDCO_Pos          24
#define XCAN_PC_XBTP_XTDCO_Mask         (0xFFu << XCAN_PC_XBTP_XTDCO_Pos)
#define XCAN_PC_XBTP_XTDCO_SET(value)   (((uint32_t)(value) << XCAN_PC_XBTP_XTDCO_Pos) & XCAN_PC_XBTP_XTDCO_Mask) //!< Set Transmitter Delay Compensation Offset for XL frames
#define XCAN_PC_XBTP_XTDCO_GET(value)   (((uint32_t)(value) & XCAN_PC_XBTP_XTDCO_Mask) >> XCAN_PC_XBTP_XTDCO_Pos) //!< Get Transmitter Delay Compensation Offset for XL frames

//-----------------------------------------------------------------------------

/*! XCAN PWME Configuration Register (Read/Write, Offset: 0x970, Initial value: 0x00000000)
 * Configuration register that is writable while the CAN communication is stopped and is read-only after the CAN communication is started.
 * This register defines the parameters needed for the PWM coding (as described in [2]) in the PWME module for CAN XL transceivers with switchable operating modes
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_PC_PCFG_Register
{
  uint32_t PCFG;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t PWMS:  6; //!<  0- 5 - PWM phase Short
    uint32_t     :  2; //!<  6- 7
    uint32_t PWML:  6; //!<  8-13 - PWM phase Long
    uint32_t     :  2; //!< 14-15
    uint32_t PWMO:  6; //!< 16-21 - PWM Offset
    uint32_t     : 10; //!< 22-31
  } Bits;
} XCAN_PC_PCFG_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_PC_PCFG_Register, 4);

#define XCAN_PC_PCFG_PWMS_Pos         0
#define XCAN_PC_PCFG_PWMS_Mask        (0x3Fu << XCAN_PC_PCFG_PWMS_Pos)
#define XCAN_PC_PCFG_PWMS_SET(value)  (((uint32_t)(value) << XCAN_PC_PCFG_PWMS_Pos) & XCAN_PC_PCFG_PWMS_Mask) //!< Set PWM phase Short
#define XCAN_PC_PCFG_PWMS_GET(value)  (((uint32_t)(value) & XCAN_PC_PCFG_PWMS_Mask) >> XCAN_PC_PCFG_PWMS_Pos) //!< Get PWM phase Short
#define XCAN_PC_PCFG_PWML_Pos         8
#define XCAN_PC_PCFG_PWML_Mask        (0x3Fu << XCAN_PC_PCFG_PWML_Pos)
#define XCAN_PC_PCFG_PWML_SET(value)  (((uint32_t)(value) << XCAN_PC_PCFG_PWML_Pos) & XCAN_PC_PCFG_PWML_Mask) //!< Set PWM phase Long
#define XCAN_PC_PCFG_PWML_GET(value)  (((uint32_t)(value) & XCAN_PC_PCFG_PWML_Mask) >> XCAN_PC_PCFG_PWML_Pos) //!< Get PWM phase Long
#define XCAN_PC_PCFG_PWMO_Pos         16
#define XCAN_PC_PCFG_PWMO_Mask        (0x3Fu << XCAN_PC_PCFG_PWMO_Pos)
#define XCAN_PC_PCFG_PWMO_SET(value)  (((uint32_t)(value) << XCAN_PC_PCFG_PWMO_Pos) & XCAN_PC_PCFG_PWMO_Mask) //!< Set PWM Offset
#define XCAN_PC_PCFG_PWMO_GET(value)  (((uint32_t)(value) & XCAN_PC_PCFG_PWMO_Mask) >> XCAN_PC_PCFG_PWMO_Pos) //!< Get PWM Offset

//-----------------------------------------------------------------------------





//********************************************************************************************************************
// XCAN Interrupt Controller Registers
//********************************************************************************************************************

/*! XCAN Functional raw event status register (Read-only, Offset: 0xA00, Initial value: 0x00000000)
 * This register provides information about the occurrence of functional relevant events inside the MH and the PRT.
 * A flag is set when the related event is detected, independent of FUNC_ENA.
 * The flags remain set until the Host CPU clears them by writing a 1 to the corresponding bit position at register FUNC_CLR
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_FR_Register
{
  uint32_t FUNC_RAW;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_TX_FQ0_IRQ   : 1; /*!<  0    - MH interrupt of the TX FIFO Queue 0: '1' = Interrupt event
                                   *           This interrupt is triggered when an invalid TX descriptor is fetched from this TX FIFO Queue, a TX message from that FIFO Queue is sent (if set in TX descriptor),
                                   *           or a TX message of that TX FIFO Queue is skipped, see description of TX_FQ_IRQ[7:0] in MH section
                                   */
    uint32_t MH_TX_FQ1_IRQ   : 1; //!<  1    - MH interrupt of the TX FIFO Queue 1: '1' = Interrupt event
    uint32_t MH_TX_FQ2_IRQ   : 1; //!<  2    - MH interrupt of the TX FIFO Queue 2: '1' = Interrupt event
    uint32_t MH_TX_FQ3_IRQ   : 1; //!<  3    - MH interrupt of the TX FIFO Queue 3: '1' = Interrupt event
    uint32_t MH_TX_FQ4_IRQ   : 1; //!<  4    - MH interrupt of the TX FIFO Queue 4: '1' = Interrupt event
    uint32_t MH_TX_FQ5_IRQ   : 1; //!<  5    - MH interrupt of the TX FIFO Queue 5: '1' = Interrupt event
    uint32_t MH_TX_FQ6_IRQ   : 1; //!<  6    - MH interrupt of the TX FIFO Queue 6: '1' = Interrupt event
    uint32_t MH_TX_FQ7_IRQ   : 1; //!<  7    - MH interrupt of the TX FIFO Queue 7: '1' = Interrupt event
    uint32_t MH_RX_FQ0_IRQ   : 1; /*!<  8    - MH interrupt of the RX FIFO Queue 0: '1' = Interrupt event
                                   *           This interrupt is triggered when an invalid RX descriptor is fetched from this RX FIFO Queue, or an RX
                                   *           message is received (if set in RX descriptor) in this RX FIFO Queue, see description of RX_FQ_IRQ[7:0] in MH section
                                   */
    uint32_t MH_RX_FQ1_IRQ   : 1; //!<  9    - MH interrupt of the RX FIFO Queue 1: '1' = Interrupt event
    uint32_t MH_RX_FQ2_IRQ   : 1; //!< 10    - MH interrupt of the RX FIFO Queue 2: '1' = Interrupt event
    uint32_t MH_RX_FQ3_IRQ   : 1; //!< 11    - MH interrupt of the RX FIFO Queue 3: '1' = Interrupt event
    uint32_t MH_RX_FQ4_IRQ   : 1; //!< 12    - MH interrupt of the RX FIFO Queue 4: '1' = Interrupt event
    uint32_t MH_RX_FQ5_IRQ   : 1; //!< 13    - MH interrupt of the RX FIFO Queue 5: '1' = Interrupt event
    uint32_t MH_RX_FQ6_IRQ   : 1; //!< 14    - MH interrupt of the RX FIFO Queue 6: '1' = Interrupt event
    uint32_t MH_RX_FQ7_IRQ   : 1; //!< 15    - MH interrupt of the RX FIFO Queue 7: '1' = Interrupt event
    uint32_t MH_TX_PQ_IRQ    : 1; /*!< 16    - Interrupt of TX Priority Queue: '1' = Interrupt event
                                   *           Any TX message sent from the TX Priority Queue can be configured to trigger this interrupt.
                                   *           The SW would then need to look at the MH register TX_PQ_INT_STS to identify which slot has generated the interrupt and for which reason
                                   */
    uint32_t MH_STOP_IRQ     : 1; //!< 17    - The interrupt is triggered when the PRT is stopped. The MH finishes its task and switches to idle mode: '1' = Interrupt event
    uint32_t MH_RX_FILTER_IRQ: 1; //!< 18    - In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful: '1' = Interrupt event
    uint32_t MH_TX_FILTER_IRQ: 1; //!< 19    - The interrupt is triggered when the TX filter is enabled, and a TX message is rejected: '1' = Interrupt event
    uint32_t MH_TX_ABORT_IRQ : 1; //!< 20    - This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT: '1' = Interrupt event
    uint32_t MH_RX_ABORT_IRQ : 1; //!< 21    - This interrupt line is triggered when the MH needs to abort a RX message being received from PRT: '1' = Interrupt event
    uint32_t MH_STATS_IRQ    : 1; //!< 22    - One of the RX/TX counters have reached the threshold: '1' = Interrupt event
    uint32_t                 : 1; //!< 23
    uint32_t PRT_E_ACTIVE    : 1; //!< 24    - PRT switched from Error-Passive to Error-Active state: '1' = Interrupt event
    uint32_t PRT_BUS_ON      : 1; //!< 25    - PRT started CAN communication, after start or end of BusOff: '1' = Interrupt event
    uint32_t PRT_TX_EVT      : 1; //!< 26    - PRT transmitted a valid CAN message: '1' = Interrupt event
    uint32_t PRT_RX_EVT      : 1; //!< 27    - PRT received a valid CAN message: '1' = Interrupt event
    uint32_t                 : 3; //!< 28-31
  } Bits;
} XCAN_IC_FC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_FC_Register, 4);

#define XCAN_IC_FR_MH_TX_FQ0_IRQ_EVENT     (1u <<  0) //!< Event MH interrupt of the TX FIFO Queue 0 interrupt
#define XCAN_IC_FR_MH_TX_FQ1_IRQ_EVENT     (1u <<  1) //!< Event MH interrupt of the TX FIFO Queue 1 interrupt
#define XCAN_IC_FR_MH_TX_FQ2_IRQ_EVENT     (1u <<  2) //!< Event MH interrupt of the TX FIFO Queue 2 interrupt
#define XCAN_IC_FR_MH_TX_FQ3_IRQ_EVENT     (1u <<  3) //!< Event MH interrupt of the TX FIFO Queue 3 interrupt
#define XCAN_IC_FR_MH_TX_FQ4_IRQ_EVENT     (1u <<  4) //!< Event MH interrupt of the TX FIFO Queue 4 interrupt
#define XCAN_IC_FR_MH_TX_FQ5_IRQ_EVENT     (1u <<  5) //!< Event MH interrupt of the TX FIFO Queue 5 interrupt
#define XCAN_IC_FR_MH_TX_FQ6_IRQ_EVENT     (1u <<  6) //!< Event MH interrupt of the TX FIFO Queue 6 interrupt
#define XCAN_IC_FR_MH_TX_FQ7_IRQ_EVENT     (1u <<  7) //!< Event MH interrupt of the TX FIFO Queue 7 interrupt
#define XCAN_IC_FR_MH_RX_FQ0_IRQ_EVENT     (1u <<  8) //!< Event MH interrupt of the RX FIFO Queue 0 interrupt
#define XCAN_IC_FR_MH_RX_FQ1_IRQ_EVENT     (1u <<  9) //!< Event MH interrupt of the RX FIFO Queue 1 interrupt
#define XCAN_IC_FR_MH_RX_FQ2_IRQ_EVENT     (1u << 10) //!< Event MH interrupt of the RX FIFO Queue 2 interrupt
#define XCAN_IC_FR_MH_RX_FQ3_IRQ_EVENT     (1u << 11) //!< Event MH interrupt of the RX FIFO Queue 3 interrupt
#define XCAN_IC_FR_MH_RX_FQ4_IRQ_EVENT     (1u << 12) //!< Event MH interrupt of the RX FIFO Queue 4 interrupt
#define XCAN_IC_FR_MH_RX_FQ5_IRQ_EVENT     (1u << 13) //!< Event MH interrupt of the RX FIFO Queue 5 interrupt
#define XCAN_IC_FR_MH_RX_FQ6_IRQ_EVENT     (1u << 14) //!< Event MH interrupt of the RX FIFO Queue 6 interrupt
#define XCAN_IC_FR_MH_RX_FQ7_IRQ_EVENT     (1u << 15) //!< Event MH interrupt of the RX FIFO Queue 7 interrupt
#define XCAN_IC_FR_MH_TX_PQ_IRQ_EVENT      (1u << 16) //!< Event Interrupt of TX Priority Queue interrupt
#define XCAN_IC_FR_MH_STOP_IRQ_EVENT       (1u << 17) //!< Event The interrupt is triggered when the PRT is stopped interrupt
#define XCAN_IC_FR_MH_RX_FILTER_IRQ_EVENT  (1u << 18) //!< Event In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful interrupt
#define XCAN_IC_FR_MH_TX_FILTER_IRQ_EVENT  (1u << 19) //!< Event The interrupt is triggered when the TX filter is enabled, and a TX message is rejected interrupt
#define XCAN_IC_FR_MH_TX_ABORT_IRQ_EVENT   (1u << 20) //!< Event This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT interrupt
#define XCAN_IC_FR_MH_RX_ABORT_IRQ_EVENT   (1u << 21) //!< Event This interrupt line is triggered when the MH needs to abort a RX message being received from PRT interrupt
#define XCAN_IC_FR_MH_STATS_IRQ_EVENT      (1u << 22) //!< Event One of the RX/TX counters have reached the threshold interrupt

#define XCAN_IC_FR_PRT_E_ACTIVE_EVENT      (1u << 24) //!< Event PRT switched from Error-Passive to Error-Active state interrupt
#define XCAN_IC_FR_PRT_BUS_ON_EVENT        (1u << 25) //!< Event PRT started CAN communication, after start or end of BusOff interrupt
#define XCAN_IC_FR_PRT_TX_EVT_EVENT        (1u << 26) //!< Event PRT transmitted a valid CAN message interrupt
#define XCAN_IC_FR_PRT_RX_EVT_EVENT        (1u << 27) //!< Event PRT received a valid CAN message interrupt

//-----------------------------------------------------------------------------

/*! XCAN Error raw event status register (Read-only, Offset: 0xA04, Initial value: 0x00000000)
 * This register provides information about the occurrence of functional error relevant events inside the MH and the PRT.
 * A flag is set when the related event is detected, independent of ERR_ENA.
 * The flags remain set until the Host CPU clears them by writing a 1 to the corresponding bit position at register ERR_CLR
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_ER_Register
{
  uint32_t ERR_RAW;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_RX_FILTER_ERR: 1; //!<  0    - MH RX filtering has not finished in time, i.e. current RX filtering has not been completed before next incoming RX message requires RX filtering: '1' = Interrupt event
    uint32_t MH_MEM_SFTY_ERR : 1; /*!<  1    - MH detected error in L_MEM error interrupt: '1' = Interrupt event
                                   *           This interrupt is triggered when either the MEM_SFTY_CE or MEM_SFTY_UE input signal is active.
                                   *           The Message Handler provides the information, which signal was active, see flags MH:SFTY_INT_STS.MEM_SFTY_CE and MH:SFTY_INT_STS.MEM_SFTY_UE
                                   */
    uint32_t MH_REG_CRC_ERR  : 1; //!<  2    - MH detected CRC error at the register bank. See also description of REG_CRC_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DESC_ERR     : 1; /*!<  3    - CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt: '1' = Interrupt event
                                   *           A status flag can define if it is on TX or RX path, see SFTY_INT_STS register
                                   */
    uint32_t MH_AP_PARITY_ERR: 1; /*!<  4    - MH detected parity error at address pointers, used to manage the MH Queues (RX/TX FIFO Queues and TX Priority Queues) error interrupt: '1' = Interrupt event
                                   *           See also description of AP_PARITY_ERR in MH section
                                   */
    uint32_t MH_DP_PARITY_ERR: 1; /*!<  5    - MH detected parity error at RX message data error interrupt: '1' = Interrupt event
                                   *           (received from PRT and written to AXI system bus) respective parity error detected at TX message data (read from AXI system bus and transferred to PRT).
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX message or TX message was affected
                                   */
    uint32_t MH_DP_SEQ_ERR   : 1; /*!<  6    - MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt: '1' = Interrupt event
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX or TX interface was affected
                                   */
    uint32_t MH_DP_DO_ERR    : 1; //!<  7    - MH detected a data overflow at RX buffer, see description of DP_DO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DP_TO_ERR    : 1; //!<  8    - MH detected timeout at TX_MSG interface located between MH and PRT, see description of DP_TO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DMA_TO_ERR   : 1; //!<  9    - MH detected timeout at DMA_AXI interface, see description of DMA_TO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DMA_CH_ERR   : 1; //!< 10    - MH detected routing error, i.e. data received or sent are not properly routed to or from DMA channel interfaces, see description of DMA_CH_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_RD_RESP_ERR  : 1; //!< 11    - MH detected a bus error caused by a read access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_WR_RESP_ERR  : 1; //!< 12    - MH detected a bus error caused by a write access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_MEM_TO_ERR   : 1; //!< 13    - MH detected timeout at local memory interface MEM_AXI, see description of MEM_TO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t                 : 2; //!< 14-15
    uint32_t PRT_ABORTED     : 1; //!< 16    - PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT: '1' = Interrupt event
    uint32_t PRT_USOS        : 1; //!< 17    - PRT detected unexpected Start of Sequence during TX_MSG sequence: '1' = Interrupt event
    uint32_t PRT_TX_DU       : 1; //!< 18    - PRT detected underrun condition at TX_MSG sequence: '1' = Interrupt event
    uint32_t PRT_RX_DO       : 1; //!< 19    - PRT detected overflow condition at RX_MSG sequence: '1' = Interrupt event
    uint32_t PRT_IFF_RQ      : 1; //!< 20    - PRT detected invalid Frame Format at TX_MSG: '1' = Interrupt event
    uint32_t PRT_BUS_ERR     : 1; //!< 21    - PRT detected error on the CAN Bus: '1' = Interrupt event
    uint32_t PRT_E_PASSIVE   : 1; //!< 22    - PRT switched from Error-Active to Error-Passive state: '1' = Interrupt event
    uint32_t PRT_BUS_OFF     : 1; //!< 23    - PRT entered Bus_Off state: '1' = Interrupt event
    uint32_t                 : 4; //!< 24-27
    uint32_t TOP_MUX_TO_ERR  : 1; //!< 28    - Timeout at top-level multiplexer error interrupt: '1' = Interrupt event
    uint32_t                 : 3; //!< 29-31
  } Bits;
} XCAN_IC_ER_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_ER_Register, 4);

#define XCAN_IC_ER_SR_MH_RX_FILTER_ERR_EVENT  (1u <<  0) //!< Event MH RX filtering has not finished in time
#define XCAN_IC_ER_SR_MH_MEM_SFTY_ERR_EVENT   (1u <<  1) //!< Event MH detected error in L_MEM error interrupt
#define XCAN_IC_ER_SR_MH_REG_CRC_ERR_EVENT    (1u <<  2) //!< Event MH detected CRC error at the register bank error interrupt
#define XCAN_IC_ER_SR_MH_DESC_ERR_EVENT       (1u <<  3) //!< Event CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_ER_SR_MH_AP_PARITY_ERR_EVENT  (1u <<  4) //!< Event MH detected parity error at address pointers, used to manage the MH Queues
#define XCAN_IC_ER_SR_MH_DP_PARITY_ERR_EVENT  (1u <<  5) //!< Event MH detected parity error at RX message data error interrupt
#define XCAN_IC_ER_SR_MH_DP_SEQ_ERR_EVENT     (1u <<  6) //!< Event MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_ER_SR_MH_DP_DO_ERR_EVENT      (1u <<  7) //!< Event MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_ER_SR_MH_DP_TO_ERR_EVENT      (1u <<  8) //!< Event MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_ER_SR_MH_DMA_TO_ERR_EVENT     (1u <<  9) //!< Event MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_ER_SR_MH_DMA_CH_ERR_EVENT     (1u << 10) //!< Event MH detected routing error interrupt
#define XCAN_IC_ER_SR_MH_RD_RESP_ERR_EVENT    (1u << 11) //!< Event MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_ER_SR_MH_WR_RESP_ERR_EVENT    (1u << 12) //!< Event MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_ER_SR_MH_MEM_TO_ERR_EVENT     (1u << 13) //!< Event MH detected timeout at local memory interface MEM_AXI error interrupt

#define XCAN_IC_ER_SR_PRT_ABORTED_EVENT       (1u << 16) //!< Event PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT error interrupt
#define XCAN_IC_ER_SR_PRT_USOS_EVENT          (1u << 17) //!< Event PRT detected unexpected Start of Sequence during TX_MSG sequence error interrupt
#define XCAN_IC_ER_SR_PRT_TX_DU_EVENT         (1u << 18) //!< Event PRT detected underrun condition at TX_MSG sequence error interrupt
#define XCAN_IC_ER_SR_PRT_RX_DO_EVENT         (1u << 19) //!< Event PRT detected overflow condition at RX_MSG sequence error interrupt
#define XCAN_IC_ER_SR_PRT_IFF_RQ_EVENT        (1u << 20) //!< Event PRT detected invalid Frame Format at TX_MSG error interrupt
#define XCAN_IC_ER_SR_PRT_BUS_ERR_EVENT       (1u << 21) //!< Event PRT detected error on the CAN Bus error interrupt
#define XCAN_IC_ER_SR_PRT_E_PASSIVE_EVENT     (1u << 22) //!< Event PRT switched from Error-Active to Error-Passive state error interrupt
#define XCAN_IC_ER_SR_PRT_BUS_OFF_EVENT       (1u << 23) //!< Event PRT entered Bus_Off state error interrupt

#define XCAN_IC_ER_SR_TOP_MUX_TO_ERR_EVENT    (1u << 28) //!< Event Timeout at top-level multiplexer error interrupt

//-----------------------------------------------------------------------------

/*! XCAN Safety raw event clear register (Read-only, Offset: 0xA08, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register SAFETY_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_SR_Register
{
  uint32_t SAFETY_RAW;
  uint8_t Bytes[sizeof(uint32_t)];
  struct
  {
    uint32_t MH_RX_FILTER_ERR: 1; //!<  0    - MH RX filtering has not finished in time, i.e. current RX filtering has not been completed before next incoming RX message requires RX filtering: '1' = Interrupt event
    uint32_t MH_MEM_SFTY_ERR : 1; /*!<  1    - MH detected error in L_MEM error interrupt: '1' = Interrupt event
                                   *           This interrupt is triggered when either the MEM_SFTY_CE or MEM_SFTY_UE input signal is active.
                                   *           The Message Handler provides the information, which signal was active, see flags MH:SFTY_INT_STS.MEM_SFTY_CE and MH:SFTY_INT_STS.MEM_SFTY_UE
                                   */
    uint32_t MH_REG_CRC_ERR  : 1; //!<  2    - MH detected CRC error at the register bank. See also description of REG_CRC_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DESC_ERR     : 1; /*!<  3    - CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt: '1' = Interrupt event
                                   *           A status flag can define if it is on TX or RX path, see SFTY_INT_STS register
                                   */
    uint32_t MH_AP_PARITY_ERR: 1; /*!<  4    - MH detected parity error at address pointers, used to manage the MH Queues (RX/TX FIFO Queues and TX Priority Queues) error interrupt: '1' = Interrupt event
                                   *           See also description of AP_PARITY_ERR in MH section
                                   */
    uint32_t MH_DP_PARITY_ERR: 1; /*!<  5    - MH detected parity error at RX message data error interrupt: '1' = Interrupt event
                                   *           (received from PRT and written to AXI system bus) respective parity error detected at TX message data (read from AXI system bus and transferred to PRT).
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX message or TX message was affected
                                   */
    uint32_t MH_DP_SEQ_ERR   : 1; /*!<  6    - MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt: '1' = Interrupt event
                                   *           Associated information provided by MH register ERR_INT_STS, e.g. if RX or TX interface was affected
                                   */
    uint32_t MH_DP_DO_ERR    : 1; //!<  7    - MH detected a data overflow at RX buffer, see description of DP_DO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DP_TO_ERR    : 1; //!<  8    - MH detected timeout at TX_MSG interface located between MH and PRT, see description of DP_TO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DMA_TO_ERR   : 1; //!<  9    - MH detected timeout at DMA_AXI interface, see description of DMA_TO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_DMA_CH_ERR   : 1; //!< 10    - MH detected routing error, i.e. data received or sent are not properly routed to or from DMA channel interfaces, see description of DMA_CH_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_RD_RESP_ERR  : 1; //!< 11    - MH detected a bus error caused by a read access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_WR_RESP_ERR  : 1; //!< 12    - MH detected a bus error caused by a write access to S_MEM respective L_MEM, see description of RESP_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t MH_MEM_TO_ERR   : 1; //!< 13    - MH detected timeout at local memory interface MEM_AXI, see description of MEM_TO_ERR in MH section error interrupt: '1' = Interrupt event
    uint32_t                 : 2; //!< 14-15
    uint32_t PRT_ABORTED     : 1; //!< 16    - PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT: '1' = Interrupt event
    uint32_t PRT_USOS        : 1; //!< 17    - PRT detected unexpected Start of Sequence during TX_MSG sequence: '1' = Interrupt event
    uint32_t PRT_TX_DU       : 1; //!< 18    - PRT detected underrun condition at TX_MSG sequence: '1' = Interrupt event
    uint32_t PRT_RX_DO       : 1; //!< 19    - PRT detected overflow condition at RX_MSG sequence: '1' = Interrupt event
    uint32_t PRT_IFF_RQ      : 1; //!< 20    - PRT detected invalid Frame Format at TX_MSG: '1' = Interrupt event
    uint32_t PRT_BUS_ERR     : 1; //!< 21    - PRT detected error on the CAN Bus: '1' = Interrupt event
    uint32_t PRT_E_PASSIVE   : 1; //!< 22    - PRT switched from Error-Active to Error-Passive state: '1' = Interrupt event
    uint32_t PRT_BUS_OFF     : 1; //!< 23    - PRT entered Bus_Off state: '1' = Interrupt event
    uint32_t                 : 4; //!< 24-27
    uint32_t TOP_MUX_TO_ERR  : 1; //!< 28    - Timeout at top-level multiplexer error interrupt: '1' = Interrupt event
    uint32_t                 : 3; //!< 29-31
  } Bits;
} XCAN_IC_SR_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_SR_Register, 4);

//-----------------------------------------------------------------------------

/*! XCAN Functional raw event clear register (Write-only, Offset: 0xA10, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register FUNC_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_FC_Register
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
                                   *           This interrupt is triggered when an invalid RX descriptor is fetched from this RX FIFO Queue, or an RX
                                   *           message is received (if set in RX descriptor) in this RX FIFO Queue, see description of RX_FQ_IRQ[7:0] in MH section
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
    uint32_t                 : 3; //!< 28-31
  } Bits;
} XCAN_IC_FC_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_FC_Register, 4);

#define XCAN_IC_FC_MH_TX_FQ0_IRQ_CLEAR     (1u <<  0) //!< Clear MH interrupt of the TX FIFO Queue 0 interrupt
#define XCAN_IC_FC_MH_TX_FQ1_IRQ_CLEAR     (1u <<  1) //!< Clear MH interrupt of the TX FIFO Queue 1 interrupt
#define XCAN_IC_FC_MH_TX_FQ2_IRQ_CLEAR     (1u <<  2) //!< Clear MH interrupt of the TX FIFO Queue 2 interrupt
#define XCAN_IC_FC_MH_TX_FQ3_IRQ_CLEAR     (1u <<  3) //!< Clear MH interrupt of the TX FIFO Queue 3 interrupt
#define XCAN_IC_FC_MH_TX_FQ4_IRQ_CLEAR     (1u <<  4) //!< Clear MH interrupt of the TX FIFO Queue 4 interrupt
#define XCAN_IC_FC_MH_TX_FQ5_IRQ_CLEAR     (1u <<  5) //!< Clear MH interrupt of the TX FIFO Queue 5 interrupt
#define XCAN_IC_FC_MH_TX_FQ6_IRQ_CLEAR     (1u <<  6) //!< Clear MH interrupt of the TX FIFO Queue 6 interrupt
#define XCAN_IC_FC_MH_TX_FQ7_IRQ_CLEAR     (1u <<  7) //!< Clear MH interrupt of the TX FIFO Queue 7 interrupt
#define XCAN_IC_FC_MH_RX_FQ0_IRQ_CLEAR     (1u <<  8) //!< Clear MH interrupt of the RX FIFO Queue 0 interrupt
#define XCAN_IC_FC_MH_RX_FQ1_IRQ_CLEAR     (1u <<  9) //!< Clear MH interrupt of the RX FIFO Queue 1 interrupt
#define XCAN_IC_FC_MH_RX_FQ2_IRQ_CLEAR     (1u << 10) //!< Clear MH interrupt of the RX FIFO Queue 2 interrupt
#define XCAN_IC_FC_MH_RX_FQ3_IRQ_CLEAR     (1u << 11) //!< Clear MH interrupt of the RX FIFO Queue 3 interrupt
#define XCAN_IC_FC_MH_RX_FQ4_IRQ_CLEAR     (1u << 12) //!< Clear MH interrupt of the RX FIFO Queue 4 interrupt
#define XCAN_IC_FC_MH_RX_FQ5_IRQ_CLEAR     (1u << 13) //!< Clear MH interrupt of the RX FIFO Queue 5 interrupt
#define XCAN_IC_FC_MH_RX_FQ6_IRQ_CLEAR     (1u << 14) //!< Clear MH interrupt of the RX FIFO Queue 6 interrupt
#define XCAN_IC_FC_MH_RX_FQ7_IRQ_CLEAR     (1u << 15) //!< Clear MH interrupt of the RX FIFO Queue 7 interrupt
#define XCAN_IC_FC_MH_TX_PQ_IRQ_CLEAR      (1u << 16) //!< Clear Interrupt of TX Priority Queue interrupt
#define XCAN_IC_FC_MH_STOP_IRQ_CLEAR       (1u << 17) //!< Clear The interrupt is triggered when the PRT is stopped interrupt
#define XCAN_IC_FC_MH_RX_FILTER_IRQ_CLEAR  (1u << 18) //!< Clear In order to track RX filtering results, an interrupt can be triggered when the comparison between a RX message header and a defined filter is successful interrupt
#define XCAN_IC_FC_MH_TX_FILTER_IRQ_CLEAR  (1u << 19) //!< Clear The interrupt is triggered when the TX filter is enabled, and a TX message is rejected interrupt
#define XCAN_IC_FC_MH_TX_ABORT_IRQ_CLEAR   (1u << 20) //!< Clear This interrupt line is triggered when the MH needs to abort a TX message being sent to the PRT interrupt
#define XCAN_IC_FC_MH_RX_ABORT_IRQ_CLEAR   (1u << 21) //!< Clear This interrupt line is triggered when the MH needs to abort a RX message being received from PRT interrupt
#define XCAN_IC_FC_MH_STATS_IRQ_CLEAR      (1u << 22) //!< Clear One of the RX/TX counters have reached the threshold interrupt

#define XCAN_IC_FC_PRT_E_ACTIVE_CLEAR      (1u << 24) //!< Clear PRT switched from Error-Passive to Error-Active state interrupt
#define XCAN_IC_FC_PRT_BUS_ON_CLEAR        (1u << 25) //!< Clear PRT started CAN communication, after start or end of BusOff interrupt
#define XCAN_IC_FC_PRT_TX_EVT_CLEAR        (1u << 26) //!< Clear PRT transmitted a valid CAN message interrupt
#define XCAN_IC_FC_PRT_RX_EVT_CLEAR        (1u << 27) //!< Clear PRT received a valid CAN message interrupt

//-----------------------------------------------------------------------------

/*! XCAN Error raw event clear register (Write-only, Offset: 0xA14, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register ERR_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_EC_Register
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

#define XCAN_IC_EC_SC_MH_RX_FILTER_ERR_CLEAR  (1u <<  0) //!< Clear MH RX filtering has not finished in time
#define XCAN_IC_EC_SC_MH_MEM_SFTY_ERR_CLEAR   (1u <<  1) //!< Clear MH detected error in L_MEM error interrupt
#define XCAN_IC_EC_SC_MH_REG_CRC_ERR_CLEAR    (1u <<  2) //!< Clear MH detected CRC error at the register bank error interrupt
#define XCAN_IC_EC_SC_MH_DESC_ERR_CLEAR       (1u <<  3) //!< Clear CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_EC_SC_MH_AP_PARITY_ERR_CLEAR  (1u <<  4) //!< Clear MH detected parity error at address pointers, used to manage the MH Queues
#define XCAN_IC_EC_SC_MH_DP_PARITY_ERR_CLEAR  (1u <<  5) //!< Clear MH detected parity error at RX message data error interrupt
#define XCAN_IC_EC_SC_MH_DP_SEQ_ERR_CLEAR     (1u <<  6) //!< Clear MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_EC_SC_MH_DP_DO_ERR_CLEAR      (1u <<  7) //!< Clear MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_EC_SC_MH_DP_TO_ERR_CLEAR      (1u <<  8) //!< Clear MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_EC_SC_MH_DMA_TO_ERR_CLEAR     (1u <<  9) //!< Clear MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_EC_SC_MH_DMA_CH_ERR_CLEAR     (1u << 10) //!< Clear MH detected routing error interrupt
#define XCAN_IC_EC_SC_MH_RD_RESP_ERR_CLEAR    (1u << 11) //!< Clear MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EC_SC_MH_WR_RESP_ERR_CLEAR    (1u << 12) //!< Clear MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EC_SC_MH_MEM_TO_ERR_CLEAR     (1u << 13) //!< Clear MH detected timeout at local memory interface MEM_AXI error interrupt

#define XCAN_IC_EC_SC_PRT_ABORTED_CLEAR       (1u << 16) //!< Clear PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT error interrupt
#define XCAN_IC_EC_SC_PRT_USOS_CLEAR          (1u << 17) //!< Clear PRT detected unexpected Start of Sequence during TX_MSG sequence error interrupt
#define XCAN_IC_EC_SC_PRT_TX_DU_CLEAR         (1u << 18) //!< Clear PRT detected underrun condition at TX_MSG sequence error interrupt
#define XCAN_IC_EC_SC_PRT_RX_DO_CLEAR         (1u << 19) //!< Clear PRT detected overflow condition at RX_MSG sequence error interrupt
#define XCAN_IC_EC_SC_PRT_IFF_RQ_CLEAR        (1u << 20) //!< Clear PRT detected invalid Frame Format at TX_MSG error interrupt
#define XCAN_IC_EC_SC_PRT_BUS_ERR_CLEAR       (1u << 21) //!< Clear PRT detected error on the CAN Bus error interrupt
#define XCAN_IC_EC_SC_PRT_E_PASSIVE_CLEAR     (1u << 22) //!< Clear PRT switched from Error-Active to Error-Passive state error interrupt
#define XCAN_IC_EC_SC_PRT_BUS_OFF_CLEAR       (1u << 23) //!< Clear PRT entered Bus_Off state error interrupt

#define XCAN_IC_EC_SC_TOP_MUX_TO_ERR_CLEAR    (1u << 28) //!< Clear Timeout at top-level multiplexer error interrupt

//-----------------------------------------------------------------------------

/*! XCAN Safety raw event clear register (Write-only, Offset: 0xA18, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register SAFETY_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_SC_Register
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

//-----------------------------------------------------------------------------

/*! XCAN Error raw event clear register (Read/Write, Offset: 0xA20, Initial value: 0x00000000)
 * Writing a 1 to a certain bit position clears the corresponding bit of register ERR_RAW. Writing a ’0’ has no effect
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_FE_Register
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
                                   *           This interrupt is triggered when an invalid RX descriptor is fetched from this RX FIFO Queue, or an RX
                                   *           message is received (if set in RX descriptor) in this RX FIFO Queue, see description of RX_FQ_IRQ[7:0] in MH section
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
    uint32_t                 : 3; //!< 28-31
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
typedef union __XCAN_PACKED__ XCAN_IC_EE_Register
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

#define XCAN_IC_EE_SE_MH_RX_FILTER_ERR_EN   (1u <<  0) //!< Enable MH RX filtering has not finished in time
#define XCAN_IC_EE_SE_MH_RX_FILTER_ERR_DIS  (0u <<  0) //!< Disable MH RX filtering has not finished in time
#define XCAN_IC_EE_SE_MH_MEM_SFTY_ERR_EN    (1u <<  1) //!< Enable MH detected error in L_MEM error interrupt
#define XCAN_IC_EE_SE_MH_MEM_SFTY_ERR_DIS   (0u <<  1) //!< Disable MH detected error in L_MEM error interrupt
#define XCAN_IC_EE_SE_MH_REG_CRC_ERR_EN     (1u <<  2) //!< Enable MH detected CRC error at the register bank error interrupt
#define XCAN_IC_EE_SE_MH_REG_CRC_ERR_DIS    (0u <<  2) //!< Disable MH detected CRC error at the register bank error interrupt
#define XCAN_IC_EE_SE_MH_DESC_ERR_EN        (1u <<  3) //!< Enable CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_EE_SE_MH_DESC_ERR_DIS       (0u <<  3) //!< Disable CRC error detected on RX/TX descriptor or RX/TX descriptor not expected detected error interrupt
#define XCAN_IC_EE_SE_MH_AP_PARITY_ERR_EN   (1u <<  4) //!< Enable MH detected parity error at address pointers, used to manage the MH Queues
#define XCAN_IC_EE_SE_MH_AP_PARITY_ERR_DIS  (0u <<  4) //!< Disable MH detected parity error at address pointers, used to manage the MH Queues
#define XCAN_IC_EE_SE_MH_DP_PARITY_ERR_EN   (1u <<  5) //!< Enable MH detected parity error at RX message data error interrupt
#define XCAN_IC_EE_SE_MH_DP_PARITY_ERR_DIS  (0u <<  5) //!< Disable MH detected parity error at RX message data error interrupt
#define XCAN_IC_EE_SE_MH_DP_SEQ_ERR_EN      (1u <<  6) //!< Enable MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_EE_SE_MH_DP_SEQ_ERR_DIS     (0u <<  6) //!< Disable MH detected an incorrect sequence at RX_MSG respective TX_MSG interfaces located between MH and PRT error interrupt
#define XCAN_IC_EE_SE_MH_DP_DO_ERR_EN       (1u <<  7) //!< Enable MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_EE_SE_MH_DP_DO_ERR_DIS      (0u <<  7) //!< Disable MH detected a data overflow at RX buffer error interrupt
#define XCAN_IC_EE_SE_MH_DP_TO_ERR_EN       (1u <<  8) //!< Enable MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_EE_SE_MH_DP_TO_ERR_DIS      (0u <<  8) //!< Disable MH detected timeout at TX_MSG interface located between MH and PRT error interrupt
#define XCAN_IC_EE_SE_MH_DMA_TO_ERR_EN      (1u <<  9) //!< Enable MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_EE_SE_MH_DMA_TO_ERR_DIS     (0u <<  9) //!< Disable MH detected timeout at DMA_AXI interface error interrupt
#define XCAN_IC_EE_SE_MH_DMA_CH_ERR_EN      (1u << 10) //!< Enable MH detected routing error interrupt
#define XCAN_IC_EE_SE_MH_DMA_CH_ERR_DIS     (0u << 10) //!< Disable MH detected routing error interrupt
#define XCAN_IC_EE_SE_MH_RD_RESP_ERR_EN     (1u << 11) //!< Enable MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_SE_MH_RD_RESP_ERR_DIS    (0u << 11) //!< Disable MH detected a bus error caused by a read access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_SE_MH_WR_RESP_ERR_EN     (1u << 12) //!< Enable MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_SE_MH_WR_RESP_ERR_DIS    (0u << 12) //!< Disable MH detected a bus error caused by a write access to S_MEM respective L_MEM error interrupt
#define XCAN_IC_EE_SE_MH_MEM_TO_ERR_EN      (1u << 13) //!< Enable MH detected timeout at local memory interface MEM_AXI error interrupt
#define XCAN_IC_EE_SE_MH_MEM_TO_ERR_DIS     (0u << 13) //!< Disable MH detected timeout at local memory interface MEM_AXI error interrupt

#define XCAN_IC_EE_SE_PRT_ABORTED_EN        (1u << 16) //!< Enable PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT error interrupt
#define XCAN_IC_EE_SE_PRT_ABORTED_DIS       (0u << 16) //!< Disable PRT detected stop of TX_MSG sequence by TX_MSG_WUSER code ABORT error interrupt
#define XCAN_IC_EE_SE_PRT_USOS_EN           (1u << 17) //!< Enable PRT detected unexpected Start of Sequence during TX_MSG sequence error interrupt
#define XCAN_IC_EE_SE_PRT_USOS_DIS          (0u << 17) //!< Disable PRT detected unexpected Start of Sequence during TX_MSG sequence error interrupt
#define XCAN_IC_EE_SE_PRT_TX_DU_EN          (1u << 18) //!< Enable PRT detected underrun condition at TX_MSG sequence error interrupt
#define XCAN_IC_EE_SE_PRT_TX_DU_DIS         (0u << 18) //!< Disable PRT detected underrun condition at TX_MSG sequence error interrupt
#define XCAN_IC_EE_SE_PRT_RX_DO_EN          (1u << 19) //!< Enable PRT detected overflow condition at RX_MSG sequence error interrupt
#define XCAN_IC_EE_SE_PRT_RX_DO_DIS         (0u << 19) //!< Disable PRT detected overflow condition at RX_MSG sequence error interrupt
#define XCAN_IC_EE_SE_PRT_IFF_RQ_EN         (1u << 20) //!< Enable PRT detected invalid Frame Format at TX_MSG error interrupt
#define XCAN_IC_EE_SE_PRT_IFF_RQ_DIS        (0u << 20) //!< Disable PRT detected invalid Frame Format at TX_MSG error interrupt
#define XCAN_IC_EE_SE_PRT_BUS_ERR_EN        (1u << 21) //!< Enable PRT detected error on the CAN Bus error interrupt
#define XCAN_IC_EE_SE_PRT_BUS_ERR_DIS       (0u << 21) //!< Disable PRT detected error on the CAN Bus error interrupt
#define XCAN_IC_EE_SE_PRT_E_PASSIVE_EN      (1u << 22) //!< Enable PRT switched from Error-Active to Error-Passive state error interrupt
#define XCAN_IC_EE_SE_PRT_E_PASSIVE_DIS     (0u << 22) //!< Disable PRT switched from Error-Active to Error-Passive state error interrupt
#define XCAN_IC_EE_SE_PRT_BUS_OFF_EN        (1u << 23) //!< Enable PRT entered Bus_Off state error interrupt
#define XCAN_IC_EE_SE_PRT_BUS_OFF_DIS       (0u << 23) //!< Disable PRT entered Bus_Off state error interrupt

#define XCAN_IC_EE_SE_TOP_MUX_TO_ERR_EN     (1u << 28) //!< Enable Timeout at top-level multiplexer error interrupt
#define XCAN_IC_EE_SE_TOP_MUX_TO_ERR_DIS    (0u << 28) //!< Disable Timeout at top-level multiplexer error interrupt

//-----------------------------------------------------------------------------

/*! XCAN Safety raw event enable register (Read/Write, Offset: 0xA28, Initial value: 0x00000000)
 * Any bit in the SAFETY_ENA register enables the corresponding bit in the SAFETY_RAW to trigger the interrupt line SAFETY_INT.
 * The interrupt line gets active high, when at least one RAW/ENA pair is 1, e.g. SAFETY_RAW.MH_TX_FQ_IRQ = SAFETY_ENA.MH_TX_FQ_IRQ = 1
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_SE_Register
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
} XCAN_IC_SE_Register;
XCAN_UNPACKITEM;
XCAN_CONTROL_ITEM_SIZE(XCAN_IC_SE_Register, 4);

//-----------------------------------------------------------------------------

/*! XCAN IRC configuration register (Read, Offset: 0xA30, Initial value: 0x00000007)
 * This register shows the hardware configuration of the IRC concerning the capturing mode of the event inputs.
 * The IP internal events signals coming from the MH and the PRT require an 'edge sensitive' capturing.
 * That is why the value of this register is 0x7 and cannot be changed
 */
XCAN_PACKITEM
typedef union __XCAN_PACKED__ XCAN_IC_CM_Register
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
typedef union __XCAN_PACKED__ XCAN_IC_HDP_Register
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