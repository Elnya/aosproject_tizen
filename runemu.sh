#!/bin/bash


# Modify path variables to your own path
TIZEN_SDK_HOME="/home/dahye/tizen-sdk"
TIZEN_SDK_DATA_HOME="/home/dahye/tizen-sdk-data"
#PROJECT_HOME="/home/dahye/aosproject_tizen"
VM_NAME="tizen_test"

$TIZEN_SDK_HOME/tools/emulator/bin/emulator-x86 --skin-args width=720 height=1280 skin.path=$TIZEN_SDK_HOME/tools/emulator/skins/emul-general-3btn --qemu-args -drive file=$TIZEN_SDK_DATA_HOME/emulator-vms/vms/$VM_NAME/emulimg-$VM_NAME.x86,if=virtio,index=1 -boot c -append "console=ttyS0 video=uvesafb:ywrap,720x1280-32@60 dpi=3160 ip=10.0.2.16::10.0.2.2:255.255.255.0::eth0:none 5 vm_name=tizen_test" -serial file:$TIZEN_SDK_DATA_HOME/emulator-vms/vms/$VM_NAME/logs/emulator.klog -m 1024 -M maru-x86-machine -net nic,model=virtio,macaddr=E8:11:32:33:38:81 -soundhw all -usb -device virtio-touchscreen-pci -vga maru -bios bios.bin -L $TIZEN_SDK_HOME/tools/emulator/data/bios -kernel /home/dahye/emulator-kernel/arch/x86/boot/bzImage -net user -rtc base=utc -drive file=$TIZEN_SDK_DATA_HOME/emulator-vms/vms/$VM_NAME/swap-$VM_NAME.img,if=virtio,index=2 -enable-gl -enable-hax -max-touch-point 6 -device virtio-esm-pci
# /home/dahye/tizen-sdk/tools/emulator/bin/emulator-x86 --skin-args width=720 height=1280 log.level=Debug test.hb.ignore=true skin.path=/home/dahye/tizen-sdk/tools/emulator/skins/emul-general-3btn --qemu-args -drive file=/home/dahye/tizen-sdk-data/emulator-vms/vms/tizen_test/emulimg-tizen_test.x86,if=virtio,index=1 -boot c -append "console=ttyS0 video=uvesafb:ywrap,720x1280-32@60 dpi=3160 ip=10.0.2.16::10.0.2.2:255.255.255.0::eth0:none 5" -serial file:/home/dahye/tizen-sdk-data/emulator-vms/vms/tizen_test/logs/emulator.klog -m 512 -M maru-x86-machine -net nic,model=virtio,macaddr=E8:11:32:33:38:81 -soundhw all -usb -device virtio-touchscreen-pci -vga maru -bios bios.bin -L /home/dahye/tizen-sdk/tools/emulator/data/bios -kernel /home/dahye/tizen-sdk/tools/emulator/data/kernel/bzImage.x86 -net user -rtc base=utc -drive file=/home/dahye/tizen-sdk-data/emulator-vms/vms/tizen_test/swap-tizen_test.img,if=virtio,index=2 -enable-gl -enable-kvm -max-touch-point 6 -device virtio-esm-pci
