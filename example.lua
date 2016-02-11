local lak = require "odielak";

local escape = lak:New({ -- compile from the given table
	['&'] = '&amp;',	-- each key (ascii char < 128) will be replaced with the corresponding value
	['<'] = '&lt;',
	['>'] = '&gt;',
	['"'] = '&qout;',
	['\''] = '&#x27;',
	['/'] = '&#x2F;',
	['12'] = 'ignored value',
	[9] = '', -- this will remove tabs (string.byte('\t') == 9)
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
