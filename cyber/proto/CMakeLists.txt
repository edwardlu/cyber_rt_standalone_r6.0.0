project(cyber_proto)

execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/generate_proto.sh ${CMAKE_CURRENT_SOURCE_DIR})
