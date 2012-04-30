define allow_flash_upload
monitor flash device = stm32f103c8
monitor flash download = 1
end

define upload
monitor reset 8
monitor sleep 10
load
end
