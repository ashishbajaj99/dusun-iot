#ifndef __Z3_H_
#define __Z3_H_


#ifdef __cplusplus
extern "C" {
#endif

/*=============
 *Struct Define
 *===========================================================================================*/
typedef struct stAttr {
	short					attrid;
	unsigned char type;
	int						len;
	char					*data;
	long					last;		/* last update time (ms)*/
} stAttr_t;

typedef struct stCluster {
	short					clsid;
	int						attrcnt;
	stAttr_t			*attrs;
}stCluster_t;

typedef struct stNodeDesc {
	unsigned char nodetype;
	unsigned char complex_desc_available;
	unsigned char user_desc_available;
	unsigned char reserved;
	unsigned char apsflag;
	unsigned char freqband;
	unsigned char mac_cap;
	short					manufacturer_code;
	unsigned char max_buffer_size;
	short					min_incomming_transfer_size;
	short					server_mask_capabilities;
	unsigned char max_outgoing_transfer_size;
	unsigned char desc_capability;
}stNodeDesc_t;

typedef struct stSimplerDesc {
	unsigned char							ep;
	short											appproid;
	short											appdevid;
	unsigned char							appdevver;
	char											reserved;

	int												incnt;
	stCluster_t								*incls;
	int												outcnt;
	stCluster_t								*outcls;
}stSimplerDesc_t;

typedef struct stZ3Device {
	unsigned char					extaddr[8]; 
	short									nwkaddr; 

	int										epcnt;
	stSimplerDesc_t				*epdescs;

	stNodeDesc_t		nodedesc;
} stZ3Device_t;


typedef struct stZclAttr {
	short	aid;
	const char *name;
	char	type;
	int		min;
	int		max;
	char  *rw;
	int		len;
	const char	*def;
	char	mad;
	char  cr;		/* config report */
	short	minrptdlt;
	short	maxrptdlt;
	int		changerpt;
}stZclAttr_t;

typedef struct stZclRcmd {
	char cmd;
	const char *name;
	char mad;
}stZclRcmd_t;

typedef struct stZclGcmd {
	char cmd;
	const char *name;
	char mad;
}stZclGcmd_t;

typedef struct stZclServer {
	int					support;

	int					acnt;
	stZclAttr_t		attrs[30];

	int					rcnt;
	stZclRcmd_t		rcmds[30];

	int					gcmt;
	stZclGcmd_t		gcmds[30];
}stZclServer_t;

typedef struct stZclServer stZclClient_t;

typedef struct stZclCluster {
	short 				cid;
	char					*name;
	stZclServer_t		server;
	stZclClient_t		client;
}stZclCluster_t;

enum {
	t_NODATA	= 0x00,
	t_DATA8		= 0x08,
	t_DATA16	= 0x09,
	t_DATA24	= 0x0a,
	t_DATA32	= 0x0b,
	t_DATA40	= 0x0c,
	t_DATA48	= 0x0d,
	t_DATA56	= 0x0e,
	t_DATA64	= 0x0f,
	t_BOOL		= 0x10,

	t_MAP8		= 0X18,
	t_MAP16		= 0x19,
	t_MAP24		= 0x1a,
	t_MAP32		= 0x1b,
	t_MAP40		= 0x1c,
	t_MAP48		= 0x1d,
	t_MAP56		= 0x1e,
	t_MAP64		= 0x1f,

	t_UINT8		= 0x20,
	t_UINT16	= 0x21,
	t_UINT24	= 0x22,
	t_UINT32	= 0x23,
	t_UINT40	= 0x24,
	t_UINT48	= 0x25,
	t_UINT56	= 0x26,
	t_UINT64	= 0x27,

	t_INT8		= 0x28,
	t_INT16		= 0x29,
	t_INT24		= 0x2a,
	t_INT32		= 0x2b,
	t_INT40		= 0x2c,
	t_INT48		= 0x2d,
	t_INT56		= 0x2e,
	t_INT64		= 0x2f,


	t_ENUM8		= 0x30,
	t_ENUM16	= 0X31,

	t_SEMI		= 0x38,
	t_SINGLE	= 0x39,
	t_DOUBLE	= 0x3a,

	t_OCTSTR		= 0x41,
	t_STRING		= 0x42,
	t_OCTSTR16	= 0x43,
	t_STRING16	= 0x44,

	t_ARRAY			= 0x48,
	t_STRUCT		= 0x4c,

	t_SET			= 0x50,
	t_BAG			= 0x51,
	
	t_TOD				= 0xe0,
	t_DATE			= 0xe1,
	t_UTC				= 0xe2,
	t_CLUSTERID	= 0xe8,
	t_ATTRIBID	= 0xe9,
	t_BACOID		= 0xea,
	t_EUI64			= 0xf0,
	t_KEY128		= 0xf1,
	/*t_OPAUE			= 0x, */
	t_UNK				= 0xff,

	/*
	t_UINT8 	= 0x00, 
	t_MAP32		= 0x01,
	t_UINT16 	= 0x02,
	t_MAP8		= 0x03,
	t_BOOL		= 0x04,
	t_ENUM8		= 0x05,
	t_UTC			= 0x06,
	t_DATA8		= 0x07,
	t_INT16		= 0x08,
	t_INT8		= 0x09,
	t_ENUM16	= 0x0a,
	t_MAP16		= 0x0b,
	t_EUI64		= 0x0c,
	t_STRING	= 0x0d,
	t_INT24		= 0x0e,
	*/
};

/*=============
 * To Implement 
 *===========================================================================================*/
stZclCluster_t *z3_get_clusters();
int z3_get_clusters_cnt();

/*=============
 * Callback Api 
 *===========================================================================================*/
/* 
 * @param[in] addr				- 8bytes device's mac 
 * */
int z3_register(char *addr, int *epList, int epListLen, char *ModelStr, char *model, int type, int battery);
int z3_unregister(char *addr);
int z3_online(char *addr, long last, int online);
int z3_attr(char *addr, unsigned char ep, unsigned short clsid,  const char *buf, int len, int linkQuality);
int z3_cmd(char *addr, unsigned char ep, unsigned short clsid, const char *buf, int len, int linkQuality);
int z3_simple_desc(char *addr, unsigned char ep, char *buf, int len);

/**
 * -1 any faile
 * 0  accoc
 * 1  bind success any given cluster
 * 2  configure success
 **/
int z3_exploration(char *addr, int status, int cluster);

int z3_zcl_cmd_ret(char *addr, unsigned char ep, unsigned short clsid, char cmdid, int status, int linkQuality, void *context);
int z3_set_att_ret(char *addr, unsigned char ep, unsigned short clsid, unsigned short attr, int status, int linkQuality, void *context);
int z3_get_att_ret(char *addr, unsigned char ep, unsigned short clsid, unsigned short attr, int status, int linkQuality, void *context);

int z3_zcl_cmd_res(char *addr, unsigned char ep, unsigned short clusterid, unsigned char cmd, unsigned char seq, char *buf, int len);

/*=============
 * Call Api Sync & Async
 * Sync Api only can be called in the same thread with sdk
 * Async can be called in multi threads
 *===========================================================================================*/
/** z3_get_device(char *addr);
 *
 * get local device 
 *
 * @param[in] addr			subdevice addr to send to
 *
 * @return local device pointer or NULL.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
stZ3Device_t *z3_get_device(char *addr);

/** z3_list_device(void (*cb)(char *addr, int *epList, int epListLen, char *ModelStr, char *model, int type, int battery));
 *
 * list local device 
 *
 * @param[in] cb			callback function to each device
 *
 * @return 
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
void z3_list_device(void (*cb)(char *addr, int *epList, int epListLen, char *ModelStr, char *model, int type, int battery));
void z3_list_device_a(void (*cb)(char *addr, int *epList, int epListLen, char *ModelStr, char *model, int type, int battery));


/** z3_run
 *
 * start zigbee 3 stack, it will not return 
 *
 * @param[in] argc	arguments count
 * @param[in] argv	arguments 
 *
 * @return 0->ok, else failed.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_run(int argc, char *argv[]);

/** z3_versoin
 *
 * get sdk version
 *
 * @param[in] none
 *
 * @return version
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
char *z3_version();

/** z3_fd_reg(int fd, void (*func)(int fd))
 *
 * register a fd to sdk io loop
 *
 * @param[in] fd , fd to register
 *
 * @return 0 -> ok, -1 error
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_fd_reg(int fd, void (*func)(int fd));


/** z3_fd_unreg(int fd, void (*func)(int fd))
 *
 * unregister a fd from sdk io loop
 *
 * @param[in] fd , fd to unregister, will match fd and func
 *
 * @return 0 -> ok, -1 error
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_fd_unreg(int fd, void (*func)(int fd));

/** z3_permit 
 *
 * open zigbee network, permit subdevice to join.
 *
 * @param[in] addr			subdevice addr, "ffffffffffffffff" will allow all device to join, othewise it only allow the addr-device to join.
 * @param[in] duration	time of open the network, must in 0 ~ 120
 *
 * @return 0->ok, else failed.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_permit(char *addr, int duration);
int z3_permit_a(char *addr, int duration);

/** z3_remove
 *
 * remove subdevice form local list with force or not.
 *
 * @param[in] addr			subdevice addr to remove
 * @param[in] force			0->send a leave message to the device and wait response, remove this when receive this response
 *											1->flag the deivce to leaved status, and send leave message when receive any message from this device and then remove it
 *											2->direct remove this device from local
 *
 * @return 0->ok, else failed.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_remove(char *addr, int force);
int z3_remove_a(char *addr, int force);

/** z3_get_attr
 *
 * send a read attribute message to device
 *
 * @param[in] addr		subdevice addr to send to
 * @param[in] ep			subdevice endpoint to send to
 * @param[in] cluster	subdevice cluster to send to
 * @param[in] attrid	subdevice attribute id to send to
 *
 * @return 0->ok, else failed.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_get_att(char *addr, char ep, short cluster, short attrid);
int z3_get_att_a(char *addr, char ep, short cluster, short attrid);

/** z3_set_attr
 *
 * send a write attribute message to device
 *
 * @param[in] addr			subdevice addr to send to
 * @param[in] ep				subdevice endpoint to send to
 * @param[in] cluster		subdevice cluster to send to
 * @param[in] attrid		subdevice attribute id to send to
 * @param[in] attrtype	subdevice attribute type
 * @param[in] data			data to write to attribute
 * @param[in] len				data len
 *
 * @return 0->ok, else failed.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_set_att(char *addr, char ep, short cluster, short attrid, int attrtype, char *data, int len);
int z3_set_att_a(char *addr, char ep, short cluster, short attrid, int attrtype, char *data, int len);

/** z3_zcl_cmd
 *
 * send a zcl command message to device
 *
 * @param[in] addr			subdevice addr to send to
 * @param[in] manufacturer manufacturer if this cluster is manufacturer cluster, else set to zero
 * @param[in] ep				subdevice endpoint to send to
 * @param[in] cluster		subdevice cluster to send to
 * @param[in] cmdid			subdevice command id to send to
 * @param[in] data			data to send 
 * @param[in] len				data len
 *
 * @return 0->ok, else failed.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_zcl_cmd(char *addr, short manufacturer, char ep, short cluster, char cmdid, char *data, int len, void *context);
int z3_zcl_cmd_a(char *addr, short manufacturer, char ep, short cluster, char cmdid, char *data, int len, void *context);

/** z3_grp_cmd
 *
 * send a zcl command message to device
 *
 * @param[in] grpid			group id to send
 * @param[in] manufacturer manufacturer if this cluster is manufacturer cluster, else set to zero
 * @param[in] ep				subdevice endpoint to send to
 * @param[in] cluster		subdevice cluster to send to
 * @param[in] cmdid			subdevice command id to send to
 * @param[in] data			data to send 
 * @param[in] len				data len
 *
 * @return 0->ok, else failed.
 *
 * @warning none
 *
 * @note none
 *
 * @see none
 */
int z3_grp_cmd(short grpid, short manufacturer, char ep, short cluster, char cmdid, char *data, int len, void *context);
int z3_grp_cmd_a(short grpid, short manufacturer, char ep, short cluster, char cmdid, char *data, int len, void *context);

/**
 * Problem :
 * 
 * 1. Provide End Point List Array in dev_added callback:  int (*rpt_dev_added)(char *mac, char *ModelStr, int *epList, int epListLen, char *model, int type, int battery);
 *		ok you can get this inormation from z3_get_device
 *
 * 2. Provide linkQuality parameter in ZCL  data: int (*rpt_cmd)(char *mac, int ep, int cluster, int cmd, char *buf, int len, int linkQuality);
 *		ok
 *
 * 3. Provide linkQuality parameter in ZCL attribute data: int (*rpt_cmd)(char *mac, int ep, int cluster, int cmd, char *buf, int len, int linkQuality);
 *		ok
 *
 * 4. Provide Simple Descriptor Response callback: int (*rpt_simple_desc_rsp)(char *mac, int ep, char *buf, int len);
 *
 * 5. Support manufacturer specific frame control and application context parameter: 
 *		int rbsdk_zcl_cmd(char *mac, int ep, int cluster, int manufacturerSpecific, int cmdid, char *data, int len, void *context);
 *
 * 6. Provide ZCL Command status callback with application context parameter returned: 
 *		int (*rpt_zcl_cmd_status)(char *mac, int ep, int cluster, int cmdid, int status, int linkQuality, void *context);
 *
 * 7. Add Bind request: Endpoint binding should be done for *all* the clusters specifically IAS_ZONE and IAS_ACE clusters 
 *		– we currently see that these clusters are not sent the command. We have devices that don’t report unless you explicitly bind.
 *
 * 8. Configure Reporting for battery operated devices: This should have minimum reporting interval as 900 sec to reduce battery impact. Currently it is 60 sec. It should remain 60 seconds for mains powered devices.
 * */

#ifdef __cplusplus
}
#endif
#endif
