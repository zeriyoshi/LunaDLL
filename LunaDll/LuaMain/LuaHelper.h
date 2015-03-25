// LuaHelper.h Helper Functions
#ifndef LuaHelper_HHH
#define LuaHelper_HHH

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/detail/call_function.hpp>
#include "../Misc/VB6StrPtr.h"

namespace LuaHelper {
    luabind::object getEventCallbase(lua_State* base, std::string eventTable);
    bool is_function(struct lua_State * luaState, const char * fname);
	void assignVB6StrPtr(VB6StrPtr* ptr, const luabind::object &value, lua_State* L);


    class invalidIDException : std::exception {
    public:
        invalidIDException(int usedID) : std::exception() {
            m_usedID = usedID;
        }
        int usedID() const { return m_usedID; }
        void usedID(int val) { m_usedID = val; }
    private:
        int m_usedID;
    };
    class invalidTypeException : std::exception {
    public:
        invalidTypeException() : std::exception() {}
    };

    bool* generateFilterTable(lua_State* L, luabind::object theFilter, int maxVal, int minVal = 1);
}

#endif
