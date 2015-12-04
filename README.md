# ODIELAK

A small lua lib, that performs char (ASCII char from 0 to 127) to value replacements, like <b>string.gsub</b>, but more faster

```lua
	-- with lak
	local lak = require "odielak";

	local escape = lak:New({
		['&'] = '&amp;',
		['<'] = '&lt;',
		['>'] = '&gt;'
	});

	local escaped_str = escape("escape >th&is<");
	-- escaped_str = "escape &gt;th&amp;is&lt;"
```

```lua
	-- same with gsub

	local t = {
		['&'] = '&amp;',
		['<'] = '&lt;',D
		['>'] = '&gt;'
	}

	local r = "[&<>]";

	local escaped_str = string.gsub("escape >th&is<", r, t);
	-- escaped_str = "escape &gt;th&amp;is&lt;"
```

---

<b>checkout [example.lua](./example.lua) for examples and more info </b>

---

# BUILDING & INSTALLATION

<h3>Downloading:</h3>

```bash
git clone https://github.com/Darvame/teateatea.git
cd teateatea
```

<h3>Setting Variables:</h3>

```bash
#by default sudo does not pass variables
#you may need to run everything bellow (from this point) with root
#in order to use '$ make install'
su -
```
- <b>with pgk-config</b>
```bash
export LUAPKG=lua5.2 #or any ('luajit', 'lua5.1', 'lua5.3')
```
- <b>without pgk-config</b>
```bash
# this values are defaults (from luajit), do not set if have the same
export PREFIX=/usr/local #prefix
export LUALIBDIR=$PREFIX/lib/lua/5.1 #path to lua libs, used only for '$ make install'
export LUAINC=-I$PREFIX/include/luajit-2.0 #path to lua headers
export LUA=$PREFIX/bin/luajit #lua executable, used only for '$ make test'
```
- <b>with clang</b>
```bash
export CC=clang #if build on osx
```

<h3>Building & Installing:</h3>

```bash
make && make install
```
