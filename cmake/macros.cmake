#add header to install
MACRO(GLOBAL_ADD_INCLUDE COMPONENT_LOCATION )
  INSTALL(FILES ${ARGN} DESTINATION include/${COMPONENT_LOCATION} COMPONENT headers)
ENDMACRO(GLOBAL_ADD_INCLUDE COMPONENT_LOCATION )

MACRO(GLOBAL_ADD_INCLUDE_V2)
    file(RELATIVE_PATH _relative_path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
    string(REPLACE "/CMakeLists.txt" "" _relative_dir "${_relative_path}" )
    INSTALL(FILES ${ARGN} DESTINATION include/${_relative_dir} COMPONENT headers)
    #message(==== ${_relative_dir})
ENDMACRO()

#add source file to global source variable
MACRO(GLOBAL_ADD_SRC )
    STRING(LENGTH "${ARGN}" NOTEMPTY)
    IF(NOTEMPTY)
        SET(ENV{GLOBAL_SRCS} "$ENV{GLOBAL_SRCS}${ARGN};" )
    ENDIF(NOTEMPTY)
ENDMACRO(GLOBAL_ADD_SRC )

MACRO(GLOBAL_ADD_MODULE_SRC MODULE_VAR)
    SET(ENV{${MODULE_VAR}} "$ENV{${MODULE_VAR}}${ARGN};")
ENDMACRO( GLOBAL_ADD_MODULE_SRC MODULE_VAR)

#get dir list
MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

MACRO(GENPROTO)
    STRING(REPLACE ${CMAKE_BINARY_DIR} ${PROTO_PATH} _search_path ${CMAKE_CURRENT_BINARY_DIR})  
    if(GENERATE_PROTO STREQUAL "ON")
        FILE(GLOB PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)
        FOREACH(proto ${PROTO_FILES})
        FILE(TO_NATIVE_PATH ${proto} proto_native)
        message("-- protobuf message generate " ${proto_native} )
        EXECUTE_PROCESS(COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
            --proto_path=${PROJECT_SOURCE_DIR}
            --cpp_out=${PROTO_PATH} ${proto_native} 
        )

	#message("1##### ${PROJECT_SOURCE_DIR}")
	#message("2##### ${PROTO_PATH}")
	#message("3##### ${proto_native}")
	#message("4##### ${CMAKE_CURRENT_SOURCE_DIR}")

        #EXECUTE_PROCESS(COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
        #    --proto_path=${PROJECT_SOURCE_DIR}
        #    --python_out=${PROTO_PATH} ${proto_native} 
        #)
        ENDFOREACH(proto)
    endif()
    FILE( GLOB CPPS ${_search_path}/[^.]*.cpp ${_search_path}/[^.]*.cc )
    FILE( GLOB HPPS ${_search_path}/[^.]*.hpp ${_search_path}/[^.]*.h ${_search_path}/[^.]*.inl)
ENDMACRO()

MACRO(GENPROTO_CYBER)
    STRING(REPLACE ${CMAKE_BINARY_DIR} ${PROTO_PATH} _search_path ${CMAKE_CURRENT_BINARY_DIR})  
    if(GENERATE_PROTO STREQUAL "ON")
        FILE(GLOB PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)
        FOREACH(proto ${PROTO_FILES})
        FILE(TO_NATIVE_PATH ${proto} proto_native)
        message("-- protobuf message generate " ${proto_native} )
        EXECUTE_PROCESS(COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
            --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
            --cpp_out=${PROTO_PATH} ${proto_native} 
        )

	#message("1##### ${PROJECT_SOURCE_DIR}")
	#message("2##### ${PROTO_PATH}")
	#message("3##### ${proto_native}")
	#message("4##### ${CMAKE_CURRENT_SOURCE_DIR}")

        #EXECUTE_PROCESS(COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
        #    --proto_path=${PROJECT_SOURCE_DIR}
        #    --python_out=${PROTO_PATH} ${proto_native} 
        #)
        ENDFOREACH(proto)
    endif()
    FILE( GLOB CPPS ${_search_path}/[^.]*.cpp ${_search_path}/[^.]*.cc )
    FILE( GLOB HPPS ${_search_path}/[^.]*.hpp ${_search_path}/[^.]*.h ${_search_path}/[^.]*.inl)
ENDMACRO()
#for test target
MACRO(GENTEST)
    foreach(_file ${ARGN})
        string(REGEX REPLACE "\\.[^.]*$" "" file_without_ext ${_file})
        get_filename_component( file_name_without_ext ${file_without_ext} NAME)
        add_executable(${file_name_without_ext} ${_file})
        target_include_directories(${file_name_without_ext} PRIVATE ${CMAKE_SOURCE_DIR})
        target_link_libraries(${file_name_without_ext} PRIVATE ${cyber_lib})
    endforeach(_file ${CPPS})
ENDMACRO()
