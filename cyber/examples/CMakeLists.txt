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

target_link_libraries(talker cyber)
target_link_libraries(listener cyber)
target_link_libraries(paramserver cyber)
target_link_libraries(service cyber)
target_link_libraries(record cyber cyber_proto)

add_library(common_component_example SHARED common_component_example/common_component_example.cc ${PROTO_SRCS})

add_library(timer_100ms_01 SHARED timer_component_example/timer_100ms_01.cc ${PROTO_SRCS})
add_library(timer_100ms_02 SHARED timer_component_example/timer_100ms_02.cc ${PROTO_SRCS})
add_library(timer_100ms_03 SHARED timer_component_example/timer_100ms_03.cc ${PROTO_SRCS})
add_library(timer_100ms_04 SHARED timer_component_example/timer_100ms_04.cc ${PROTO_SRCS})
add_library(timer_100ms_05 SHARED timer_component_example/timer_100ms_05.cc ${PROTO_SRCS})
add_library(timer_100ms_06 SHARED timer_component_example/timer_100ms_06.cc ${PROTO_SRCS})

add_library(timer_150ms_01 SHARED timer_component_example/timer_150ms_01.cc ${PROTO_SRCS})
add_library(timer_150ms_02 SHARED timer_component_example/timer_150ms_02.cc ${PROTO_SRCS})
add_library(timer_150ms_03 SHARED timer_component_example/timer_150ms_03.cc ${PROTO_SRCS})
add_library(timer_150ms_04 SHARED timer_component_example/timer_150ms_04.cc ${PROTO_SRCS})
add_library(timer_150ms_05 SHARED timer_component_example/timer_150ms_05.cc ${PROTO_SRCS})
add_library(timer_150ms_06 SHARED timer_component_example/timer_150ms_06.cc ${PROTO_SRCS})

target_link_libraries(common_component_example cyber)

target_link_libraries(timer_100ms_01 cyber)
target_link_libraries(timer_100ms_02 cyber)
target_link_libraries(timer_100ms_03 cyber)
target_link_libraries(timer_100ms_04 cyber)
target_link_libraries(timer_100ms_05 cyber)
target_link_libraries(timer_100ms_06 cyber)

target_link_libraries(timer_150ms_01 cyber)
target_link_libraries(timer_150ms_02 cyber)
target_link_libraries(timer_150ms_03 cyber)
target_link_libraries(timer_150ms_04 cyber)
target_link_libraries(timer_150ms_05 cyber)
target_link_libraries(timer_150ms_06 cyber)


file(GLOB EXAMPLE_FILES "*/*.dag" "*/*.launch")
install(TARGETS common_component_example talker listener paramserver service record
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/cyber/examples
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/cyber/examples)
install(FILES ${EXAMPLE_FILES} DESTINATION ${CMAKE_INSTALL_LIBDIR}/cyber/examples)
