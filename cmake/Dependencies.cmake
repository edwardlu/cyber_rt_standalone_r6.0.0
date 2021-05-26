find_package(fastrtps REQUIRED CONFIG)
message(STATUS "Found fastrtps ${fastrtps_FIND_VERSION}")
message(STATUS "${fastrtps_LIB_DIR}, ${fastrtps_INCLUDE_DIR}")
include_directories(${fastrtps_INCLUDE_DIR})
link_directories(${fastrtps_LIB_DIR})

find_package(fastcdr REQUIRED CONFIG)
message(STATUS "Found fastcdr ${fastcdr_FIND_VERSION}")
message(STATUS "${fastcdr_LIB_DIR}, ${fastcdr_INCLUDE_DIR}")
include_directories(${fastcdr_INCLUDE_DIR})
link_directories(${fastcdr_LIB_DIR})

find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
	message(STATUS "Protobuf library found")
else()
	message(FATAL_ERROR "Protobuf library is needed but can not found")
endif()
include_directories(${PROTOBUF_INCLUDE_DIR})
link_directories(${PROTOBUF_LIBRARIES})

find_package(Poco REQUIRED COMPONENTS Foundation CONFIG)
message(STATUS "Found Poco: ${Poco_LIBRARIES}")

#basical libs for modules
find_package(Eigen3 REQUIRED)
#find_package(GTest REQUIRED)
#find_package(absl REQUIRED)
#find_package(osqp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_library(Tinyxml REQUIRED)
find_library(Uuid REQUIRED)
find_package(CUDA REQUIRED)
#find_package(nlohmann_json REQUIRED)

find_package(OpenCV REQUIRED)

find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

find_package(VTK REQUIRED)
include_directories(${VTK_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
