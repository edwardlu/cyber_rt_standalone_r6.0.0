set(shared_blocks_include_dir
	${PROJECT_SOURCE_DIR}/shared_mem
)
include_directories(${shared_blocks_include_dir})

set(shared_blocks_source_files
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/shared_mem/block_map.cc
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/shared_mem/block_interface.cc
)

add_library(shared_blocks SHARED ${shared_blocks_source_files})

set(VTK_LIBS
	vtkRenderingLOD-8.2
	vtkRenderingOpenGL2-8.2
	vtkInteractionStyle-8.2
	vtkRenderingFreeType-8.2
	vtkRenderingCore-8.2
	vtkFiltersSources-8.2
	vtkCommonExecutionModel-8.2
	vtkCommonCore-8.2
)

set(livox_source_files
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/communication/protocol/livox_protocol.cc
        ${PROJECT_SOURCE_DIR}/modules/drivers/livox/communication/protocol/FastCRC/FastCRCsw.cpp	
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/communication/livox_command/command.cc
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/communication/device_controller/dev_controller.cc
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/communication/device_data_recv/device_data_recv.cc
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/communication/device_data_recv/time_check.cc
        ${PROJECT_SOURCE_DIR}/modules/drivers/livox/device_manager/device_info.cc
        ${PROJECT_SOURCE_DIR}/modules/drivers/livox/device_manager/socket_util.cc
	${PROJECT_SOURCE_DIR}/modules/drivers/livox/livox_daemon/livox_sdk_daemon.cc
)

add_library(livox_sdk SHARED ${livox_source_files})
target_link_libraries(livox_sdk cyber shared_blocks)
#target_link_libraries(livox_sdk cyber shared_blocks ${PCL_LIBRARIES} ${VTK_LIBS} ${Boost_LIBRARIES})

add_executable(livox_daemon livox_daemon/livox_sdk.cc)
target_link_libraries(livox_daemon cyber livox_sdk)
