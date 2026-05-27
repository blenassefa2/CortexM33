//#include "RTE_Components.h"                 /* This mechanism is the standard way to include */
#include "ARMCM33_DSP_FP.h"              /* the device header file, in this case ARMCM33.h */
#include <stddef.h>
#include <stdint.h>
#include "mpu.h"

struct region regions[CONFIGURED_SIZE];
struct attribute attributes[ATTRIBUTE_SIZE];

uint32_t mpu_attribute_count = 2;
uint32_t mpu_region_count = CONFIGURED_SIZE;

uint32_t convert_a(struct attribute *attr)
{
    // uint32_t v;

    // if (attr == NULL) {
    //     return 0;
    // }
    // v = (uint32_t)(attr->all & 0xFFu);
    // if (v > (uint32_t)GRE) {
    //     return v;
    // }
    // switch ((enum device_attribute_type)v) {
    //     case NGNRNE:
    //         return ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGnRnE);
    //     case NGNRE:
    //         return ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGnRE);
    //     case NGRE:
    //         return ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGRE);
    //     case GRE:
    //         return ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_GRE);
    // }
    return 0;
}

uint32_t convert_r(struct rlar_s *rl, struct rbar_s *rb)
{
    if (rl != NULL)
    {
        return (rl->limit_address << 5 ) | (rl->attr_indx << 1) | rl->region_enable;
    }
    else if (rb != NULL)
    {
        return (rb->base_address << 5) | (rb->sharability << 3) | (rb->wr << 1) | rb->executable;
    }
    return 0;
}



void mpu_load_default_configuration(void)
{
    attributes[0].all = (uint32_t)ARM_MPU_ATTR(
        ARM_MPU_ATTR_NON_CACHEABLE,
        ARM_MPU_ATTR_NON_CACHEABLE
    );
    attributes[1].all = (uint32_t)ARM_MPU_ATTR(
        ARM_MPU_ATTR_DEVICE,
        ARM_MPU_ATTR_DEVICE_nGnRnE
    );

    regions[0].base.executable = 0;
    regions[0].base.wr = 2;
    regions[0].base.sharability = 0;
    regions[0].base.base_address = 0;
    regions[0].limit.region_enable = 1;
    regions[0].limit.attr_indx = 0;
    regions[0].limit.ignored = 0;
    regions[0].limit.limit_address = 0x3FFFu >> 5;

    regions[1].base.executable = 0;
    regions[1].base.wr = 1;
    regions[1].base.sharability = 0;
    regions[1].base.base_address = 0x4000u >> 5;
    regions[1].limit.region_enable = 1;
    regions[1].limit.attr_indx = 0;
    regions[1].limit.ignored = 0;
    regions[1].limit.limit_address = 0x7FFFFFu >> 5;

    regions[2].base.executable = 0;
    regions[2].base.wr = 0;
    regions[2].base.sharability = 0;
    regions[2].base.base_address = 0x28200000u >> 5;
    regions[2].limit.region_enable = 1;
    regions[2].limit.attr_indx = 0;
    regions[2].limit.ignored = 0;
    regions[2].limit.limit_address = 0x28207FFFu >> 5;

    mpu_attribute_count = 2;
    mpu_region_count = CONFIGURED_SIZE;
}



void mpu_setup_start( void ) {
   __DMB(); /* Force any outstanding transfers to complete before disabling MPU */
    /* Disable MPU */
   
    MPU->CTRL = 0;
    /* Exercise 2 -  Initialize two Memory Attributes  */
   
   
}

void mpu_set_attribute(uint32_t idx, struct attribute *attr) {
    if (idx >= ATTRIBUTE_SIZE || attr == NULL  ) {
        return;
    }
    ARM_MPU_SetMemAttr((uint8_t)idx, (uint8_t)convert_a(attr));

    /*
    void mpu_set_attribute(int idx) {
         // Configure memory types
        
        // Set Memory Attr 0
        
        ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(
            ARM_MPU_ATTR_NON_CACHEABLE, // 0100 if write through and no allocation it is considered non cachable
            ARM_MPU_ATTR_NON_CACHEABLE
        ));
        
        // Set Memory Attr 1
        ARM_MPU_SetMemAttr(1UL, ARM_MPU_ATTR(
            ARM_MPU_ATTR_DEVICE,
            ARM_MPU_ATTR_DEVICE_nGnRnE
        ));

    }
    */
}

void mpu_apply_attributes(uint32_t count)
{
    uint32_t n = count;

    if (n > ATTRIBUTE_SIZE) {
        n = ATTRIBUTE_SIZE;
    }
    for (uint32_t i = 0; i < n; i++) {
        mpu_set_attribute(i, &attributes[i]);
    }
}

void mpu_set_region(uint32_t idx, struct rbar_s *base, struct rlar_s *limit) {
    if (idx >= CONFIGURED_SIZE || base == NULL || limit == NULL) {
        return;
    }
    uint32_t rbar = convert_r(NULL, base); 
    uint32_t rlar = convert_r(limit, NULL); 
    ARM_MPU_SetRegion(idx, rbar, rlar); // ARM_MPU_SetRegion is a function to set the region

    /*
    void mpu_set_region() {
        
        // Set up Attr 2-7 for further Normal and Device memory configurations: pass

        // Exercise 3 -  Initialize memory regions
        MPU->RNR = 0; // for all 3 regions 

        // Configure region 0 - Mappable memory
        // instead of defining local struct I'll use global array regions
        // struct region mappable_memory; 

        // if it was  a local struct I might get garbage values so I must set each attribute
        // but now since i am using global variable I can assim all are initially set to 0
        regions[0].base.wr = 2; // RO priv only

        regions[0].limit.region_enable = 1;
        regions[0].limit.limit_address = 0x3FFF >> 5; 

        MPU->RBAR = convert_r(NULL, &regions[0].base);
        MPU->RLAR = convert_r(&regions[0].limit, NULL);


        // Configure region 1 - ZBTSRAM 1
        // struct region zbtsram1;
        regions[1].base.wr = 1;
        regions[1].base.base_address = 0x4000 >> 5;


        regions[1].limit.region_enable = 1;
        regions[1].limit.limit_address = 0x7FFFFF >> 5; 

        MPU->RBAR_A1 = convert_r(NULL, &regions[1].base);
        MPU->RLAR_A1 = convert_r(&regions[1].limit, NULL);

        //
        //  Configure region 2 - ZBTSRAM 2 & 3 (2x32-bit) 
        //  I am using this address [0x28200000 - 0x28207FFF] because this is where None-secure RAM is found at
        //  (0)r/w by priviledged only
        //
        regions[2].base.wr = 0;
        regions[2].base.base_address = 0x28200000 >> 5; 


        regions[2].limit.region_enable = 1;
        regions[2].limit.limit_address = 0x28207FFF >> 5; 

        MPU->RBAR_A2 = convert_r(NULL, &regions[2].base);
        MPU->RLAR_A2 = convert_r(&regions[2].limit, NULL);

    }
    */
}

void mpu_apply_regions(uint32_t count)
{
    uint32_t n = count;

    if (n > CONFIGURED_SIZE) {
        n = CONFIGURED_SIZE;
    }
    for (uint32_t i = 0; i < n; i++) {
        mpu_set_region(i, &regions[i].base, &regions[i].limit);
    }
}

void mpu_set_base(uint32_t idx,   uint32_t executable, uint32_t wr, uint32_t sharability, uint32_t base_address) {
    if (idx >= CONFIGURED_SIZE) {
        return;
    }
    regions[idx].base.executable = executable;
    regions[idx].base.wr = wr;
    regions[idx].base.sharability = sharability;
    regions[idx].base.base_address = base_address;
}

void mpu_set_limit(uint32_t idx, uint32_t region_enable, uint32_t attr_indx, uint32_t limit_address) {
    if (idx >= CONFIGURED_SIZE) {
        return;
    }
    regions[idx].limit.region_enable = region_enable;
    regions[idx].limit.attr_indx = attr_indx;
    regions[idx].limit.limit_address = limit_address;
}
void mpu_setup_finish(){
    MPU->CTRL |= 5; /* Enable the MPU */
    SCB->SHCSR |= (1 << 16);  // enable MemManage fault
    
    __DSB(); /* Force memory writes before continuing */
    __ISB(); /* Flush and refill pipeline with updated permissions */
}

void mpu_setup(void)
{
    mpu_setup_start();
    mpu_load_default_configuration();
    mpu_apply_attributes(mpu_attribute_count);
    mpu_apply_regions(mpu_region_count);
    mpu_setup_finish();
}
