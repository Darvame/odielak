local lak = require "odielak"

local escape = lak:New({ -- compile from table
	['&'] = '&amp;',	-- each key (ascii char < 128) will be replaced with corresponding value
	['<'] = '&lt;',
	['>'] = '&gt;',
	['"'] = '&qout;',
	['\''] = '&#x27;',
	['/'] = '&#x2F;',
	['\t'] = '', -- this will remove tabs
})

-- 'escape' is userdata with metatable,
-- has __call metaattr by default
-- lak:New() method sets 'lak._meta' table
-- as metatable during compilation

-- now just call it

local str = escape("escape >this< 'string' \t\t&pls& ");

print(str); -- 'escape &gt;this&lt; &#x27;string&#x27; &amp;pls&amp; '

-- also we can use table with __tostring metaattr

local str = escape(setmetatable({"escape >this< 'string' \t\t&pls& "}, {
	__tostring = function(self)
		return self[1];
	end
}));

print(str); -- 'escape &gt;this&lt; &#x27;string&#x27; &amp;pls&amp; '
