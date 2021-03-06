local lak = require "odielak";

local escape = lak.new({ -- compile from the given table (or tables!)
	['&'] = '&amp;',	-- each key (1 byte / an ascii char < 256) will be replaced with the corresponding value
	['<'] = function(self, str, byte) return '&lt;'; end, -- this function will be called only once per FIRST match during the replacement procedure
	['>'] = '&gt;',
	['12'] = 'ignored value',
	[512] = 'also ignored',
	[9] = '', -- this will remove tabs (string.byte('\t') == 9)
	['&'] = setmetatable({'&amp;'}, { __tostring = function(self) return self[1] end}), -- lak.new() will call this __tostring metaattr
},{ -- we also can add more tables
	['"'] = '&qout;',
	['\''] = '&#x27;',
	['/'] = '&#x2F;',
});

-- 'escape' is a table, wich has __call metaattr by default from the 'lak._meta' metatable
-- So now just call it like a function

local str = escape("escape >this< 'string' \t\t&pls& ");

print(str); -- 'escape &gt;this&lt; &#x27;string&#x27; &amp;pls&amp; '

-- also we can use a table with __tostring metaattr, not only a string

local str = escape(setmetatable({"escape >this< 'string' \t\t&pls& "}, {
	__tostring = function(self)
		return self[1];
	end
}));

print(str); -- 'escape &gt;this&lt; &#x27;string&#x27; &amp;pls&amp; '

-- multiple arguments

local str, str2 = escape("escape >this< 'string' \t\t&pls& ", "<&and this one&>");

print(str); -- 'escape &gt;this&lt; &#x27;string&#x27; &amp;pls&amp; '
print(str2); -- '&lt;&amp;and this one&amp;&gt;'
