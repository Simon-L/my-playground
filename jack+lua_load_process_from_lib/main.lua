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