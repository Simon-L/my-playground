Little challenge/exercise that doubles as POC:  
Split the simple_client jack example into 2 libs that don't depend on each other: One is a base client loading the process function from the other lib  
Use LuaJIT super-powers to call init() from base lib, providing process function from process lib.  

POC interest is to use this concept to hot-reload Faust DSPs using its LLVM backend.

#### Building:
`make libs`  

If you have luajit for the cool lua stuff: `make lua`


###### Makefile
```makefile
OUTDIR=build
LIB_PROCESS=$(OUTDIR)/process.so
LIB_BASE=$(OUTDIR)/base.so

dirs:
	mkdir -p build

$(LIB_PROCESS): dirs
	$(CC) process.c -fPIC -shared -lm -ljack -o $@
	
$(LIB_BASE): dirs
	$(CC) base.c -fPIC -shared -lm -ljack -o $@

libs: $(LIB_PROCESS) $(LIB_BASE)

lua: clean libs 
	luajit main.lua ./build/process.so

clean:
	rm -rf build
```
###### main.lua
```lua
if arg[1] == nil then
  print("Usage: luajit main.lua <path_to_process.so>")
  os.exit(-1)
end

local ffi = require("ffi")
ffi.cdef[[
  typedef struct jack_port_t jack_port_t;
  typedef uint32_t        jack_nframes_t;
  void* setup_process(jack_port_t *_output_port1, jack_port_t *_output_port2);
  int process (jack_nframes_t nframes, void *arg);
  int init(void* (*arg_setup_process)(jack_port_t*, jack_port_t*), int (*arg_process)(jack_nframes_t, void*));
]]

local process = ffi.load(arg[1])
local base = ffi.load("./build/base.so")

base.init(process.setup_process, process.process)
```

