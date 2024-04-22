# luaref

完美替代luaL_ref。

## luaL_ref的缺点

luaL_ref(L, LUA_REGISTRYINDEX)需要占用LUA_REGISTRYINDEX的所有整数key，不同业务共用一个表不利于调试和维护。使用自定义的表可以解决这个问题，但是每次访问需要两次查表。

## luaref的优点

luaref利用了lua_newthread可以同时被c侧和lua侧同时引用的特点，只要lua_newthread的对象不被gc回收，lua_newthread返回的指针就可以被随意地保存下来使用，无需每次从lua侧找回。这就省略了luaL_ref使用自定义表时的第一次查表。

lua_State栈可以大约视为一个c的数组，访问它比访问一个table更加高效。这就省略了luaL_ref使用自定义表时的第二次查表。

简而言之，利用luaref从lua找回一个对象，只需要进行一次数组的访问。
