

#	fatal error equal function
function(F_FATAL_EQUAL data1 data2 errorMessage)
	if(NOT ${data1} EQUAL ${data2})
		message(FATAL_ERROR ${errorMessage})
	endif()
endfunction()

#   fing git package
find_package(Git QUIET)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "Could not find_package(Git QUIET)!")
endif()

#	git init
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/.git")
	message(STATUS "Inits Git for submodules download")
	execute_process(COMMAND ${GIT_EXECUTABLE} init
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE process_result)
	F_FATAL_EQUAL(${process_result} 0 "git init => failed with: ${process_result}")
endif()

#	git submodule update --init --remote --merge (updates(clone from remote repository) all submodules,
#	and makes (checkout) for each submodule, to last (commit) in remote repository (of selected branch in submodule, default main)).
message(STATUS "Git submodule update --init --remote --merge")
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --remote --merge
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                RESULT_VARIABLE process_result)
F_FATAL_EQUAL(${process_result} 0 "git submodule update --init --recursive => failed with: ${process_result}")