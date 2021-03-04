project(cyber_example)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${cyber_SOURCE_DIR})
include_directories(${cyber_BINARY_DIR})
add_compile_options(-O0)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS proto/examples.proto)

add_executable(talker talker.cc ${PROTO_SRCS} )
add_executable(listener listener.cc ${PROTO_SRCS})
add_executable(paramserver paramserver.cc)
add_executable(service service.cc ${PROTO_SRCS})
add_executable(record record.cc)

add_executable(tcp_echo_server tcp_echo_server.cc)
add_executable(tcp_echo_client tcp_echo_client.cc)
add_executable(udp_echo_server udp_echo_server.cc)
add_executable(udp_echo_client udp_echo_client.cc)

target_link_libraries(talker cyber)
target_link_libraries(listener cyber)
target_link_libraries(paramserver cyber)
target_link_libraries(service cyber)
target_link_libraries(record cyber)

target_link_libraries(tcp_echo_server cyber)
target_link_libraries(tcp_echo_client cyber)
target_link_libraries(udp_echo_server cyber)
target_link_libraries(udp_echo_client cyber)

add_library(common_component_example SHARED common_component_example/common_component_example.cc ${PROTO_SRCS})
add_library(timer_component_example SHARED timer_component_example/timer_component_example.cc ${PROTO_SRCS})
target_link_libraries(common_component_example cyber)
target_link_libraries(timer_component_example cyber)

add_library(timer_sender_01 SHARED timer_send_component/timer_send_01.cc ${PROTO_SRCS})
target_link_libraries(timer_sender_01 cyber)
add_library(timer_sender_02 SHARED timer_send_component/timer_send_02.cc ${PROTO_SRCS})
target_link_libraries(timer_sender_02 cyber)
add_library(timer_sender_03 SHARED timer_send_component/timer_send_03.cc ${PROTO_SRCS})
target_link_libraries(timer_sender_03 cyber)

file(GLOB EXAMPLE_FILES "*/*.dag" "*/*.launch")
install(TARGETS common_component_example timer_component_example timer_sender_01 timer_sender_02 timer_sender_03 talker listener paramserver service record tcp_echo_server tcp_echo_client udp_echo_server udp_echo_client
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/cyber/examples
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/cyber/examples)
install(FILES ${EXAMPLE_FILES} DESTINATION ${CMAKE_INSTALL_LIBDIR}/cyber/examples)
