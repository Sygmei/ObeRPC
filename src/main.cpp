#include <Engine/Engine.hpp>

#include <PluginSDK.hpp>
#include <rxi.json.lua.hpp>
#include <sol/sol.hpp>

#include <httplib/httplib.h>

class LuaRpcClient
{
private:
    httplib::Client m_client;
    sol::table m_json_module;
public:
    LuaRpcClient(sol::this_state lua, const std::string& url);
    sol::lua_value call(const sol::table& content);
};

LuaRpcClient::LuaRpcClient(sol::this_state this_state, const std::string& url) : m_client(url)
{
    sol::state_view lua(this_state);
    const sol::protected_function_result json_module_res = lua.do_string(rxi_json_lua_source);
    m_json_module = json_module_res.get<sol::table>();
}

sol::lua_value LuaRpcClient::call(const sol::table& content)
{
    const sol::protected_function_result encoded_content_res = m_json_module["encode"](content);
    const std::string json_content = encoded_content_res.get<std::string>();
    auto res = m_client.Post("/", json_content, "application/json");
    if (res->status == 200) {
        const sol::protected_function_result decoded_content_res = m_json_module["decode"](res->body);
        return decoded_content_res.get<sol::table>();
    }
    else {
        return sol::nil;
    }
}

void load_lua_jsonrpc(sol::state_view lua)
{
    const sol::protected_function_result json_module_res = lua.do_string(rxi_json_lua_source);
    sol::table json_module = json_module_res.get<sol::table>();


    sol::usertype<LuaRpcClient> bind_jsonrpc
        = lua.new_usertype<LuaRpcClient>("jsonrpc",
            sol::call_constructor, sol::constructors<LuaRpcClient(sol::this_state, const std::string&)>());
    bind_jsonrpc[sol::meta_function::call] = &LuaRpcClient::call;
}

PLUGIN_FUNC void OnInit(obe::engine::Engine& engine)
{
    sol::state_view lua = engine.get_lua_state();
    load_lua_jsonrpc(lua);
}