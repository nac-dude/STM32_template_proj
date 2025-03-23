
cmake_minimum_required(VERSION 3.22)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wa,-adhln")

# Specify the output directory for list files
set(LST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/lst)
file(MAKE_DIRECTORY ${LST_OUTPUT_DIR})


# Specify the output directory for assembly files
set(ASM_OUTPUT_DIR ${CMAKE_BINARY_DIR}/asm)
file(MAKE_DIRECTORY ${ASM_OUTPUT_DIR})


# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})


#
# Core project settings
#
project(STM32_template_proj)          # Modified
enable_language(C CXX ASM)




message("top: CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("top: CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message("top: CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("top: CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")


message("top: CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("top: CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
message("lwbtn: CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("lwbtn: CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message("lwbtn: CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("lwbtn: CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    -mthumb

    # This needs attention to properly set for used MCU
    -mcpu=cortex-m3                 # Modified
    -mfpu=auto                     # Modified
    -mfloat-abi=soft                # Modified
)

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/STM32F103RBTX_FLASH.ld) # Modified
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})



#
# List of source files to compile
#



set(sources_SRCS                    # Modified
    ${PROJ_PATH}/Core/Src/main.c
    ${PROJ_PATH}/Core/Src/nt_encoder.c
	${PROJ_PATH}/Core/Src/nt_debounce.c
    ${PROJ_PATH}/Core/Src/nt_time_utils.c
    ${PROJ_PATH}/Core/Src/nt_system_init.c
    ${PROJ_PATH}/Core/Src/nt_time.c
	${PROJ_PATH}/Core/Src/nt_gpio.c
	${PROJ_PATH}/Core/Src/nt_usart.c
	${PROJ_PATH}/Core/Src/nt_LCD_1602.c
    
	${PROJ_PATH}/Core/Src/stm32f1xx_it.c
    ${PROJ_PATH}/Core/Src/syscalls.c
    ${PROJ_PATH}/Core/Src/sysmem.c
    ${PROJ_PATH}/Core/Src/system_stm32f1xx.c
	
    ${PROJ_PATH}/Core/Startup/startup_stm32f103rbtx.s
	
    ${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dma.c
	${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_exti.c
	${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_gpio.c
	${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_pwr.c
	${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rcc.c
	${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_tim.c
	${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usart.c
	${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_utils.c
	   

)

#
# Include directories
#
set(include_path_DIRS               # Modified
    ${PROJ_PATH}/Core/Inc
    ${PROJ_PATH}/Core/Src
    ${PROJ_PATH}/Drivers/STM32F1xx_HAL_Driver/Inc
    ${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ${PROJ_PATH}/Drivers/CMSIS/Include

)




#
# Symbols definition
#
set(symbols_SYMB                    # Modified
    "DEBUG"
    "STM32F103xB"
    "USE_FULL_LL_DRIVER"
    "HSE_VALUE=8000000"
    "HSE_STARTUP_TIMEOUT=100"
    "LSE_STARTUP_TIMEOUT=5000"
    "LSE_VALUE=32768"
    "HSI_VALUE=8000000"
    "LSI_VALUE=40000"
    "VDD_VALUE=3300"
    "PREFETCH_ENABLE=1"
)

# Executable files

add_executable(${EXECUTABLE} ${sources_SRCS})


# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb 
)


# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -u _printf_float                # STDIO float formatting support
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)



add_subdirectory(lwbtn)
target_link_libraries(${CMAKE_PROJECT_NAME} lwbtn)

add_subdirectory(lwprintf)
target_link_libraries(${CMAKE_PROJECT_NAME} lwprintf)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)


# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)


foreach(source_file IN LISTS sources_SRCS)
    # Construct the full path for the .lst file
    get_filename_component(source_file_name ${source_file} NAME) # Extract the filename only
    set(lst_file "${LST_OUTPUT_DIR}/${source_file_name}.lst") # Use binary directory

    # Apply the compile option for generating the .lst file
    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${lst_file}")
endforeach()



