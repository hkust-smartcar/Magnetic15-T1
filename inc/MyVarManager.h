/*
 * MyVarManager.h
 *
 * Author: PeterLau
 * Version: 2.9.0
 *
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

// TODO: enable following preprocessor command
//#ifdef LIBSC_USE_UART

#include <vector>
#include <typeinfo>
#include <string.h>
#include <cxxabi.h>

#include <libsc/system.h>
#include <libsc/k60/ftdi_ft232r.h>
#include <libbase/k60/sys_tick.h>
#include <libsc/k60/jy_mcu_bt_106.h>

#define MAX(a, b) ((a > b)? a : b)
#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))

using namespace libsc;
using namespace libsc::k60;
using namespace libbase::k60;

class MyVarManager
{
public:

	class ObjMng
	{
	public:

		explicit ObjMng(void *pObj, Byte len, const std::string &typeName, const std::string &objName)
		:
			obj(pObj),
			len(len),
			typeName(typeName),
			varName(objName)
		{}

		~ObjMng() {};

		void						*obj;
		Byte						len;
		std::string					typeName;
		std::string					varName;
	};

	class TypeId
	{
	public:

		static void Init()
		{
			if (!m_instance)
				m_instance = new TypeId;
		}

		static std::string getTypeId(uint8_t o) { return "unsigned char"; }
		static std::string getTypeId(int8_t o) { return "signed char"; }
		static std::string getTypeId(uint16_t o) { return "unsigned short"; }
		static std::string getTypeId(int16_t o) { return "short"; }
		static std::string getTypeId(uint32_t o) { return "unsigned int"; }
		static std::string getTypeId(int32_t o) { return "int"; }
		static std::string getTypeId(float o) { return "float"; }
		template<typename T>
		static std::string getTypeId(T o) { return "wtf?"; }

	private:

		static TypeId *m_instance;
	};

	typedef std::function<void(const std::vector<Byte>&)> OnReceiveListener;

	explicit MyVarManager(void);
	~MyVarManager(void);

	void Init(void);
	void Init(const OnReceiveListener &oriListener);
	void UnInit(void);

	template<typename ObjType>
	void addSharedVar(ObjType *sharedObj, std::string s)
	{
		if (!isStarted)
		{
			ObjMng newObj(sharedObj, sizeof(*sharedObj), TypeId::getTypeId(*sharedObj), s);
			sharedObjMng.push_back(newObj);
		}
	}

	template<typename ObjType>
	void addWatchedVar(ObjType *watchedObj)
	{
		if (!isStarted)
		{
			ObjMng newObj(watchedObj, sizeof(*watchedObj), TypeId::getTypeId(*watchedObj), "");
			watchedObjMng.push_back(newObj);
		}
	}

	void sendWatchData(void);

private:

	OnReceiveListener				m_origin_listener;

	std::vector<ObjMng>				sharedObjMng;
	std::vector<ObjMng>				watchedObjMng;

	bool							isStarted;
	const Byte						rx_threshold;

	JyMcuBt106						m_uart;

	std::vector<Byte>				rx_buffer;

	static bool listener(const std::vector<Byte> &bytes);

	SysTick::Config getTimerConfig(void);
	JyMcuBt106::Config get106UartConfig(const uint8_t id);
	FtdiFt232r::Config get232UartConfig(const uint8_t id);

	void sendWatchedVarInfo(void);
	void sendSharedVarInfo(void);

	void changeSharedVars(const std::vector<Byte> &msg);

};

//#endif /* LIBSC_USE_UART */
