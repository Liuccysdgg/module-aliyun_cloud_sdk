#include "aliyunsdk.h"
#include "dll_interface.h"
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/core/CommonRequest.h>
#include <alibabacloud/core/CommonClient.h>
#include <alibabacloud/core/CommonResponse.h>
extern "C" {
#ifdef _WIN32
    DLL_EXPORT
#endif
        int fastweb_module_regist(void* sol2, void* lua)
    {
        sol::state* state = static_cast<sol::state*>(sol2);
        module::aliyun_sdk::regist(state);
        return 0;
    }
}

module::aliyun_sdk::aliyun_sdk()
{
    AlibabaCloud::InitializeSdk();
}

module::aliyun_sdk::~aliyun_sdk()
{
    AlibabaCloud::ShutdownSdk();
}

void module::aliyun_sdk::set(const std::string& access_key_id,const std::string& access_key_secret)
{
    m_access_key_id = access_key_id;
    m_access_key_secret = access_key_secret;
}


std::tuple<bool,std::string> module::aliyun_sdk::exec(const std::string& domain,const std::string& version,sol::table parameter)
{
    AlibabaCloud::ClientConfiguration configuration( "cn-qingdao" );
    configuration.setConnectTimeout(1500);
    configuration.setReadTimeout(4000);
    AlibabaCloud::Credentials credential(m_access_key_id, m_access_key_secret );

    AlibabaCloud::CommonClient client( credential, configuration );
    AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
    request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
    request.setDomain(domain);
    request.setVersion(version);

    for (const auto& pair : parameter) {
        sol::object key = pair.first;
        sol::object value = pair.second;

        std::string key_str;
        std::string value_str;
        if (key.is<std::string>()) {
            key_str = key.as<std::string>();
        } else if (key.is<int>()) {
            key_str = std::to_string(key.as<int>());
        } else {
            throw std::string("unsupported key type");
        }

        if (value.is<std::string>()) {
            value_str = value.as<std::string>();
        } else if (value.is<int>()) {
            value_str = std::to_string(value.as<int>());
        } else if (value.is<bool>()) {
            value_str = (value.as<bool>() ? "true" : "false");
        } else {
            throw std::string("unsupported value type");
        }
        request.setQueryParameter(key_str, value_str);
    }
    auto response = client.commonResponse(request);
    if (response.isSuccess())
        return std::make_tuple(true, response.result().payload());
    return std::make_tuple(false, response.error().errorMessage());
}

void module::aliyun_sdk::regist(sol::state* lua)
{
    lua->new_usertype<module::aliyun_sdk>("fw_aliyunsdk",
        "new", sol::constructors<module::aliyun_sdk()>(),
        "set", &module::aliyun_sdk::set,
        "exec", &module::aliyun_sdk::exec
    );
}

void module::aliyun_sdk::regist_global(const char* name, sol::state* lua)
{
    lua->registry()[name] = this;
    (*lua)[name] = this;
}
