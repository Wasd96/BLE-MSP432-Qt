################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MSP_EXP432P401R.obj: ../MSP_EXP432P401R.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="MSP_EXP432P401R.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main_tirtos.obj: ../main_tirtos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="main_tirtos.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mspble.obj: ../mspble.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="mspble.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mspble_params.obj: ../mspble_params.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/Volk/workspace_v7/MSPBLE_MSP_EXP432P401R_tirtos_ccs/Profile" --include_path="C:/ti/simplelink_msp432_sdk_bluetooth_plugin_1_10_00_42/source" --include_path="C:/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --advice:power_severity=suppress --define=__MSP432P401R__ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="mspble_params.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


