#ifndef _DRIVERRTU_H_
#define _DRIVERRTU_H_

#define SIP_RTU_API_VERSION 19000

/*	Инициализация статических параметров
 *	Принимаемые параметры:
 *	log_file_mask
 *	rtu_url
 *	debug
 *	параметры разделены символом ';'
 */
int __stdcall InitService (char *p_pszInitParameters, char *p_pszResult);

/*	Функция освобождает ресурсы,
 *	занятые драйвером
 */
int __stdcall DeInitService (char *p_pszResult);

/*	Функция CreateUser создает абонента на сервисной платформе
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *			user login
 *			user password
 *			sorm translation
 *	Возвращаемое значение:
 *	0 - успешно
 */
int __stdcall CreateUser (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	Функция ChangePhoneNumber создает абонента на сервисной платформе
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *			user new id
 *			sorm translation
 *	Возвращаемое значение:
 *	0 - успешно
 */
int __stdcall ChangePhoneNumber (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	Функция UpdateUserIdentity обновляет логин/пароль учетной записи
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			admin domain
 *			user id
 *			user login
 *			user password
 *	Возвращаемое значение:
 *	0 - успешно
 */
int __stdcall UpdateUserIdentity (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	Функция UpdateUserPolicy создает абонента на сервисной платформе
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			admin domain
 *			user id
 *			group list
 *			attr list
 *	Возвращаемое значение:
 *	0 - успешно
 */
int __stdcall UpdateUserPolicy (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	Функция DeleteUser создает абонента на сервисной платформе
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *	Возвращаемое значение:
 *	0 - успешно
 */
int __stdcall DeleteUser (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
/*	Функция CreateUserTerminal создает абонента на сервисной платформе
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *			user terminal id
 *			terminal login
 *			terminal password
 *	Возвращаемое значение:
 *	0 - успешно
 */
#else
/*	Функция CreateUserTerminal создает абонента на сервисной платформе
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *			terminal login
 *			terminal password
 *			user terminal id
 *			terminal type : Static | Registerable
 *			registration adress port
 *			registration date
 *	Возвращаемое значение:
 *	0 - успешно
 */
#endif
int __stdcall CreateUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
#else
/*	Функция GetUserTerminalGUId
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			admin domain
 *			abon id
 *			terminal id
 *	Возвращаемое значение:
 *	0 - успешно
 */
#endif
int __stdcall GetUserTerminalGUId (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
/*	Функция UpdateUserTerminal
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			admin domain
 *			abon id
 *			terminal id
 *			terminal login
 *			terminal password
 *	Возвращаемое значение:
 *	0 - успешно
 */
#else
/*	Функция UpdateUserTerminal
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			admin domain
 *			terminal GUId
 *			terminal login
 *			terminal password
 *	Возвращаемое значение:
 *	0 - успешно
 */
#endif
int __stdcall UpdateUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
#else
/*	Функция DeleteUserTerminal
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			admin domain
 *			terminal GUId
 *	Возвращаемое значение:
 *	0 - успешно
 */
#endif
int __stdcall DeleteUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	Функция GetUserInfo
 *	Параметры:
 *		обязательные
 *			admin login
 *			admin password
 *			admin domain
 *			abon id
 *	Возвращаемое значение:
 *	0 - успешно
 */
int __stdcall GetUserInfo (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#endif
