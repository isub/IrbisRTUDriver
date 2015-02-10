#include "DriverRTU.h"
#include "AuxiliaryFunction.h"

#if !defined (SIP_RTU_API_VERSION)
#error API version not defined
#endif

SCommandRule g_msoCommandRule[] = {
	{
		"CreateUser",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Create\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#elif SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{-2,"commands\0command\0item\0enabled","","true"},
			{-2,"commands\0command\0item\0type","","sip"},
			{-2,"commands\0command\0item\0audio_space_limit","","5"},
			{-2,"commands\0command\0item\0is_web_accessible","","false"},
			{-2,"commands\0command\0item\0is_virtual_number","","false"},
			{-2,"commands\0command\0item\0add_domain_name","","false"},
			{-2,"commands\0command\0item\0enabledDisaPIN","","false"},
			{-2,"commands\0command\0item\0autorize_call","","false"},
			{-2,"commands\0command\0item\0radius_autorize_number","","false"},
			{-2,"commands\0command\0item\0radius_authorize_registration","","false"},
			{-2,"commands\0command\0item\0radius_accounting","","false"},
			{-2,"commands\0command\0item\0user_mobile_request_access","","false"},
			{-2,"commands\0command\0item\0is_clir_allowed","","false"},
			{-2,"commands\0command\0item\0enable_groups","","false"},
			{-2,"commands\0command\0item\0play_error_prompt","","true"},
			{-2,"commands\0command\0item\0compare_address","","false"},
			{-2,"commands\0command\0item\0language", "", "ru-RU"},
			{-2,"commands\0command\0item\0clir", "", "disabled"},
			{-2,"commands\0command\0item\0early_notify_200_on_refer", "", "false"},
			{-2,"commands\0command\0item\0is_clip_allowed","","true"},
			{-2,"commands\0command\0item\0isRedialable","","true"},
			{-2,"commands\0command\0item\0behindgateway","","false"},
			{-2,"commands\0command\0item\0display_name_passing_method","","Transparent"},
			{-2,"commands\0command\0item\0block_anonymous_calls","","false"},
			{-2,"commands\0command\0item\0enable_record","","false"},
#if SIP_RTU_API_VERSION >= 19000
			{ 3,"commands\0command\0item\0subscriber"},				// копируем id
			{ 4,"commands\0command\0item\0web_login"},
			{ 5,"commands\0command\0item\0web_password"},
			{ 4,"commands\0command\0item\0user"},
#elif SIP_RTU_API_VERSION >= 18120
			{ 5,"commands\0command\0item\0web_password"},
			{-2,"commands\0command\0item\0in_capacity", "", "2"},
			{-2,"commands\0command\0item\0out_capacity", "", "2"},
			{-2,"commands\0command\0item\0total_capacity", "", "2"},
/*			{-2,"commands\0command\0item\0max_rtu_terminals", "", "1"},	   CORE-1358 */
			{-2,"commands\0command\0item\0max_rtu_terminals", "", "0"}, /* CORE-1358 */
			{ 3,"commands\0command\0item\0name"},				// копируем id
			{ 4,"commands\0command\0item\0user"},
			{ 5,"commands\0command\0item\0password"},
#else
			{-2,"commands\0command\0item\0display_name_passing_method","","3"}, /* новая версия ругается на значение "3" */
			{-2,"commands\0command\0item\0cpc","","736"}, /* новая версия ругается на значение "736" */
			{-2,"commands\0command\0item\0options\0capacity_in", "", "2"}, /* отсутствует в API 1.8.1-20*/
			{-2,"commands\0command\0item\0options\0capacity_out", "", "2"}, /* отсутствует в API 1.8.1-20*/
			{-2,"commands\0command\0item\0options\0capacity_all", "", "2"}, /* отсутствует в API 1.8.1-20*/
			{-2,"commands\0command\0item\0audio_codec_group","","PCM+729"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0video_codec_group","","All_video"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0NatMode", "", "detect"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0t38", "", "true"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0early_connect", "", "false"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0proxy_rbt", "", "true"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0Alerting", "", "media"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0AllowSipRedirect", "", "false"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0dtmfOutChoice", "", "rfc2833"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0dtmfInRFC2833_h323", "", "any"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0dtmfInRFC2833", "", "any"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0Tunneling", "", "true"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0FastStart", "", "true"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0StartH245", "", "callproceeding"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0codec_passing_policy", "", "ChangeGeneralCapability"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0proxy_media_policy", "", "Proxy"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0g711_as_fax", "", "false"}, /* исключить в версии 18120 */
			{-2,"commands\0command\0item\0options\0rtp_timeout", "", "180"}, /* является свойством профиля терминала в API 1.8.1-20*/
#ifdef _DEBUG
			{-2,"commands\0command\0item\0options\0zone", "", "voip"}, /* является свойством терминала в API 1.8.1-20*/
#else
			{-2,"commands\0command\0item\0options\0zone", "", "vip-ext"}, /* является свойством терминала в API 1.8.1-20*/
#endif
#endif
			{ 3,"commands\0command\0item\0id"},
			{ 6,"commands\0command\0item\0sorm_translation"},
			/* CORE-1359 *************************************************************************/
			{ -2,"commands\0command\0item\0user_terminals\0user_terminal\0terminal_id", "", "0"},
			{ 7,"commands\0command\0item\0user_terminals\0user_terminal\0login"},
			{ 8,"commands\0command\0item\0user_terminals\0user_terminal\0password"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0terminal_type","","Registerable"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0ttl","","120"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0profile","","User SIP profile"},
#ifdef _DEBUG
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0zone", "", "voip"}
#else
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0zone", "", "vip-ext"}
#endif
			/* CORE-1359 *************************************************************************/
		},
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,""}
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
	{
		"UpdateUser",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Edit\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
			{ 4,"commands\0command\0item\0web_password"},
#elif SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","version\0\"1.8\""},
			{ 4,"commands\0command\0item\0web_password"},
#endif
			{ 3,"commands\0command\0item\0id"}
		},
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,""}
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
	{
		"ChangePhoneNumber",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Edit\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#elif SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{ 3,"commands\0command\0item\0id"},
			{ 4,"commands\0command\0item\0new_id"},
#if SIP_RTU_API_VERSION >= 19000
			{ 4,"commands\0command\0item\0subscriber"},
#elif SIP_RTU_API_VERSION >= 18120
			{ 4,"commands\0command\0item\0name"},
			{ 6,"commands\0command\0item\0web_password"},
#else
			{ 4,"commands\0command\0item\0name"},
#endif
			{ 5,"commands\0command\0item\0sorm_translation"},
			{ 7,"commands\0command\0item\0jid"}
		},
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,""}
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
	{
		"UpdateUserIdentity",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Edit\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#elif SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{ 3,"commands\0command\0item\0id"},
#if SIP_RTU_API_VERSION >= 19000
			{ 4,"commands\0command\0item\0web_login"},
			{ 5,"commands\0command\0item\0web_password"}
#elif SIP_RTU_API_VERSION >= 18120
			{ 5,"commands\0command\0item\0web_password"}
			{ 4,"commands\0command\0item\0user"},
			{ 5,"commands\0command\0item\0password"},
			{ 4,"commands\0command\0item\0name"}
#else
			{ 4,"commands\0command\0item\0user"},
			{ 5,"commands\0command\0item\0password"},
			{ 4,"commands\0command\0item\0name"}
#endif
		},
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,""}
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
	{
		"DeleteUser",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Delete\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#elif SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{ 3,"commands\0command\0item\0id"}
		},
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,""}
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
	{
		"CreateUserTerminal",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
#if SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","name\0\"Edit\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#else
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{ 3,"commands\0command\0item\0id"},
			{ 4,"commands\0command\0item\0user_terminals\0user_terminal\0terminal_id"},
			{ 5,"commands\0command\0item\0user_terminals\0user_terminal\0login"},
			{ 6,"commands\0command\0item\0user_terminals\0user_terminal\0password"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0terminal_type","","Registerable"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0ttl","","120"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0profile","","User SIP profile"},
#ifdef _DEBUG
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0zone", "", "voip"}
#else
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0zone", "", "vip-ext"}
#endif
		},
#else
			{-1,"commands\0command","name\0\"Create\""},
			{-1,"commands\0command","table\0\"UserTerminal\""},
			{ 3,"commands\0command\0item\0user_id"},
			{ 4,"commands\0command\0item\0login"},
			{ 5,"commands\0command\0item\0password"},
			{-2,"commands\0command\0item\0terminal_id","","0"},
			{-2,"commands\0command\0item\0terminal_type","","Registerable"},
			{ 6,"commands\0command\0item\0terminal_id"},
			{ 7,"commands\0command\0item\0terminal_type"},
			{ 8,"commands\0command\0item\0registration_adress_port"},
			{ 9,"commands\0command\0item\0registration_date"}
		},
#endif
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,""}
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
#if SIP_RTU_API_VERSION >= 18120
	{
		"GetUserTerminalGUId",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Get\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#else
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{ 3,"commands\0command\0item\0id"}
		},
		{
			"Root\0command\0User\0user_terminals",
			{ 4,"user_terminal\0terminal_id"},
			{-2,"terminal_id"}
		},
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"" }
		}
	},
#else
	{
		"GetUserTerminalGUId",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Get\""},
			{-1,"commands\0command","table\0\"UserTerminal\""},
			{ 3,"commands\0command\0item\0user_id"}
		},
		{
			"Root\0command",
			{ 4,"terminal\0terminal_id"},
			{-2,"guid"}
		},
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason"}
		},
		{
			"Root\0Errors",
			{ 0 },
			{-1,"error"}
		}
	},
#endif
	{
		"UpdateUserTerminal",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Edit\""},
#if SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#else
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{ 3,"commands\0command\0item\0id"},
			{ 4,"commands\0command\0item\0user_terminals\0user_terminal\0terminal_id"},
			{ 5,"commands\0command\0item\0user_terminals\0user_terminal\0login"},
			{ 6,"commands\0command\0item\0user_terminals\0user_terminal\0password"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0terminal_type","","Registerable"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0ttl","","120"},
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0profile","","User SIP profile"},
#ifdef _DEBUG
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0zone", "", "voip"}
#else
			{-2,"commands\0command\0item\0user_terminals\0user_terminal\0zone", "", "vip-ext"}
#endif
#else
			{-1,"commands\0command","table\0\"UserTerminal\""},
			{ 3,"commands\0command\0item\0guid"},
			{ 4,"commands\0command\0item\0login"},
			{ 5,"commands\0command\0item\0password"}
#endif
		},
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,""}
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
#ifdef SIP_RTU_API_VERSION >= 18120
#else
	{
		"DeleteUserTerminal",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Delete\""},
			{-1,"commands\0command","table\0\"UserTerminal\""},
			{ 3,"commands\0command\0item\0guid"}
		},
		{
			"Root\0command\0item",
			{-2,"result","","true"}
		},
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
	},
#endif
	{
		"GetUserInfo",
		{
			{ 0,"commands\0authorize\0login"},
			{ 1,"commands\0authorize\0password"},
			{ 2,"commands\0authorize\0domain"},
			{-1,"commands\0command","name\0\"Get\""},
			{-1,"commands\0command","table\0\"User\""},
#if SIP_RTU_API_VERSION >= 19000
			{-1,"commands\0command","version\0\"1.9\""},
#elif SIP_RTU_API_VERSION >= 18120
			{-1,"commands\0command","version\0\"1.8\""},
#endif
			{ 3,"commands\0command\0item\0id"}
		},
		{
			"Root\0command",
			{ 3,"User\0id"}
		},
#if SIP_RTU_API_VERSION >= 18120
		{
			"Root\0command",
			{-2,"item\0result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"" }
		}
#else
		{
			"Root\0command\0item",
			{-2,"result","","false"},
			{-2,"reason\0error"}
		},
		{
			"Root\0Errors\0error",
			{ 0 },
			{-1,"reason"}
		}
#endif
	},
	{0}
};

size_t g_stCmdRuleSize = sizeof (g_msoCommandRule);
