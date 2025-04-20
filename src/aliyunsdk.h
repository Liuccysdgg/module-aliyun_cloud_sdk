#pragma once
#include "basemodule.h"
namespace module
{
	/// <summary>
	/// AliyunSDK
	/// </summary>
	class aliyun_sdk :public module::base {
	public:
		aliyun_sdk();
		~aliyun_sdk() override;
		
		
		void set(const std::string& access_key_id,const std::string& access_key_secret);
		
		std::tuple<bool,std::string> exec(const std::string& domain,const std::string& version,sol::table parameter);

		static void regist(sol::state* lua);
	private:
		// 通过 imodule 继承
		virtual void regist_global(const char* name, sol::state* lua);
		virtual void delete_global() { delete this; }
	private:
		std::string m_access_key_id;
		std::string m_access_key_secret;

	};
}

