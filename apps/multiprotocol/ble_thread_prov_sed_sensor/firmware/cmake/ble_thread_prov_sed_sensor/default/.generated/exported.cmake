set(DEPENDENT_MP_BIN2HEXble_thread_prov_sed_sensor_default_agqgMInt "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFble_thread_prov_sed_sensor_default_agqgMInt ${CMAKE_CURRENT_LIST_DIR}/../../../../out/ble_thread_prov_sed_sensor/default.elf)
set(DEPENDENT_TARGET_DIRble_thread_prov_sed_sensor_default_agqgMInt ${CMAKE_CURRENT_LIST_DIR}/../../../../out/ble_thread_prov_sed_sensor)
set(DEPENDENT_BYPRODUCTSble_thread_prov_sed_sensor_default_agqgMInt ${DEPENDENT_TARGET_DIRble_thread_prov_sed_sensor_default_agqgMInt}/${sourceFileNameble_thread_prov_sed_sensor_default_agqgMInt}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRble_thread_prov_sed_sensor_default_agqgMInt}/${sourceFileNameble_thread_prov_sed_sensor_default_agqgMInt}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXble_thread_prov_sed_sensor_default_agqgMInt} --image ${DEPENDENT_DEPENDENT_TARGET_ELFble_thread_prov_sed_sensor_default_agqgMInt} --image-generated-c ${sourceFileNameble_thread_prov_sed_sensor_default_agqgMInt}.c --image-generated-h ${sourceFileNameble_thread_prov_sed_sensor_default_agqgMInt}.h --image-copy-mode ${modeble_thread_prov_sed_sensor_default_agqgMInt} --image-offset ${addressble_thread_prov_sed_sensor_default_agqgMInt} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRble_thread_prov_sed_sensor_default_agqgMInt}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFble_thread_prov_sed_sensor_default_agqgMInt})
add_custom_target(
    dependent_produced_source_artifactble_thread_prov_sed_sensor_default_agqgMInt 
    DEPENDS ${DEPENDENT_TARGET_DIRble_thread_prov_sed_sensor_default_agqgMInt}/${sourceFileNameble_thread_prov_sed_sensor_default_agqgMInt}.c
    )
