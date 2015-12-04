	-- same with gsub

	local t = {
		['&'] = '&amp;',
		['<'] = '&lt;',
		['>'] = '&gt;'
	}

	local r = "[&<>]";

	local escaped_str = string.gsub("escape >th&is<", r, t);
	-- escaped_str = "escape &gt;th&amp;is&lt;"
print(escaped_str)
