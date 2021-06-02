cmake_minimum_required(VERSION 3.0)

project(pcl_viewer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(VTK REQUIRED)
find_package(PCL REQUIRED)
find_package(CUDA REQUIRED)

include_directories(${PCL_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS})

message(STATUS "main project dir: " ${PROJECT_SOURCE_DIR})

set(PCL_LIBS
    pcl_common
    #pcl_io
    #pcl_kdtree
    #pcl_octree
    pcl_visualization
)

add_executable(pcl_viewer livox_viewer.cpp)
target_link_libraries(pcl_viewer shared_blocks cudafilter ${PCL_LIBS} ${CUDA_LIBRARIES}
        boost_system
        boost_thread
        vtkRenderingLOD-8.2
        vtkRenderingOpenGL2-8.2
        vtkInteractionStyle-8.2
        vtkRenderingFreeType-8.2
        vtkRenderingCore-8.2
        vtkFiltersSources-8.2
        vtkCommonExecutionModel-8.2
        vtkCommonCore-8.2
        vtkCommonDataModel-8.2
        vtkCommonMath-8.2
)

