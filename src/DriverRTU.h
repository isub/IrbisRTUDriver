#ifndef _DRIVERRTU_H_
#define _DRIVERRTU_H_

#define SIP_RTU_API_VERSION 19000

/*	������������� ����������� ����������
 *	����������� ���������:
 *	log_file_mask
 *	rtu_url
 *	debug
 *	��������� ��������� �������� ';'
 */
int __stdcall InitService (char *p_pszInitParameters, char *p_pszResult);

/*	������� ����������� �������,
 *	������� ���������
 */
int __stdcall DeInitService (char *p_pszResult);

/*	������� CreateUser ������� �������� �� ��������� ���������
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *			user login
 *			user password
 *			sorm translation
 *	������������ ��������:
 *	0 - �������
 */
int __stdcall CreateUser (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	������� ChangePhoneNumber ������� �������� �� ��������� ���������
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *			user new id
 *			sorm translation
 *	������������ ��������:
 *	0 - �������
 */
int __stdcall ChangePhoneNumber (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	������� UpdateUserIdentity ��������� �����/������ ������� ������
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			admin domain
 *			user id
 *			user login
 *			user password
 *	������������ ��������:
 *	0 - �������
 */
int __stdcall UpdateUserIdentity (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	������� UpdateUserPolicy ������� �������� �� ��������� ���������
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			admin domain
 *			user id
 *			group list
 *			attr list
 *	������������ ��������:
 *	0 - �������
 */
int __stdcall UpdateUserPolicy (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	������� DeleteUser ������� �������� �� ��������� ���������
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *	������������ ��������:
 *	0 - �������
 */
int __stdcall DeleteUser (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
/*	������� CreateUserTerminal ������� �������� �� ��������� ���������
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			user domain
 *			user id
 *			user terminal id
 *			terminal login
 *			terminal password
 *	������������ ��������:
 *	0 - �������
 */
#else
/*	������� CreateUserTerminal ������� �������� �� ��������� ���������
 *	���������:
 *		������������
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
 *	������������ ��������:
 *	0 - �������
 */
#endif
int __stdcall CreateUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
#else
/*	������� GetUserTerminalGUId
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			admin domain
 *			abon id
 *			terminal id
 *	������������ ��������:
 *	0 - �������
 */
#endif
int __stdcall GetUserTerminalGUId (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
/*	������� UpdateUserTerminal
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			admin domain
 *			abon id
 *			terminal id
 *			terminal login
 *			terminal password
 *	������������ ��������:
 *	0 - �������
 */
#else
/*	������� UpdateUserTerminal
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			admin domain
 *			terminal GUId
 *			terminal login
 *			terminal password
 *	������������ ��������:
 *	0 - �������
 */
#endif
int __stdcall UpdateUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#if SIP_RTU_API_VERSION >= 18120
#else
/*	������� DeleteUserTerminal
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			admin domain
 *			terminal GUId
 *	������������ ��������:
 *	0 - �������
 */
#endif
int __stdcall DeleteUserTerminal (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

/*	������� GetUserInfo
 *	���������:
 *		������������
 *			admin login
 *			admin password
 *			admin domain
 *			abon id
 *	������������ ��������:
 *	0 - �������
 */
int __stdcall GetUserInfo (void **p_ppvParameters, int p_iParamQuantity, char *p_pszResult);

#endif
