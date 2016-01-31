#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "stdlib.h"
#include "string.h"

#define LAK_DICT_SIZE 128
#define LAK_BUF_STACK_SIZE 2048

struct lak_string {
	const char *str;
	size_t len;
};

struct lak_header {
	struct lak_string *repls;
	unsigned char dict[LAK_DICT_SIZE];
};

static int replace_ut_tostring(lua_State *l, int obj)
{
	if (luaL_callmeta(l, obj, "__tostring")) {
		if (lua_isstring(l, -1)) {
			lua_replace(l, obj - 1);
			return 1;
		}
	}

	return 0;
}

static int replace(lua_State *l)
{
	if (!lua_isuserdata(l, 1)) {
		return 0;
	}

	switch (lua_type(l, 2)) {
		case LUA_TTABLE:
		case LUA_TUSERDATA: if (!replace_ut_tostring(l, 1 - lua_gettop(l))) return 0;
		case LUA_TSTRING:
		case LUA_TNUMBER: break;
		default: return 0;
	}

	struct lak_header *header = lua_touserdata(l, 1);
	struct lak_string *rep = header->repls;

	if (!rep) {
		lua_pushvalue(l, 2);
		return 1;
	}

	size_t i;
	size_t len;

	const unsigned char *str = (const unsigned char *) lua_tolstring(l, 2, &len);
	const unsigned char *dict = header->dict;

	size_t matched = 0;
	size_t oversize = 0;

	for (i = 0; i < len; ++i) {
		if (str[i] < LAK_DICT_SIZE && dict[str[i]]) {
			oversize+= rep[dict[str[i]] - 1].len;
			++matched;
		}
	}

	if (!matched) {
		lua_pushvalue(l, 2);
		return 1;
	}

	oversize+= len - matched;

	char sbuf[LAK_BUF_STACK_SIZE];
	struct lak_string *repstr;
	char *new;

	if (oversize > LAK_BUF_STACK_SIZE) {
		new = (char *) malloc(oversize);

		if (!new) {
			lua_pushstring(l, "bad malloc();");
			lua_error(l);
		}
	} else {
		new = sbuf;
	}

	char *push = new;

	for (i = 0; i < len; ++i) {
		if (str[i] < LAK_DICT_SIZE && dict[str[i]]) {
			repstr = &rep[dict[str[i]] - 1];
			memcpy(new, repstr->str, repstr->len * sizeof(char));
			new+= repstr->len;
		} else {
			*(new++) = str[i];
		}
	}

	lua_pushlstring(l, push, oversize);

	if (oversize > LAK_BUF_STACK_SIZE) {
		free(push);
	}

	return 1;
}

static int make_set_meta(lua_State *l)
{
	int error = 1;

	if (lua_istable(l, 1)) {
		lua_getfield(l, 1, "_meta");

		if (lua_istable(l, -1)) {
			lua_setmetatable(l, -2);
		}

		error = 0;
	}

	if (error) {
		lua_pushstring(l, "failed to setmetatable();");
		lua_error(l);
	}

	return 1;
}

static int make_new(lua_State *l)
{
	struct lak_string repty[LAK_DICT_SIZE];
	unsigned char dict[LAK_DICT_SIZE] = {};

	size_t i;
	size_t lnk;
	size_t used = 0;
	size_t size = 0;

	const unsigned char *key;

	if (lua_istable(l, 2)) {
		lua_pushnil(l);

		while(lua_next(l, 2)) {
			lua_pushvalue(l, -2);

			key = (const unsigned char *) lua_tolstring(l, -1, &lnk);

			if (!lnk || lnk > 1 || key[0] > LAK_DICT_SIZE) {
				lua_pop(l, 2);
				continue;
			}

			if (!dict[key[0]]) {
				lnk = used++;
				dict[key[0]] = lnk + 1;
			} else {
				lnk = dict[key[0]];
			}

			repty[lnk].str = lua_tolstring(l, -2, &(repty[lnk].len));
			size+= (repty[lnk].len);

			lua_pop(l, 2);
		}
	}

	struct lak_header *header;
	struct lak_string *rep;
	char *value;

	if (!used) {
		header = (struct lak_header *)(lua_newuserdata(l, sizeof(struct lak_string *)));
		header->repls = NULL;
		return make_set_meta(l);
	}

	header =  (struct lak_header *)(lua_newuserdata(l, sizeof(struct lak_header) + used * sizeof(struct lak_string) + size));
	rep = (struct lak_string *)(header + 1);
	value = (char *)(rep + used);

	memcpy(header->dict, dict, LAK_DICT_SIZE * sizeof(char));
	header->repls = rep;

	for (i = 0; i < used; ++i) {
		lnk = repty[i].len;

		rep[i].len = lnk;
		rep[i].str = value;

		memcpy(value, repty[i].str, lnk * sizeof(char));
		value+= lnk;
	}

	return make_set_meta(l);
}

int luaopen_odielak(lua_State *l)
{
	lua_createtable(l, 0, 2);

	lua_pushcfunction(l, make_new);
	lua_setfield(l, -2, "New");

	lua_createtable(l, 0, 1);
	lua_pushcfunction(l, replace);
	lua_setfield(l, -2, "__call");
	lua_setfield(l, -2, "_meta");

	return 1;
}
