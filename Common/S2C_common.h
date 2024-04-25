#pragma once

namespace S2C {


	//Message ID that replies to each RMI method. 
               
    static const ::Proud::RmiID Rmi_recvString = (::Proud::RmiID)(4000+1);

	// List that has RMI ID.
	extern ::Proud::RmiID g_RmiIDList[];
	// RmiID List Count
	extern int g_RmiIDListCount;

}


 
