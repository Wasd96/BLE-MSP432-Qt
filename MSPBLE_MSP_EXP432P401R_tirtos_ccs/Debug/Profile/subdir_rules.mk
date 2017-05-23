################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Profile/data_service.obj: ../Profile/data_service.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Profile/data_service.d" --obj_directory="Profile" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Profile/manager_service.obj: ../Profile/manager_service.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Profile/manager_service.d" --obj_directory="Profile" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Profile/profile_util.obj: ../Profile/profile_util.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Profile/profile_util.d" --obj_directory="Profile" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Profile/simple_gatt_profile.obj: ../Profile/simple_gatt_profile.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Profile/simple_gatt_profile.d" --obj_directory="Profile" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Profile/stream_service.obj: ../Profile/stream_service.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Profile/stream_service.d" --obj_directory="Profile" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


