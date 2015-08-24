function(get_group path default_group group)
	# extract the group information from path
	# e.g. "src/details" will be group "src\\details"
	get_filename_component(dir ${path} DIRECTORY)

	if(dir)
		string(REGEX REPLACE "(^src$|^src/)" "" dir ${dir})	# special handling for src/ in libs.

		if(dir)
			string(REGEX REPLACE "/" "\\\\" dir ${dir})
			set(${group} "${default_group}\\${dir}" PARENT_SCOPE)
		else()
			set(${group} ${default_group} PARENT_SCOPE)
		endif()
	else()
		set(${group} ${default_group} PARENT_SCOPE)
	endif()
endfunction(get_group)