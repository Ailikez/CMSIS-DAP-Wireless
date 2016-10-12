/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"
#include "DAP_Common.h"
/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CUSTOM_HID 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Private_Variables
 * @{
 */
 
uint16_t USB_RequestIndexI;      // Request  Index In
uint16_t USB_RequestIndexO;      // Request  Index Out

uint16_t USB_ResponseIndexI;     // Response Index In
uint16_t USB_ResponseIndexO;     // Response Index Out

uint8_t  USB_Request [USB_PACKET_COUNT][DAP_PACKET_SIZE + 1];  // Request  Buffer
uint8_t  USB_Response[USB_PACKET_COUNT][DAP_PACKET_SIZE + 1];  // Response Buffer
    
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */ 

    0x06, 0x00, 0xFF,           /* HID_UsagePageVendor */
    0x09, 0x01,                 /* HID_Usage */
    0xA1, 0x01,                 /* HID_Collection: HID_Application */
    /* 7 */
    0x15, 0x00,                 /* value range: 0 - 0xFF */
    0x26, 0xFF, 0x00,
    0x75, 0x08,                 /* HID_ReportSize: 8 bits */
    /* 14 */
    0x95, CUSTOM_HID_EPIN_SIZE, /* HID_ReportCount */
    0x09, 0x01,                 /* HID_Usage */
    0x81, 0x02,                 /* HID_Input: HID_Data, HID_Variable, HID_Absolute */
    /* 20 */
    0x95, CUSTOM_HID_EPOUT_SIZE,/* HID_ReportCount */
    0x09, 0x01,                 /* HID_Usage */
    0x91, 0x02,                 /* HID_Output: HID_Data, HID_Variable, HID_Absolute */
    /* 26 */
    0x95, 0x01,                 /* HID_ReportCount: USBD_HID0_FEAT_REPORT_MAX_SZ */
    0x09, 0x01,                 /* HID_Usage */
    0xB1, 0x02,                 /* HID_Feature: HID_Data, HID_Variable, HID_Absolute */
    /* 32 */

  /* USER CODE END 0 */ 
  0xC0    /*     END_COLLECTION	             */
   
}; 

/* USER CODE BEGIN PRIVATE_VARIABLES */
/* USER CODE END PRIVATE_VARIABLES */
/**
  * @}
  */ 
  
/** @defgroup USBD_CUSTOM_HID_IF_Exported_Variables
  * @{
  */ 
  extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */ 
  
/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @{
  */
static int8_t CUSTOM_HID_Init_FS     (void);
static int8_t CUSTOM_HID_DeInit_FS   (void);
static int8_t CUSTOM_HID_OutEvent_FS (uint8_t event_idx, uint8_t state);
 

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS = 
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS,
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  CUSTOM_HID_Init_FS
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{ 
  /* USER CODE BEGIN 4 */ 
  USB_RequestIndexI = 0;
  USB_RequestIndexO = 0;
  USB_ResponseIndexI = 0;
  USB_ResponseIndexO = 0;

  return (0);
  /* USER CODE END 4 */ 
}

/**
  * @brief  CUSTOM_HID_DeInit_FS
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */ 
  return (0);
  /* USER CODE END 5 */ 
}

/**
  * @brief  CUSTOM_HID_OutEvent_FS
  *         Manage the CUSTOM HID class events       
  * @param  event_idx: event index
  * @param  state: event state
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS  (uint8_t event_idx, uint8_t state)
{ 
  /* USER CODE BEGIN 6 */ 
    
    USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef*)hUsbDeviceFS.pClassData;
//    (USBD_CUSTOM_HID_HandleTypeDef*)hUsbDeviceFS.ep_in->rem_length;

    memcpy(USB_Request[USB_RequestIndexI], hhid->Report_buf, DAP_PACKET_SIZE);
    
    USB_RequestIndexI = (USB_RequestIndexI + 1) % USB_PACKET_COUNT;
    
    return (0);
    
  /* USER CODE END 6 */ 
}

/* USER CODE BEGIN 7 */ 
/**
  * @brief  USBD_CUSTOM_HID_SendReport_FS
  *         Send the report to the Host       
  * @param  report: the report to be sent
  * @param  len: the report length
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
/*  
static int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len); 
}
*/
/* USER CODE END 7 */ 

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */ 

/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
