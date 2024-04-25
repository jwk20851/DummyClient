﻿  






// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.
   
#pragma once


#include "S2C_common.h"

     
namespace S2C {


	class Stub : public ::Proud::IRmiStub
	{
	public:
               
		virtual bool recvString ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const int & )		{ 
			return false;
		} 

#define DECRMI_S2C_recvString bool recvString ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & txt, const int & num) PN_OVERRIDE

#define DEFRMI_S2C_recvString(DerivedClass) bool DerivedClass::recvString ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & txt, const int & num)
#define CALL_S2C_recvString recvString ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & txt, const int & num)
#define PARAM_S2C_recvString ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & txt, const int & num)
 
		virtual bool ProcessReceivedMessage(::Proud::CReceivedMessage &pa, void* hostTag) PN_OVERRIDE;
		static const PNTCHAR* RmiName_recvString;
		static const PNTCHAR* RmiName_First;
		virtual ::Proud::RmiID* GetRmiIDList() PN_OVERRIDE { return g_RmiIDList; }
		virtual int GetRmiIDListCount() PN_OVERRIDE { return g_RmiIDListCount; }
	};

#ifdef SUPPORTS_CPP11 
	
	class StubFunctional : public Stub 
	{
	public:
               
		std::function< bool ( ::Proud::HostID, ::Proud::RmiContext& , const Proud::String & , const int & ) > recvString_Function;
		virtual bool recvString ( ::Proud::HostID remote, ::Proud::RmiContext& rmiContext , const Proud::String & txt, const int & num) 
		{ 
			if (recvString_Function==nullptr) 
				return true; 
			return recvString_Function(remote,rmiContext, txt, num); 
		}

	};
#endif

}


