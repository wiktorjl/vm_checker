import cpuinfo
from cpuid import *

known_vendors = [
    "XenVMMXenVMM",
    "VMwareVMware",
    "Microsoft Hv",
    "VBoxVBoxVBox",
    "prl hyperv",
    "KVMKVMKVM",
    "bhyve bhyve"


]
def get_cpuid():
    info = cpuinfo.get_cpu_info()
    return info.get('vendor_id_raw', 'Unknown Vendor') in known_vendors


def is_run_in_hypervisor():
    eax, ebx, ecx, edx = cpuid(1)
    return (ecx >> 31) & 1 == 1


is_in_vm = get_cpuid() or is_run_in_hypervisor()

if is_in_vm:
    print("You are inside the Matrix")
else:
    print("Bare metal")