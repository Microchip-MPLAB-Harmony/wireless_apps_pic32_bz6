# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "C:/Users/I73764/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(ble_thread_prov_sed_sensor_default_default_XC32_assemble_rule target)
    set(options
        "-g"
        "${ASSEMBLER_PRE}"
        "-mprocessor=32WM_BZ6204"
        "-Wa,--defsym=__MPLAB_BUILD=1${MP_EXTRA_AS_POST},--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--gdwarf-2,-I${CMAKE_CURRENT_SOURCE_DIR}/../../../ble_thread_prov_sed_sensor.X"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CX-BZ6_DFP/1.3.38/PIC32WM_BZ6024")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target} PRIVATE "__DEBUG=1")
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../ble_thread_prov_sed_sensor.X")
endfunction()
function(ble_thread_prov_sed_sensor_default_default_XC32_assembleWithPreprocess_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "${MP_EXTRA_AS_PRE}"
        "${DEBUGGER_NAME_AS_MACRO}"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CX-BZ6_DFP/1.3.38/PIC32WM_BZ6024"
        "-mprocessor=32WM_BZ6204"
        "-Wa,--defsym=__MPLAB_BUILD=1${MP_EXTRA_AS_POST},--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,-I${CMAKE_CURRENT_SOURCE_DIR}/../../../ble_thread_prov_sed_sensor.X")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../ble_thread_prov_sed_sensor.X")
endfunction()
function(ble_thread_prov_sed_sensor_default_default_XC32_compile_rule target)
    set(options
        "-g"
        "${CC_PRE}"
        "-x"
        "c"
        "-c"
        "-mprocessor=32WM_BZ6204"
        "-ffunction-sections"
        "-fdata-sections"
        "-O2"
        "-fcommon"
        "-include../src/config/default/driver/thread/inc/openthread_stack_config.h"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CX-BZ6_DFP/1.3.38/PIC32WM_BZ6024")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "ENABLE_LARGE_BUFFER"
        PRIVATE "ENABLE_QUEUE_CAPACITY"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../ble_thread_prov_sed_sensor.X"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/app_ble"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/ble/lib/include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/ble/middleware_ble"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/ble/profile_ble"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/ble/service_ble"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/pal/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/phy/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/resources/buffer/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/resources/queue/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/pds/include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/packs/CMSIS"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/packs/CMSIS/CMSIS/Core/Include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/packs/PIC32WM_BZ6204_DFP"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/third_party/rtos/FreeRTOS/Source/include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F")
endfunction()
function(ble_thread_prov_sed_sensor_default_default_XC32_compile_cpp_rule target)
    set(options
        "-g"
        "${CC_PRE}"
        "${DEBUGGER_NAME_AS_MACRO}"
        "-mprocessor=32WM_BZ6204"
        "-frtti"
        "-fexceptions"
        "-fno-check-new"
        "-fenforce-eh-specs"
        "-ffunction-sections"
        "-O1"
        "-fno-common"
        "-include../src/config/default/driver/thread/inc/openthread_stack_config.h"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CX-BZ6_DFP/1.3.38/PIC32WM_BZ6024")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../ble_thread_prov_sed_sensor.X"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/pal/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/phy/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/resources/buffer/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/config/default/driver/IEEE_802154_PHY/resources/queue/inc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/packs/CMSIS"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/packs/CMSIS/CMSIS/Core/Include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/packs/PIC32WM_BZ6204_DFP"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/third_party/rtos/FreeRTOS/Source/include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/ARM_CM4F")
endfunction()
function(ble_thread_prov_sed_sensor_default_dependentObject_rule target)
    set(options
        "-mprocessor=32WM_BZ6204"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CX-BZ6_DFP/1.3.38/PIC32WM_BZ6024")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(ble_thread_prov_sed_sensor_default_link_rule target)
    set(options
        "-g"
        "${MP_EXTRA_LD_PRE}"
        "${DEBUGGER_OPTION_TO_LINKER}"
        "${DEBUGGER_NAME_AS_MACRO}"
        "-mprocessor=32WM_BZ6204"
        "-mno-device-startup-code"
        "-Wl,--defsym=__MPLAB_BUILD=1${MP_EXTRA_LD_POST},--script=${ble_thread_prov_sed_sensor_default_LINKER_SCRIPT},--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=_min_heap_size=1024,--gc-sections,-L${CMAKE_CURRENT_SOURCE_DIR}/../../../ble_thread_prov_sed_sensor.X,-Map=mem.map,--memorysummary,memoryfile.xml"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CX-BZ6_DFP/1.3.38/PIC32WM_BZ6024")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target} PRIVATE "XPRJ_default=default")
endfunction()
function(ble_thread_prov_sed_sensor_default_bin2hex_rule target)
    add_custom_target(
        ble_thread_prov_sed_sensor_default_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${ble_thread_prov_sed_sensor_default_image_name}
        WORKING_DIRECTORY ${ble_thread_prov_sed_sensor_default_output_dir}
        BYPRODUCTS "${ble_thread_prov_sed_sensor_default_output_dir}/${ble_thread_prov_sed_sensor_default_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(ble_thread_prov_sed_sensor_default_Bin2Hex ${target})
endfunction()
