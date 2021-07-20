################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/AppSw/Tricore/User/subdir.mk
-include src/AppSw/Tricore/Driver/subdir.mk
-include src/AppSw/Tricore/APP/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/_PinMap/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/_Lib/DataHandling/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/_Impl/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Stm/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Src/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Scu/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Qspi/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Qspi/SpiSlave/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Qspi/SpiMaster/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Port/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Port/Io/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Pms/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Mtu/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/I2c/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/I2c/I2c/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gtm/Trig/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gtm/Tom/Timer/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gtm/Tom/PwmHl/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gtm/Tom/Pwm/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gtm/Tim/In/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gtm/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gtm/Atom/Pwm/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gpt12/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Gpt12/IncrEnc/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Flash/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Evadc/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Evadc/Adc/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Dma/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Dma/Dma/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Cpu/Trap/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Cpu/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Cpu/Irq/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Ccu6/Timer/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Ccu6/TPwm/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Ccu6/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Ccu6/PwmBc/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Ccu6/Icu/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Asclin/Std/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Asclin/Spi/subdir.mk
-include Libraries/iLLD/TC37A/Tricore/Asclin/Asc/subdir.mk
-include Libraries/Service/CpuGeneric/SysSe/Math/subdir.mk
-include Libraries/Service/CpuGeneric/SysSe/Bsp/subdir.mk
-include Libraries/Service/CpuGeneric/StdIf/subdir.mk
-include Libraries/Service/CpuGeneric/If/subdir.mk
-include Libraries/Infra/Ssw/TC37A/Tricore/subdir.mk
-include Libraries/Infra/Platform/Tricore/Compilers/subdir.mk
-include Configurations/Debug/subdir.mk
-include Configurations/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
demoLQ_ALL.siz \


# All Target
all: demoLQ_ALL.elf secondary-outputs

# Tool invocations
demoLQ_ALL.elf: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TASKING Linker'
	cctc -lrt -lfp_fpu -lcs_fpu  -Wl-Oc -Wl-OL -Wl-Ot -Wl-Ox -Wl-Oy -Wl--map-file="demoLQ_ALL.map" -Wl-mc -Wl-mf -Wl-mi -Wl-mk -Wl-ml -Wl-mm -Wl-md -Wl-mr -Wl-mu --no-warnings= -Wl--error-limit=42 --strict --anachronisms --force-c++ -Ctc37x   -o"demoLQ_ALL.elf" -Wl-o"demoLQ_ALL.hex:IHEX" --lsl-core=vtc  --lsl-file=../Lcf_Tasking_Tricore_Tc.lsl $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

demoLQ_ALL.siz: demoLQ_ALL.elf
	@echo 'Invoking: Print Size'
	elfsize  "demoLQ_ALL.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	find . ! -name "*.mk" ! -name "makefile" ! -name "." -type f -delete 
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
