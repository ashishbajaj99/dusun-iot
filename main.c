/*===================================================================================*/
/*************************************************************************************/
/**
 * @file      main.c
 * @brief     Main event loop management
 * @details   Contains the main event loop management logic using select() system call.
 *            Calls MQTT Rx Message Handler and Smartamm Rx Message Handler with the
 *            corresponding file descriptors get ready.
 *            Expectation is that this code forms the boilerplate for future socket
 *            interfaces.
 *            Examples of future socket interfaces are for Z-wave (zipgateway),
 *            ONVIF (camgateway), etc.
 * @see
 * @author    Ashish Bajaj, ashish@elear.solutions
 * @author    Manav Kumar Mehta, manav@elear.solutions
 * @author    Avinash Singh Chauhan, avinashchauhan@elear.solutions
 * @copyright Copyright (c) 2018 Elear Solutions Tech Private Limited. All rights
 *            reserved.
 * @license   To any person (the "Recipient") obtaining a copy of this software and
 *            associated documentation files (the "Software"):\n
 *            All information contained in or disclosed by this software is
 *            confidential and proprietary information of Elear Solutions Tech
 *            Private Limited and all rights therein are expressly reserved.
 *            By accepting this material the recipient agrees that this material and
 *            the information contained therein is held in confidence and in trust
 *            and will NOT be used, copied, modified, merged, published, distributed,
 *            sublicensed, reproduced in whole or in part, nor its contents revealed
 *            in any manner to others without the express written permission of
 *            Elear Solutions Tech Private Limited.
 */
/*************************************************************************************/
/*===================================================================================*/
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "rbsdk.h"

/*************************************************************************************
 *                          LOCAL MACROS                                             *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL TYPEDEFS                                           *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL PROTOTYPES                                         *
 *************************************************************************************/
static int zigbee_sdk_init(int argc, char *argv[]);
static int zigbee_sdk_discover_resource(void);
static int zigbee_sdk_rem_resource(char *deviceEui);
static int zigbee_sdk_add_res_cb(char *deviceEui, char *modelStr, char *model, int type, int battery,
                                 int *epList, int epCount);
static int zigbee_sdk_rem_res_cb(char *deviceEui);
static int zigbee_sdk_online_res_cb(char *deviceEui, int online);
static int zigbee_sdk_gen_cmd_cb(char *deviceEui, int endpoint, int clusterId, int cmdId,
                                 char *payload, int payloadLen, int linkQuality);
static int zigbee_sdk_attrib_cb(char *deviceEui, int endpoint, int clusterId, int attribId,
                                char *payload, int payloadLen, int linkQuality);
static int zigbee_sdk_simple_desc_cb(char *deviceEui, unsigned char endpoint, char *payload, int payloadLen);
static int zigbee_sdk_explore_status_cb(char *deviceEui, int status, int clusterId);
static int zigbee_sdk_cmd_status_cb(char *deviceEui, int endpoint, int clusterId, int cmdId,
                                    int status, int linkQuality, void *context);
static int string_to_uint8(const char *str, char *retval);
static int string_to_byte_stream(const char *str, char *bytes);
static char *byte_stream_to_str(char *bytes, char *str);
static void zigbee_sdk_list_resource();
static void callback_list_resource(char *deviceEui, int *epList, int epCount, char *modelStr,
                                   char *model, int type, int battery);

/*************************************************************************************
 *                          GLOBAL VARIABLES                                         *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL VARIABLES                                          *
 *************************************************************************************/

/*************************************************************************************
 *                          PRIVATE FUNCTIONS                                        *
 *************************************************************************************/
/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int string_to_uint8(const char *str, char *retval) {
  char *endptr;
  *retval = 0;
  if(NULL == str) {
    printf("Invalid (null) input\n");
    return -1;
  }

  if(2 != strlen(str)) {
    printf("Invalid uint8 string '%s', should consist of 2 hex digits\n", str);
    return -1;
  }

  *retval = strtol(str, &endptr, 16);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int string_to_byte_stream(const char *str, char *bytes) {
  char byteString[3];
  int i;
  int slen;
  int numBytes;

  if(NULL == str) {
    printf("Invalid (null) input\n");
    return 0;
  }

  slen = strlen(str);
  numBytes = slen/2;

  if (0 != (slen % 2) ) {
    printf("Invalid string '%s', length of string should be an even number\n", str);
    return 0;
  }

  for (i = 0; i < numBytes; i++) {
    byteString[0] = str[i * 2];
    byteString[1] = str[(i * 2) + 1];
    byteString[2] = '\0';
    if(-1 == string_to_uint8(byteString, &bytes[i])) {
      return 0;
    }
  }

  return i;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static char *byte_stream_to_str(char *bytes, char *str) {
	int i = 0, len = 0;

	for (i = 0; i < 8; i++) {
		len += sprintf(str + len, "%02X", bytes[i] & 0xff);
	}

	return str;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_discover_resource(void) {
  printf("Discovering resources to include\n");
  char *deviceEui = "\xff\xff\xff\xff\xff\xff\xff\xff";
  char *type = "0000";
  rbsdk_add_dev(deviceEui, type);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_rem_resource(char *deviceEui) {
  printf("Removing resource: %s\n", deviceEui);
  rbsdk_del_dev(deviceEui);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_add_res_cb(char *deviceEui, char *modelStr, char *model, int type, int battery,
                                 int *epList, int epCount) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);
  printf("Resource : %s with Model: %s and Model : %s, with type %d having batteryPct :"
         " %d added successfully with noOf endpoints :%d\n", deviceId, modelStr, model,
         type, battery, epCount);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_rem_res_cb(char *deviceEui) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);
  printf("Resource : %s removed successfully\n", deviceId);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_online_res_cb(char *deviceEui, int online) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);
  printf("Resource : %s is online : %d\n", deviceId, online);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_gen_cmd_cb(char *deviceEui, int endpoint, int clusterId, int cmdId,
                                 char *payload, int payloadLen, int linkQuality) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);
  printf("Resource : %s, endpointId: %d, clusterId: %d generated command: %d "
         "with payloadLen: %d and linkQuality: %d\n", deviceId, endpoint, clusterId, cmdId,
         payloadLen, linkQuality);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_attrib_cb(char *deviceEui, int endpoint, int clusterId, int attribId,
                                char *payload, int payloadLen, int linkQuality) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);
  printf("Resource : %s, endpointId: %d, clusterId: %d attribute report for attributeId: %d "
         "with payloadLen: %d and linkQuality: %d\n", deviceId, endpoint, clusterId, attribId,
         payloadLen, linkQuality);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_simple_desc_cb(char *deviceEui, unsigned char endpoint, char *payload,
                                     int payloadLen) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);
  printf("Simple descriptor Report for Resource : %s and endpointId: %d "
         "with payloadLen: %d\n", deviceId, endpoint, payloadLen);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_explore_status_cb(char *deviceEui, int status, int clusterId) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);
  printf("Exploration Status for Resource : %s and clusterId: %d is: %d\n", deviceId, clusterId, status);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static int zigbee_sdk_cmd_status_cb(char *deviceEui, int endpoint, int clusterId, int cmdId,
                                    int status, int linkQuality, void *context) {
  char deviceId[17] = { 0 };
  int *seqNum = (int *)context;

  byte_stream_to_str(deviceEui, deviceId);
  printf("Resource : %s, endpointId: %d, clusterId: %d command: %d processed "
         "with status: %d and linkQuality: %d having seqNum: %d\n", deviceId, endpoint, clusterId, cmdId,
         status, linkQuality, *seqNum);
  return 0;
}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static void callback_list_resource(char *deviceEui, int *epList, int epCount, char *modelStr,
                            char *model, int type, int battery) {
  char deviceId[17] = { 0 };

  byte_stream_to_str(deviceEui, deviceId);

  printf("Resource : %s with Model: %s and Model : %s, with type %d having batteryPct :"
         " %d added successfully with noOf endpoints :%d\n", deviceId, modelStr, model,
         type, battery, epCount);

}

/*************************************************************************************
 * Refer to the header file for a detailed description                               *
 *************************************************************************************/
static void zigbee_sdk_list_resource() {
  printf("Listing resources\n");
	rbsdk_list_dev(callback_list_resource);
}

/******************************************************************************
Name        : zigbee_sdk_init
Input(s)    : int argc, char *argv[] - standard inputs to the main routine
Output(s)   : int: 0 on SUCCESS, else FAILURE
Description : initializes required modules to use zigbee sdk library
*******************************************************************************/
static int zigbee_sdk_init(int argc, char *argv[] ) {
  printf("TestApp rbsdk_init Started\n");
  char sdkVer[33];
  stDevMsgCb_t *zigbeeSdkMsgCb = malloc(sizeof(stDevMsgCb_t));

  memset(zigbeeSdkMsgCb, 0, sizeof(stDevMsgCb_t));

  if (0 != rbsdk_init(NULL, NULL)) {
    printf("Fatal: Unable to initialize zigbee library\n");
    return EXIT_FAILURE;
  }
  sleep(2);
  printf("TestApp rbsdk_init Done\n");

  zigbeeSdkMsgCb->rpt_attr = zigbee_sdk_attrib_cb;
  zigbeeSdkMsgCb->rpt_cmd = zigbee_sdk_gen_cmd_cb;
  zigbeeSdkMsgCb->rpt_exploration = zigbee_sdk_explore_status_cb;
  zigbeeSdkMsgCb->rpt_simp_desc = zigbee_sdk_simple_desc_cb;
  zigbeeSdkMsgCb->rpt_zcl_cmd_status = zigbee_sdk_cmd_status_cb;
  zigbeeSdkMsgCb->rpt_dev_added = zigbee_sdk_add_res_cb;
  zigbeeSdkMsgCb->rpt_dev_deled = zigbee_sdk_rem_res_cb;
  zigbeeSdkMsgCb->rpt_dev_online = zigbee_sdk_online_res_cb;

  printf("TestApp rbsdk_dev_msgcb_set Started\n");
  if (0 != rbsdk_dev_msgcb_set(zigbeeSdkMsgCb)) {
    printf("Fatal: Unable to set library callbacks\n");
    return EXIT_FAILURE;
  }
  printf("TestApp rbsdk_dev_msgcb_set Done\n");

  printf("TestApp rbsdk_vers Started\n");
  if (0 != rbsdk_vers(sdkVer)) {
    printf("Fatal: Unable to get library version\n");
    return EXIT_FAILURE;
  }
  printf("TestApp rbsdk_vers Started\n");

  printf("TestApp zigbee sdk initialized successfully with version: %s\n", sdkVer);
  return 0;
}

/******************************************************************************
Name        : main
Input(s)    : int argc, char *argv[] - standard inputs to the main routine
Output(s)   : int: EXIT_SUCCESS to notify the calling function of success
Description : Inits the application and then falls into a dispatch loop
              using select() system call.
              The timeout paramter of select is used to keep the timer module
              upto date for timer management to work without utilizing too many
              system resources and system timers
              Manages clean exit in case select() returns an error
*******************************************************************************/
int main(int argc, char *argv[]) {
  zigbee_sdk_init(argc, argv);
  int i, choice, epId, clusterId, attributeId, commandId, payloadLen, manuf, cmdCnt;
  char *resourceEui = malloc(17 * sizeof(char));
  char *payload = NULL, *cmdData = NULL;
  char deviceId[8] = { 0 };
  int seqNUm = 1;

  do {
    printf("Select one of the below:\n");
    printf("0. Exit\n");
    printf("1. List resources\n");
    printf("2. Discover and Add resources\n");
    printf("3. Remove resource\n");
    printf("4. Read Attribute\n");
    printf("5. Send Command \n");
    printf("6. Send Multiple Command \n");
    scanf("%d", &choice);

    switch(choice) {
      case 0:
        return EXIT_SUCCESS;

			case 1:
				printf("list devices...\n");
				zigbee_sdk_list_resource();
				break;

      case 2:
        zigbee_sdk_discover_resource();
        break;

      case 3:
        printf("Sending remove resource command to device\n");
        printf("Enter the resourceEui\n");
        memset(resourceEui, 0, 17);
        scanf("%s", resourceEui);
        string_to_byte_stream(resourceEui, deviceId);
        zigbee_sdk_rem_resource(deviceId);
        break;

      case 4:
        printf("Sending read attribute command\n");
        printf("Enter the resourceEui\n");
        memset(resourceEui, 0, 17);
        scanf("%s", resourceEui);
        printf("Enter the endpointId\n");
        scanf("%d", &epId);
        printf("Enter the clusterId\n");
        scanf("%d", &clusterId);
        printf("Enter the attributeId\n");
        scanf("%d", &attributeId);
        string_to_byte_stream(resourceEui, deviceId);
        rbsdk_get_attr(deviceId, epId, clusterId, attributeId);
        break;

      case 5:
        seqNUm++;
        payload = NULL;
        cmdData = NULL;
        printf("Sending ZCL command with SeqNum = %d\n", seqNUm);
        printf("Enter the resourceEui\n");
        memset(resourceEui, 0, 17);
        scanf("%s", resourceEui);
        printf("Is Command Manufacturer Specific?\n 0-> No\n manufId->Yes\n");
        scanf("%d", &manuf);
        printf("Enter the endpointId\n");
        scanf("%d", &epId);
        printf("Enter the clusterId\n");
        scanf("%d", &clusterId);
        printf("Enter the commandId\n");
        scanf("%d", &commandId);
        printf("Enter the payloadLen\n");
        scanf("%d", &payloadLen);
        if (0 != payloadLen) {
          printf("Enter payload byte stream:\n");
          payload = malloc((payloadLen * 2) * sizeof(char));
          memset(payload, 0, (payloadLen * 2));
          scanf("%s", payload);
          cmdData = malloc(payloadLen * sizeof(char));
          memset(cmdData, 0, payloadLen);
          string_to_byte_stream(payload, cmdData);
        }
        string_to_byte_stream(resourceEui, deviceId);
        printf("Enter number of commands to be sent: \n");
        scanf("%d", &cmdCnt);

        for (i = 0; i < cmdCnt; i++) {
          seqNUm++;
          printf("Sending ZCL command with SeqNum = %d, payloadLen = %d\n", seqNUm, payloadLen);
          rbsdk_zcl_cmd(deviceId, manuf, epId, clusterId, commandId, cmdData, payloadLen, &seqNUm);
        }
        break;

      default:
        printf("Invalid input. Please try again\n");
    }

  } while (1);

  return EXIT_SUCCESS;
}
