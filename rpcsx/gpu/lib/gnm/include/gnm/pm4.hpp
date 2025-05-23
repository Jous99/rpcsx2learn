#pragma once

namespace gnm {

enum Pm4Opcode {
  IT_NOP = 0x10,
  IT_SET_BASE = 0x11,
  IT_CLEAR_STATE = 0x12,
  IT_INDEX_BUFFER_SIZE = 0x13,
  IT_DISPATCH_DIRECT = 0x15,
  IT_DISPATCH_INDIRECT = 0x16,
  IT_ATOMIC_GDS = 0x1d,
  IT_OCCLUSION_QUERY = 0x1f,
  IT_SET_PREDICATION = 0x20,
  IT_REG_RMW = 0x21,
  IT_COND_EXEC = 0x22,
  IT_PRED_EXEC = 0x23,
  IT_DRAW_INDIRECT = 0x24,
  IT_DRAW_INDEX_INDIRECT = 0x25,
  IT_INDEX_BASE = 0x26,
  IT_DRAW_INDEX_2 = 0x27,
  IT_CONTEXT_CONTROL = 0x28,
  IT_INDEX_TYPE = 0x2a,
  IT_DRAW_INDIRECT_MULTI = 0x2c,
  IT_DRAW_INDEX_AUTO = 0x2d,
  IT_NUM_INSTANCES = 0x2f,
  IT_DRAW_INDEX_MULTI_AUTO = 0x30,
  IT_INDIRECT_BUFFER_CNST = 0x33,
  IT_STRMOUT_BUFFER_UPDATE = 0x34,
  IT_DRAW_INDEX_OFFSET_2 = 0x35,
  IT_DRAW_PREAMBLE = 0x36,
  IT_WRITE_DATA = 0x37,
  IT_DRAW_INDEX_INDIRECT_MULTI = 0x38,
  IT_MEM_SEMAPHORE = 0x39,
  IT_COPY_DW = 0x3b,
  IT_WAIT_REG_MEM = 0x3c,
  IT_INDIRECT_BUFFER = 0x3f,
  IT_COPY_DATA = 0x40,
  IT_PFP_SYNC_ME = 0x42,
  IT_SURFACE_SYNC = 0x43,
  IT_COND_WRITE = 0x45,
  IT_EVENT_WRITE = 0x46,
  IT_EVENT_WRITE_EOP = 0x47,
  IT_EVENT_WRITE_EOS = 0x48,
  IT_RELEASE_MEM = 0x49,
  IT_PREAMBLE_CNTL = 0x4a,
  IT_DMA_DATA = 0x50,
  IT_ACQUIRE_MEM = 0x58,
  IT_REWIND = 0x59,
  IT_LOAD_UCONFIG_REG = 0x5e,
  IT_LOAD_SH_REG = 0x5f,
  IT_LOAD_CONFIG_REG = 0x60,
  IT_LOAD_CONTEXT_REG = 0x61,
  IT_LOAD_SH_REG_INDEX = 0x63,
  IT_LOAD_UCONFIG_REG_INDEX = 0x64,
  IT_SET_CONFIG_REG = 0x68,
  IT_SET_CONTEXT_REG = 0x69,
  IT_SET_CONTEXT_REG_INDIRECT = 0x73,
  IT_SET_SH_REG = 0x76,
  IT_SET_SH_REG_OFFSET = 0x77,
  IT_SET_QUEUE_REG = 0x78,
  IT_SET_UCONFIG_REG = 0x79,
  IT_SET_UCONFIG_REG_INDEX = 0x7a,
  IT_SCRATCH_RAM_WRITE = 0x7d,
  IT_SCRATCH_RAM_READ = 0x7e,
  IT_LOAD_CONST_RAM = 0x80,
  IT_WRITE_CONST_RAM = 0x81,
  IT_DUMP_CONST_RAM = 0x83,
  IT_INCREMENT_CE_COUNTER = 0x84,
  IT_INCREMENT_DE_COUNTER = 0x85,
  IT_WAIT_ON_CE_COUNTER = 0x86,
  IT_WAIT_ON_DE_COUNTER_DIFF = 0x88,
  IT_SET_CE_DE_COUNTERS = 0x89,
  IT_WAIT_ON_AVAIL_BUFFER = 0x8a,
  IT_SWITCH_BUFFER = 0x8b,
  IT_DISPATCH_DRAW_PREAMBLE = 0x8c,
  IT_DISPATCH_DRAW = 0x8d,
  IT_WAIT_REG_MEM64 = 0x93,
  IT_LOAD_CONTEXT_REG_INDEX = 0x9f,
  IT_SET_RESOURCES = 0xa0,
  IT_MAP_PROCESS = 0xa1,
  IT_MAP_QUEUES = 0xa2,
  IT_UNMAP_QUEUES = 0xa3,
  IT_QUERY_STATUS = 0xa4,
  IT_RUN_LIST = 0xa5,

  _count
};

const char *pm4OpcodeToString(int opcode);
} // namespace gnm
