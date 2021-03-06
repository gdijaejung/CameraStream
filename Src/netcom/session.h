//
// 2015-11-28, jjuiddong
//
// Session 구조체
//
//
// 2016-02-06
//		- gcc 용 작업
//
#pragma once

#include "network.h"


namespace network
{

	namespace SESSION_STATE {
		enum TYPE {
			DISCONNECT,
			LOGIN_WAIT,
			LOGIN,					// 인증이 된 상태
			LOGOUT_WAIT,	// 제거 대기 목록에 있는 상태
		};
	}


	struct sSession
	{
		SESSION_STATE::TYPE state;
		SOCKET socket;
	};

}
