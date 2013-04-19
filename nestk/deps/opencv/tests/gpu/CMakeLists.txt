# ----------------------------------------------------------------------------
#  CMake file for gputest. See root CMakeLists.txt
# ----------------------------------------------------------------------------
project(opencv_test_gpu)

file(GLOB test_srcs "src/*.cpp")
source_group("Src" FILES ${test_srcs})
file(GLOB test_hdrs "src/*.h*")
source_group("Include" FILES ${test_hdrs})

set(the_target "opencv_test_gpu")

include_directories	(
			"${OpenCV_SOURCE_DIR}/include/opencv"
			"${OpenCV_SOURCE_DIR}/modules/core/include"
			"${OpenCV_SOURCE_DIR}/modules/imgproc/include"
			"${OpenCV_SOURCE_DIR}/modules/features2d/include"
			"${OpenCV_SOURCE_DIR}/modules/flann/include"
			"${OpenCV_SOURCE_DIR}/modules/calib3d/include"
			"${OpenCV_SOURCE_DIR}/modules/highgui/include"
			"${OpenCV_SOURCE_DIR}/modules/objdetect/include"
			"${OpenCV_SOURCE_DIR}/modules/video/include"
			"${OpenCV_SOURCE_DIR}/modules/legacy/include"
			"${OpenCV_SOURCE_DIR}/modules/contrib/include"
			"${OpenCV_SOURCE_DIR}/modules/gpu/include"
			"${OpenCV_SOURCE_DIR}/modules/ml/include"
			"${CMAKE_CURRENT_SOURCE_DIR}/src"
			"${CMAKE_CURRENT_BINARY_DIR}"
			)

include_directories(../cxts)

add_executable(${the_target} ${test_srcs} ${test_hdrs})

# Additional target properties
set_target_properties(${the_target} PROPERTIES
	DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
	)

add_dependencies(${the_target} opencv_ts opencv_gpu opencv_highgui opencv_imgproc)

# Add the required libraries for linking:
target_link_libraries(${the_target} ${OPENCV_LINKER_LIBS} opencv_ts opencv_gpu opencv_highgui opencv_imgproc)

enable_testing()
get_target_property(LOC ${the_target} LOCATION)
add_test(${the_target} "${LOC}")

if(WIN32)
	install(TARGETS ${the_target} RUNTIME DESTINATION bin COMPONENT main)
endif()

