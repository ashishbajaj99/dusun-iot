#ifndef __RBSDK_H_
#define __RBSDK_H_

#ifdef __cplusplus
extern "C" {
#endif
	/** rbsdk_init
   *
	 * init the sdk must be called first before any rbsdk_* api to be called.
	 *
	 * @param[in] unused	reserved, not used now 
	 * @param[in] msgcb		reserved, not used now
	 *
	 * @return 0->ok, else failed.
	 *
	 * @warning none
	 *
	 * @note none
	 *
	 * @see none
	 */
	int rbsdk_init(void *unused, int (*msgcb)(char *src, char *dest, char *msg));

	/** rbsdk_vers
   *
   * get rbsdk version
   *
   * @param[in] version the buffer to return the version string, must be lagger than 32 bytes.
   *
   * @return 0->ok, else failed
   *
   * @warning none
   *
   * @note none
   *
   * @see none
   */
	int rbsdk_vers(char *version);

	/** rbsdk_list_dev
   *
   * get rbsdk version
   *
   * @param[in] cb list callback function
   *
   * @return 0->ok, else failed
   *
   * @warning none
   *
   * @note none
   *
   * @see none
   */
	int rbsdk_list_dev(void (*cb)(char *addr, int *epList, int epListLen, char *ModelStr, char *model, int type, int battery));


	/**> Externed */
	typedef struct stDevMsgCb {
		int (*rpt_dev_added)(char *mac, char *ModelStr, char *model, int type, int battery, int *epList, int epListLen);
		int (*rpt_dev_deled)(char *mac);
		int (*rpt_dev_online)(char *mac, int online);
	
		/**> rpt zcl command */
		int (*rpt_cmd)(char *mac, int ep, int cluster, int cmd, char *buf, int len, int linkQuality);
		/**> rpt attribute */
		int (*rpt_attr)(char *mac, int ep, int cluster, int attr, char *buf, int len, int linkQuality);
		/**> rpt simple desc */
		int (*rpt_simp_desc)(char *mac, unsigned char ep, char *buf, int len);
		/**> exploration */
		int (*rpt_exploration)(char *mac, int status, int cluster);
		/**> zcl cmd status */
		int (*rpt_zcl_cmd_status)(char *mac, int ep, int cluster, int cmdid, int status, int linkQuality, void *context);
		/**> zcl cmd response */
		int (*rpt_zcl_cmd_response)(char *mac, int ep, int cluster, int cmdid, int seq, char *buf, int len);
	} stDevMsgCb_t;


	/** rbsdk_dev_msgcb_set
   * 
   * set the callback functions for rbsdk
   *
   * @param[in] dmc the callback struct pointer to set
   *
   * @return 0->ok, else failed
   *
   * @warning none
   *
   * @note none
   *
   * @see none
   */
	int rbsdk_dev_msgcb_set(stDevMsgCb_t *dmc);

	/** rbsdk_add_dev
   * 
   * requset add device, this will open the zigbee network to permit new device to join.
   *
   * @param[in] mac the device's mac address to add
   * @param[in] type, the device's type to add, not used now, use "" or "0000"
   *
   * @return 0->ok, else failed
   *
   * @warning none
   *
   * @note when send this request, it will wait a few minitues or seconds to commission this device 
   *       if this device success joined to network, there will have a callback rpt_dev_added function
   *       to be called.
   *
   * @see none
   */
	int rbsdk_add_dev(char *mac, char *type);

	/** rbsdk_del_dev
   * 
   * request delete device, this will delete the device that has been added in the zigbee network.
   *
   * @param[in] mac the deivce's mac to delete.
   *
   * @return 0->ok, else failed
   *
   * @warning none
   *
   * @note when send this request, if the device has joinded to the nework, there will have a callback 
   *       rpt_dev_deled function to be called.
   *
   * @see none
   */
	int rbsdk_del_dev(char *mac);

	/** rbsdk_zcl_command
   * 
   * request zcl command
   *
   * @param[in] mac the device's mac address
   * @param[in] manufacturer value
   * @param[in] cluster the cluster to send to
   * @param[in] cmdid the command id to send
   * @param[in] data the data to send
   * @param[in] len the len of the data
   * @param[in] context pointer to the user defined data
   *
   * @return 0-ok, else failed
   *
   * @warning none
   *
   * @note none
   *
   * @see none
   */
	int rbsdk_zcl_cmd(char *mac, int manufacturer, int ep,	 int cluster, int cmdid,	char *data, int len, void *context);

	/** rbsdk_set_attr
   *
   * request set attribute
   *
   * @param[in] mac the device's mac address
   * @param[in] cluster the cluster to send to
   * @param[in] attrid attribute id to set
   * @param[in] atype attribute data type
   * @param[in] data the attribute's data 
   * @param[in] len the len for the data
   *
   * @warning none
   *
   * @note atype can referenc the zigbee cluster specification.
   *
   * @see none
   */
	int rbsdk_set_attr(char *mac, int ep, int cluster, int attrid, int atype, char *data, int len);

	/** rbsdk_get_attr
   *
   * request get attribute 
   *
   * @param[in] mac the device's mac address
   * @param[in] cluster the cluster to send to
   * @param[in] attrid attribute id to get
   *
   * @warning none
   * 
   * @note none
   *
   * @see none
   */
	int rbsdk_get_attr(char *mac, int ep, int cluster, int attrid);
#ifdef __cplusplus
}
#endif

#endif

