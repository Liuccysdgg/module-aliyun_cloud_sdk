local aliyun_sdk = {}
aliyun_sdk.__index = aliyun_sdk

--[[
    创建一个新的 fw_aliyun_sdk 对象
    @return 返回一个新的 fw_aliyun_sdk 对象
]]
function aliyun_sdk.new(db)
    local instance = setmetatable({}, aliyun_sdk)
    if db == nil then
        instance.module = fw_aliyunsdk.new()
    else
        instance.module = db    
    end
    
    return instance
end

function aliyun_sdk:set(access_key_id, access_key_secret)
    return self.module:set(access_key_id, access_key_secret)
end

function aliyun_sdk:exec(domain, version, parameter)
    return self.module:exec(domain, version, parameter)
end

return aliyun_sdk
