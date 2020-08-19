#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "z3.h"
#include "rbsdk.h"
#include "z3_slog.h"

/*============
 * clusters
 *=============================================================*/
stZclCluster_t my_zcl_clusters[] = {
	/*[0x0000] = */{
		0x0000, "Basic",
		{ /* server */
			1,
			2, {	/* attributes */
				{0x0005, "ModelIdentifier", 		t_STRING,	0x00, 0x00, 	"r", 	32, "\x00", 'O', 0, 0, 0, 0},
				{0x0001, "AppVersion",					t_UINT8,	0x00, 0xff, 	"r", 	1,  "\x00", 'O', 0, 0, 0, 0},
				{0x0000, "ZCLVersion", 					t_UINT8, 	0x00, 0xff, 	"r", 	1,	"\x02", 'M', 0, 0, 0, 0},
				{0x0003, "HwVersion",						t_UINT8,	0x00, 0xff, 	"r", 	1,  "\x00", 'O', 0, 0, 0, 0},
				{0x0004, "ManufacturerName", 		t_STRING,	0x00, 0x00, 	"r", 	32, "\x00", 'O', 0, 0, 0, 0},
			},
			0, {	/* command received */
			},
			0, {	/* command generated */
			},
		},
		{0},
	},

	/*[0x0001] = */{
		0x0001, "Power Configuration",
		{
			1,
			2,
			{
				{0x0020, "BatteryVoltage",						 t_UINT8,	0x00, 0xff,	"r",	1, "\x00", 'O', 0},
				{0x0021, "BatteryPercentageRemaining", t_UINT8, 0x00, 0xff,	"rr",	1, "\x00", 'O', 1, 1, 1800, 0x2},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0002] = */{
		0x0002, "Device Temperature Configuration",
		{
			1,
			1, {
				{0x0000, 	"CurrentTemperature", t_UINT16, -200, 	+200, 	"r", 	2, "\x00\x00", 		'm', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0003] = */{
		0x0003, "Identify",
		{
			1,
			0, {
				{0x0000, "IdentifyTime", t_UINT16, 0x0000, 0xffff, "rw", 2, "\x00\x00", 'M', 0, 0, 0, 0},
			},
			2, {
				{0x00,	"Identify",				'M'},
				{0x01,	"Identify Query", 'M'},
			},
			1, {
				{0x00, "Idnetify Query Response", 'M'},
			},
		},
		{0},
	},

	/*[0x0004] = */{
		0x0004, "Groups",
		{
			1,
			0, {
				{0x0000, "NameSupport", t_MAP8, 0x0, 0xffffffff, "r", 1, "\x00", 'M', 0, 0, 0, 0},
			},
			6, {
				{0x00, "Add Group", 							'M'},
				{0x01, "View Group", 							'M'},
				{0x02, "Get group membership", 		'M'},
				{0x03, "Remove group", 						'M'},
				{0x04, "Remove all groups", 			'M'},
				{0x05, "Add group if identifying",'M'},
			},
			4, {
				{0x00, "Add group response",						'M'},
				{0x01, "View group response",						'M'},
				{0x02, "Get group membership response", 'M'},
				{0x03, "Remove group respnse",					'M'},
			},
		},
		{0},
	},

	/*[0x0005] = */{
		0x0005, "Scenes",
		{
			1,
			0, {
				{0x0000, "SceneCount",		t_UINT8,	0x00, 	0xff, 			"r", 1, "\x00",  		'M', 0, 0, 0, 0},
				/*{0x0001, "CurrentScene",	t_UINT8, 	0x00, 	0xff, 			"r", 1, "\x00", 		'M', 0, 0, 0, 0},
          {0x0002, "CurrentGroup", 	t_UINT16,	0x0000,	0xffff, 		"r", 2, "\x00\x00", 'M', 0, 0, 0, 0},
          {0x0003, "SceneValid",		t_BOOL,		0x00, 	0x01, 			"r", 1, "\x00", 		'M', 0, 0, 0, 0},
          {0x0004, "NameSupport",		t_MAP8,		0x0,		0xffffffff, "r", 1, "\x00", 	  'M', 0, 0, 0, 0},*/
			},
			7, {
				{0x00, "Add Sence", 						'M'},
				{0x01, "View Scene", 						'M'},
				{0x02, "Remove Scene", 					'M'},
				{0x03, "Remove All Scene", 			'M'},
				{0x04, "Store Scene", 					'M'},
				{0x05, "Recall Scene", 					'M'},
				{0x06, "Get Scene Membership ", 'M'},
			},
			7, {
				{0x00, "Add Sence Response", 							'M'},
				{0x01, "View Scene Response", 						'M'},
				{0x02, "Remove Scene Response", 					'M'},
				{0x03, "Remove All Scene Response", 			'M'},
				{0x04, "Store Scene Response", 						'M'},
				{0x05, "Recall Scene Response", 					'M'},
				{0x06, "Get Scene Membership Response ", 	'M'},
			},
		},
		{0},
	},

	/*[0x0006] = */{
		0x0006, "On/Off",
		{
			1,
			1, {
				{0x0000, "OnOff", t_BOOL, 0x00, 0x01, "rr", 1, "\x00", 'm', 1, 1, 1800, 0x1},
			},
			3, {
				{0x00, "Off",			'M'},
				{0x01, "On",			'M'},
				{0x02, "Toggle",	'M'},
			},
			0, {
			},
		},
		{0},
	},

	/*[0xA002] = */{
		0xA002, "On/Off Custom",
		{
			1,
			1, {
				{0x0000, "OnOff", t_UINT16, 0x00, 0x01, "rr", 1, "\x00", 'm', 1, 1, 1800, 0x1},
			},
			3, {
				{0x00, "Off",			'M'},
				{0x01, "On",			'M'},
				{0x02,	"Toggle",	'M'},
			},
			0, {
			},
		},
		{0},
	},

  /*[0xA003] = */{
		0xA002, "On/Off Sence Custom",
		{
			1,
			1, {
				{0x0000, "On/Off", t_UINT8, 0x00, 0x01, "rr", 1, "\x00", 'm', 1, 1, 1800, 0x1},
			},
			3, {
				{0x00, "Off",			'M'},
				{0x01, "On",			'M'},
				{0x02,	"Toggle",	'M'},
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0006] = */{
		0xA002, "On/Off Custom",
		{
			1,
			1, {
				{0x0000, "OnOff", t_BOOL, 0x00, 0x01, "rr", 1, "\x00", 'm', 1, 1, 1800, 0x1},
			},
			3, {
				{0x00, "Off",			'M'},
				{0x01, "On",			'M'},
				{0x02, "Toggle",	'M'},
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0007] = */{
		0x0007, "On/Off Switch Configuration",
		{
			1,
			2, {
				{0x0000, "SwitchType", 		t_ENUM8, 0x00, 0x01, "r", 	1, "\x00",  'M', 0, 0, 0, 0},
				{0x0010, "SwitchActions", t_ENUM8, 0x00, 0x02, "rw", 	1, "\x00", 	'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0008] = */{
		0x0008, "Level Control",
		{
			1,
			1, {
				{0x0000, "CurrentLevel", t_UINT8, 0x00, 0xfe, "rr", 1, "\x00",  'm', 1, 1, 1800, 0x1},
			},
			8, {
				{0x00, "Move to Level", 'M'},
				{0x01, "Move", 'M'},
				{0x02, "Step", 'M'},
				{0x03, "Stop", 'M'},
				{0x04, "Move to Level(with On/Off)", 'M'},
				{0x05, "Move(with On/Off)", 'M'},
				{0x06, "Step(with On/Off)", 'M'},
				{0x07, "Stop", 'M'},
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0300] = */{
		0x0300, "Color Control",
		{
			1,
			3, {
				{0x0000, "CurrentHue",             t_UINT8,  0x00, 0xfe,   "rr", 1, "\x00",     'm', 1, 1, 1800, 0x1},
				{0x0001, "CurrentSaturation",      t_UINT8,  0x00, 0xfe,   "rr", 1, "\x00",     'm', 1, 1, 1800, 0x1},
				{0x0003, "CurrentX",               t_UINT16, 0x00, 0xfeff, "rr", 2, "\x61\x6b", 'm', 1, 1, 1800, 0x1},
				{0x0004, "CurrentY",               t_UINT16, 0x00, 0xfeff, "rr", 2, "\x60\x7d", 'm', 1, 1, 1800, 0x1},
				{0x0007, "ColorTemperatureMireds", t_UINT16, 0x00, 0xfeff, "rr", 2, "\x00\xfa", 'm', 1, 1, 1800, 0x1},
				{0x0008, "ColorMode",              t_ENUM8,  0x00, 0x02,   "r",  1, "\x00",     'm', 1, 1, 1800, 0x1},
				{0xF000, "TempOrColor",				     t_UINT8,  0x00, 0x01,   "rr", 1, "\x00",     'm', 0, 1, 1800, 0x1},
			},
			11, {
				{0x00, "Move to Hue", 'M'},
				{0x01, "Move Hue", 'M'},
				{0x02, "Step Hue", 'M'},
				{0x03, "Move to Saturation", 'M'},
				{0x04, "Move Saturation", 'M'},
				{0x05, "Step Saturation", 'M'},
				{0x06, "Move to Hue and Saturation", 'M'},
				{0x07, "Move to Color", 'M'},
				{0x08, "Move Color", 'M'},
				{0x09, "Step Color", 'M'},
				{0x40, "Enhaced Move To Hue", 'M'},
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0009] = */{
		0x0009, "Alarms",
		{
			1,
			0, {
			},
			2, {
				{0x00, "Reset Alarm", 'M'},
				{0x01, "Reset all alarms", 'M'},
			},
			1, {
				{0x00, "Alarm", 'M'},
			},
		},
		{0},
	},

	/*[0x000a] = */{
		0x000a, "Time",
		{
			1,
			1, {
				{0x0000, "Time", 				t_UTC, 	0x00000000, 	0xfffffffe, "rw", 4, "\x00\x00\x00\x00",  'M', 0, 0, 0, 0},
				{0x0001, "TimeStatus", 	t_MAP8, 0x0, 					0xffff, 		"rw", 2, "\x00\x00", 					'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x000b] = */{
		0x000b, "RSSI",
		{
			1,
			7, {
				{0x0000, "LocationType", 		t_DATA8, 0x0, 0xffff, "rw", 1, "\x00",  'M', 0, 0, 0, 0},
				{0x0001, "LocationMethod", 	t_ENUM8, 0x00, 0xff, 	"rw", 1, "\x00",  'M', 0, 0, 0, 0},

				{0x0010, "Coordinate1",						t_INT16,	0x8000, 0x7fff,	"rw", 2, "\x00\x00", 	'M', 0, 0, 0, 0},
				{0x0010, "Coordinate2",						t_INT16,	0x8000, 0x7fff,	"rw",	2, "\x00\x00", 	'M', 0, 0, 0, 0},
				{0x0013, "Power",									t_INT16,	0x8000, 0x7fff, "rw", 2, "\x00", 			'M', 0, 0, 0, 0},
				{0x0014, "PathLossExponent",			t_INT16,	0x0000, 0xffff, "rw", 2, "\x00", 			'M', 0, 0, 0, 0},
				{0x0017, "NumberRSSImeasurements",t_INT8,		0x01, 	0xff, 	"rw", 1, "\x00", 			'M', 0, 0, 0, 0},
			},
			4, {
				{0x00, "Set Absolute Location", 		'M'},
				{0x01, "Set Device Configuration", 	'M'},
				{0x02, "Get Device Configuration", 	'M'},
				{0x03, "Get Location Data",					'M'},
			},
			5, {
				{0x00, "Device configuration response", 		'M'},
				{0x01, "Location data response", 		'M'},
				{0x02, "Location data notification", 		'M'},
				{0x03, "Compact location data nofification", 		'M'},
				{0x04, "RSSI Ping", 		'M'},
			},
		},
		{0},
	},

	/*[0x0500] = */{
		0x0500, "IAS Zone",
		{
			1,
			5, {
				{0x0000, "ZoneState", 			t_ENUM8, 	-1, -1, 		"r", 1, "\x00", 															'M', 0, 0, 0, 0},
				{0x0001, "ZoneType", 				t_ENUM16, -1, -1, 		"r", 2, "\x00\x00", 													'M', 0, 0, 0, 0},
				{0x0002, "ZoneStatus", 			t_MAP16,	-1, -1, 		"rr", 2, "\x00\x00", 													'm', 1, 1, 1800, 0x1},
				{0x0010, "IAS_CIE_Address", t_EUI64,	-1,	-1, 		"rw",8, "\x00\x00\x00\x00\x00\x00\x00\x00",  	'M', 0, 0, 0, 0},
				{0x0011, "ZoneID",					t_UINT8, 0x00, 0xff,	"r", 1, "\xff", 															'M', 0, 0, 0, 0},
			},
			2, {
				{0x00, "Zone Enroll Response", 'M'},
			},
			2, {
				{0x00, "Zone Status Change Notification", 'M'},
				{0x01, "Zone Enroll Request", 'M'},
			},
		},
		{0},
	},

	/*[0x0501] = */{
		0x0501, "IAS ACE",
		{
			1,
			0, {
				{0x0000, "ZoneState", 			t_ENUM8, 	-1, -1, 		"r", 1, "\x00", 'M', 0, 0, 0, 0},
			},
			0, {
				{0x00, "Zone Enroll Response", 'M'},
			},
			2, {
				{0x00, "Arm", 'M'},
				{0x01, "Bypass", 'M'},
				{0x02, "Emergency", 'M'},
				{0x03, "Fire", 'M'},
				{0x04, "Panic", 'M'},
				{0x05, "GetZoneId", 'M'},
				{0x06, "GetZoneInfo", 'M'},
				{0x07, "GetPanel", 'M'},
				{0x08, "GetByPass", 'M'},
				{0x09, "GetZone", 'M'},
			},
		},
		{0},
	},

	/*[0x0b01] = */{
		0x0b01, "Meter Identification",
		{
			1,
			6, {
				{0x0000, "CompanyName", 			t_STRING, 	0, 			16, 				"r", 	1, "\x00", 							'M', 0, 0, 0, 0},
				{0x0001, "MeterTypeID", 			t_UINT16, 	0x0000, 0xffff, 		"r",  2, "\x00\x00", 					'M', 0, 0, 0, 0},
				{0x0004, "DataQualityID", 		t_UINT16, 	0x0000, 0xffff, 		"r", 	2, "\x00\x00", 					'M', 0, 0, 0, 0},
				{0x000C, "POD", 							t_STRING, 	0,			16,					"r", 	1, "\x00", 							'M', 0, 0, 0, 0},
				{0x000D, "AvailablePower", 		t_INT24, 	 	0x0000, 0xffffff,		"r", 	3, "\x00\x00\x00", 			'M', 0, 0, 0, 0},
				{0x000E, "PowerThreshold", 		t_INT24, 		0x0000, 0xffffff,		"r", 	3, "\x00\x00\x00", 			'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0b04] = */{
		0x0b04, "Electrical Measurement",
		{
			1,
			6, {
				{0x0505, "RMSVoltage", 			t_UINT16, 	0, 			0xffff, 		"r",	 2, "\x00\x00",							'M', 1, 60, 900, 0x0},
				{0x0508, "RMSCurrent", 			t_UINT16, 	0, 			0xffff, 		"r",	 2, "\x00\x00",							'M', 1, 60, 900, 0x0},
				{0x050b, "Power",					  t_UINT16, 	0, 			0xffff, 		"r",	 2, "\x00\x00",							'M', 1, 60, 900, 0x0},
				{0x0510, "PowerFactor", 		t_INT8, 	-100, 			100, 		"r",	 1, "\x00\x00",							'M', 1, 60, 900, 0x0},
				{0x0600, "ACVoltageMultiplier", 		t_UINT16, 	0, 			0xffff, 		"r",	 2, "\x00\x01",							'M', 0, 60, 900, 0x0},
				{0x0601, "ACVoltageDivisor", 		t_UINT16, 	0, 			0xffff, 		"r",	 2, "\x00\x01",							'M', 0, 60, 900, 0x0},
				{0x0000, "MeasurementType", 			t_MAP32, 	0, 			0xffffffff, 		"r",	 4, "\x00\x00\x00\x00", 							'M', 0, 0, 0, 0},
				{0x05E0, "Energy", 			t_UINT32, 	0, 			0xffffffff, 		"r",	 4, "\x00\x00\x00\x00",							'M', 1, 60, 900, 0x0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0400] = */{
		0x0400, "Illuminance Measurement",
		{
			1,
			3, {
				{0x0000, "MeasuredValue", 	 t_UINT16, 0x0000, 0xffff, "rr", 2, "\x00\x00", 'm', 1, 1, 1800, 0x01F4},
				{0x0001, "MinMeasuredValue", t_UINT16, 0x0001, 0xfffd, "rp", 2, "\x00\x00", 'M', 0, 0, 0},
				{0x0002, "MaxMeasuredValue", t_UINT16, 0x0001, 0xfffe, "r", 	2, "\x00\x00", 'M', 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0401] = */{
		0x0401, "Illuminance Level Sensing",
		{
			1,
			2, {
				{0x0000, "LevelStatus", 					t_ENUM8, 	0x00, 	0xfe, 		"rp", 	1, "\x00", 			'm', 1, 60, 900, 0x0},
				{0x0010, "IlluminanceTargetLevel",t_UINT16, 0x0000, 0xfffe, 	"rw", 	2, "\x00\x00",	'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0402] = */{
		0x0402, "Temperature Measurement",
		{
			1,
			3, {
				{0x0000, "MeasuredValue", 	 t_INT16, 0x0000, 0xffff, "rp", 2, "\x00\x00",	'm', 1, 1, 1800, 0x000A},
				{0x0001, "MinMeasuredValue", t_INT16, 0x954d, 0x7ffe, "r",  2, "\x00\x00",	'M', 0},
				{0x0002, "MaxMeasuredValue", t_INT16, 0x954e, 0x7fff, "r",  2, "\x00\x00",	'M', 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0403] = */{
		0x0403, "Pressure Measurement",
		{
			1,
			3, {
				{0x0000, "MeasuredValue", 					t_INT16,	0x0000, 0xfffe, 	"rp", 	2, "\x00\x00",	'm', 1, 60, 900, 0x0},
				{0x0001, "MinMeasuredValue",				t_INT16, 0x8001, 0x7ffe, 	"r", 		2, "\x00\x00",	'M', 0, 0, 0, 0},
				{0x0002, "MaxMeasuredValue",				t_INT16, 0x8002, 0x7fff, 	"r", 		2, "\x00\x00",	'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0404] = */{
		0x0404, "Flow Measurement",
		{
			1,
			3, {
				{0x0000, "MeasuredValue", 					t_UINT16,	0x0000, 0xfffe, 	"rp", 	2, "\x00\x00",	'm', 1, 1, 1800, 0},
				{0x0001, "MinMeasuredValue",				t_UINT16, 0x0001, 0xfffd, 	"r", 		2, "\x00\x00",	'M', 0, 0, 0, 0},
				{0x0002, "MaxMeasuredValue",				t_UINT16, 0x0002, 0xfffe, 	"r", 		2, "\x00\x00",	'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0405] = */{
		0x0405, "Relative Humidity",
		{
			1,
			3, {
				{0x0000, "MeasuredValue", 	 t_UINT16, 0x0000, 0xfffe, 	"rp", 2, "\x00\x00",	'm', 1, 1, 1800, 0x000A},
				{0x0001, "MinMeasuredValue", t_UINT16, 0x0000, 0x270f, 	"r", 	2, "\x00\x00",	'M', 0, 0, 0, 0},
				{0x0002, "MaxMeasuredValue", t_UINT16, 0x0001, 0x2701, 	"r", 	2, "\x00\x00",	'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0406] = */{
		0x0406, "Occupancy Sensing",
		{
			1,
			2, {
				{0x0000, "Occupancy", 					 t_MAP8,	0x00, 0xff, "rp", 1, "\x00", 'm', 1, 1, 1800, 0x0},
				{0x0001, "OccupancySensorType ", t_ENUM8, 0x00, 0xfe, "r", 	1, "\x00", 'M', 0, 0, 0, 0},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0019] = */{
		0x0019, "OTA Upgrade",
		{
			1,
			0, {
				/* now not support OTA.
				{0x0000, "UpgradeServerID", 		t_EUI64, 0x00, 0x00, "r", "\xff\xff\xff\xff\xff\xff\xff\xff", 'M'},
				{0x0006, "ImageUpgradeStatus", 	t_ENUM8, 0x00, 0x00, "r", "\x00", 'M'},
				{0x0006, "ImageUpgradeStatus",						t_ENUM8,	0x0, 0x0,	"r",	1,		"\x00", 'o', 0, 900, 900, 0x1},
				*/
			},
			5, {
				{0x00, "Image Notify",							'M'},
				{0x02, "Query Next Image Response",	'M'},
				{0x05, "Image Block Response",			'M'},
				{0x07, "Upgrade End Response",			'M'},
				{0x09, "Query Specific Response",		'M'}
			},
			5, {
				{0x01, "Query Next Image Request",	'M'},
				{0x03, "Query Block Request",				'M'},
				{0x04, "Query Page Request",				'M'},
				{0x06, "Upgrade End Request",				'M'},
				{0x08, "Query Specific Request",		'M'},
			},
		},
		{0},
	},

	/*[0xfe00] = */{
		0xfe00, "AliCerify",
		{
			1,
			4, {
				{0x0000, "ModelId",				t_UINT32, 0X00,		0Xffffffff, "r", 4,		"\x00\x00\x00\x00",   'M', 0, 0, 0, 0},
				{0x0001, "Rand",					t_STRING, 0x00,		0x00,				"r", 16,	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",  'M', 0, 0, 0, 0},
				{0x0002, "Sign",					t_STRING, 0x00,		0x00,				"r", 16,	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",  'M', 0, 0, 0, 0},
				{0x0003, "Supervision",		t_UINT16, 0x0000, 0xffff,			"r", 2,		"\x00\x00", 'O', 0, 0, 0, 0},
			},
			0, {
			},
			0, {

			},
		},
		{0},
	},

	/*[0x0702] = */{
		0x0702, "Metering",
		{
			1,
			8, {
				{0x0000, "CurrentSummationDelivered",	t_UINT48, 0x0, -1,			"rr", 6,	"\x00\x00\x00\x00\x00\x00", 'm', 1, 1, 1800, 0x000000000001},
				{0x0001, "CurrentSummationReceived",	t_UINT48, 0x0, -1,			"rr", 6,	"\x00\x00\x00\x00\x00\x00", 'm', 1, 1, 1800, 0x000000000001},
				{0x0200, "Status",										t_MAP8,		0x0, 0xff,		"r",  1,	"\x00", 'M', 0, 0, 0, 0},
				{0x0300, "UnitofMeasure",							t_ENUM8,	0x0, 0xff,		"r",  1,	"\x00", 'M', 0, 0, 0, 0},
				{0x0301, "Multiplier",								t_UINT24, 0x0, 0xffffff,"r",  3,	"\x00\x00\x00", 'O', 0, 0, 0, 0},
				{0x0302, "Divisor",										t_UINT24, 0x0, 0xffffff,"r",  3,  "\x00\x00\x00", 'O', 0, 0, 0, 0},
				{0x0303, "SummationFormatting",				t_MAP8,		0x0, 0xff,		"r",  1,  "\x00", 'M', 0, 0, 0, 0},
				{0x0306, "MeteringDeviceType",				t_MAP8,		0x0, 0xff,		"r",  1,	"\x00", 'M', 0, 0, 0, 0},
				{0x0400, "InstantaneousDemand",				t_INT24,	0x0, 0xffffff,"rr", 3,	"\x00\x00\x00", 'o', 1, 1, 1800, 0x000001},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0101] = */{
		0x0101, "Door Lock",
		{
			1,
			//18,{
			3,{
				{0x0000, "LockState",					t_ENUM8,	0x0, 0xff,		"rr", 1,		"\x00", 'm', 1, 60, 900, 0x0},
				{0x0023, "AutoRelockTime",		t_INT32,	0x0, 180,			"r",  4,		"\x00",	'm', 0, 0, 0, 0x0},
				{0x0003, "DoorState",					t_ENUM8,	0x0, 0xff,		"r",  1,		"\x00",	'O', 0, 0, 0, 0x0},
				/*
				{0x0001, "LockType",					t_ENUM8,	0x0, 0xff,		"r",  1,		"\x00", 'm', 0, 0, 0, 0x0},
				{0x0002, "ActuatorEnabled",		t_BOOL,		0x0, 0x01,		"r",  1,		"\x00", 'm', 0, 0, 0, 0x0},
				{0x0004, "DoorOpenEvents",		t_UINT32,	0x0, 0xffffffff,		"r",  4,		"\x00\x00\x00\x00", 'O', 0, 0, 0, 0x0},
				{0x0005, "DoorClosedEvents",	t_UINT32,	0x0, 0xffffffff,		"r",  4,		"\x00\x00\x00\x00", 'O', 0, 0, 0, 0x0},
				{0x0006, "OpenPeriod",				t_UINT16,	0x0, 0xffff,				"r",  2,		"\x00\x00",					'O', 0, 0, 0, 0x0},
				*/

				{0x0010, "NumberOfLogRecordsSupported",	t_UINT16,	0x0, 0xffff,	"r",  2,		"\x00\x00",			'm', 0, 0, 0, 0x0},
				{0x0011, "NumberOfTotalUsersSupported",	t_UINT16,	0x0, 0xffff,	"r",  2,		"\x00\x00",			'm', 0, 0, 0, 0x0},
				{0x0012, "NumberOfPINUsersSupported",		t_UINT16,	0x0, 0xffff,	"r",  2,		"\x00\x00",			'm', 0, 0, 0, 0x0},
				{0x0013, "NumberOfRFIDUsersSupported",	t_UINT16,	0x0, 0xffff,	"r",  2,		"\x00\x00",			'm', 0, 0, 0, 0x0},

				/*
				{0x0014, "NumberOfWDSSupportedPerUser",	t_UINT8,	0x0, 0xff,		"r",  1,		"\x00",					'O', 0, 0, 0, 0x0},
				{0x0015, "NumberOfYDSSupportedPerUser",	t_UINT8,	0x0, 0xff,		"r",  1,		"\x00",					'O', 0, 0, 0, 0x0},
				{0x0016, "NumberOfHDSSupported",				t_UINT8,	0x0, 0xff,		"r",  1,		"\x00",					'O', 0, 0, 0, 0x0},
				{0x0017, "NumberOfMaxPINCodeLength",		t_UINT8,	0x0, 0xff,		"r",  1,		"\x00",					'O', 0, 0, 0, 0x0},
				{0x0018, "NumberOfMinPINCodeLength",		t_UINT8,	0x0, 0xff,		"r",  1,		"\x00",					'O', 0, 0, 0, 0x0},
				{0x0019, "NumberOfMaxRFIDCodeLength",		t_UINT8,	0x0, 0xff,		"r",  1,		"\x00",					'O', 0, 0, 0, 0x0},
				{0x001A, "NumberOfMinRFIDCodeLength",		t_UINT8,	0x0, 0xff,		"r",  1,		"\x00",					'O', 0, 0, 0, 0x0},
				*/
			},
			26, {
				{0x00, "Lock Door", 'M'},
				{0x01, "Unlock Door", 'M'},
				{0x02, "Toggle", 'M'},
				{0x03, "Unlock with Timeout", 'M'},
				{0x04, "Get Log Record", 'M'},
				{0x05, "Set PIN Code", 'M'},
				{0x06, "Get PIN Code", 'M'},
				{0x07, "Clear PIN Code", 'M'},
				{0x08, "Clear All PIN Codes", 'M'},
				{0x09, "Set User Status", 'M'},
				{0x0a, "Get User Status", 'M'},
				{0x0b, "Set Weekday Schedule", 'M'},
				{0x0c, "Get Weekday Schedule", 'M'},
				{0x0d, "Clear Weekday Schedule", 'M'},
				{0x0e, "Set Year Day Schedule", 'M'},
				{0x0f, "Get Year Day Schedule", 'M'},
				{0x10, "Clear Year Day Schedule", 'M'},
				{0x11, "Set Holiday Schedule", 'M'},
				{0x12, "Get Holiday Schedule", 'M'},
				{0x13, "Clear Holiday Schedule", 'M'},
				{0x14, "Set User Type", 'M'},
				{0x15, "Get User Type", 'M'},
				{0x16, "Set RFID Code", 'M'},
				{0x17, "Get RFID Code", 'M'},
				{0x18, "Clear RFID Code", 'M'},
				{0x19, "Clear All RFID Codes", 'M'},
			},
			0, {
			},
		},

		{0},
	},

	/*[0x0102] = */{
		0x0102, "Window Covering",
		{
			1,
			5,{
				{0x0000, "WindowCoveringType",t_ENUM8,	0x0, 0x09,		"r",  1,		"\x00", 'm', 0, 0, 0, 0x0},
				{0x0007, "Config/Status",			t_MAP8,		0x0, 0xff,		"r",  1,		"\x00", 'm', 0, 0, 0, 0x0},
				{0x0008, "Current Pos Lift %",t_UINT8,	0x0, 0x64,		"rsp",1,		"\x00", 'm', 1, 900, 900, 0x5},
				{0x0009, "Current Pos Tift %",t_UINT8,	0x0, 0x64,		"rsp",1,		"\x00", 'm', 1, 900, 900, 0x5},
				{0x0107, "Mode",							t_MAP8,		0x0, 0xff,		"rw", 1,		"\x00", 'm', 0, 0, 0, 0},
			},
			7, {
				{0x00, "Up/Open",								'M'},
				{0x01, "Down/Close",						'M'},
				{0x02, "Stop",									'M'},
				{0x04, "Go To Lift Value",			'M'},
				{0x05, "Go To Lift Percentage", 'M'},
				{0x07, "Go To Tift Value",			'M'},
				{0x08, "Go To Lift Percentage", 'M'},
			},
			0, {
			},
		},

		{0},
	},

	/*[0x0201] = */{
		0x0201, "Thermostat",
		{
			1,
			5,{
				{0x0000, "LocalTemperature",						t_INT16,	0x954d, 0x7fff,	"rp",  2,		"\x00\x00", 'm', 1, 900, 900, 0x1},  //-
				{0x001b, "ControlSequenceOfOperation",	t_ENUM8,	0x0,    0x5,		"rw",  1,		"\x04",			'M', 0, 0,		0,	0x0},
				{0x001c, "SystemMode",									t_ENUM8,	0x0,		0x9,		"rws",  1,	"\x01",			'm', 1, 900,	900,	0x1}, //-
				{0x0012, "OccupiedCoolingSetpoint",			t_INT16,	0x0,		0xff,		"rws",  2,	"\x0a\x28",		'M', 1, 900,	900,	0x1}, //-
				{0x0013, "OccupiedHeatingSetpoint",			t_INT16,	0x0,		0xff,		"rws",  2,	"\x07\xd0",		'M', 1, 900,	900,	0x1}, //--
			},
			5, {
				{0x00, "SetPoint Raise/Lower",			'M'},
				{0x01, "Set Weekly Schedule",				'O'},
				{0x02, "Get Weekly Schedule",				'O'},
				{0x03, "Clear Weekly Schedule",			'O'},
				{0x04, "Get Relay Status Log",			'O'},
			},
			2, {
				{0x00, "Get Weekly Schedule Response",			'O'},
				{0x01, "Get Weekly Schedule Response",			'O'},
			},
		},
		{0},
	},

	/*[0x0202] = */{
		0x0202, "Fan Control",
		{
			1,
			2,{
				{0x0000, "FanMode",						t_ENUM8,	0x0, 0x6,	"rw",  1,		"\x05", 'm', 1, 900, 900, 0x1}, //-
				{0x0001, "FanModeSequence",		t_ENUM8,	0x0, 0x4,	"rw",  1,		"\x02", 'M', 0, 900, 900, 0x1},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0203] = */{
		0x0203, "Dehumidification Control",
		{
			1,
			5,{
				{0x0000, "RelativeHumidity",						t_UINT8,	0x0, 0x64,	"r",	1,		"\x00", 'o', 1, 900, 900, 0x1}, //-
				{0x0001, "DehumidificationCooling",			t_UINT8,	0x0, 0xff,	"rp", 1,		"\x00", 'M', 0, 900, 900, 0x1},
				{0x0010, "DehumidificationHysteresis",	t_UINT8,	0x0, 0xff,	"rw", 1,		"\x32", 'M', 0, 900, 900, 0x1},
				{0x0013, "DehumidificationMaxCool",			t_UINT8,	0x0, 0xff,	"rw", 1,		"\x02", 'M', 0, 900, 900, 0x1},
				{0x0014, "RelativeHumidityDisplay",			t_UINT8,	0x0, 0xff,	"rw", 1,		"\x14", 'M', 0, 900, 900, 0x1},
			},
			0, {
			},
			0, {
			},
		},
		{0},
	},

	/*[0x0502] = */{
		0x0502, "IAS WD",
		{
			1,
			1,{
				{0x0000, "MaxDuration",						t_UINT16,	0x0, 0xfffe,	"rw",	2,		"\x00\xf0", 'M', 0, 900, 900, 0x1},
			},
			2, {
				{0x00, "Start warning",			'M'},
				{0x01, "Squawk",						'M'},
			},
			0, {
			},
		},
		{0},
	},

	/*[0xFC82] = */{
		0xFC82, "Infrared Controller",
		{
			1,
			0, {
			},
			3, {
				{0xF0, "SendKeyCommand",			'M'},
				{0xF1, "StudyKey",			'M'},
				{0xF2, "StudyKeyResult",			'M'},
			},
			0, {
			},
		},
		{0},
	},

	/*[0xFFAD] = */{
		0xFFAD, "Infrared Blaster",
		{
			1,
			0, {
			},
			3, {
				{0x10, "Learning Mode",			'M'},
				{0x1F, "Send Learned command",			'M'},
			},
			0, {
			},
		},
		{0},
	},
};

stZclCluster_t *z3_get_clusters() {
	return &my_zcl_clusters[0];
}

int z3_get_clusters_cnt() {
	return sizeof(my_zcl_clusters)/sizeof(my_zcl_clusters[0]);
}

/*============
 * utils
 *=============================================================*/
static char *mac2str(char *mac, char *str) {
	int i = 0;
	int len = 0;
	for (i = 0; i < 8; i++) {
		len += sprintf(str + len, "%02X", mac[i]&0xff);
	}
	return str;
}

/*============
 * callbacks
 *=============================================================*/
stDevMsgCb_t *dmc = NULL;

int z3_register(char *addr, int *epList, int epListLen, char *ModelStr, char *model, int type, int battery) {
	char buf[256];
	int i = 0;
	int len = 0;
	for (i = 0; i < epListLen; i++) {
		len += sprintf(buf + len, "%s%d", (i == 0) ? "" : ",", epList[i]);
	}

	char str[32];
	log_info("%s,epList:%s, ModelStr:%s, model:%s, type:%d, battery:%d",
		mac2str(addr, str), buf, ModelStr, model, type, battery);

	if (dmc->rpt_dev_added != NULL) {
		dmc->rpt_dev_added(addr, ModelStr, model, type, battery, epList, epListLen);
	}

	return 0;
}

int z3_unregister(char *addr) {
	char str[32];
	log_info("%s", mac2str(addr, str));

	if (dmc->rpt_dev_deled != NULL) {
		dmc->rpt_dev_deled(addr);
	}

	return 0;
}

int z3_online(char *addr, long last, int online) {
	char str[32];
	log_info("%s", mac2str(addr, str));

	if (dmc->rpt_dev_online != NULL) {
		dmc->rpt_dev_online(addr, online);
	}

	return 0;
}

int z3_attr(char *addr, unsigned char ep, unsigned short clsid,  const char *buf, int len, int linkQuality) {
	char str[32];
	log_info("%s, ep:%02X, cluster:%02X, linkQuality:%02X", mac2str(addr, str), ep&0xff, clsid&0xffff, linkQuality&0xff);
	log_debug_hex("data:", buf, len);

	if (dmc->rpt_attr != NULL) {
		short attrid = ((buf[0]&0xff) | ((buf[1]&0xff) << 8))&0xffff;
		dmc->rpt_attr(addr, ep, clsid&0xffff, attrid&0xffff, (char *)(buf + 2), len - 2, linkQuality);
	}

	return 0;
}

int z3_cmd(char *addr, unsigned char ep, unsigned short clsid, const char *buf, int len, int linkQuality) {
	char str[32];
	log_info("%s, ep:%02X, cluster:%02X, linkQuality:%02X", mac2str(addr, str), ep&0xff, clsid&0xffff, linkQuality&0xff);
	log_debug_hex("data:", buf, len);

	if (dmc->rpt_cmd != NULL) {
		char cmd = buf[2]&0xff;
		dmc->rpt_cmd(addr, ep, clsid&0xffff, cmd&0xff, (char *)(buf + 3), len - 3, linkQuality);
	}

	return 0;
}

int z3_zcl_cmd_ret(char *addr, unsigned char ep, unsigned short clsid, char cmdid, int status, int linkQuality, void *context) {
	char str[32];
	log_info("%s, ep:%02X, cluster:%02X, linkQuality:%02X, status:%d", mac2str(addr, str), ep&0xff, clsid&0xffff, linkQuality&0xff, status);

	if (dmc->rpt_zcl_cmd_status != NULL) {
		dmc->rpt_zcl_cmd_status(addr, ep&0xff, clsid&0xffff, cmdid&0xff, status, linkQuality, context);
	}
	return 0;
}

int z3_set_att_ret(char *addr, unsigned char ep, unsigned short clsid, unsigned short attr, int status, int linkQuality, void *context) {
	char str[32];
	log_info("%s, ep:%02X, cluster:%02X, linkQuality:%02X, status:%d", mac2str(addr, str), ep&0xff, clsid&0xffff, linkQuality&0xff, status);
	return 0;
}

int z3_get_att_ret(char *addr, unsigned char ep, unsigned short clsid, unsigned short attr, int status, int linkQuality, void *context) {
	char str[32];
	log_info("%s, ep:%02X, cluster:%02X, linkQuality:%02X, status:%d", mac2str(addr, str), ep&0xff, clsid&0xffff, linkQuality&0xff, status);
	return 0;
}

int z3_simple_desc(char *addr, unsigned char ep, char *buf, int len) {
	char str[32];
	log_info("%s, ep:%02X", mac2str(addr, str), ep&0xff);
	log_debug_hex("data:", buf, len);

	if (dmc->rpt_simp_desc != NULL) {
			dmc->rpt_simp_desc(addr, ep, buf, len);
	}
	return 0;
}

int z3_exploration(char *addr, int status, int cluster) {
	char str[32];
	log_info("%s, status:%02X", mac2str(addr, str), status&0xff);

	if (dmc->rpt_exploration != NULL) {
		dmc->rpt_exploration(addr, status, cluster);
	}
	return 0;
}

int z3_zcl_cmd_res(char *addr, unsigned char ep, unsigned short clusterid, unsigned char cmd, unsigned char seq, char *buf, int len) {
	char str[32];
	log_info("%s, seq:%02X, buf[0]:%02X, buf[1]:%02X", mac2str(addr, str), seq&0xff, buf[0]&0xff, buf[1]&0xff);

	if (dmc->rpt_zcl_cmd_response != NULL) {
		dmc->rpt_zcl_cmd_response(addr, ep&0xff, clusterid&0xffff, cmd&0xff, seq&0xff, buf, len&0xff);
	}

	return 0;
}

/*============
 * rbsdk
 *=============================================================*/
static int run_flag = 0;

void *rbsdk_thread(void *arg) {
  printf("z3Imp rbsdk_thread Started\n");
	static char *argv[] = {
		"rbsdk", "-n1", "-p/dev/ttyS1", "-b115200", "-d", NULL
	};

	const char *system_model_get();
	const char *model = system_model_get();
	if (strstr(model, "DSI0177") != NULL) {
		argv[2] = "-p/dev/ttyS2";
	}
	int argc = sizeof(argv)/sizeof(argv[0]);

	run_flag = 1;

	z3_run(argc-1, argv);
  printf("z3Imp rbsdk_thread Done\n");
	run_flag = 0;
}

int rbsdk_init(void *unused, int (*msgcb)(char *src, char *dest, char *msg)) {
  printf("z3Imp rbsdk_init Started\n");
	unused = unused;
	msgcb = msgcb;

	pthread_t tid;
	pthread_attr_t attr;
	memset(&attr, 0, sizeof(attr));
  pthread_attr_setdetachstate(&attr,1);
	//pthread_attr_setstacksize(&attr, 10 * 1024);
	pthread_create(&tid, NULL, rbsdk_thread, NULL);

	pthread_attr_destroy(&attr);

	while (run_flag == 0) {
		sleep(2);
	}
	sleep(1);
  printf("z3Imp rbsdk_init Done\n");
	return 0;
}

int rbsdk_vers(char *version) {
  printf("z3Imp rbsdk_vers Started\n");
	if (version == NULL) {
		return -1;
	}
	strcpy(version, z3_version());
  printf("z3Imp rbsdk_vers Done\n");
	return 0;
}

int rbsdk_list_dev(void (*cb)(char *addr, int *epList, int epListLen, char *ModelStr, char *model, int type, int battery)) {
	printf("Z3Imp rbsdk_list_dev\n");
	z3_list_device_a(cb);
	printf("Z3Imp rbsdk_list_dev Done\n");
	return 0;
}

int rbsdk_dev_msgcb_set(stDevMsgCb_t *_dmc) {
  printf("z3Imp msgCb_set Started\n");
	dmc = _dmc;
  printf("z3Imp msgCb_set Done\n");
	return 0;
}

int rbsdk_add_dev(char *mac, char *type) {
	return z3_permit_a(mac, 120);
}

int rbsdk_del_dev(char *mac) {
	return z3_remove_a(mac, 2);
}

int rbsdk_zcl_cmd(char *mac, int manufacturer, int ep, int cluster, int cmdid,	char *data, int len, void *context) {
	return z3_zcl_cmd_a(mac, manufacturer&0xffff, ep&0xff, cluster&0xffff, cmdid&0xff, data, len, context);
}

int rbsdk_set_attr(char *mac, int ep, int cluster, int attrid, int atype, char *data, int len) {
	return z3_set_att_a(mac, ep&0xff, cluster&0xffff, attrid&0xffff, atype&0xff, data, len);
}

int rbsdk_get_attr(char *mac, int ep, int cluster, int attrid) {
	return z3_get_att_a(mac, ep&0xff, cluster&0xffff, attrid&0xffff);
}

#if 0
/*============
 * test api
 *=============================================================*/
void *test_thread(void *arg) {
	sleep(5);
	char mac[8] = {0};
	log_info("z3_permit_a");
	int ret = z3_permit_a(mac, 120);
	log_info("z3_permit_a ret:%d", ret);
}

int main(int argc, char *argv[]) {

	pthread_t tid;
	/*
	pthread_attr_t attr;
	memset(&attr, 0, sizeof(attr));
  pthread_attr_setdetachstate(&attr,1);
	pthread_attr_setstacksize(&attr, 10 * 1024);
	*/
	//pthread_create(&tid, &attr, test_thread, NULL);
	pthread_create(&tid, NULL, test_thread, NULL);

	/*
	pthread_attr_destroy(&attr);
	*/

	appname = argv[0];
	z3_run(argc, argv);

	return 0;
}
#endif
