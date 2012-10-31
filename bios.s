.text
.code 16

.align 2
.global bitUnpack
.thumb_func

bitUnpack:
	swi 0x10
	bx lr

.align 2
.global decompressLZ77VRAM
.thumb_func

decompressLZ77VRAM:
	swi 0x11
	bx lr

.align 2
.global vBlankIntrWait
.thumb_func

vBlankIntrWait:
	swi 0x05
	bx lr
	
@Sound functions:

.align 2
.global soundDriverInit
.thumb_func

soundDriverInit:
	swi 0x1A
	bx lr
	
.align 2
.global soundDriverMain
.thumb_func

soundDriverMain:
	swi 0x1C
	bx lr

.align 2
.global soundDriverVSync
.thumb_func

soundDriverVSync:
	swi 0x1D
	bx lr
	
.align 2
.global soundDriverVSyncOff
.thumb_func

soundDriverVSyncOff:
	swi 0x28
	bx lr
	
.align 2
.global soundDriverVSyncOn
.thumb_func

soundDriverVSyncOn:
	swi 0x29
	bx lr